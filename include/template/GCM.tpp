#pragma once

#include "GCM.h"
#include "ZeroPadding.h"
#include <cassert>

template <size_t keySize>
    requires AESKeySize<keySize>
void GCM<keySize>::setIV(ByteSpan<> iv) {
    rawIv.resize(iv.size());
    std::copy(iv.begin(), iv.end(), this->rawIv.begin());

    if (rawIv.size() == 12) { // 96 bits iv
        std::copy(rawIv.begin(), rawIv.end(), j0.begin());
        std::fill(this->j0.begin() + rawIv.size(), j0.end() - 1, std::byte(0));
        j0.back() = std::byte(1);
    } else {
        ZeroPadding<toByteCount(Algorithm::BLOCK_SIZE)> padding;
        auto paddedIv = padding.pad(rawIv);
        paddedIv.resize(paddedIv.size() + sizeof(uint32_t), std::byte(0));
        auto length = toByteArray<uint64_t>(rawIv.size());
        paddedIv = concatenate(ByteSpan<>(paddedIv), ByteSpan<>(length));
        j0 = ghash(paddedIv);
    }

    auto bitset = toBitset(ByteSpan<16>(j0));
    increment(bitset);
    this->iv = toByteArray(bitset);
}

template <size_t keySize>
    requires AESKeySize<keySize>
void GCM<keySize>::setAAD(ByteSpan<> aad) {
    this->aad.resize(aad.size());
    std::copy(aad.begin(), aad.end(), this->aad.begin());
}


template <size_t keySize>
    requires AESKeySize<keySize>
ByteArray<> GCM<keySize>::encrypt(ByteSpan<> bytes) const {
    ByteArray<> ciphertext = CTR<AES<keySize>>::encrypt(bytes);
    Tag tag = createTag(ciphertext);
    return concatenate(ByteSpan<>(ciphertext), ByteSpan<>(tag));
}

template <size_t keySize>
    requires AESKeySize<keySize>
ByteArray<> GCM<keySize>::decrypt(ByteSpan<> bytes) const {
    return encrypt(bytes);
}

template <size_t keySize>
    requires AESKeySize<keySize>
GCM<keySize>::Tag GCM<keySize>::ghash(ByteArray<> bytes, Tag initialTag) const {
    constexpr size_t blockSize  = toByteCount(Algorithm::BLOCK_SIZE);

    // Padding
    bytes = ZeroPadding<blockSize>().pad(std::move(bytes));

    const size_t blockCount = bytes.size() / blockSize;
    
    // H, aka encryption of 128 zeros
    ByteArray<blockSize> h = toByteArray(this->algorithm.encrypt(std::bitset<Algorithm::BLOCK_SIZE>()));

    // Alias for readability
    Tag &tag = initialTag;

    for (int i = 0; i < blockCount; i++) {
        const size_t blockIndex = i * blockSize;
        ByteSpan<blockSize> currentBlock(bytes.data() + blockIndex, blockSize);
        tag = ByteSpan<blockSize>(tag) ^ currentBlock;
        tag = GF128::multiply(tag, h);
    }

    return tag;
}

template <size_t keySize>
    requires AESKeySize<keySize>
GCM<keySize>::Tag GCM<keySize>::createTag(const ByteArray<> &ciphertext) const {
    Tag  aadPart          = ghash(aad);
    Tag  ciphertextPart   = ghash(ciphertext, aadPart);
    auto cipherTextLength = toByteArray<uint64_t>(ciphertext.size());
    auto aadLength        = toByteArray<uint64_t>(aad.size());
    ByteArray<> lengths   = concatenate(ByteSpan<>(cipherTextLength), ByteSpan<>(aadLength));
    Tag  lengthsPart      = ghash(lengths, ciphertextPart);
    Tag  tag              = operator^(ByteSpan<16>(lengthsPart), ByteSpan<16>(j0));

    return tag;
}
#pragma once

#include "GCM.h"
#include "ZeroPadding.h"
#include "bytes_stream.h"
#include <iostream>
#include <cassert>

template <size_t keySize>
    requires AESKeySize<keySize>
void GCM<keySize>::setAlgorithm(const Algorithm &algorithm) {
    CTR<AES<keySize>>::setAlgorithm(algorithm);
    setIV(rawIv);
}

template <size_t keySize>
    requires AESKeySize<keySize>
void GCM<keySize>::setIV(ByteSpan<> iv) {
    rawIv.resize(iv.size());
    std::copy(iv.begin(), iv.end(), rawIv.begin());

    if (iv.size() == PREFERRED_IV_SIZE) { // 96 bits iv
        std::copy(iv.begin(), iv.end(), j0.begin());
        std::fill(j0.begin() + PREFERRED_IV_SIZE, j0.end() - 1, std::byte(0));
        j0.back() = std::byte(1);
    } else {
        ByteArray<> paddedIv(iv.size());
        std::copy(iv.begin(), iv.end(), paddedIv.begin());
        size_t padding = (BLOCK_SIZE - iv.size() % BLOCK_SIZE) % BLOCK_SIZE;
        paddedIv.resize(paddedIv.size() + padding +  sizeof(uint64_t), std::byte(0));
        HalfTag length = toByteArray(toBitCount(iv.size()));
        paddedIv = concatenate(ByteSpan<>(paddedIv), ByteSpan<>(length));
        j0 = ghash(paddedIv);
    }
    
    auto bitset = toBitset(ByteSpan<BLOCK_SIZE>(j0));
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
    Tag         tag        = createTag(ciphertext);

    return concatenate(ByteSpan<>(ciphertext), ByteSpan<>(tag));
}

template <size_t keySize>
    requires AESKeySize<keySize>
ByteArray<> GCM<keySize>::decrypt(ByteSpan<> bytes) const {
    if (bytes.size() < BLOCK_SIZE) {
        throw TagMismatchException();
    }

    TagSpan    tag(bytes.end() - BLOCK_SIZE, BLOCK_SIZE);
    ByteSpan<> ciphertext(bytes.begin(), bytes.end() - BLOCK_SIZE);
    Tag        computedTag = createTag(ciphertext);

    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (tag[i] != computedTag[i]) {
            throw TagMismatchException();
        }
    }

    return  CTR<AES<keySize>>::decrypt(ciphertext);
}

template <size_t keySize>
    requires AESKeySize<keySize>
GCM<keySize>::Tag GCM<keySize>::getHashSubkey() const {
    typename Algorithm::Block onlyZeros;
    typename Algorithm::Block hashSubkey = this->algorithm.encrypt(onlyZeros);

    return toByteArray(hashSubkey);
}

template <size_t keySize>
    requires AESKeySize<keySize>
GCM<keySize>::IV GCM<keySize>::getEncryptedJ0() const {
    typename Algorithm::Block bitsetJ0    = toBitset(TagSpan(j0));
    typename Algorithm::Block encryptedJ0 = this->algorithm.encrypt(bitsetJ0);

    return toByteArray(encryptedJ0);
}

template <size_t keySize>
    requires AESKeySize<keySize>
GCM<keySize>::Tag GCM<keySize>::ghash(ByteSpan<> bytes, Tag initialTag) const {
    ByteArray<> copy(bytes.begin(), bytes.end());
    copy = ZeroPadding<Algorithm::BLOCK_SIZE>().pad(std::move(copy));

    const size_t blockCount = copy.size() / BLOCK_SIZE;
    Tag hashSubkey = getHashSubkey();

    for (int i = 0; i < blockCount; i++) {
        const size_t blockIndex = i * BLOCK_SIZE;
        TagSpan currentBlock(copy.data() + blockIndex, BLOCK_SIZE);
        initialTag = TagSpan(initialTag) ^ currentBlock;
        initialTag = GF128::multiply(initialTag, hashSubkey);
    }

    return initialTag;
}

template <size_t keySize>
    requires AESKeySize<keySize>
GCM<keySize>::Tag GCM<keySize>::createTag(ByteSpan<> ciphertext) const {
    Tag     aadTag           = ghash(aad);
    Tag     ciphertextTag    = ghash(ciphertext, aadTag);
    HalfTag cipherTextLength = toByteArray<uint64_t>(toBitCount(ciphertext.size()));
    HalfTag aadLength        = toByteArray<uint64_t>(toBitCount(aad.size()));
    Tag     lengths          = concatenate(HalfTagSpan(aadLength), HalfTagSpan(cipherTextLength));
    Tag     lengthsTag       = ghash(lengths, ciphertextTag);
    IV      encryptedJ0      = getEncryptedJ0();
    Tag     tag              = operator^(TagSpan(lengthsTag), TagSpan(encryptedJ0));

    return tag;
}
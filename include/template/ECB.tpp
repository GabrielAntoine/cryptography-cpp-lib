#pragma once

#include <cassert>

#include "ECB.h"


template <BlockCipherAlgorithm TAlgorithm>
void ECB<TAlgorithm>::setAlgorithm(TAlgorithm algorithm) {
    this->algorithm = algorithm;
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> ECB<TAlgorithm>::run(ByteSpan<> plainText, const bool encrypt) const {
    constexpr size_t blockSize = (TAlgorithm::BLOCK_SIZE / CHAR_BIT);
    const size_t blockCount = plainText.size() / blockSize;

    ByteArray<> encrypted(plainText.size());

    // Chose between encryption and decryption (method pointer)
    auto encryptOrDecrypt = encrypt ? &TAlgorithm::encrypt : &TAlgorithm::decrypt;

    for (size_t i = 0; i < blockCount; i++) {
        const size_t               byteIndex       = i * blockSize;
        ByteSpan<blockSize>        currentBlock    = ByteSpan<blockSize>(plainText.subspan(byteIndex, blockSize));
        typename TAlgorithm::Block currentBitset   = toBitset(currentBlock);
        typename TAlgorithm::Block encryptedBitset = (algorithm.*encryptOrDecrypt)(currentBitset);
        ByteArray<blockSize>       encryptedBlock  = toByteArray(encryptedBitset);
        
        std::copy(encryptedBlock.begin(), encryptedBlock.end(), encrypted.begin() + byteIndex);
    }

    return encrypted;
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> ECB<TAlgorithm>::encrypt(ByteSpan<> plainText) const {
   return run(plainText, true);
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> ECB<TAlgorithm>::decrypt(ByteSpan<> plainText) const {
    return run(plainText, false);
}
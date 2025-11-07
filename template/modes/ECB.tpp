#pragma once

#include <cassert>

#include "ECB.h"


template <BlockCipherAlgorithm TAlgorithm>
void ECB<TAlgorithm>::setAlgorithm(TAlgorithm algorithm) {
    this->algorithm = algorithm;
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> ECB<TAlgorithm>::encrypt(ByteSpan<> plainText) const {
    return mapForEachBlock<TAlgorithm::BLOCK_SIZE>(plainText, [this](const TAlgorithm::Block &currentBitset) {
        return algorithm.encrypt(currentBitset);
    });
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> ECB<TAlgorithm>::decrypt(ByteSpan<> plainText) const {
    return mapForEachBlock<TAlgorithm::BLOCK_SIZE>(plainText, [this](const TAlgorithm::Block &currentBitset) {
        return algorithm.decrypt(currentBitset);
    });
}
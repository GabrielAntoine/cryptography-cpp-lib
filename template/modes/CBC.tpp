#include "CBC.h"

#include <iostream>

template <BlockCipherAlgorithm TAlgorithm>
void CBC<TAlgorithm>::setAlgorithm(const Algorithm &algorithm) {
    this->algorithm = algorithm;
}

template <BlockCipherAlgorithm TAlgorithm>
void CBC<TAlgorithm>::setIV(IVSpan iv) {
    std::copy(iv.begin(), iv.end(), this->iv.begin());
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> CBC<TAlgorithm>::encrypt(ByteSpan<> bytes) const {
    typename TAlgorithm::Block currentIv = toBitset(IVSpan(iv));

    return mapForEachBlock<TAlgorithm::BLOCK_SIZE>(bytes, [this, &currentIv](TAlgorithm::Block currentBitset) {
        typename TAlgorithm::Block encryptedBitset = algorithm.encrypt(currentBitset ^ currentIv);
        currentIv = encryptedBitset;

        return encryptedBitset;
    });
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> CBC<TAlgorithm>::decrypt(ByteSpan<> bytes) const {
    typename TAlgorithm::Block currentIv = toBitset(IVSpan(iv));

    return mapForEachBlock<TAlgorithm::BLOCK_SIZE>(bytes, [this, &currentIv](TAlgorithm::Block currentBitset) {
        typename TAlgorithm::Block decryptedBitset = algorithm.decrypt(currentBitset) ^ currentIv;
        currentIv = currentBitset;

        return decryptedBitset;
    });
}

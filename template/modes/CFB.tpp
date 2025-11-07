#include "CFB.h"
#include "bytes_stream.h"

#include <iostream>

template <BlockCipherAlgorithm TAlgorithm>
void CFB<TAlgorithm>::setAlgorithm(const Algorithm &algorithm) {
    this->algorithm = algorithm;
}

template <BlockCipherAlgorithm TAlgorithm>
void CFB<TAlgorithm>::setIV(IVSpan iv) {
    std::copy(iv.begin(), iv.end(), this->iv.begin());
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> CFB<TAlgorithm>::encrypt(ByteSpan<> bytes) const {
    typename TAlgorithm::Block currentIv = toBitset(IVSpan(iv));

    auto cfbTransformation = [this, &currentIv](TAlgorithm::Block currentBlock) {
        typename TAlgorithm::Block encrypted = algorithm.encrypt(currentIv) ^ currentBlock;
        currentIv = encrypted;

        return encrypted;
    };
    
    return mapForEachBlock<TAlgorithm::BLOCK_SIZE>(bytes, cfbTransformation, cfbTransformation);
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> CFB<TAlgorithm>::decrypt(ByteSpan<> bytes) const {
    typename TAlgorithm::Block currentIv = toBitset(IVSpan(iv));

    auto cfbTransformation = [this, &currentIv](TAlgorithm::Block currentBlock) {
        typename TAlgorithm::Block decrypted = algorithm.encrypt(currentIv) ^ currentBlock;
        currentIv = currentBlock;

        return decrypted;
    };
    
    return mapForEachBlock<TAlgorithm::BLOCK_SIZE>(bytes, cfbTransformation, cfbTransformation);
}

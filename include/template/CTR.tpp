#include "CTR.h"
#include <iostream>

template <BlockCipherAlgorithm TAlgorithm>
void CTR<TAlgorithm>::setAlgorithm(const Algorithm &algorithm) {
    this->algorithm = algorithm;
}

template <BlockCipherAlgorithm TAlgorithm>
void CTR<TAlgorithm>::setIV(IVSpan iv) {
    std::copy(iv.begin(), iv.end(), this->iv.begin());
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> CTR<TAlgorithm>::encrypt(ByteSpan<> bytes) const {
    typename TAlgorithm::Block currentIv = toBitset(IVSpan(iv));

    auto ctrTransformation = [this, &currentIv](TAlgorithm::Block currentBlock) {
        std::clog << "iv = " << currentIv << std::endl;

        typename TAlgorithm::Block encrypted = algorithm.encrypt(currentIv) ^ currentBlock;
        increment(currentIv);

        return encrypted;
    };
    
    return mapForEachBlock<TAlgorithm::BLOCK_SIZE>(bytes, ctrTransformation, ctrTransformation);
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> CTR<TAlgorithm>::decrypt(ByteSpan<> bytes) const {
    return encrypt(bytes);
}

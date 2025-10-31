#include "OFB.h"

template <BlockCipherAlgorithm TAlgorithm>
void OFB<TAlgorithm>::setAlgorithm(const Algorithm &algorithm) {
    this->algorithm = algorithm;
}

template <BlockCipherAlgorithm TAlgorithm>
void OFB<TAlgorithm>::setIV(IVSpan iv) {
    std::copy(iv.begin(), iv.end(), this->iv.begin());
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> OFB<TAlgorithm>::encrypt(ByteSpan<> bytes) const {
    typename TAlgorithm::Block currentIv = toBitset(IVSpan(iv));

    auto ofbTransformation = [this, &currentIv](TAlgorithm::Block currentBlock) {
        typename TAlgorithm::Block encryptedIv = algorithm.encrypt(currentIv);
        typename TAlgorithm::Block encrypted = encryptedIv ^ currentBlock;
        currentIv = encryptedIv;

        return encrypted;
    };
    
    return mapForEachBlock<TAlgorithm::BLOCK_SIZE>(bytes, ofbTransformation, ofbTransformation);
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> OFB<TAlgorithm>::decrypt(ByteSpan<> bytes) const {
    return encrypt(bytes);
}

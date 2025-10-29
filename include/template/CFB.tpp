#include "CFB.h"

#include <iostream>

template <BlockCipherAlgorithm TAlgorithm>
void CFB<TAlgorithm>::setAlgorithm(const Algorithm &algorithm) {
    this->algorithm = algorithm;
}

template <BlockCipherAlgorithm TAlgorithm>
void CFB<TAlgorithm>::setIV(IVSpan iv) {
    std::copy(iv.begin(), iv.end(), this->iv.begin());
}

// encrypt and decrypt are very similar but I think it's still worth the readability.
// The only difference is the order of the lines encrypt/decrypt and xor with iv.
// Note : I tried to avoid the redundancy but it was way too difficult. 

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> CFB<TAlgorithm>::encrypt(ByteSpan<> bytes) const {
    constexpr size_t blockSize = toByteCount(Algorithm::BLOCK_SIZE);
    const size_t blockCount = bytes.size() / blockSize;

    ByteArray<> encrypted(bytes.size());

    IVSpan currentIv(iv);
    for (int i = 0; i < blockCount; i++) {
        const size_t               byteIndex       = i * blockSize;
        ByteSpan<blockSize>        currentBlock    = ByteSpan<blockSize>(bytes.subspan(byteIndex, blockSize));
        typename TAlgorithm::Block currentBitset   = toBitset(currentBlock);
        
        typename TAlgorithm::Block ivBitset        = toBitset(currentIv);
        typename TAlgorithm::Block encryptedIv     = algorithm.encrypt(ivBitset);
        typename TAlgorithm::Block encryptedBitset = currentBitset ^ encryptedIv;
        
        ByteArray<blockSize>       encryptedBlock  = toByteArray(encryptedBitset);
        currentIv = encryptedBlock;
        
        std::copy(encryptedBlock.begin(), encryptedBlock.end(), encrypted.begin() + byteIndex);
    }

    return encrypted;
}

template <BlockCipherAlgorithm TAlgorithm>
ByteArray<> CFB<TAlgorithm>::decrypt(ByteSpan<> bytes) const {
    constexpr size_t blockSize = toByteCount(Algorithm::BLOCK_SIZE);
    const size_t blockCount = bytes.size() / blockSize;

    ByteArray<> decrypted(bytes.size());

    IVSpan currentIv(iv);
    for (int i = 0; i < blockCount; i++) {
        const size_t               byteIndex       = i * blockSize;
        ByteSpan<blockSize>        currentBlock    = ByteSpan<blockSize>(bytes.subspan(byteIndex, blockSize));
        typename TAlgorithm::Block currentBitset   = toBitset(currentBlock);
        typename TAlgorithm::Block ivBitset        = toBitset(currentIv);
        typename TAlgorithm::Block encryptedIv     = algorithm.encrypt(ivBitset);
        typename TAlgorithm::Block decryptedBitset = encryptedIv ^ currentBitset;
        ByteArray<blockSize>       decryptedBlock  = toByteArray(decryptedBitset);

        currentIv = currentBlock;
        
        std::copy(decryptedBlock.begin(), decryptedBlock.end(), decrypted.begin() + byteIndex);
    }

    return decrypted;
}

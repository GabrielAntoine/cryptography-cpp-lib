#pragma once

#include <utility>

#include "BlockCipher.h"

template<BlockCipherMode TMode, BlockCipherPadding TPadding>
BlockCipher<TMode, TPadding>::BlockCipher(const TMode::Algorithm &algorithm, const TMode &mode, const TPadding &padding)
    : algorithm(algorithm), mode(mode), padding(padding) {
    this->mode.setAlgorithm(algorithm);
}

template <BlockCipherMode TMode, BlockCipherPadding TPadding>
ByteArray<> BlockCipher<TMode, TPadding>::encrypt(ByteSpan<> bytes) const {
    ByteArray<> copy(bytes.begin(), bytes.end());
    ByteArray<> padded    = padding.pad(std::move(copy)); // 'padded' and 'copy' points logically to the same memory thanks to std::move
    ByteArray<> encrypted = mode.encrypt(padded);
    
    return encrypted;
}

template<BlockCipherMode TMode, BlockCipherPadding TPadding>
ByteArray<> BlockCipher<TMode, TPadding>::decrypt(ByteSpan<> bytes) const {
    ByteArray<> decrypted = mode.decrypt(bytes);
    ByteArray<> unpadded  = padding.unpad(std::move(decrypted));

    return unpadded;
}
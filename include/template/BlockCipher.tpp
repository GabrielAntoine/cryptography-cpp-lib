#pragma once


#include "BlockCipher.h"

template<BlockCipherMode TMode, BlockCipherPadding TPadding>
BlockCipher<TMode, TPadding>::BlockCipher(const TMode::Algorithm &algorithm, const TMode &mode, const TPadding &padding)
    : algorithm(algorithm), mode(mode), padding(padding) {
    this->mode.setAlgorithm(algorithm);
}

template <BlockCipherMode TMode, BlockCipherPadding TPadding>
ByteArray BlockCipher<TMode, TPadding>::encrypt(ByteSpan<> bytes) const {
    ByteArray copy(bytes.begin(), bytes.end());
    padding.pushPadding(copy);
    return mode.encrypt(copy);
}

template<BlockCipherMode TMode, BlockCipherPadding TPadding>
ByteArray BlockCipher<TMode, TPadding>::decrypt(ByteSpan<> bytes) const {
    ByteArray decrypted = mode.decrypt(bytes);
    padding.popPadding(decrypted);
    return decrypted;
}
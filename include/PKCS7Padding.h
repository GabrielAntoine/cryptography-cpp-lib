#pragma once

#include "bytes.h"
#include "BlockCipherPadding.h"
#include "DES.h"

template <size_t blockSize>
class PKCS7Padding {

public:
    static constexpr size_t BLOCK_SIZE       = blockSize;
    static constexpr size_t BLOCK_SIZE_BYTES = toByteCount(blockSize);

    ByteArray<> pad(ByteArray<> bytes) const;
    ByteArray<> unpad(ByteArray<> bytes) const;
};

static_assert(BlockCipherPadding<PKCS7Padding<128>>);

#include "PKCS7Padding.tpp"
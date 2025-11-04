#pragma once

#include "bytes.h"
#include "BlockCipherPadding.h"

template <size_t blockSize>
class ZeroPadding {

public:
    static constexpr size_t BLOCK_SIZE       = blockSize;
    static constexpr size_t BLOCK_SIZE_BYTES = toByteCount(blockSize);

    ByteArray<> pad(ByteArray<> bytes) const;
    ByteArray<> unpad(ByteArray<> bytes) const;

    static_assert(BlockCipherPadding<ZeroPadding<blockSize>>);
};


#include "ZeroPadding.tpp"
#include "ZeroPadding.h"

template <size_t blockSize>
ByteArray<> ZeroPadding<blockSize>::pad(ByteArray<> bytes) const {
    uint8_t padding = BLOCK_SIZE_BYTES - bytes.size() % BLOCK_SIZE_BYTES;

    if (padding == BLOCK_SIZE_BYTES) {
        return bytes;
    }

    bytes.resize(bytes.size() + padding, std::byte(0));

    return bytes;
}

template <size_t blockSize>
ByteArray<> ZeroPadding<blockSize>::unpad(ByteArray<> bytes) const {
    int ZeroCount = 0;

    for (int i = 1; i <= BLOCK_SIZE_BYTES; i++) {
        if (*(bytes.end() - i) == std::byte(0)) {
            ZeroCount++;
        } else {
            break;
        }
    }

    bytes.resize(bytes.size() - ZeroCount);

    return bytes;
}


#include "PKCS7Padding.h"

template <size_t blockSize>
ByteArray<> PKCS7Padding<blockSize>::pad(ByteArray<> bytes) const {
    // Determines the integer n which is repeated n times to pad
    uint8_t padding = BLOCK_SIZE_BYTES - bytes.size() % BLOCK_SIZE_BYTES;

    // Adds an extra block if no padding is needed
    if (padding == 0) {
        padding = BLOCK_SIZE_BYTES;
    }

    bytes.resize(bytes.size() + padding, std::byte(padding));

    return bytes;
}

template <size_t blockSize>
ByteArray<> PKCS7Padding<blockSize>::unpad(ByteArray<> bytes) const {
    // The last byte determines how many bytes needs to be unpad.
    // Ex: if the last byte is the integer '5', then the function will remove 5 bytes from the right.
    const uint8_t padding = std::to_integer<uint8_t>(bytes.back());
    bytes.resize(bytes.size() - padding);

    return bytes;
}


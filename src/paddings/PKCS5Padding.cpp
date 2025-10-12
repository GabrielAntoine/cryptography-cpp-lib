#include "PKCS5Padding.h"

void PKCS5Padding::pushPadding(ByteArray &bytes) const {
    uint8_t padding = DES::BLOCK_SIZE / CHAR_BIT - (bytes.size() % (DES::BLOCK_SIZE / CHAR_BIT));

    // Add an extra block if no padding is needed
    if (padding == 0) {
        padding = 8;
    }

    bytes.resize(bytes.size() + padding, std::byte(padding));
}

void PKCS5Padding::popPadding(ByteArray &bytes) const {
    const uint8_t padding = std::to_integer<uint8_t>(bytes.back());
    bytes.resize(bytes.size() - padding);
}


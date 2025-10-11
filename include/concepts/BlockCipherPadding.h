#include "bytes.h"

template <typename TPadding, size_t bitsSize, size_t blockSize>
concept BlockCipherPadding = requires(TPadding padding, std::bitset<bitsSize> bytes) {
    { padding.pushPadding(bytes, blockSize) } -> std::same_as<std::bitset<blockSize>>;
};
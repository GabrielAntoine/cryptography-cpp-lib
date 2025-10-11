#include "bytes.h"

template <typename TMode, typename TAlgorithm, size_t bitsSize>
concept BlockCipherMode = requires(TMode mode, std::bitset<bitsSize> bytes, TAlgorithm algorithm) {
    { mode.setAlgorithm(algorithm) } -> std::same_as<void>;
    { mode.encrypt(bytes)          } -> std::same_as<std::bitset<bitsSize>>;
    { mode.decrypt(bytes)          } -> std::same_as<std::bitset<bitsSize>>;
};
#pragma once

#include "bytes.h"
#include "Key.h"

// template <typename TAlgorithm, typename TKey, size_t bitsSize>
// concept CipherAlgorithm = requires(TAlgorithm algorithm, std::bitset<bitsSize> bytes, TKey key) {
//     requires Key<TKey>;

//     { algorithm.encrypt(bytes) } -> std::same_as<std::bitset<bitsSize>>;
//     { algorithm.decrypt(bytes) } -> std::same_as<std::bitset<bitsSize>>;
//     { algorithm.setKey(key)    } -> std::same_as<void>;
// };
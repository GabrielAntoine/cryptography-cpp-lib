#pragma once

#include "bytes.h"
#include "Key.h"
// #include "CipherAlgorithm.h"

template <typename TAlgorithm>
concept BlockCipherAlgorithm =
    requires {
        { TAlgorithm::BLOCK_SIZE } -> std::convertible_to<size_t>;
        typename TAlgorithm::Block;
        typename TAlgorithm::SecretKey;
    } &&
    std::same_as<typename TAlgorithm::Block, std::bitset<TAlgorithm::BLOCK_SIZE>> &&
    requires(
        TAlgorithm algorithm,
        std::bitset<TAlgorithm::BLOCK_SIZE> bytes,
        typename TAlgorithm::SecretKey key
    ) {
        requires Key<typename TAlgorithm::SecretKey>;

        { algorithm.encrypt(bytes) } -> std::same_as<typename TAlgorithm::Block>;
        { algorithm.decrypt(bytes) } -> std::same_as<typename TAlgorithm::Block>;
        { algorithm.setKey(key)    } -> std::same_as<void>;
    };
#pragma once

#include "bytes.h"

template <typename TPadding>
concept BlockCipherPadding = requires(TPadding paddingAlgorithm, ByteArray<> bytes) {
    { paddingAlgorithm.pad(bytes)   } -> std::same_as<ByteArray<>>;
    { paddingAlgorithm.unpad(bytes) } -> std::same_as<ByteArray<>>;
};
#pragma once

#include "bytes.h"
#include "BlockCipherAlgorithm.h"

template <typename TMode>
concept BlockCipherMode = requires(TMode mode, ByteSpan<> bytes, typename TMode::Algorithm algorithm) {
    requires BlockCipherAlgorithm<typename TMode::Algorithm>;

    { mode.setAlgorithm(algorithm) } -> std::same_as<void>;
    { mode.encrypt(bytes)          } -> std::same_as<ByteArray<>>;
    { mode.decrypt(bytes)          } -> std::same_as<ByteArray<>>;
};

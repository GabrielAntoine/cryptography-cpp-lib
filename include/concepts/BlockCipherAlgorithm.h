#include "bytes.h"
#include "Key.h"
#include "CipherAlgorithm.h"

template <typename TAlgorithm, typename TKey>
concept BlockCipherAlgorithm = CipherAlgorithm<TAlgorithm, TKey, TAlgorithm::BLOCK_SIZE> && requires {
    { TAlgorithm::BLOCK_SIZE } -> std::convertible_to<size_t>;
};
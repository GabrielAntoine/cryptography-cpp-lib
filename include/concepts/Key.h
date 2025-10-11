#include <concepts>

#include "bytes.h"

template <typename T, size_t size>
concept Key = requires(T t) {
    { t.getBytes() } -> std::same_as<ByteArray<size>>;
};
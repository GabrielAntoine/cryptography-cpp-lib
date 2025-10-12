#pragma once

#include <concepts>

#include "bytes.h"

template <typename T>
concept Key = requires(T t) {
    { t.getBytes() } -> std::same_as<ByteArray>;
};
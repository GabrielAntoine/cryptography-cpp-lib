#include <array>
#include <cstddef>
#include <functional>
#include <cstdint>
#include <span>
#include <vector>
#include <algorithm>

#include "bytes.h"



// // ===================================================================================
// // SECTION : Utils
// // ===================================================================================

ByteArray<> toByteArray(const size_t n) {
    ByteArray<> output(sizeof n);

    for (int i = sizeof n - 1; i >= 0; i--) {
        output[sizeof n - i - 1] = std::byte((n >> (CHAR_BIT * i)) & UINT8_MAX);
    }

    return output;
}
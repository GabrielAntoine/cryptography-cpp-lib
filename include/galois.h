#include "bytes.h"


#include <cstddef>

namespace GF8 {

std::byte multiplyBy2(std::byte byte);
std::byte multiply(std::byte byte, std::byte factor);

}

namespace GF128 {

static constexpr uint8_t SIZE = 16;

ByteArray<SIZE> multiply(ByteSpan<SIZE> left, ByteSpan<SIZE> right);

}
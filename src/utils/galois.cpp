#include "galois.h"

#include <cstdint>
#include <climits>

namespace Galois256 {

std::byte multiplyBy2(std::byte byte) {
    constexpr std::byte msb = std::byte(0x80);
    constexpr std::byte irreductiblePolynomial = std::byte(0x1B);

    if ((byte & msb) != msb) {
        return byte << 1;
    } else {
        return (byte << 1) ^ irreductiblePolynomial;
    }
}

std::byte multiply(std::byte byte, std::byte factor) {
    if (factor == std::byte(2)) {
        return multiplyBy2(byte);
    }

    std::byte result{0};
    std::byte term{factor};

    for (uint8_t i = 0; i < CHAR_BIT; i++) {
        const bool hasBit = ((byte >> i) & std::byte(1)) == std::byte(1);

        if (hasBit) {
            result ^= term;
        }

        term = multiplyBy2(term);
    }

    return result;
}

}
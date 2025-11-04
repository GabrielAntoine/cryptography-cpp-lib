#include "galois.h"

#include <cstdint>
#include <climits>

namespace GF8 {

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

namespace GF128 {

// No idea how that works. You can thank ChatGPT though.
ByteArray<SIZE> multiply(ByteSpan<SIZE> left, ByteSpan<SIZE> right) {
    constexpr std::byte msb  = std::byte(0x80);
    constexpr std::byte one  = std::byte(0x01);
    constexpr std::byte zero = std::byte(0x00);
    constexpr std::byte irreductiblePolynomial = std::byte(0xE1);

    ByteArray<SIZE> output{zero}, tmp;
    std::copy(left.begin(), left.end(), tmp.begin());

    for (int i = 0; i < toBitCount(SIZE); i++) {
        std::byte currentBit = right[toByteCount(i)] & std::byte(msb >> (i % CHAR_BIT));
        if (currentBit != zero) {
            output = ByteSpan<SIZE>(output) ^ ByteSpan<SIZE>(tmp);
        }

        std::byte lsb = tmp[SIZE - 1] & one;
        for (int j = SIZE - 1; j > 0; j--) {
            tmp[j] = (tmp[j] >> 1) | ((tmp[j - 1] & one) << 7);
        }

        tmp.front() >>= 1;
        if (lsb == one) {
            tmp.front() ^= irreductiblePolynomial;
        } 
    }

    return output;
}

}
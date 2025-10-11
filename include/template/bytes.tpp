#include <cstddef>

#include "bytes.h"

template<size_t size>
std::bitset<size * CHAR_BIT> toBitset(const ByteSpan<size> &bytes) {
    std::bitset<size * CHAR_BIT> bitset;

    for (const auto byte : bytes) {
        bitset = (bitset << CHAR_BIT) | std::bitset<size * CHAR_BIT>(std::to_integer<uint8_t>(byte));
    }

    return bitset;
}

template<size_t outputSize, size_t inputSize>
std::bitset<outputSize> sliceBitset(const std::bitset<inputSize> &input, const size_t offset) {
    std::bitset<outputSize> output;

    for (size_t i = 0; i < outputSize; i++) {
        output[i] = input[i + offset];
    }

    return output;
}

template<size_t difference, size_t inputSize>
std::bitset<inputSize + difference> extendBitset(const std::bitset<inputSize> &input) {
    std::bitset<inputSize + difference> output;

    for (size_t i = 0; i < inputSize; i++) {
        output[i] = input[i];
    }

    return output;
}

template<size_t size>
std::bitset<size> rotl(const std::bitset<size> &bitset, const size_t shift) {
    const size_t localShift = shift % size;

    if (localShift == 0) {
        return bitset;
    }

    return (bitset << localShift) | (bitset >> (size - localShift));
}

template<size_t size>
std::bitset<size> rotr(const std::bitset<size> &bitset, const size_t shift) {
    const size_t localShift = shift % size;

    if (localShift == 0) {
        return bitset;
    }

    return (bitset << localShift) | (bitset << (size - localShift));
}

template<size_t bitsCount>
ByteArray toByteArray(const std::bitset<bitsCount> &bitset) {
    constexpr size_t bytesCount = bitsCount / CHAR_BIT;
    ByteArray output(bytesCount);

    for (int i = 0; i < bytesCount; i++) {
        uint8_t byte = ((bitset >> ((bytesCount - 1 - i) * CHAR_BIT)) & std::bitset<bitsCount>(UINT8_MAX)).to_ulong();
        output[i] = std::byte(byte);
    }

    return output;
}
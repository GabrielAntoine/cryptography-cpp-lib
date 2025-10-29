#pragma once

#include <cstddef>
#include <cassert>
#include <stdexcept>

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
ByteArray<toByteCount(bitsCount)> toByteArray(const std::bitset<bitsCount> &bitset) {
    constexpr size_t bytesCount = toByteCount(bitsCount);
    ByteArray<bytesCount> output;

    for (int i = 0; i < bytesCount; i++) {
        uint8_t byte = ((bitset >> ((bytesCount - 1 - i) * CHAR_BIT)) & std::bitset<bitsCount>(UINT8_MAX)).to_ulong();
        output[i] = std::byte(byte);
    }

    return output;
}

template<size_t charSize>
ByteArray<(charSize - 1) / 2> toByteArray(const char (&hexa)[charSize]) {
    static_assert((charSize - 1) % 2 == 0); // Because one byte is two hexa digits
    static constexpr size_t byteCount = (charSize - 1) / 2;
    static constexpr auto hexaToInt = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        else {
            throw std::invalid_argument("Invalid hexa digit");
        }
    };

    ByteArray<byteCount> output;

    for (size_t i = 0; i < charSize - 1; i += 2) {
        output[i / 2] = std::byte((hexaToInt(hexa[i]) << 4)  + hexaToInt(hexa[i + 1]));
    }

    return output;
}

template<size_t charSize>
ByteArray<charSize - 1> toByteArrayFromAscii(const char(&ascii)[charSize]) {
    ByteArray<charSize - 1> output;

    for (int i = 0; i < charSize; i++) {
        output[i] = std::byte(ascii[i]);
    }

    return output;
}

template<size_t bitsCount>
ByteArray<> toDynamicByteArray(const std::bitset<bitsCount> &bitset) {
    constexpr size_t bytesCount = toByteCount(bitsCount);
    ByteArray<> output(bytesCount);

    for (int i = 0; i < bytesCount; i++) {
        uint8_t byte = ((bitset >> ((bytesCount - 1 - i) * CHAR_BIT)) & std::bitset<bitsCount>(UINT8_MAX)).to_ulong();
        output[i] = std::byte(byte);
    }

    return output;
}
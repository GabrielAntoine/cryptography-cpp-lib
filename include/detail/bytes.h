// This file provides the alias type "ByteArray<N>" which is equivalent to std::array<std::byte, N>.
// Then, it adds all the bitwise operators such as &, |, ^, ~, <<, >>, rotr, rotl and maybe more.
// The operations on two ByteArray work so that the output array will have the size of the smallest array.

#include <array>
#include <cstddef>
#include <span>
#include <concepts>
#include <functional>
#include <iostream>
#include <bitset>
#include <cstdint>

template<size_t size>
using ByteArray = std::array<std::byte, size>;

// Additionnal operators to display the bytes on screen

std::ostream& operator<<(std::ostream& os, std::byte b) {
    return os << std::bitset<8>(std::to_integer<int>(b));
}

template <size_t size>
std::ostream& operator<<(std::ostream& os, const ByteArray<size> &bytes) {
    for (auto byte : bytes) {
        os << byte << '.';
    }

    return os;
}

// AND, OR, XOR, NOT

template <size_t leftSize, size_t rightSize>
ByteArray<std::min(leftSize, rightSize)> doBytesToBytesOperation(
    const ByteArray<leftSize>      &left, 
    const ByteArray<rightSize>     &right, 
    std::function<std::byte(std::byte, std::byte)> doByteToByteOperation
) {
    constexpr size_t outputSize = std::min(leftSize, rightSize);
    ByteArray<outputSize> output;

    for (int i = outputSize - 1; i >= 0; i--) {
        output[i] = doByteToByteOperation(left[i], right[i]);
    }

    return output;
}

template <size_t leftSize, size_t rightSize>
ByteArray<std::min(leftSize, rightSize)> operator&(const ByteArray<leftSize> &left, const ByteArray<rightSize> &right) {
    return doBytesToBytesOperation(left, right, std::bit_and<std::byte>());
}

template <size_t leftSize, size_t rightSize>
ByteArray<std::min(leftSize, rightSize)> operator|(const ByteArray<leftSize> &left, const ByteArray<rightSize> &right) {
    return doBytesToBytesOperation(left, right, std::bit_or<std::byte>());
}

template <size_t leftSize, size_t rightSize>
ByteArray<std::min(leftSize, rightSize)> operator^(const ByteArray<leftSize> &left, const ByteArray<rightSize> &right) {
    return doBytesToBytesOperation(left, right, std::bit_xor<std::byte>());
}

template <size_t size>
ByteArray<size> operator~(const ByteArray<size> &bytes) {
    ByteArray<size> output;

    std::transform(bytes.begin(), bytes.end(), output.begin(), std::bit_not());

    return output;
}

template <size_t size>
ByteArray<size>& operator&=(ByteArray<size> &left, const ByteArray<size> &right) {
    return left = left & right;
}

template <size_t size>
ByteArray<size>& operator|=(ByteArray<size> &left, const ByteArray<size> &right) {
    return left = left | right;
}

template <size_t size>
ByteArray<size>& operator^=(ByteArray<size> &left, const ByteArray<size> &right) {
    return left = left ^ right;
}

// LEFT SHIFT, RIGHT SHIFT, LEFT CIRCULAR SHIFT, RIGHT CIRCULAR SHIFT

constexpr uint8_t bitsPerByte = 8;

template <size_t size, typename IntegerType>
ByteArray<size> operator<<(const ByteArray<size> &bytes, const IntegerType shift) {

    const std::uint8_t bitsInTheLeftBlock  = shift % bitsPerByte;
    const std::uint8_t bitsInTheRightBlock = bitsPerByte - bitsInTheLeftBlock;
    const IntegerType  blockShift          = shift / bitsPerByte;
    ByteArray<size> output{};

    for (int i = 0; i < size; i++) {
        std::byte block        = bytes[i];
        std::byte leftBlock    = block >> bitsInTheRightBlock;
        std::byte rightBlock   = block << bitsInTheLeftBlock;
        size_t rightBlockIndex = i - blockShift;
        size_t leftBlockIndex  = rightBlockIndex - 1;

        if (leftBlockIndex >= 0) {
            output[leftBlockIndex] |= leftBlock;
        }
        if (rightBlockIndex >= 0) {
            output[rightBlockIndex] |= rightBlock;
        }
    }

    return output;
}

template <size_t size, typename IntegerType>
ByteArray<size> operator>>(const ByteArray<size> &bytes, const IntegerType shift) {

    const std::uint8_t bitsInTheRightBlock = shift % bitsPerByte;
    const std::uint8_t bitsInTheLeftBlock  = bitsPerByte - bitsInTheRightBlock;
    const IntegerType  blockShift          = shift / bitsPerByte; 
    ByteArray<size> output{};

    for (int i = size - 1; i >= 0; i--) {
        std::byte block        = bytes[i];
        std::byte leftBlock    = block >> bitsInTheRightBlock;
        std::byte rightBlock   = block << bitsInTheLeftBlock;
        size_t leftBlockIndex  = i + blockShift;
        size_t rightBlockIndex = leftBlockIndex + 1;

        if (leftBlockIndex < size) {
            output[leftBlockIndex] |= leftBlock;
        }
        if (rightBlockIndex < size) {
            output[rightBlockIndex] |= rightBlock;
        }
    }

    return output;
}

template <size_t size, typename IntegerType>
ByteArray<size>& operator<<=(ByteArray<size> &bytes, const IntegerType shift) {
    return bytes = bytes << shift;
}

template <size_t size, typename IntegerType>
ByteArray<size>& operator>>=(ByteArray<size> &bytes, const IntegerType shift) {
    return bytes = bytes >> shift;
}

template <size_t size, typename IntegerType>
ByteArray<size> rotr(const ByteArray<size> &bytes, const IntegerType shift);

template <size_t size, typename IntegerType>
ByteArray<size> rotl(const ByteArray<size> &bytes, const IntegerType shift) {
    const size_t bitsCount = size * bitsPerByte;
    const IntegerType localShift = shift % bitsCount;

    if (localShift == 0) {
        return bytes;
    } else if (localShift > 0) {
        return (bytes << localShift) | (bytes >> (bitsCount - localShift));
    } else {
        return rotr(bytes, -localShift);
    }
}

template <size_t size, typename IntegerType>
ByteArray<size> rotr(const ByteArray<size> &bytes, const IntegerType shift) {
    const size_t bitsCount = size * bitsPerByte;
    const IntegerType localShift = shift % bitsCount;

    if (localShift == 0) {
        return bytes;
    } else if (localShift > 0) {
        return (bytes >> localShift) | (bytes << (bitsCount - localShift));
    } else {
        return rotr(bytes, -localShift);
    }
}
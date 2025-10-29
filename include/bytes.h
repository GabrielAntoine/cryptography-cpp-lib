#pragma once

// This file provides the alias type "ByteArray" which is equivalent to std::vector<std::byte> or std::array (see below how).
// Then, it adds all the bitwise operators such as &, |, ^, ~, <<, >>, rotr, rotl and maybe more.
// The operations on two ByteArray work so that the output array will have the size of the smallest array.

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>
#include <bitset>
#include <type_traits>


template <size_t _Extent = std::dynamic_extent>
using ByteSpan = std::span<const std::byte, _Extent>;

// Uses a std::vector if no size specified, std::array otherwise
// Allows not to have two distinct types that have the 'same' meaning
template <size_t s = std::dynamic_extent>
using ByteArray = std::conditional_t<s == std::dynamic_extent, std::vector<std::byte>, std::array<std::byte, s>>;

// These two types are unused for now but should be used to securize padding algorithms
template <size_t blockSize>
using ByteBlockSpan = std::span<ByteArray<blockSize>>;

template <size_t blockSize>
using ByteBlockArray = std::vector<ByteArray<blockSize>>;

// Bitset function (TODO: move them to another file)

template<size_t size>
std::bitset<size * CHAR_BIT> toBitset(const ByteSpan<size> &bytes);

template<size_t bitsetSize>
std::bitset<bitsetSize> toBitset(ByteSpan<> &bytes);

template<size_t outputSize, size_t inputSize>
std::bitset<outputSize> sliceBitset(const std::bitset<inputSize> &input, const size_t offset = 0);

template<size_t difference, size_t inputSize>
std::bitset<inputSize + difference> extendBitset(const std::bitset<inputSize> &input);

template<size_t size>
std::bitset<size> rotl(const std::bitset<size> &bitset, const size_t shift);

template<size_t size>
std::bitset<size> rotr(const std::bitset<size> &bitset, const size_t shift);

ByteArray<> toByteArray(const size_t n);

constexpr size_t toByteCount(size_t bitCount) {
    return bitCount / CHAR_BIT;
}

constexpr size_t bitCountFromBytes(size_t byteCount) {
    return byteCount * 8;
}

template<size_t bitsCount>
ByteArray<toByteCount(bitsCount)> toByteArray(const std::bitset<bitsCount> &bitset);

// This function's only purpose is to easily create a ByteArray from a hexa string
// Trailing zeros are mandatory
template<size_t charSize>
ByteArray<(charSize - 1) / 2> toByteArray(const char (&hexa)[charSize]);

// This function's only purpose is to easily create a ByteArray from a ascii string
template<size_t charSize>
ByteArray<charSize - 1> toByteArrayFromAscii(const char(&ascii)[charSize]);

template<size_t bitsCount>
ByteArray<> toDynamicByteArray(const std::bitset<bitsCount> &bitset, size_t offset = 0, size_t size = toByteCount(bitsCount));

#include "bytes.tpp"
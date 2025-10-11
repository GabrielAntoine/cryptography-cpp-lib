#pragma once

// This file provides the alias type "ByteArray" which is equivalent to std::vector<std::byte>.
// Then, it adds all the bitwise operators such as &, |, ^, ~, <<, >>, rotr, rotl and maybe more.
// The operations on two ByteArray work so that the output array will have the size of the smallest array.

// TODO: Add arithmetic operations (if needed)

#include <array>
#include <cstddef>
#include <functional>
#include <cstdint>
#include <span>
#include <vector>
#include <algorithm>
#include <bitset>

using ByteArray = std::vector<std::byte>;
// using ByteToByteOperation = std::function<std::byte(std::byte, std::byte)>;

template <size_t _Extent = std::dynamic_extent>
using ByteSpan = std::span<std::byte, _Extent>;

// constexpr uint8_t BITS_PER_BYTE = 8;

template<size_t size>
std::bitset<size * CHAR_BIT> toBitset(const ByteSpan<size> &bytes);

template<size_t outputSize, size_t inputSize>
std::bitset<outputSize> sliceBitset(const std::bitset<inputSize> &input, const size_t offset = 0);

template<size_t difference, size_t inputSize>
std::bitset<inputSize + difference> extendBitset(const std::bitset<inputSize> &input);

template<size_t size>
std::bitset<size> rotl(const std::bitset<size> &bitset, const size_t shift);

template<size_t size>
std::bitset<size> rotr(const std::bitset<size> &bitset, const size_t shift);


// ByteArray &doBytesToBytesOperation(
//     ByteArray &left, 
//     const ByteArray &right, 
//     ByteToByteOperation doByteToByteOperation
// );

// ByteArray& operator&=(ByteArray &left, const ByteArray &right);
// ByteArray operator&(const ByteArray& left, const ByteArray &right);
// ByteArray operator&(const ByteArray& left, const size_t right);
// ByteArray& operator|=(ByteArray &left, const ByteArray &right);
// ByteArray operator|(const ByteArray& left, const ByteArray &right);
// ByteArray operator|(const ByteArray& left, const size_t right);
// ByteArray& operator^=(ByteArray &left, const ByteArray &right);
// ByteArray operator^(const ByteArray& left, const ByteArray &right);
// ByteArray operator^(const ByteArray& left, const size_t right);
// ByteArray operator~(const ByteArray &bytes);


// ByteArray operator<<(const ByteArray &bytes, const size_t shift);
// ByteArray operator>>(const ByteArray &bytes, const size_t shift);
// ByteArray& operator<<=(ByteArray &bytes, const size_t shift);
// ByteArray& operator>>=(ByteArray &bytes, const size_t shift);
// ByteArray rotl(const ByteArray &bytes, const size_t shift);
// ByteArray rotr(const ByteArray &bytes, const size_t shift);


// bool bitAt(const ByteArray &bytes, std::size_t index);
ByteArray toByteArray(const size_t n);

template<size_t bitsCount>
ByteArray toByteArray(const std::bitset<bitsCount> &bitset);

#include "bytes.tpp"
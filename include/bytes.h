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

using ByteArray = std::vector<std::byte>;
using ByteToByteOperation = std::function<std::byte(std::byte, std::byte)>;

constexpr uint8_t BITS_PER_BYTE = 8;



ByteArray &doBytesToBytesOperation(
    ByteArray &left, 
    const ByteArray &right, 
    ByteToByteOperation doByteToByteOperation
);

ByteArray& operator&=(ByteArray &left, const ByteArray &right);
ByteArray operator&(const ByteArray& left, const ByteArray &right);
ByteArray& operator|=(ByteArray &left, const ByteArray &right);
ByteArray operator|(const ByteArray& left, const ByteArray &right);
ByteArray& operator^=(ByteArray &left, const ByteArray &right);
ByteArray operator^(const ByteArray& left, const ByteArray &right);
ByteArray operator~(const ByteArray &bytes);


ByteArray operator<<(const ByteArray &bytes, const size_t shift);
ByteArray operator>>(const ByteArray &bytes, const size_t shift);
ByteArray& operator<<=(ByteArray &bytes, const size_t shift);
ByteArray& operator>>=(ByteArray &bytes, const size_t shift);
ByteArray rotl(const ByteArray &bytes, const size_t shift);
ByteArray rotr(const ByteArray &bytes, const size_t shift);


bool bitAt(const ByteArray &bytes, std::size_t index);
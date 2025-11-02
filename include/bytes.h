#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>
#include <bitset>
#include <type_traits>
#include <functional>

/**
 * 
 * Types
 * 
 * 
 */

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

using BitArray = std::vector<bool>;


/**
 * 
 * Constexpr functions
 * 
 * 
 */

constexpr size_t toByteCount(size_t bitCount) {
    return bitCount / CHAR_BIT;
}

constexpr size_t toBitCount(size_t byteCount) {
    return byteCount * CHAR_BIT;
}


/**
 * 
 * Functions to convert to std::bitset
 * 
 * 
 */

template<size_t size>
auto toBitset(const ByteSpan<size> &bytes) -> std::bitset<toBitCount(size)>;

template<size_t bitsetSize>
auto toBitset(ByteSpan<> &bytes) -> std::bitset<bitsetSize>;


/**
 * 
 * Functions to convert to static or dynamic ByteArray 
 * 
 * 
 */

auto toByteArray(const size_t n) -> ByteArray<>;

template<size_t bitsCount>
auto toByteArray(const std::bitset<bitsCount> &bitset) -> ByteArray<toByteCount(bitsCount)>;

template<size_t bitsCount>
auto toDynamicByteArray(const std::bitset<bitsCount> &bitset, size_t offset = 0, size_t size = toByteCount(bitsCount)) -> ByteArray<>;

// This function's only purpose is to easily create a ByteArray from a hexa string
// Trailing zeros are mandatory
template<size_t charSize>
auto toByteArrayFromHexa(const char (&hexa)[charSize]) -> ByteArray<(charSize - 1) / 2>;

// This function's only purpose is to easily create a ByteArray from a ascii string
template<size_t charSize>
auto toByteArrayFromAscii(const char(&ascii)[charSize]) -> ByteArray<charSize - 1>;

/**
 * 
 * Operators
 * 
 * 
 * 
 */

template <size_t size>
ByteArray<size> operator^(ByteSpan<size> a, ByteSpan<size> b);

template <size_t size>
ByteArray<size> rotl(ByteSpan<size> a, size_t shift);

/**
 * 
 * Other functions that definitily should go in another file
 * 
 * 
 * 
 */

// generic type that should be moved somewhere else
template <typename T>
concept Null = std::same_as<std::decay_t<T>, std::nullptr_t>;

template <typename Callback, size_t blockSize>
concept BlockMapper = requires(Callback f, const std::bitset<blockSize>& bytes) {
    { f(bytes) } -> std::same_as<std::bitset<blockSize>>;
};

template <typename Callback, size_t blockSize>
concept IncompleteBlockMapper = 
    Null<Callback> ||
    requires(Callback f, const std::bitset<blockSize>& bytes, size_t effectiveBitCount) {
        { f(bytes, effectiveBitCount) } -> std::same_as<std::bitset<blockSize>>;
    } ||
    BlockMapper<Callback, blockSize>;

template <size_t blockSize, BlockMapper<blockSize> _BlockMapper, IncompleteBlockMapper<blockSize> _IncompleteBlockMapper = std::nullptr_t>
ByteArray<> mapForEachBlock(
    ByteSpan<> bytes, 
    _BlockMapper transform, 
    _IncompleteBlockMapper transformRemaining = nullptr
);


#include "bytes.tpp"

// because the content from bits.h was written here before. 
// TODO: update the includes in every file so that this line can be removed
#include "bits.h"
#pragma once

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <iostream>

#include "bytes.h"
#include "bytes_stream.h"

// corriger le problème avec les auto 

template<size_t size>
std::bitset<toBitCount(size)> toBitset(const ByteSpan<size> &bytes) {
    std::bitset<size * CHAR_BIT> bitset;

    for (const auto byte : bytes) {
        bitset = (bitset << CHAR_BIT) | std::bitset<size * CHAR_BIT>(std::to_integer<uint8_t>(byte));
    }

    return bitset;
}

template<size_t bitsetSize>
std::bitset<bitsetSize> toBitset(ByteSpan<> &bytes) {
    assert(bytes.size() <= toByteCount(bitsetSize));

    std::bitset<bitsetSize> bitset;

    for (const auto byte : bytes) {
        bitset = (bitset << CHAR_BIT) | std::bitset<bitsetSize>(std::to_integer<uint8_t>(byte));
    }

    return bitset;
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

template<size_t bitsCount>
ByteArray<> toDynamicByteArray(const std::bitset<bitsCount> &bitset, size_t offset, size_t size) {
    ByteArray<> output(size);

    for (int i = offset; i < offset + size; i++) {
        uint8_t byte = ((bitset >> ((toByteCount(bitsCount) - 1 - i) * CHAR_BIT)) & std::bitset<bitsCount>(UINT8_MAX)).to_ulong();
        output[i - offset] = std::byte(byte);
    }

    return output;
}

template<size_t charSize>
ByteArray<(charSize - 1) / 2> toByteArrayFromHexa(const char (&hexa)[charSize]) {
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

template <size_t blockSize, BlockMapper<blockSize> _BlockMapper, IncompleteBlockMapper<blockSize> _IncompleteBlockMapper>
ByteArray<> mapForEachBlock(
    ByteSpan<> bytes, 
    _BlockMapper transform, 
    _IncompleteBlockMapper transformRemaining
) {
    constexpr size_t blockSizeBytes     = toByteCount(blockSize);
    const     size_t blockCount         = bytes.size() / blockSizeBytes;
    const     size_t remainingBlockSize = bytes.size() % blockSizeBytes;

    ByteArray<> output(bytes.size());

    for (size_t i = 0; i < blockCount; i++) {
        const size_t              byteIndex         = i * blockSizeBytes;
        ByteSpan<blockSizeBytes>  currentBlock      = ByteSpan<blockSizeBytes>(bytes.subspan(byteIndex, blockSizeBytes));
        std::bitset<blockSize>    currentBitset     = toBitset(currentBlock);
        std::bitset<blockSize>    transformedBitset = transform(currentBitset);
        ByteArray<blockSizeBytes> transformedBlock  = toByteArray(transformedBitset);
        
        std::copy(transformedBlock.begin(), transformedBlock.end(), output.begin() + byteIndex);
    }

    if constexpr (!Null<decltype(transformRemaining)>) {
        if (remainingBlockSize > 0) {
            const size_t           byteIndex         = blockCount * blockSizeBytes;
            ByteSpan<>             remainingBlock    = ByteSpan<>(bytes.subspan(byteIndex, remainingBlockSize));
            std::bitset<blockSize> remainingBitset   = toBitset<blockSize>(remainingBlock) << ((blockSizeBytes - remainingBlockSize) * CHAR_BIT);
            std::bitset<blockSize> transformedBitset;

            if constexpr (BlockMapper<decltype(transformRemaining), blockSize>) {
                transformedBitset = transformRemaining(remainingBitset);
            } else {
                transformedBitset = transformRemaining(remainingBitset, remainingBlockSize);
            }

            ByteArray<> transformedBlock  = toDynamicByteArray(transformedBitset, 0, remainingBlockSize);

            std::copy(transformedBlock.begin(), transformedBlock.end(), output.begin() + byteIndex);
        }
    }

    return output;
}
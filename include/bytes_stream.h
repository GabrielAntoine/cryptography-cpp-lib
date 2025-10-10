#pragma once

// This files allow multiple ways to visualize ByteArray

#include "bytes.h"
#include <ostream>
#include <format>
#include <bitset>



// Allows the user to chose between different bytes representation
static int displayModeIndex = std::ios_base::xalloc();

// Allows the user to add a separtor between each bytes
// A default separator of '.' is used for decimal notation
// No separator can be used for ascii representation
static int separatorIndex   = std::ios_base::xalloc();

struct _ByteSeparator {
    static constexpr char DEFAULT = '.';
    char separator = DEFAULT;
};




// ===================================================================================
// SECTION : Output stream
// ===================================================================================


// Operator to add, change or remove the current separator
std::ostream& operator<<(std::ostream& os, _ByteSeparator bs) {
    os.iword(separatorIndex) = static_cast<long>(bs.separator);

    return os;
}

// Stream manipulator to add a separator between bytes
_ByteSeparator byteSeparator(char c) {
    return _ByteSeparator{c};
}

// Stream manipulator to remove the separator between bytes
_ByteSeparator noByteSeparator() {
    return _ByteSeparator{'\0'};
}

// Enum to identify the current mode to display bytes
enum ByteArrayDisplayMode {
    BINARY = 0,
    HEXA,

    // Each byte will have its own decimal representation
    DECIMAL,

    // Each byte will be displayed in its ascii value
    ASCII
};

// Used to effectively change the display mode
std::ostream& operator<<(std::ostream& os, ByteArrayDisplayMode mode) {
    os.iword(displayModeIndex) = mode;

    return os;
}

template <size_t size, typename Callback>
void streamBytesWithSeparator(std::ostream& os, const ByteArray<size> &bytes, Callback streamOneByte, const char separator) {    
    for (int i = 0; i < bytes.size() - 1; i++) {
        streamOneByte(os, bytes[i]);
        os << separator;
    }

    streamOneByte(os, bytes.back());
}

template <size_t size, typename Callback>
void streamBytesWithoutSeparator(std::ostream& os, const ByteArray<size> &bytes, Callback streamOneByte) {    
    for (const auto byte : bytes) {
        streamOneByte(os, byte);
    }
}

template <size_t size, typename Callback>
void streamBytesWithOrWithoutSeparator(std::ostream& os, const ByteArray<size> &bytes, Callback streamOneByte) {
    const char separator = static_cast<char>(os.iword(separatorIndex));
    
    if (separator) {
        streamBytesWithSeparator(os, bytes, streamOneByte, separator);
    } else {
        streamBytesWithoutSeparator(os, bytes, streamOneByte);
    }
}

template <size_t size>
void streamBinary(std::ostream& os, const ByteArray<size> &bytes) {
    streamBytesWithOrWithoutSeparator(os, bytes, [](std::ostream& os, std::byte byte) {
         os << std::bitset<8>(std::to_integer<int>(byte));
    });
}

template <size_t size>
void streamDecimal(std::ostream& os, const ByteArray<size> &bytes) {
    const char separator = static_cast<char>(os.iword(separatorIndex));

    streamBytesWithSeparator(os, bytes, [](std::ostream& os, std::byte byte) {
         os << std::to_integer<int>(byte);
    }, separator ? separator : _ByteSeparator::DEFAULT);
}

template <size_t size>
void streamHexa(std::ostream& os, const ByteArray<size> &bytes) {
    streamBytesWithOrWithoutSeparator(os, bytes, [](std::ostream& os, std::byte byte) {
        os << std::format("{:02X}", std::to_integer<uint8_t>(byte));
    });
}

template <size_t size>
void streamAscii(std::ostream& os, const ByteArray<size> &bytes) {
    streamBytesWithoutSeparator(os, bytes, [](std::ostream& os, std::byte byte) {
        os << static_cast<char>(byte);
    });
}

// Stream bytes according to the current display mode and separator
template <size_t size>
std::ostream& operator<<(std::ostream& os, const ByteArray<size> &bytes) {
    const auto mode = static_cast<ByteArrayDisplayMode>(os.iword(displayModeIndex));

    switch (mode) {
        case HEXA    : streamHexa(os, bytes);    break;
        case DECIMAL : streamDecimal(os, bytes); break;
        case ASCII   : streamAscii(os, bytes);   break;
        default      : streamBinary(os, bytes);  break;
    }

    return os;
}
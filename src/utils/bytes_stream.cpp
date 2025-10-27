// This files allow multiple ways to visualize ByteArray

#include "bytes.h"
#include "bytes_stream.h"
#include <ostream>
#include <format>
#include <bitset>

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

// Used to effectively change the display mode
std::ostream& operator<<(std::ostream& os, ByteArrayDisplayMode mode) {
    os.iword(displayModeIndex) = mode;

    return os;
}

void streamBytesWithSeparator(std::ostream& os, const ByteArray<> &bytes, ByteSreamCallback streamOneByte, const char separator) {    
    for (int i = 0; i < bytes.size() - 1; i++) {
        streamOneByte(os, bytes[i]);
        os << separator;
    }

    streamOneByte(os, bytes.back());
}

void streamBytesWithoutSeparator(std::ostream& os, const ByteArray<> &bytes, ByteSreamCallback streamOneByte) {    
    for (const auto byte : bytes) {
        streamOneByte(os, byte);
    }
}

void streamBytesWithOrWithoutSeparator(std::ostream& os, const ByteArray<> &bytes, ByteSreamCallback streamOneByte) {
    const char separator = static_cast<char>(os.iword(separatorIndex));
    
    if (separator) {
        streamBytesWithSeparator(os, bytes, streamOneByte, separator);
    } else {
        streamBytesWithoutSeparator(os, bytes, streamOneByte);
    }
}

void streamBinary(std::ostream& os, const ByteArray<> &bytes) {
    streamBytesWithOrWithoutSeparator(os, bytes, [](std::ostream& os, std::byte byte) {
         os << std::bitset<8>(std::to_integer<int>(byte));
    });
}

void streamDecimal(std::ostream& os, const ByteArray<> &bytes) {
    const char separator = static_cast<char>(os.iword(separatorIndex));

    streamBytesWithSeparator(os, bytes, [](std::ostream& os, std::byte byte) {
         os << std::to_integer<int>(byte);
    }, separator ? separator : _ByteSeparator::DEFAULT);
}

void streamHexa(std::ostream& os, const ByteArray<> &bytes) {
    streamBytesWithOrWithoutSeparator(os, bytes, [](std::ostream& os, std::byte byte) {
        os << std::format("{:02X}", std::to_integer<uint8_t>(byte));
    });
}

void streamAscii(std::ostream& os, const ByteArray<> &bytes) {
    streamBytesWithoutSeparator(os, bytes, [](std::ostream& os, std::byte byte) {
        os << static_cast<char>(byte);
    });
}

// Stream bytes according to the current display mode and separator
std::ostream& operator<<(std::ostream& os, const ByteArray<> &bytes) {
    const auto mode = static_cast<ByteArrayDisplayMode>(os.iword(displayModeIndex));

    switch (mode) {
        case HEXA    : streamHexa(os, bytes);    break;
        case DECIMAL : streamDecimal(os, bytes); break;
        case ASCII   : streamAscii(os, bytes);   break;
        default      : streamBinary(os, bytes);  break;
    }

    return os;
}
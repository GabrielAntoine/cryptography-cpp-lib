#pragma once

// This files allow multiple ways to visualize ByteArray

#include "bytes.h"



// Allows the user to chose between different bytes representation
static int displayModeIndex = std::ios_base::xalloc();

// Allows the user to add a separtor between each bytes
// A default separator of '.' is used for decimal notation
// No separator can be used for ascii representation
static int separatorIndex   = std::ios_base::xalloc();

// A structure to help operator<< change the current separator
struct _ByteSeparator {
    static constexpr char DEFAULT = '.';
    char separator = DEFAULT;
};

// Enum to identify the current mode to display bytes
enum ByteArrayDisplayMode {
    BINARY = 0,
    HEXA,

    // Each byte will have its own decimal representation
    DECIMAL,

    // Each byte will be displayed in its ascii value
    ASCII
};


// ===================================================================================
// SECTION : Output stream
// ===================================================================================


// Operator to add, change or remove the current separator
std::ostream& operator<<(std::ostream& os, _ByteSeparator bs);

// Stream manipulator to add a separator between bytes
_ByteSeparator byteSeparator(char c);

// Stream manipulator to remove the separator between bytes
_ByteSeparator noByteSeparator();

// Used to effectively change the display mode
std::ostream& operator<<(std::ostream& os, ByteArrayDisplayMode mode);

template <size_t size, typename Callback>
void streamBytesWithSeparator(std::ostream& os, const ByteArray<size> &bytes, Callback streamOneByte, const char separator);

template <size_t size, typename Callback>
void streamBytesWithoutSeparator(std::ostream& os, const ByteArray<size> &bytes, Callback streamOneByte);

template <size_t size, typename Callback>
void streamBytesWithOrWithoutSeparator(std::ostream& os, const ByteArray<size> &bytes, Callback streamOneByte);

template <size_t size>
void streamBinary(std::ostream& os, const ByteArray<size> &bytes);

template <size_t size>
void streamDecimal(std::ostream& os, const ByteArray<size> &bytes);

template <size_t size>
void streamHexa(std::ostream& os, const ByteArray<size> &bytes);

template <size_t size>
void streamAscii(std::ostream& os, const ByteArray<size> &bytes);

// Stream bytes according to the current display mode and separator
template <size_t size>
std::ostream& operator<<(std::ostream& os, const ByteArray<size> &bytes);

#include "bytes_stream.tpp"
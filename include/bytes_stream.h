#pragma once

// This files allow multiple ways to visualize ByteArray

#include <functional>
#include <ostream>
#include <cstddef>

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

using ByteSreamCallback = std::function<void(std::ostream&, std::byte)>;

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

void streamBytesWithSeparator(std::ostream& os, const ByteArray<> &bytes, ByteSreamCallback streamOneByte, const char separator);
void streamBytesWithoutSeparator(std::ostream& os, const ByteArray<> &bytes, ByteSreamCallback streamOneByte);
void streamBytesWithOrWithoutSeparator(std::ostream& os, const ByteArray<> &bytes, ByteSreamCallback streamOneByte);
void streamBinary(std::ostream& os, const ByteArray<> &bytes);
void streamDecimal(std::ostream& os, const ByteArray<> &bytes);
void streamHexa(std::ostream& os, const ByteArray<> &bytes);
void streamAscii(std::ostream& os, const ByteArray<> &bytes);

// Stream bytes according to the current display mode and separator
std::ostream& operator<<(std::ostream& os, const ByteArray<> &bytes);
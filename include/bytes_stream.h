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
struct ByteSeparator {
    static const ByteSeparator DEFAULT;
    static const ByteSeparator NONE;

    char separator = DEFAULT.separator;
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
std::ostream& operator<<(std::ostream& os, ByteSeparator bs);

// Used to effectively change the display mode
std::ostream& operator<<(std::ostream& os, ByteArrayDisplayMode mode);

void streamBytesWithSeparator(std::ostream& os, const ByteSpan<> &bytes, ByteSreamCallback streamOneByte, const char separator);
void streamBytesWithoutSeparator(std::ostream& os, const ByteSpan<> &bytes, ByteSreamCallback streamOneByte);
void streamBytesWithOrWithoutSeparator(std::ostream& os, const ByteSpan<> &bytes, ByteSreamCallback streamOneByte);
void streamBinary(std::ostream& os, const ByteSpan<> &bytes);
void streamDecimal(std::ostream& os, const ByteSpan<> &bytes);
void streamHexa(std::ostream& os, const ByteSpan<> &bytes);
void streamAscii(std::ostream& os, const ByteSpan<> &bytes);

// Stream bytes according to the current display mode and separator
std::ostream& operator<<(std::ostream& os, const ByteSpan<> &bytes);

std::string toString(ByteSpan<> bytes, ByteArrayDisplayMode mode = HEXA, ByteSeparator separator = ByteSeparator::NONE);
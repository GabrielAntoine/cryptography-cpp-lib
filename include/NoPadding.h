#pragma once


#include "bytes.h"
#include "BlockCipherPadding.h"


class NoPadding {

public:
    ByteArray<> pad(ByteArray<> bytes) const { return bytes; };
    ByteArray<> unpad(ByteArray<> bytes) const { return bytes; };
};

static_assert(BlockCipherPadding<NoPadding>);

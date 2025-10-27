#pragma once


#include <bitset>

#include "DES.h"
#include "bytes.h"
#include "BlockCipherPadding.h"


class PKCS5Padding {

public:
    ByteArray<> pad(ByteArray<> bytes) const;
    ByteArray<> unpad(ByteArray<> bytes) const;
};

static_assert(BlockCipherPadding<PKCS5Padding>);

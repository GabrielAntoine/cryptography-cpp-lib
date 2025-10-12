#pragma once


#include <bitset>

#include "DES.h"
#include "bytes.h"
#include "BlockCipherPadding.h"


class PKCS5Padding {

public:
    void pushPadding(ByteArray &bytes) const;
    void popPadding(ByteArray &bytes) const;
};

static_assert(BlockCipherPadding<PKCS5Padding>);

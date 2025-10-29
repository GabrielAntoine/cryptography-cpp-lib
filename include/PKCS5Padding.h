#pragma once

#include "bytes.h"
#include "BlockCipherPadding.h"
#include "DES.h"
#include "PKCS7Padding.h"

class PKCS5Padding : public PKCS7Padding<DES::BLOCK_SIZE> {

public:
    static constexpr size_t BLOCK_SIZE = DES::BLOCK_SIZE;
};

static_assert(BlockCipherPadding<PKCS5Padding>);

#pragma once

#include "bytes.h"

// The algorithm padding should return the full extra block
// Ex: if the last block of the DES algorithm is 0x1234 (incomplete block)
//     then the PKCS5Padding should return the block 0x1234060606060606.
//     The cipher algorithm will firstly encrypt all the possible block and only after encrypt the padding block

template <typename TPadding>
concept BlockCipherPadding = requires(TPadding padding, ByteArray &bytes) {
    { padding.pushPadding(bytes) };
    { padding.popPadding(bytes)  };
};
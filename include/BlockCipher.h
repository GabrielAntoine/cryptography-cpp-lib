#pragma once

#include "BlockCipherAlgorithm.h"
#include "BlockCipherMode.h"
#include "BlockCipherPadding.h"

template<BlockCipherMode TMode, BlockCipherPadding TPadding>
class BlockCipher {
private:
    TMode::Algorithm algorithm;
    TMode            mode;
    TPadding         padding;

public:
    BlockCipher(const typename TMode::Algorithm &algorithm, const TMode &mode, const TPadding &padding);

    ByteArray encrypt(ByteSpan<> plainText) const;
    ByteArray decrypt(ByteSpan<> plainText) const;
};

#include "BlockCipher.tpp"
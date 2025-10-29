#pragma once

#include "BlockCipherAlgorithm.h"
#include "BlockCipherMode.h"

template <BlockCipherAlgorithm TAlgorithm>
class CBC {
public:
    using Algorithm   = TAlgorithm;
    using IV          = ByteArray<toByteCount(Algorithm::BLOCK_SIZE)>;
    using IVSpan      = ByteSpan<toByteCount(Algorithm::BLOCK_SIZE)>;

private:
    Algorithm algorithm;
    IV iv;
    
public:

    void setAlgorithm(const Algorithm &algorithm);
    void setIV(IVSpan iv);
    ByteArray<> encrypt(ByteSpan<> bytes) const;
    ByteArray<> decrypt(ByteSpan<> bytes) const;

    static_assert(BlockCipherMode<CBC<Algorithm>>);
};

#include "CBC.tpp"
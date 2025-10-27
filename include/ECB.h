#pragma once

#include "bytes.h"
#include "Key.h"
#include "BlockCipherAlgorithm.h"
#include "BlockCipherMode.h"

template <BlockCipherAlgorithm TAlgorithm>
class ECB {
public:
    using Algorithm = TAlgorithm;
    
private:
    Algorithm algorithm;

    ByteArray<> run(ByteSpan<> plainText, const bool encrypt) const;

public:
    void setAlgorithm(TAlgorithm algorithm);
    ByteArray<> encrypt(ByteSpan<> plainText) const;
    ByteArray<> decrypt(ByteSpan<> plainText) const;
    
    // Will not fail without a template instanciation
    static_assert(BlockCipherMode<ECB<Algorithm>>);
};

#include "ECB.tpp"
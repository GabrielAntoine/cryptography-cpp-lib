#pragma once

#include "BlockCipherMode.h"
#include "AES.h"
#include "CTR.h"

template <size_t keySize>
    requires AESKeySize<keySize>
class GCM : public CTR<AES<keySize>> {
public:
    using Algorithm = CTR<AES<keySize>>::Algorithm;
    using IV = CTR<AES<keySize>>::IV;
    using Tag = ByteArray<toByteCount(AES<keySize>::BLOCK_SIZE)>;

private:
    // Because the iv is not necesseraly 128 bits, the one given from setIv
    // will be stocked here but the one (calculated) used in the encryption process
    // is stocked in the iv field from the CTR parent class.
    ByteArray<> rawIv;

    // For ease of use ('iv' field being j0 + 1)
    IV j0;

    ByteArray<> aad;
    
public:
    using CTR<AES<keySize>>::setAlgorithm;

    void setIV(ByteSpan<> iv);
    void setAAD(ByteSpan<> aad);
    ByteArray<> encrypt(ByteSpan<> bytes) const;
    ByteArray<> decrypt(ByteSpan<> bytes) const;

private:

    // Will pad bytes during process if needed but the input won't be altered at the end
    Tag ghash(ByteArray<> bytes, Tag initialTag = Tag{std::byte(0)}) const;
    Tag createTag(const ByteArray<> &ciphertext) const;

    static_assert(BlockCipherMode<GCM<keySize>>);
};

#include "GCM.tpp"

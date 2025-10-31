#include <iostream>
#include <cstdint>
#include <array>
#include <cstddef>

#include "DESTables.h"
#include "DES.h"
#include "bytes_stream.h"

void DES::setKey(const SecretKey &key) {
    this->key = key;
}

DES::HalfBlock DES::feistel(const HalfBlock block, SecretKey::RoundKey roundKey) const {
    // Expansion from 32 bits to 48 bits
    HalfBlockExtended expandedBits = permuteBitsByTable(block, ebox);
    
    // Xor with the round key
    HalfBlockExtended keyifiedBits = expandedBits ^ roundKey;

    // S-Boxes
    HalfBlock sboxedBits;
    for (int i = 0; i < sboxes.size(); i++) {
        size_t firstBitOfTheBlockIndex = i * SBOX_BLOCK_SIZE;
        SboxBlock block = slice<SBOX_BLOCK_SIZE>(keyifiedBits, firstBitOfTheBlockIndex);
        size_t row      = ((block & SboxBlock(0b100000)) >> 4 | block & SboxBlock(1)).to_ullong();
        size_t column   = (block >> 1 & SboxBlock(0b1111)).to_ullong();
        sboxedBits      = sboxedBits << 4 | HalfBlock(sboxes[i][row][column]);
    }

    // P-box
    return permuteBitsByTable(sboxedBits, pbox);
}

DES::Block DES::run(const Block plainBits, bool encrypt) const {
    // Initial permutation
    const Block initiallyPermutedBytes = permuteBitsByTable(plainBits, initialPermutationTable);
    HalfBlock leftHalf = slice<HALF_BLOCK_SIZE>(initiallyPermutedBytes);
    HalfBlock rightHalf = slice<HALF_BLOCK_SIZE>(initiallyPermutedBytes, HALF_BLOCK_SIZE);

    
    
    // 16 rounds
    for (int i = 1; i <= 16; i++) {
        const HalfBlock oldRightHalf = rightHalf;
        int roundKeyIndex = i;
        if (!encrypt) {
            roundKeyIndex = 17 - i;
        }
        rightHalf = feistel(rightHalf, key.getRoundKey(roundKeyIndex)) ^ leftHalf;
        leftHalf = oldRightHalf;
    }
    
    // last swap
    const Block beforeEndBytes = lpad<HALF_BLOCK_SIZE>(rightHalf) << 32 | lpad<HALF_BLOCK_SIZE>(leftHalf);
    
    // last permutation
    const Block encryptedBytes = permuteBitsByTable(beforeEndBytes, finalPermutationTable);
    
    return encryptedBytes;
}

DES::Block DES::encrypt(const Block plainBits) const {
    return run(plainBits, true);
}

DES::Block DES::decrypt(const Block plainBits) const {
    return run(plainBits, false);
}
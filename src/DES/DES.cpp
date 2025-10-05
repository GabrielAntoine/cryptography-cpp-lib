#include <iostream>
#include <cstdint>
#include <array>

#include "DESTables.h"
#include "DES.h"

uint32_t mangler(const uint32_t bits, const uint64_t roundKey) {
    // Expansion from 32 bits to 48 bits
    uint64_t expandedBits = permuteBitsByTable<uint32_t, uint64_t>(bits, ebox);
    
    // Xor with the round key
    uint64_t keyifiedBits = expandedBits ^ roundKey;

    // S-Boxes
    uint32_t sboxedBits = 0;
    for (int i = 0; i < 8; i++) {
        
        size_t firstBitOfTheBlockIndex = 47 - (i * 6);
        uint8_t block = (keyifiedBits >> (firstBitOfTheBlockIndex - 5)) & 0b111111;
        size_t row = ((block & 0b100000) >> 4) | (block & 1);
        size_t column = (block >> 1) & 0b1111;
        sboxedBits = (sboxedBits << 4) | sboxes[i][row][column];
    }

    // P-box
    return permuteBitsByTable(sboxedBits, pbox);
}

uint64_t encryptDES(uint64_t plainBits, DESSecretKey key, bool encrypt) {
    // Initial permutation
    const uint64_t initiallyPermutedBytes = permuteBitsByTable(plainBits, initialPermutationTable);
    uint32_t leftHalf = initiallyPermutedBytes >> 32;
    uint32_t rightHalf = initiallyPermutedBytes & (0xFFFFFFFF);

    
    // 16 rounds
    for (int i = 1; i <= 16; i++) {
        const uint32_t oldRightHalf = rightHalf;
        int roundKeyIndex = i;
        if (!encrypt) {
            roundKeyIndex = 17 - i;
        }
        rightHalf = mangler(rightHalf, key.getRoundKey(roundKeyIndex)) ^ leftHalf;
        leftHalf = oldRightHalf;
    }
    
    // last swap
    const uint64_t beforeEndBytes = (static_cast<uint64_t>(rightHalf) << 32) | leftHalf;
    
    // last permutation
    const uint64_t encryptedBytes = permuteBitsByTable(beforeEndBytes, finalPermutationTable);
    
    return encryptedBytes;
}
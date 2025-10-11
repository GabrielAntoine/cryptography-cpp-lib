#include <cassert>
#include <bit>
#include <iostream>

#include "bytes.h"
#include "DESSecretKey.h"
#include "DESTables.h"

DESSecretKey::DESSecretKey(KeyBytes key): key(toBitset(key)) {
    calculateRoundKeys();
}

ByteArray DESSecretKey::getBytes() const {
    return toByteArray(key);
}

DESSecretKey::RoundKey DESSecretKey::getRoundKey(const int roundNumber) const {
    return roundKeys[roundNumber - 1];
}

void DESSecretKey::calculateRoundKeys() {
    // Remove the 8 useless bits from the 64 bits key
    std::bitset<REAL_KEY_SIZE> baseKey = permuteBitsByTable(key, pc1Table);

    for (int i = 0; i < ROUND_COUNT; i++) {
        // Divide the key in two 28 bits parts
        std::bitset<HALF_KEY_SIZE> leftKeyPart  = sliceBitset<HALF_KEY_SIZE>(baseKey, HALF_KEY_SIZE);
        std::bitset<HALF_KEY_SIZE> rightKeyPart = sliceBitset<HALF_KEY_SIZE>(baseKey);

        // Left circular shift
        const uint8_t shift = SHIFTS_BY_ROUND[i];
        leftKeyPart = rotl(leftKeyPart, shift);
        rightKeyPart = rotl(rightKeyPart, shift);
        baseKey = (extendBitset<HALF_KEY_SIZE>(leftKeyPart) << HALF_KEY_SIZE) | extendBitset<HALF_KEY_SIZE>(rightKeyPart);

        // Compressed permutation
        RoundKey roundKey = permuteBitsByTable(baseKey, pc2Table);

        roundKeys[i] = roundKey;
    }
}

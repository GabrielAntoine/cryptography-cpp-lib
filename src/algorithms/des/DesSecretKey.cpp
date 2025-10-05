#include "DesSecretKey.h"
#include "tables.h"

DesSecretKey::DesSecretKey(const uint64_t key): key(key) {
}

uint64_t DesSecretKey::getKey() const {
    return key;
}

uint64_t DesSecretKey::getKey56() {
    if (!areRoundKeysCalculated) {
        calculateRoundKeys();
    }

    return key56;
}

uint64_t DesSecretKey::getRoundKey(const int roundNumber) {
    if (!areRoundKeysCalculated) {
        calculateRoundKeys();
    }

    return roundKeys[roundNumber - 1];
}


void DesSecretKey::calculateRoundKeys() {
    // Remove the 8 useless bits from the 64 bits key
    key56 = permuteBitsByTable(key, pc1Table);
    uint64_t baseKey = key56;

    for (int i = 0; i < 16; i++) {
        // Divide the key in two 28 bits parts
        uint32_t leftKeyPart = baseKey >> 28;
        uint32_t rightKeyPart = baseKey & 0x0FFFFFFF;

        // Left circular shift
        const uint8_t shift = shiftsByRound[i];
        leftKeyPart = ((leftKeyPart << shift) & 0x0FFFFFFF) | (leftKeyPart >> (28 - shift));
        rightKeyPart = ((rightKeyPart << shift) & 0x0FFFFFFF) | (rightKeyPart >> (28 - shift));
        baseKey = (static_cast<uint64_t>(leftKeyPart) << 28) | rightKeyPart;

        // Compressed permutation
        uint64_t roundKey = permuteBitsByTable(baseKey, pc2Table, 56);

        roundKeys[i] = roundKey;
    }

    areRoundKeysCalculated = true;
}

#include <cassert>
#include <bit>
#include <iostream>

#include "bytes.h"
#include "AESSecretKey.h"
#include "AESTables.h"
#include "galois.h"

template <size_t keySize>
    requires AESKeySize<keySize>
AESSecretKey<keySize>::AESSecretKey(KeyBytes key) {
    std::copy(key.begin(), key.end(), this->key.begin());

    calculateRoundKeys();
}

template <size_t keySize>
    requires AESKeySize<keySize>
ByteArray<> AESSecretKey<keySize>::getBytes() const {
    return ByteArray<>(key.begin(), key.end());
}

template <size_t keySize>
    requires AESKeySize<keySize>
AESSecretKey<keySize>::RoundKey AESSecretKey<keySize>::getRoundKey(const int roundNumber) const {
    RoundKey roundKey;

    for (int i = 0; i < WORD_PER_ROUND_KEY; i++) {
        int j = i + roundNumber * WORD_PER_ROUND_KEY;
        std::copy(words[j].begin(), words[j].end(), roundKey.begin() + i * WORD_PER_ROUND_KEY);
    }

    return roundKey;
}

template <size_t keySize>
    requires AESKeySize<keySize>
AESSecretKey<keySize>::Word AESSecretKey<keySize>:: rotWord(const Word& word) const {
    return rotl<WORD_SIZE_BYTES>(word, 1);
}

template <size_t keySize>
    requires AESKeySize<keySize>
AESSecretKey<keySize>::Word AESSecretKey<keySize>:: subWord(const Word& word) const {
    Word substitued;

    for (int i = 0; i < word.size(); i++) {
        uint8_t currentByte = std::to_integer<uint8_t>(word[i]);
        uint8_t rowIndex = (currentByte & 0xF0) >> 4;
        uint8_t colIndex = currentByte & 0x0F;
        substitued[i] = std::byte(AES_TABLES::SBOX[rowIndex][colIndex]);
    }

    return substitued;
}

template <size_t keySize>
    requires AESKeySize<keySize>
void AESSecretKey<keySize>::calculateRoundKeys() {
    for (int i = 0; i < WORD_PER_KEY; i++) {
        auto wordBeginning = key.begin() + i * WORD_SIZE_BYTES;
        auto wordEnd = wordBeginning + WORD_SIZE_BYTES;
        std::copy(wordBeginning, wordEnd, words[i].begin());
    }

    std::byte rcon{1};

    for (int i = WORD_PER_KEY; i < TOTAL_WORDS; i++) {
        Word previousWord  = words[i - 1];
        Word wordFromAbove = words[i - WORD_PER_KEY];

        if (i % WORD_PER_KEY == 0) {
            previousWord = subWord(rotWord(previousWord));
            previousWord.front() ^= rcon;
            rcon = GF8::multiplyBy2(rcon);
        } else if constexpr (KEY_SIZE == 256) { // AES-256 specification
            if (i % (WORD_PER_KEY / 2) == 0) {
                previousWord = subWord(previousWord);
            }
        }

        using B = ByteSpan<WORD_SIZE_BYTES>;
        words[i] = B(previousWord) ^ B(wordFromAbove);
    }
}

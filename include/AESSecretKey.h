#pragma once

#include <cstdint>
#include <array>

#include "bytes.h"
#include "Key.h"

template <size_t size>
concept AESKeySize = size == 128 || size == 192 || size == 256;

template <size_t keySize>
    requires AESKeySize<keySize>
class AESSecretKey {
public:

    template <typename T>
    static consteval T byKeySize(T for128, T for192, T for256) {
        return keySize == 128 ? for128 : keySize == 192 ? for192 : for256;
    }

    // The number of rounds in AES encryption/decryption (stored here only)
    static constexpr uint8_t  ROUND_COUNT        = byKeySize(11, 13, 15);
  
    static constexpr uint16_t KEY_SIZE           = keySize;
    static constexpr uint8_t  KEY_SIZE_BYTES     = toByteCount(KEY_SIZE);
    static constexpr uint8_t  ROUND_KEY_SIZE     = 128;
    static constexpr uint8_t  WORD_SIZE          = 32;
    static constexpr uint8_t  WORD_SIZE_BYTES    = toByteCount(WORD_SIZE);
    static constexpr uint8_t  WORD_PER_KEY       = KEY_SIZE / WORD_SIZE;
    static constexpr uint8_t  WORD_PER_ROUND_KEY = ROUND_KEY_SIZE / WORD_SIZE;
    static constexpr uint8_t  TOTAL_WORDS        = WORD_PER_ROUND_KEY * ROUND_COUNT;

    static_assert(ROUND_KEY_SIZE / WORD_SIZE == 4); // Because one round key contains four words

    using Word     = ByteArray<WORD_SIZE_BYTES>;
    using RoundKey = ByteArray<toByteCount(ROUND_KEY_SIZE)>;
    using Key      = ByteArray<KEY_SIZE_BYTES>;
    using KeyBytes = ByteSpan<KEY_SIZE_BYTES>;

private:

    Key key;
    std::array<Word, TOTAL_WORDS> words;

public:

    AESSecretKey() = default;
    AESSecretKey(KeyBytes key);

    ByteArray<> getBytes() const;
    RoundKey getRoundKey(const int roundNumber) const;

private:    
    Word rotWord(const Word& word) const;
    Word subWord(const Word& word) const;
    void calculateRoundKeys();
};

static_assert(Key<AESSecretKey<128>>);

#include "AESSecretKey.tpp"
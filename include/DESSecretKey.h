#pragma once

#include <cstdint>
#include <array>

#include "bytes.h"
#include "Key.h"

class DESSecretKey {
public:

    static constexpr uint8_t ROUND_COUNT    = 16;
    static constexpr uint8_t KEY_SIZE       = 64;
    static constexpr uint8_t KEY_SIZE_BYTES = byteCountFromBits(KEY_SIZE);
    static constexpr uint8_t REAL_KEY_SIZE  = 56;
    static constexpr uint8_t ROUND_KEY_SIZE = 48;
    static constexpr uint8_t HALF_KEY_SIZE  = 28;
    static constexpr std::array<uint8_t, ROUND_COUNT> SHIFTS_BY_ROUND = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    using KeyBytes = ByteSpan<KEY_SIZE / CHAR_BIT>;
    using RoundKey = std::bitset<ROUND_KEY_SIZE>;
    using Key      = std::bitset<KEY_SIZE>;

private:

    Key key;
    std::array<RoundKey, ROUND_COUNT> roundKeys;

public:

    DESSecretKey() = default;
    DESSecretKey(KeyBytes key);

    ByteArray<> getBytes() const;
    RoundKey getRoundKey(const int roundNumber) const;

private:    
    void calculateRoundKeys();
};

static_assert(Key<DESSecretKey>);
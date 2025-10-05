#pragma once

#include <cstdint>
#include <array>

class DESSecretKey {
    uint64_t key;
    uint64_t key56 = 0;
    std::array<uint64_t, 16> roundKeys{};
    bool areRoundKeysCalculated = false;

    static constexpr std::array<uint8_t, 16> shiftsByRound = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

public:
    explicit DESSecretKey(const uint64_t key);

    [[nodiscard]]
    uint64_t getKey() const;
    uint64_t getKey56();
    uint64_t getRoundKey(const int roundNumber);

private:    
    void calculateRoundKeys();
};
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "DESSecretKey.h"

class DESedeSecretKey {
    std::array<uint64_t, 3> key;

public:
    explicit DESedeSecretKey(const std::array<uint64_t, 3> &key);

    DESSecretKey getDesKey(const int i) const;
};
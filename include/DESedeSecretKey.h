#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "DESSecretKey.h"

class DESedeSecretKey {
public:
    using KeyBytes = ByteSpan<DESSecretKey::KEY_SIZE * 3 / CHAR_BIT>;

private:

    DESSecretKey DesKey1;
    DESSecretKey DesKey2;
    DESSecretKey DesKey3;

public:

    DESedeSecretKey() = default;
    DESedeSecretKey(KeyBytes key);

    // Key getBytes() const; TODO

    const DESSecretKey &getDesKey1() const;
    const DESSecretKey &getDesKey2() const;
    const DESSecretKey &getDesKey3() const;
};
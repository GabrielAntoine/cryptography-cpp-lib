#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "DESSecretKey.h"
#include "Key.h"

class DESedeSecretKey {
public:
    static constexpr uint8_t KEY_SIZE = DESSecretKey::KEY_SIZE * 3;
    static constexpr uint8_t KEY_SIZE_BYTES = DESSecretKey::KEY_SIZE_BYTES * 3; 

    using KeyBytes = ByteSpan<KEY_SIZE_BYTES>;

private:

    DESSecretKey DesKey1;
    DESSecretKey DesKey2;
    DESSecretKey DesKey3;

public:

    DESedeSecretKey() = default;
    DESedeSecretKey(KeyBytes key);

    ByteArray<> getBytes() const;

    const DESSecretKey &getDesKey1() const;
    const DESSecretKey &getDesKey2() const;
    const DESSecretKey &getDesKey3() const;
};

static_assert(Key<DESedeSecretKey>);
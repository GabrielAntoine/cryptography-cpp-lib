#include "DESedeSecretKey.h"
#include "DESSecretKey.h"

DESedeSecretKey::DESedeSecretKey(KeyBytes key) {
    constexpr uint8_t DES_KEY_SIZE_IN_BYTES = DESSecretKey::KEY_SIZE / CHAR_BIT;

    DesKey1 = DESSecretKey(key.subspan<DES_KEY_SIZE_IN_BYTES * 0, DES_KEY_SIZE_IN_BYTES>());
    DesKey2 = DESSecretKey(key.subspan<DES_KEY_SIZE_IN_BYTES * 1, DES_KEY_SIZE_IN_BYTES>());
    DesKey3 = DESSecretKey(key.subspan<DES_KEY_SIZE_IN_BYTES * 2, DES_KEY_SIZE_IN_BYTES>());
}

const DESSecretKey &DESedeSecretKey::getDesKey1() const {
    return DesKey1;
}

const DESSecretKey &DESedeSecretKey::getDesKey2() const {
    return DesKey2;
}

const DESSecretKey &DESedeSecretKey::getDesKey3() const {
    return DesKey3;
}
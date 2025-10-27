#include "DESedeSecretKey.h"
#include "DESSecretKey.h"

DESedeSecretKey::DESedeSecretKey(KeyBytes key) {
    constexpr uint8_t DES_KEY_SIZE_IN_BYTES = DESSecretKey::KEY_SIZE / CHAR_BIT;

    DesKey1 = DESSecretKey(key.subspan<DES_KEY_SIZE_IN_BYTES * 0, DES_KEY_SIZE_IN_BYTES>());
    DesKey2 = DESSecretKey(key.subspan<DES_KEY_SIZE_IN_BYTES * 1, DES_KEY_SIZE_IN_BYTES>());
    DesKey3 = DESSecretKey(key.subspan<DES_KEY_SIZE_IN_BYTES * 2, DES_KEY_SIZE_IN_BYTES>());
}

ByteArray<> DESedeSecretKey::getBytes() const {
    ByteArray<> output;
    output.reserve(DESSecretKey::KEY_SIZE * 3);
    ByteArray<> key1 = DesKey1.getBytes(), key2 = DesKey2.getBytes(), key3 = DesKey3.getBytes();
    output.insert(output.end(), key1.begin(), key1.end());
    output.insert(output.end(), key2.begin(), key2.end());
    output.insert(output.end(), key3.begin(), key3.end());

    return output;
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
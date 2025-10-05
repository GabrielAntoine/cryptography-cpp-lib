#include "DESedeSecretKey.h"
#include "DESSecretKey.h"

DESedeSecretKey::DESedeSecretKey(const std::array<uint64_t, 3> &key) : key(key) {

}

DESSecretKey DESedeSecretKey::getDesKey(const int i) const {
    return DESSecretKey(key[i]);
}

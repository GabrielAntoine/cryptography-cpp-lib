#include "DESede.h"
#include "DES.h"
#include "DESSecretKey.h"

uint64_t encryptDESede(uint64_t plainBits, DESedeSecretKey &key, bool encrypt) {
    if (encrypt) {
        return encryptDES(encryptDES(encryptDES(plainBits, key.getDesKey(0)), key.getDesKey(1), false), key.getDesKey(2));
    } else {
        return encryptDES(encryptDES(encryptDES(plainBits, key.getDesKey(2), false), key.getDesKey(1)), key.getDesKey(0), false);
    }
}
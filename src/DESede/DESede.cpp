#include "DESede.h"
#include "DES.h"
#include "DESSecretKey.h"

DES::Block DESede::run(const DES::Block plainBits, bool encrypt) const {
    DES des1, des2, des3;

    des1.setKey(key.getDesKey1());
    des2.setKey(key.getDesKey2());
    des3.setKey(key.getDesKey3());

    if (encrypt) {
        return des3.encrypt(des2.decrypt(des1.encrypt(plainBits)));
    } else {
        return des1.decrypt(des2.encrypt(des3.decrypt(plainBits)));
    }
}

void DESede::setKey(const DESedeSecretKey &key) {
    this->key = key;
}

DES::Block DESede::encrypt(const DES::Block plainBits) const {
    return run(plainBits, true);
}

DES::Block DESede::decrypt(const DES::Block plainBits) const {
    return run(plainBits, false);
} 

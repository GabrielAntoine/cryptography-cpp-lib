#include "DESede.h"
#include "DES.h"
#include "DESSecretKey.h"

DESede::Block DESede::run(const DESede::Block plainBits, bool encrypt) const {
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

void DESede::setKey(const DESede::SecretKey &key) {
    this->key = key;
}

DESede::Block DESede::encrypt(const DESede::Block plainBits) const {
    return run(plainBits, true);
}

DESede::Block DESede::decrypt(const DESede::Block plainBits) const {
    return run(plainBits, false);
} 

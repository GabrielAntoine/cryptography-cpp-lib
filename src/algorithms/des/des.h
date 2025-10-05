#include "DesSecretKey.h"

uint32_t mangler(const uint32_t bits, const uint64_t roundKey);
uint64_t encryptDes(uint64_t plainBits, DesSecretKey &key, bool encrypt = true);
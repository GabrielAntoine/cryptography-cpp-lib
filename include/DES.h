#pragma once

#include "DESSecretKey.h"

uint32_t mangler(const uint32_t bits, const uint64_t roundKey);
uint64_t encryptDES(uint64_t plainBits, DESSecretKey key, bool encrypt = true);
#pragma once

#include "DESedeSecretKey.h"

uint64_t encryptDESede(uint64_t plainBits, DESedeSecretKey &key, bool encrypt = true);
#include <cstdint>
#include <iostream>

#include "des.h"

int main() {
    DesSecretKey key(0x133457799BBCDFF1);
    constexpr std::uint64_t plainText = 0x0123456789ABCDEF;


    std::cout << std::hex;
    std::cout << "Plain text is     :   " << plainText << '\n';
    std::cout << "Key is            :   " << key.getKey() << '\n';
    std::cout << "Encrypted text is :   " << encryptDes(plainText, key) << " should be " << "85E813540F0AB405\n";
    std::cout << "Decrypted text is :   " << encryptDes(encryptDes(plainText, key), key, false) << '\n';

    return 0;
}
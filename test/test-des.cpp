#include <cstdint>
#include <iostream>

#include "DESede.h"
#include "DES.h"

int main() {
    DESSecretKey key(0x133457799BBCDFF1);
    constexpr std::uint64_t plainText = 0x0123456789ABCDEF;

    DESedeSecretKey key3(std::array<uint64_t, 3> {
        0x6162636465666768ULL,
        0x696A6B6C6D6E6F70ULL,
        0x7172737475767778ULL
    });


    std::cout << std::hex;
    std::cout << "Plain text is     :   " << plainText << '\n';
    std::cout << "Key is            :   " << key.getKey() << '\n';
    std::cout << "Encrypted text is :   " << encryptDES(plainText, key) << " should be " << "85E813540F0AB405\n";
    std::cout << "Decrypted text is :   " << encryptDES(encryptDES(plainText, key), key, false) << "\n\n";

    std::cout << std::hex;
    std::cout << "Plain text is     :   " << plainText << '\n';
    std::cout << "Encrypted text is :   " << encryptDESede(plainText, key3) << "\n";
    std::cout << "Decrypted text is :   " << encryptDESede(encryptDESede(plainText, key3), key3, false) << "\n\n";

    return 0;
}
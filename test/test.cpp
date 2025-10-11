#include <cstdint>
#include <iostream>
#include <bit>

#include "DESede.h"
#include "DES.h"
#include "bytes.h"
#include "bytes_stream.h"

int main() {
    DESSecretKey key(0x133457799BBCDFF1);
    constexpr std::uint64_t plainText = 0x0123456789ABCDEF;

    DESedeSecretKey key3(std::array<uint64_t, 3> {
        0x6162636465666768ULL,
        0x696A6B6C6D6E6F70ULL,
        0x7172737475767778ULL
    });


    // std::cout << std::hex;
    // std::cout << "Plain text is     :   " << plainText << '\n';
    // std::cout << "Key is            :   " << key.getKey() << '\n';
    // std::cout << "Encrypted text is :   " << encryptDES(plainText, key) << " should be " << "85E813540F0AB405\n";
    // std::cout << "Decrypted text is :   " << encryptDES(encryptDES(plainText, key), key, false) << "\n\n";

    // std::cout << std::hex;
    // std::cout << "Plain text is     :   " << plainText << '\n';
    // std::cout << "Encrypted text is :   " << encryptDESede(plainText, key3) << "\n";
    // std::cout << "Decrypted text is :   " << encryptDESede(encryptDESede(plainText, key3), key3, false) << "\n\n";

    ByteArray a{std::byte(85), std::byte(240), std::byte(23)}, b{std::byte(183), std::byte(5), std::byte(6)};

    ByteArray c {
        std::byte(0b01100111),
        std::byte(0b01100001),
        std::byte(0b01100010),
        std::byte(0b01110010),
        std::byte(0b01101001),
        std::byte(0b01100101),
        std::byte(0b01101100)
    };
    
    std::cout << byteSeparator('-');
    std::cout << ByteArrayDisplayMode::BINARY << "a              : " << a << '\n';
    std::cout << ByteArrayDisplayMode::BINARY << "b              : " << b << '\n';
    a = rotl(a, 10);
    std::cout << "a left shifted : " << a << '\n'; 
    std::cout << "a & b          : " << (a & b) << '\n';


    return 0;
}
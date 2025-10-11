#include <cstdint>
#include <iostream>
#include <bit>

#include "DESede.h"
#include "DES.h"
#include "bytes.h"
#include "bytes_stream.h"

std::array<std::byte, 8> toFixedByteArray(const size_t n) {
    std::array<std::byte, 8> output;

    for (int i = sizeof n - 1; i >= 0; i--) {
        output[sizeof n - i - 1] = std::byte((n >> (CHAR_BIT * i)) & UINT8_MAX);
    }

    return output;
}

int main() {
    std::cout << "1\n";

    auto rawKey = toFixedByteArray(0x133457799BBCDFF1);
    auto plainText = toFixedByteArray(0x0123456789ABCDEF);

    DESSecretKey key(rawKey);
    DES des;
    des.setKey(key);

    using by = std::byte;

    std::array<std::byte, 24> rawKey3{
        by(0x61), by(0x62), by(0x63), by(0x64), by(0x65), by(0x66), by(0x67), by(0x68),
        by(0x69), by(0x6A), by(0x6B), by(0x6C), by(0x6D), by(0x6E), by(0x6F), by(0x70),
        by(0x71), by(0x72), by(0x73), by(0x74), by(0x75), by(0x76), by(0x77), by(0x78)
    };

    DESedeSecretKey key3(rawKey3);

    DESede desede;
    desede.setKey(key3);


    std::array<std::byte, 2> ad{std::byte(10), std::byte(7)};

    std::cout << ByteArrayDisplayMode::HEXA << std::boolalpha;
    std::cout << "Plain text is     :   " << toByteArray(std::bitset<64>(0x0123456789ABCDEF)) << '\n';
    std::cout << "Key is            :   " << key.getBytes() << '\n';
    std::cout << "Encrypted text is :   " << toByteArray(des.encrypt(std::bitset<64>(0x0123456789ABCDEF))) << " should be " << "85E813540F0AB405\n";
    std::cout << "Decrypted text is :   " << toByteArray(des.decrypt(des.encrypt(toBitset<8>(plainText)))) << "\n\n";

    std::cout << std::hex;
    std::cout << "Plain text is     :   " << toByteArray(std::bitset<64>(0x0123456789ABCDEF)) << '\n';
    std::cout << "Encrypted text is :   " << toByteArray(desede.encrypt(toBitset<8>(plainText))) << "\n";
    std::cout << "Decrypted text is :   " << toByteArray(desede.decrypt(desede.encrypt(toBitset<8>(plainText)))) << "\n\n";

    // ByteArray a{std::byte(85), std::byte(240), std::byte(23)}, b{std::byte(183), std::byte(5), std::byte(6)};

    // ByteArray c {
    //     std::byte(0b01100111),
    //     std::byte(0b01100001),
    //     std::byte(0b01100010),
    //     std::byte(0b01110010),
    //     std::byte(0b01101001),
    //     std::byte(0b01100101),
    //     std::byte(0b01101100)
    // };
    
    // std::cout << byteSeparator('-');
    // std::cout << ByteArrayDisplayMode::BINARY << "a              : " << a << '\n';
    // std::cout << ByteArrayDisplayMode::BINARY << "b              : " << b << '\n';
    // a = rotl(a, 10);
    // std::cout << "a left shifted : " << a << '\n'; 
    // std::cout << "a & b          : " << (a & b) << '\n';


    return 0;
}
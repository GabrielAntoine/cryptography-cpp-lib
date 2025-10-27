#include <cstdint>
#include <iostream>
#include <bit>
#include <sstream>
#include <iomanip>

#include "DESede.h"
#include "DES.h"
#include "bytes.h"
#include "bytes_stream.h"
#include "ECB.h"
#include "BlockCipher.h"
#include "PKCS5Padding.h"

std::array<std::byte, 8> toFixedByteArray(const size_t n) {
    std::array<std::byte, 8> output;

    for (int i = sizeof n - 1; i >= 0; i--) {
        output[sizeof n - i - 1] = std::byte((n >> (CHAR_BIT * i)) & UINT8_MAX);
    }

    return output;
}

template <std::size_t N>
std::string toHexString(const std::array<std::byte, N>& data) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (auto b : data)
        oss << std::setw(2) << static_cast<int>(std::to_integer<unsigned char>(b));

    return oss.str();
}

void testDES() {

}

int main() {
    using b = std::byte;

    std::array<std::byte, DES::SecretKey::KEY_SIZE / CHAR_BIT> rawKey{
        b(0x13), b(0x34), b(0x57), b(0x79), b(0x9b), b(0xbc), b(0xdf), b(0xf1)
    };

    std::array<std::byte, DES::BLOCK_SIZE / CHAR_BIT * 2 - 1> plainText{
        b(0x67), b(0x61), b(0x62), b(0x72), b(0x69), b(0x65), b(0x6c), b(0x20),
        b(0x6c), b(0x65), b(0x20), b(0x67), b(0x6f), b(0x61), b(0x74)
    };

    DES::SecretKey key(rawKey);
    DES des;
    des.setKey(key);

    BlockCipher cipher(des, ECB<DES>(), PKCS5Padding());


    std::cout << ByteArrayDisplayMode::HEXA << std::boolalpha;
    std::cout << "DES/ECB/PKCS5Padding : \n";
    std::cout << "Plain text is     :   " << toHexString(plainText) << '\n';
    ByteArray<> desKeyBytes = key.getBytes();
    std::cout << "Key is            :   " << desKeyBytes << '\n';
    auto encrypted = cipher.encrypt(plainText);
    std::cout << "Encrypted text is :   " << encrypted << " should be " << "F6F2AE257F718E340894D2D97B1CD400\n";
    auto decrypted = cipher.decrypt(encrypted);
    std::cout << "Decrypted text is :   " << decrypted << "\n";

    std::cout << "Decrypted ascci   :   " << toString(decrypted, ASCII) << "\n\n";

    //

    std::array<std::byte, 24> rawKey3{
        b(0x68), b(0x65), b(0x66), b(0x7A), b(0x64), b(0x6E), b(0x73), b(0x63),
        b(0x6A), b(0x61), b(0x66), b(0x70), b(0x6E), b(0x77), b(0x66), b(0x65),
        b(0x31), b(0x63), b(0x61), b(0x35), b(0x78), b(0x33), b(0x65), b(0x6D)
    };

    DESede::SecretKey key3(rawKey3);
    DESede desede;
    desede.setKey(key3);

    BlockCipher cipher3(desede, ECB<DESede>(), PKCS5Padding());

    std::cout << ByteArrayDisplayMode::ASCII << std::boolalpha;
    std::cout << "DESede/ECB/PKCS5Padding : \n";
    std::cout << "Plain text is     :   " << toHexString(plainText) << '\n';
    auto key3Bytes = key3.getBytes();
    std::cout << "Key is            :   " << key3Bytes << '\n';
    auto encrypted3 = cipher3.encrypt(plainText);
    std::cout << "Encrypted text is :   " << encrypted3 << " should be " << "F79D8580C9DB776B88C0033420EED401\n";
    auto decrypted3 = cipher3.decrypt(encrypted3);
    std::cout << "Decrypted text is :   " << decrypted3 << "\n\n";

    return 0;
}

// Ensuite, différencier ByteStaticArray et ByteDynamicArray pour pouvoir mieux gérer le padding (je crois)
// Ajouter des fonctions to_string pour ces deux types là afin d'afficher des bytes en hexa ou en ascii 
// Ensuite, implémenter CBC, PKCS7Padding, NoPadding et ZeroPadding
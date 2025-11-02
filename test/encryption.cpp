#include <catch2/catch_test_macros.hpp>
#include "DES.h"
#include "BlockCipher.h"
#include "ECB.h"
#include "CBC.h"
#include "CFB.h"
#include "OFB.h"
#include "CTR.h"
#include "PKCS5Padding.h"
#include "NoPadding.h"
#include "ZeroPadding.h"
#include "bytes_stream.h"
#include "DESede.h"
#include "AESSecretKey.h"
#include "AES.h"

struct EncryptionFixture {
    ByteArray<15> plainText15 = toByteArrayFromAscii("gabriel le goat");
    ByteArray<24> plainText24 = toByteArrayFromAscii("No icecream for you... !");
    ByteArray<34> plainText34 = toByteArrayFromAscii("No mind to think, to will to break");
    ByteArray<12> plainText12WithZerosAtTheEnd = toByteArrayFromHexa("4e863c9314ddab3e6f000000");
    ByteArray<8>  iv64        = toByteArrayFromHexa("0123456789abcdef");
};

struct DESFixture : public EncryptionFixture {
    DES::SecretKey key;
    DES des;

    DESFixture() {
        ByteArray<DES::SecretKey::KEY_SIZE_BYTES> rawKey = toByteArrayFromHexa("133457799bbcdff1");
        
        key = DES::SecretKey(rawKey);
        des.setKey(key);
    }
};

struct DESedeFixture : public EncryptionFixture {
    DESede::SecretKey key;
    DESede desede;

    DESedeFixture() {
        std::array<std::byte, 24> rawKey = toByteArrayFromHexa("6865667a646e73636a6166706e776665316361357833656d");

        key = DESede::SecretKey(rawKey);
        desede.setKey(key);
    }
};

struct AESFixture : public EncryptionFixture {
    AESSecretKey<128> key128;
    AESSecretKey<192> key192;
    AESSecretKey<256> key256;
    AES<128> aes128;
    AES<192> aes192;
    AES<256> aes256;

    std::bitset<128> plainTextNist;

    AESFixture() : plainTextNist(std::string("00000000000100010010001000110011010001000101010101100110011101111000100010011001101010101011101111001100110111011110111011111111")) {
        ByteArray<16> rawKey128 = toByteArrayFromHexa("000102030405060708090a0b0c0d0e0f");
        ByteArray<24> rawKey192 = toByteArrayFromHexa("000102030405060708090a0b0c0d0e0f1011121314151617");
        ByteArray<32> rawKey256 = toByteArrayFromHexa("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");

        key128 = AESSecretKey<128>(rawKey128);
        key192 = AESSecretKey<192>(rawKey192);
        key256 = AESSecretKey<256>(rawKey256);

        aes128.setKey(key128);
        aes192.setKey(key192);
        aes256.setKey(key256);
    }
};

TEST_CASE_METHOD(DESFixture, "DES/ECB/PKCS5Padding", "[encryption]") {
    BlockCipher cipher(des, ECB<DES>(), PKCS5Padding());
    auto encrypted = cipher.encrypt(plainText15);
    auto decrypted = cipher.decrypt(encrypted);
    
    REQUIRE(toString(encrypted) == "F6F2AE257F718E340894D2D97B1CD400");
    REQUIRE(toString(decrypted) == toString(plainText15));
}

TEST_CASE_METHOD(DESFixture, "NoPadding full encryption", "[encryption]") {
    BlockCipher cipher(des, ECB<DES>(), NoPadding());

    auto encrypted = cipher.encrypt(plainText24);
    REQUIRE(toString(encrypted) == "AAAB52782D91889AC9EF12D1EB322671D541ADCD73CE97BD");

    auto decrypted = cipher.decrypt(encrypted);
    REQUIRE(toString(decrypted) == toString(plainText24));
}

TEST_CASE_METHOD(DESFixture, "ZeroPadding full encryption", "[encryption]") {
    BlockCipher cipher(des, ECB<DES>(), ZeroPadding<DES::BLOCK_SIZE>());

    SECTION("One zero added") {
        auto encrypted = cipher.encrypt(plainText15);
        REQUIRE(toString(encrypted) == "F6F2AE257F718E34D24B12B6E9E71F9A");

        auto decrypted = cipher.decrypt(encrypted);
        REQUIRE(toString(decrypted) == toString(plainText15));
    }

    SECTION("No zero added") {
        auto encrypted = cipher.encrypt(plainText24);
        REQUIRE(toString(encrypted) == "AAAB52782D91889AC9EF12D1EB322671D541ADCD73CE97BD");

        auto decrypted = cipher.decrypt(encrypted);
        REQUIRE(toString(decrypted) == toString(plainText24));
    }

    SECTION("Too much zeros removed ! (Bad use of ZeroPadding)") {
        auto encrypted = cipher.encrypt(plainText12WithZerosAtTheEnd);
        auto decrypted = cipher.decrypt(encrypted);

        REQUIRE(plainText12WithZerosAtTheEnd.size() == 12);
        REQUIRE(encrypted.size() == 16);
        REQUIRE(decrypted.size() == 9);
    }
}

// Not very useful to test it with DES since it's the same as PKCS5Padding
TEST_CASE_METHOD(DESFixture, "PKCS7Padding full encryption (DES)", "[encryption]") {
    BlockCipher cipher(des, ECB<DES>(), PKCS7Padding<DES::BLOCK_SIZE>());
    auto encrypted = cipher.encrypt(plainText15);
    auto decrypted = cipher.decrypt(encrypted);
    
    REQUIRE(toString(encrypted) == "F6F2AE257F718E340894D2D97B1CD400");
    REQUIRE(toString(decrypted) == toString(plainText15));
}

TEST_CASE_METHOD(DESFixture, "CBC", "[encryption]") {
    CBC<DES> cbc;
    cbc.setIV(iv64);
    BlockCipher cipher(des, cbc, PKCS5Padding());
    auto encrypted = cipher.encrypt(plainText15);

    INFO("Encrypted text  is : " << toString(encrypted, HEXA, ByteSeparator{'-'}));

    REQUIRE(encrypted.size() % toByteCount(DES::BLOCK_SIZE) == 0);
    REQUIRE(toString(encrypted) == "0377B18F0ACCCD1A87F3491E478F4B4C");

    INFO("Decrypting...");
    auto decrypted = cipher.decrypt(encrypted);

    INFO("Decrypted text in ascii is : " << toString(decrypted, ASCII));
    REQUIRE(toString(decrypted) == toString(plainText15));
}

TEST_CASE_METHOD(DESFixture, "CFB", "[encryption]") {
    CFB<DES> cfb;
    cfb.setIV(iv64);
    BlockCipher cipher(des, cfb, NoPadding());

    SECTION("Without incomplete blocks") {
        auto encrypted = cipher.encrypt(plainText24);
        REQUIRE(toString(encrypted) == "CB87333D6C6FD777DEF9C81397CE751FFA0AC4A797A36675");
    
        auto decrypted = cipher.decrypt(encrypted);
        REQUIRE(toString(decrypted) == toString(plainText24));
    }

    SECTION("With incomplete blocks") {
        auto encrypted = cipher.encrypt(plainText15);
        REQUIRE(toString(encrypted) == "E2897126666FD8259E0F0AAE138B80");
    
        auto decrypted = cipher.decrypt(encrypted);
        REQUIRE(toString(decrypted) == toString(plainText15));
    }
}

TEST_CASE_METHOD(DESFixture, "OFB", "[encryption]") {
    OFB<DES> ofb;
    ofb.setIV(iv64);
    BlockCipher cipher(des, ofb, NoPadding());

    SECTION("With incomplete blocks") {
        auto encrypted = cipher.encrypt(plainText15);
        REQUIRE(toString(encrypted) == "E2897126666FD8250BCB5A4E0EBED7");
    
        auto decrypted = cipher.decrypt(encrypted);
        REQUIRE(toString(decrypted) == toString(plainText15));
    }
}

TEST_CASE_METHOD(DESFixture, "CTR", "[encryption]") {
    CTR<DES> ctr;
    ctr.setIV(iv64);
    BlockCipher cipher(des, ctr, NoPadding());

    SECTION("With incomplete blocks") {
        auto encrypted = cipher.encrypt(plainText15);
        REQUIRE(toString(encrypted) == "E2897126666FD8257D23C36889CA13");
    
        auto decrypted = cipher.decrypt(encrypted);
        REQUIRE(toString(decrypted) == toString(plainText15));
    }
}

TEST_CASE_METHOD(DESedeFixture, "DESede", "[encryption]") {
    BlockCipher cipher(desede, ECB<DESede>(), PKCS5Padding());
    auto encrypted = cipher.encrypt(plainText15);
    auto decrypted = cipher.decrypt(encrypted);

    INFO("Decrypted text in ascii is : " << toString(decrypted, ASCII));
    INFO("Plain text size is         : " << plainText15.size() << "B");
    INFO("Encrypted size is          : " << encrypted.size() << "B");
    REQUIRE(toString(encrypted) == "F79D8580C9DB776B88C0033420EED401");
    REQUIRE(toString(decrypted) == toString(plainText15));
}

TEST_CASE_METHOD(AESFixture, "AES-192 Key scheduler", "[encryption][aes]") {
    REQUIRE(toString(key192.getRoundKey(0))  == "000102030405060708090A0B0C0D0E0F");
    REQUIRE(toString(key192.getRoundKey(1))  == "10111213141516175846F2F95C43F4FE");
    REQUIRE(toString(key192.getRoundKey(2))  == "544AFEF55847F0FA4856E2E95C43F4FE");
    REQUIRE(toString(key192.getRoundKey(3))  == "40F949B31CBABD4D48F043B810B7B342");
    REQUIRE(toString(key192.getRoundKey(4))  == "58E151AB04A2A5557EFFB5416245080C");
    REQUIRE(toString(key192.getRoundKey(5))  == "2AB54BB43A02F8F662E3A95D66410C08");
    REQUIRE(toString(key192.getRoundKey(6))  == "F501857297448D7EBDF1C6CA87F33E3C");
    REQUIRE(toString(key192.getRoundKey(7))  == "E510976183519B6934157C9EA351F1E0");
    REQUIRE(toString(key192.getRoundKey(8))  == "1EA0372A995309167C439E77FF12051E");
    REQUIRE(toString(key192.getRoundKey(9))  == "DD7E0E887E2FFF68608FC842F9DCC154");
    REQUIRE(toString(key192.getRoundKey(10)) == "859F5F237A8D5A3DC0C02952BEEFD63A");
    REQUIRE(toString(key192.getRoundKey(11)) == "DE601E7827BCDF2CA223800FD8AEDA32");
    REQUIRE(toString(key192.getRoundKey(12)) == "A4970A331A78DC09C418C271E3A41D5D");
}

TEST_CASE_METHOD(AESFixture, "AES-256 Key scheduler", "[encryption][aes]") {
    REQUIRE(toString(key256.getRoundKey(0))  == "000102030405060708090A0B0C0D0E0F");
    REQUIRE(toString(key256.getRoundKey(1))  == "101112131415161718191A1B1C1D1E1F");
    REQUIRE(toString(key256.getRoundKey(2))  == "A573C29FA176C498A97FCE93A572C09C");
    REQUIRE(toString(key256.getRoundKey(3))  == "1651A8CD0244BEDA1A5DA4C10640BADE");
    REQUIRE(toString(key256.getRoundKey(4))  == "AE87DFF00FF11B68A68ED5FB03FC1567");
    REQUIRE(toString(key256.getRoundKey(5))  == "6DE1F1486FA54F9275F8EB5373B8518D");
    REQUIRE(toString(key256.getRoundKey(6))  == "C656827FC9A799176F294CEC6CD5598B");
    REQUIRE(toString(key256.getRoundKey(7))  == "3DE23A75524775E727BF9EB45407CF39");
    REQUIRE(toString(key256.getRoundKey(8))  == "0BDC905FC27B0948AD5245A4C1871C2F");
    REQUIRE(toString(key256.getRoundKey(9))  == "45F5A66017B2D387300D4D33640A820A");
    REQUIRE(toString(key256.getRoundKey(10)) == "7CCFF71CBEB4FE5413E6BBF0D261A7DF");
    REQUIRE(toString(key256.getRoundKey(11)) == "F01AFAFEE7A82979D7A5644AB3AFE640");
    REQUIRE(toString(key256.getRoundKey(12)) == "2541FE719BF500258813BBD55A721C0A");
    REQUIRE(toString(key256.getRoundKey(13)) == "4E5A6699A9F24FE07E572BAACDF8CDEA");
    REQUIRE(toString(key256.getRoundKey(14)) == "24FC79CCBF0979E9371AC23C6D68DE36");
}

TEST_CASE_METHOD(AESFixture, "AES encryption/decryption", "[encryption][aes]") {
    SECTION("Encryption/decryption from AES-192 class") {
        auto ciphertextBitset = aes192.encrypt(plainTextNist);
        auto ciphertext = toByteArray(ciphertextBitset);

        REQUIRE(toString(ciphertext) == "DDA97CA4864CDFE06EAF70A0EC0D7191");

        auto decrypted = aes192.decrypt(ciphertextBitset);

        REQUIRE(decrypted == plainTextNist);
    }

    SECTION("AES-128") {
        BlockCipher cipher(aes128, ECB<AES<128>>(), PKCS7Padding<AES<128>::BLOCK_SIZE>());
        
        auto ciphertext = cipher.encrypt(plainText34);
        REQUIRE(toString(ciphertext) == "F4C7B19121C2EE6D40293BFAD3574A2B6C496D14AECB2B2B55A8B2FBA1A6BC7E729B749B93F0300A65BE6541328B96BA");
        
        auto decrypted = cipher.decrypt(ciphertext);
        REQUIRE(toString(decrypted) == toString(plainText34));
    }

    SECTION("AES-192") {
        BlockCipher cipher(aes192, ECB<AES<192>>(), PKCS7Padding<AES<192>::BLOCK_SIZE>());
        
        auto ciphertext = cipher.encrypt(plainText34);
        REQUIRE(toString(ciphertext) == "94FC8C39E1930A9E0B50CCA23BC134AE028A881427D6EE98214B24A44FF24B156A38BA577AB7902858E8F03B9902C47E");
        
        auto decrypted = cipher.decrypt(ciphertext);
        REQUIRE(toString(decrypted) == toString(plainText34));
    }

    SECTION("AES-256") {
        BlockCipher cipher(aes256, ECB<AES<256>>(), PKCS7Padding<AES<256>::BLOCK_SIZE>());
        
        auto ciphertext = cipher.encrypt(plainText34);
        REQUIRE(toString(ciphertext) == "62A5B9021B1D9BE2FB9A50CE4E0EB417705E50620B4713D7690B2BD410A2F566E8CF5533657CDA6C45F5091A6CDC1577");
        
        auto decrypted = cipher.decrypt(ciphertext);
        REQUIRE(toString(decrypted) == toString(plainText34));
    }
}

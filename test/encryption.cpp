#include <catch2/catch_test_macros.hpp>
#include "DES.h"
#include "BlockCipher.h"
#include "ECB.h"
#include "CBC.h"
#include "CFB.h"
#include "PKCS5Padding.h"
#include "NoPadding.h"
#include "ZeroPadding.h"
#include "bytes_stream.h"
#include "DESede.h"

struct EncryptionFixture {
    ByteArray<15> plainText15 = toByteArrayFromAscii("gabriel le goat");
    ByteArray<24> plainText24 = toByteArrayFromAscii("No icecream for you... !");
    ByteArray<12> plainText12WithZerosAtTheEnd = toByteArray("4e863c9314ddab3e6f000000");
    ByteArray<8>  iv64        = toByteArray("0123456789abcdef");
};

struct DESFixture : public EncryptionFixture {

    DES::SecretKey key;
    DES des;

    DESFixture() {
        ByteArray<DES::SecretKey::KEY_SIZE_BYTES> rawKey = toByteArray("133457799bbcdff1");
        
        key = DES::SecretKey(rawKey);
        des.setKey(key);
    }
};

struct DESedeFixture : public EncryptionFixture {
    DESede::SecretKey key;
    DESede desede;

    DESedeFixture() {
        std::array<std::byte, 24> rawKey = toByteArray("6865667a646e73636a6166706e776665316361357833656d");

        key = DESede::SecretKey(rawKey);
        desede.setKey(key);
    }
};

TEST_CASE_METHOD(DESFixture, "DES/ECB/PKCS5Padding", "[encryption]") {
    BlockCipher cipher(des, ECB<DES>(), PKCS5Padding());
    auto encrypted = cipher.encrypt(plainText15);
    auto decrypted = cipher.decrypt(encrypted);
    
    REQUIRE(toString(encrypted) == "F6F2AE257F718E340894D2D97B1CD400");
    REQUIRE(toString(decrypted) == toString(plainText15));
}

TEST_CASE_METHOD(DESFixture, "DES/ECB/NoPadding", "[encryption]") {
    BlockCipher cipher(des, ECB<DES>(), NoPadding());

    auto encrypted = cipher.encrypt(plainText24);
    REQUIRE(toString(encrypted) == "AAAB52782D91889AC9EF12D1EB322671D541ADCD73CE97BD");

    auto decrypted = cipher.decrypt(encrypted);
    REQUIRE(toString(decrypted) == toString(plainText24));
}

TEST_CASE_METHOD(DESFixture, "DES/ECB/ZeroPadding", "[encryption]") {
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
TEST_CASE_METHOD(DESFixture, "DES/ECB/PKCS7Padding", "[encryption]") {
    BlockCipher cipher(des, ECB<DES>(), PKCS7Padding<DES::BLOCK_SIZE>());
    auto encrypted = cipher.encrypt(plainText15);
    auto decrypted = cipher.decrypt(encrypted);
    
    REQUIRE(toString(encrypted) == "F6F2AE257F718E340894D2D97B1CD400");
    REQUIRE(toString(decrypted) == toString(plainText15));
}

TEST_CASE_METHOD(DESFixture, "DES/CBC/PKCS5Padding", "[encryption]") {
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

TEST_CASE_METHOD(DESFixture, "DES/CFB/NoPadding", "[encryption]") {
    CFB<DES> cfb;
    cfb.setIV(iv64);
    BlockCipher cipher(des, cfb, NoPadding());

    auto encrypted = cipher.encrypt(plainText24);
    REQUIRE(toString(encrypted) == "CB87333D6C6FD777DEF9C81397CE751FFA0AC4A797A36675");

    auto decrypted = cipher.decrypt(encrypted);
    REQUIRE(toString(decrypted) == toString(plainText24));
}

TEST_CASE_METHOD(DESedeFixture, "DESede/ECB/PKCS5Padding", "[encryption]") {
    BlockCipher cipher(desede, ECB<DESede>(), PKCS5Padding());
    auto encrypted = cipher.encrypt(plainText15);
    auto decrypted = cipher.decrypt(encrypted);

    INFO("Decrypted text in ascii is : " << toString(decrypted, ASCII));
    INFO("Plain text size is         : " << plainText15.size() << "B");
    INFO("Encrypted size is          : " << encrypted.size() << "B");
    REQUIRE(toString(encrypted) == "F79D8580C9DB776B88C0033420EED401");
    REQUIRE(toString(decrypted) == toString(plainText15));
}
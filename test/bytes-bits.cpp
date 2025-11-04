#include <catch2/catch_test_macros.hpp>

#include "bytes.h"
#include "bytes_stream.h"
#include "galois.h"

TEST_CASE("Bits methods", "[utils:bits]") {
    std::bitset<8> byte(0b11101101);

    SECTION("slice") {
        REQUIRE(slice<byte.size()>(byte)      == byte);
        REQUIRE(slice<5>(byte).to_string()    == "11101");
        REQUIRE(slice<3>(byte, 2).to_string() == "101");
    }

    SECTION("lpad") {
        REQUIRE(lpad<0>(byte)                 == byte);
        REQUIRE(lpad<3>(byte).to_string()     == "00011101101");
    }

    SECTION("rpad") {
        REQUIRE(rpad<0>(byte)                 == byte);
        REQUIRE(rpad<3>(byte).to_string()     == "11101101000");
    }

    SECTION("pad") {
        REQUIRE(pad<0, 0>(byte)             == byte);
        REQUIRE(pad<3, 0>(byte)             == lpad<3>(byte));
        REQUIRE(pad<0, 3>(byte)             == rpad<3>(byte));
        REQUIRE(pad<3, 3>(byte).to_string() == "00011101101000");
    }

    SECTION("increment") {
        REQUIRE(increment(byte).to_string() == "11101110");
        REQUIRE(increment(byte).to_string() == "11101111");
        REQUIRE(increment(byte).to_string() == "11110000");
    }
}

TEST_CASE("mapForEachBlock", "[utils:bytes]") {
    
    SECTION("no remaining block") {
        ByteArray<4> bytes = toByteArrayFromHexa("01234567");
        auto result = mapForEachBlock<16>(bytes, [](std::bitset<16> c) {
            return ~c;
        });
    
        REQUIRE(toString(result) == "FEDCBA98");
    }

    SECTION("remaining block") {
        ByteArray<5> bytes = toByteArrayFromHexa("0123456789");
        auto result = mapForEachBlock<16>(bytes, [](std::bitset<16> c) {
            return ~c;
        }, [](std::bitset<16> c, size_t b) {
            return ~c;
        });
    
        REQUIRE(toString(result) == "FEDCBA9876");
    }
}

TEST_CASE("multiplication in GF(256)", "[utils:galois]") {
    using b = std::byte;

    SECTION("By 2") {
        REQUIRE(GF8::multiply(b(  1), b(  2)) == b(  2));
        REQUIRE(GF8::multiply(b(128), b(  2)) == b( 27));
        REQUIRE(GF8::multiply(b( 87), b(  2)) == b(174));
        REQUIRE(GF8::multiply(b(193), b(  2)) == b(153));
    }

    SECTION("By 3") {
        REQUIRE(GF8::multiply(b(  1), b(  3)) == b(  3));
        REQUIRE(GF8::multiply(b( 87), b(  3)) == b(249));
        REQUIRE(GF8::multiply(b(131), b(  3)) == b(158));
        REQUIRE(GF8::multiply(b(193), b(  3)) == b( 88));
    }

    SECTION("By 9") {
        REQUIRE(GF8::multiply(b(  1), b(  9)) == b(  9));
        REQUIRE(GF8::multiply(b( 87), b(  9)) == b(217));
        REQUIRE(GF8::multiply(b(131), b(  9)) == b(247));
        REQUIRE(GF8::multiply(b(193), b(  9)) == b(147));
    }

    SECTION("By 11") {
        REQUIRE(GF8::multiply(b(  1), b( 11)) == b( 11));
        REQUIRE(GF8::multiply(b( 87), b( 11)) == b(119));
        REQUIRE(GF8::multiply(b(131), b( 11)) == b(234));
        REQUIRE(GF8::multiply(b(193), b( 11)) == b( 10));
    }

    SECTION("By 13") {
        REQUIRE(GF8::multiply(b(  1), b( 13)) == b( 13));
        REQUIRE(GF8::multiply(b( 87), b( 13)) == b(158));
        REQUIRE(GF8::multiply(b(131), b( 13)) == b(205));
        REQUIRE(GF8::multiply(b(193), b( 13)) == b(186));
    }

    SECTION("By 14") {
        REQUIRE(GF8::multiply(b(  1), b( 14)) == b( 14));
        REQUIRE(GF8::multiply(b( 87), b( 14)) == b(103));
        REQUIRE(GF8::multiply(b(131), b( 14)) == b( 83));
        REQUIRE(GF8::multiply(b(193), b( 14)) == b(226));
    }
}

TEST_CASE("multiplication in GF(2^128)", "[utils:galois]") {

    ByteArray<GF128::SIZE> left     = toByteArrayFromHexa("66e94bd4ef8a2c3b884cfa59ca342b2e");
    ByteArray<GF128::SIZE> right    = toByteArrayFromHexa("0388dace60b6a392f328c2b971b2fe78");
    ByteArray<GF128::SIZE> expected = toByteArrayFromHexa("5e2ec746917062882c85b0685353deb7");
    ByteArray<GF128::SIZE> result   = GF128::multiply(left, right);

    REQUIRE(toString(result) == toString(expected));
}
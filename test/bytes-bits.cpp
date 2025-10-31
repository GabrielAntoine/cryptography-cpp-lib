#include <catch2/catch_test_macros.hpp>

#include "bytes.h"
#include "bytes_stream.h"

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
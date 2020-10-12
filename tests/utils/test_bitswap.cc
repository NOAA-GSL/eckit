/*
 * (C) Copyright 2020- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <arpa/inet.h>
#include <vector>

#include "eckit/log/Log.h"
#include "eckit/types/Types.h"
#include "eckit/utils/BitSwap.h"

#include "eckit/testing/Test.h"

using namespace std;

using namespace eckit;
using namespace eckit::testing;


namespace eckit {
namespace test {

//----------------------------------------------------------------------------------------------------------------------

#if ECKIT_LITTLE_ENDIAN  // use htons and htonl as correctness tests, and test against specifc bit patterns

CASE("Check correctness 16 bit swap") {

    uint16_t v = 511;
    std::cout << v << " = " << bits_to_str(v) << std::endl;
    EXPECT_EQUAL(bits_to_str(v), "0000000111111111");

    uint16_t r = htons(v);
    std::cout << r << " = " << bits_to_str(r) << std::endl;
    EXPECT_EQUAL(bits_to_str(r), "1111111100000001");

    uint16_t s = eckit::bitswap(v);
    std::cout << s << " = " << bits_to_str(s) << std::endl;
    EXPECT_EQUAL(bits_to_str(s), "1111111100000001");
}

CASE("Check correctness 32 bit swap") {

    // uint32_t v = 511;
    // std::cout << v << " = " << bits_to_str(v) << std::endl;
    // EXPECT_EQUAL(bits_to_str(v), "0000000111111111");

    // uint32_t r = htonl(v);
    // std::cout << r << " = " << bits_to_str(r) << std::endl;
    // EXPECT_EQUAL(bits_to_str(r), "1111111100000001");

    // uint32_t s = eckit::bitswap(v);
    // std::cout << s << " = " << bits_to_str(s) << std::endl;
    // EXPECT_EQUAL(bits_to_str(s), "1111111100000001");

}

#endif

//----------------------------------------------------------------------------------------------------------------------

CASE("Low-level roundtrip 8 bits") {
    unsigned char v = 't';
    uint16_t s      = eckit::bitswap8(v);
    // std::cout << s << std::endl;
    EXPECT(s != v);

    uint16_t r = eckit::bitswap8(s);
    EXPECT(r == v);
}

CASE("Low-level roundtrip 16 bits") {

    uint16_t v = 123;
    uint16_t s = eckit::bitswap16(v);
    // std::cout << s << std::endl;
    EXPECT(s == 31488);
    uint16_t r = eckit::bitswap16(s);
    EXPECT(r == v);
}

CASE("Low-level roundtrip 32 bits") {

    uint32_t v = 1234;

    uint32_t s = eckit::bitswap32(v);
    std::cout << s << std::endl;
    EXPECT(s == 3523477504);

    uint32_t r = eckit::bitswap32(s);
    EXPECT(r == v);
}

CASE("Low-level roundtrip 64 bits") {

    uint64_t v = 12345;

    uint64_t s = eckit::bitswap64(v);
    std::cout << s << std::endl;
    EXPECT(s == 4120793659044003840);

    uint64_t r = eckit::bitswap64(s);
    EXPECT(r == v);
}

//----------------------------------------------------------------------------------------------------------------------

CASE("BitSwap short") {
    short v = 3145;
    std::cout << v << " = " << bits_to_str(v) << std::endl;

    short s = eckit::bitswap(v);
    std::cout << s << " = " <<  bits_to_str(s) << std::endl;
    EXPECT(s == 18700);

    short r = eckit::bitswap(s);
    std::cout << r << " = " << bits_to_str(r) << std::endl;
    EXPECT(r == v);
}

//----------------------------------------------------------------------------------------------------------------------


template <typename T>
std::vector<T> get_ref() {
    std::vector<T> ref(1024);
    for( size_t i=0; i<ref.size(); ++i ) {
        ref[i] = T(i);
    }
    return ref;
}

template< typename T> 
void test_roundtrip() {
    const auto ref = get_ref<T>();
    auto v = ref;

    // First roundtrip with array API
    eckit::bitswap(v);
    std::cout << "v[512] " << v[512] << std::endl;
    EXPECT(v != ref);
    eckit::bitswap(v.data(), v.size());
    EXPECT( v == ref );
    
    // // Second roundtrip with scalar API
    // for( auto& x : v ) {
    //   x = eckit::bitswap(x);
    // }
    // std::cout << "v[512] " << v[512] << std::endl;
    // EXPECT( v != ref );
    // eckit::bitswap(v);
    // EXPECT( v == ref );
}

CASE( "test_roundtrip" ) {
    SECTION("short") { test_roundtrip<short>(); }
    SECTION("int") { test_roundtrip<int>(); }
    SECTION("unsigned int") { test_roundtrip<unsigned int>(); }
    SECTION("long") { test_roundtrip<long>(); }
    SECTION("float") { test_roundtrip<float>(); }
    SECTION("double") { test_roundtrip<double>(); }
}

//----------------------------------------------------------------------------------------------------------------------

}  // namespace test
}  // namespace eckit


int main(int argc, char** argv) {
    return run_tests(argc, argv);
}
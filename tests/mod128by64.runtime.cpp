#include "imath.h"
#include "catch2/catch_test_macros.hpp"

#include <cstdint>
#include <random>

using u64 = uint64_t;
using u128s = imath::detail::u128;

u64 mod128by64builtin(u128s n, u64 mod) {
#if defined(__SIZEOF_INT128__)
        __uint128_t p{n.hi};
        p <<= 64;
        p |= n.lo;
        return static_cast<uint64_t>(p % mod);
#elif defined(_M_X64)
    u64 result;
    (void)_udiv128(n.hi, n.lo, mod, &result);
    return result;
#else
#error "Can't find builtin way to divide 128bit by 64 bit uint"
#endif
}

TEST_CASE( "Modulo 128 bit by 64 bit Fallback randomized", "[mod128by64f]" ) {
    std::minstd_rand rng{};
    auto gen_u64 = [&rng]() { return (u64{rng()} << 32) | rng(); };
    for (int test_case = 0; test_case < 1024; ++test_case) {
        u64 a1 = gen_u64();
        u64 a2 = gen_u64();
        u64 b = gen_u64();
        a1 %= b;
        u128s a = {a1, a2};
        INFO("a1 = " << a1 << ", a2 = " << a2 << ", b = " << b);

        u64 builtin_result = mod128by64builtin(a, b);
        u64 fallback_result = imath::detail::mod128by64Fallback(a, b);

        CHECK(builtin_result == fallback_result);
    }
}

TEST_CASE( "Modulo 128 bit by 64 bit Fallback randomized small", "[mod128by64f]" ) {
    std::minstd_rand rng{};
    auto gen_u64 = [&rng]() { return (u64{rng()} << 32) | rng(); };
    for (int test_case = 0; test_case < 1024; ++test_case) {
        u128s a = {0 , gen_u64()};
        u64 b = rng();

        INFO("a = " << a.lo << ", b = " << b);

        u64 builtin_result = mod128by64builtin(a, b);
        u64 fallback_result = imath::detail::mod128by64Fallback(a, b);

        CHECK(builtin_result == fallback_result);
    }
}

TEST_CASE( "Modulo 128 bit by 64 bit Fallback randomized empty lower bits", "[mod128by64f]" ) {
    std::minstd_rand rng{};
    auto gen_u64 = [&rng]() { return (u64{rng()} << 32) | rng(); };
    for (int test_case = 0; test_case < 1024; ++test_case) {
        u128s a = {gen_u64(), 0};
        u64 b = rng();
        a.hi %= b;

        INFO("a = " << a.hi << " * 2^64, b = " << b);

        u64 builtin_result = mod128by64builtin(a, b);
        u64 fallback_result = imath::detail::mod128by64Fallback(a, b);

        REQUIRE(builtin_result == fallback_result);
    }
}

#include "imath.h"
#include "catch2/catch_test_macros.hpp"

#include <cstdint>
#include <random>

using u64 = uint64_t;
using u128s = imath::detail::u128;

u128s mul64by64builtin(u64 a, u64 b) {
#if defined(__SIZEOF_INT128__)
    __uint128_t result = (__uint128_t{a} * b);
    return u128s{static_cast<uint64_t>(result >> 64),
                static_cast<uint64_t>(result & static_cast<uint64_t>(-1))};
#elif defined(_M_X64) || defined(_M_ARM64) || defined(_M_IA64)
    u128s result{};
    result.lo = _umul128(a, b, &result.hi);
    return result;
#else
#error "Can't find builtin way to multiply 2 64bit uints"
#endif
}


TEST_CASE( "Multiply 64 bit by 64 bit Fallback randomized", "[mul64by64f]" ) {
    std::minstd_rand rng{};
    auto gen_u64 = [&rng]() { return (u64{rng()} << 32) | rng(); };
    for (int test_case = 0; test_case < 1024; ++test_case) {
        u64 a = gen_u64();
        u64 b = gen_u64();
        INFO("a = " << a << ", b = " << b);

        u128s builtin_result = mul64by64builtin(a, b);
        u128s fallback_result = imath::detail::mul64x64Fallback(a, b);

        REQUIRE(builtin_result.hi == fallback_result.hi);
        REQUIRE(builtin_result.lo == fallback_result.lo);
    }
}

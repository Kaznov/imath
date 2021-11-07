#include "imath.h"
#include "catch2/catch_test_macros.hpp"

uint32_t constexpr operator"" _u32(unsigned long long n) {
    return static_cast<uint32_t>(n);
}
uint64_t constexpr operator"" _u64(unsigned long long n) {
    return static_cast<uint64_t>(n);
}

TEST_CASE( "CTZ32 fallback powers of 2", "[ctzf32]" ) {
    CHECK(imath::detail::countTrailingZeroesFallback(0_u32) == 32);
    CHECK(imath::detail::countTrailingZeroesFallback(1_u32) == 0);
    for (int i = 1; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::countTrailingZeroesFallback(n) == i);
    }
}

TEST_CASE( "CTZ64 fallback powers of 2", "[ctzf64]" ) {
    CHECK(imath::detail::countTrailingZeroesFallback(0_u64) == 64);
    CHECK(imath::detail::countTrailingZeroesFallback(1_u64) == 0);
    for (int i = 1; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::countTrailingZeroesFallback(n) == i);
    }
}

TEST_CASE( "CTZ32 fallback powers of 2 minus 1", "[ctzf32]" ) {
    for (int i = 2; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::countTrailingZeroesFallback(n - 1) == 0);
    }
}

TEST_CASE( "CTZ64 fallback powers of 2 minus 1", "[ctzf64]" ) {
    for (int i = 2; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::countTrailingZeroesFallback(n - 1) == 0);
    }
}

TEST_CASE( "CTZ32 powers of 2", "[ctz32]" ) {
    CHECK(imath::detail::countTrailingZeroes(0_u32) == 32);
    CHECK(imath::detail::countTrailingZeroes(1_u32) == 0);
    for (int i = 1; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::countTrailingZeroes(n) == i);
    }
}

TEST_CASE( "CTZ64 powers of 2", "[ctz64]" ) {
    CHECK(imath::detail::countTrailingZeroes(0_u64) == 64);
    CHECK(imath::detail::countTrailingZeroes(1_u64) == 0);
    for (int i = 1; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::countTrailingZeroes(n) == i);
    }
}

TEST_CASE( "CTZ32 powers of 2 minus 1", "[ctz32]" ) {
    for (int i = 2; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::countTrailingZeroes(n - 1) == 0);
    }
}

TEST_CASE( "CTZ64 powers of 2 minus 1", "[ctz64]" ) {
    for (int i = 2; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::countTrailingZeroes(n - 1) == 0);
    }
}

#include "imath.h"
#include "catch2/catch_test_macros.hpp"

uint32_t constexpr operator"" _u32(unsigned long long n) {
    return static_cast<uint32_t>(n);
}
uint64_t constexpr operator"" _u64(unsigned long long n) {
    return static_cast<uint64_t>(n);
}

TEST_CASE( "CLZ32 fallback powers of 2", "[clzf32]" ) {
    CHECK(imath::detail::clzFallback(0_u32) == 32);
    IMATHLIB_MSC_WARNING(4146);
    CHECK(imath::detail::clzFallback(-1_u32) == 0);
    for (int i = 0; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::clzFallback(n) == 32 - i - 1);
    }
}

TEST_CASE( "CLZ64 fallback powers of 2", "[clzf64]" ) {
    CHECK(imath::detail::clzFallback(0_u64) == 64);
    IMATHLIB_MSC_WARNING(4146);
    CHECK(imath::detail::clzFallback(-1_u64) == 0);
    for (int i = 0; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::clzFallback(n) == 64 - i - 1);
    }
}

TEST_CASE( "CLZ32 fallback powers of 2 minus 1", "[clzf32]" ) {
    for (int i = 1; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::clzFallback(n - 1) == 32 - i);
    }
}

TEST_CASE( "CLZ64 fallback powers of 2 minus 1", "[clzf64]" ) {
    for (int i = 2; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::clzFallback(n - 1) == 64 - i);
    }
}

TEST_CASE( "CLZ32 powers of 2", "[clz32]" ) {
    CHECK(imath::detail::clz(0_u32) == 32);
    IMATHLIB_MSC_WARNING(4146);
    CHECK(imath::detail::clz(-1_u64) == 0);
    for (int i = 1; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::clz(n) == 32 - i - 1);
    }
}

TEST_CASE( "CLZ64 powers of 2", "[clz64]" ) {
    CHECK(imath::detail::clz(0_u64) == 64);
    IMATHLIB_MSC_WARNING(4146);
    CHECK(imath::detail::clz(-1_u64) == 0);
    for (int i = 1; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::clz(n) == 64 - i - 1);
    }
}

TEST_CASE( "CLZ32 powers of 2 minus 1", "[clz32]" ) {
    for (int i = 2; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::clz(n - 1) == 32 - i);
    }
}

TEST_CASE( "CLZ64 powers of 2 minus 1", "[clz64]" ) {
    for (int i = 2; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::clz(n - 1) == 64 - i);
    }
}

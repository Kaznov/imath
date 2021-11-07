#include "imath.h"
#include "catch2/catch_test_macros.hpp"

#include <utility>

TEST_CASE( "CLZ32 fallback powers of 2", "[clzf32]" ) {
    CHECK(imath::detail::countLeadingZeroesFallback(0u) == 32);
    IMATHLIB_MSC_WARNING(4146);
    CHECK(imath::detail::countLeadingZeroesFallback(-1u) == 0);
    for (int i = 0; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::countLeadingZeroesFallback(n) == 32 - i - 1);
    }
}

TEST_CASE( "CLZ64 fallback powers of 2", "[clzf64]" ) {
    CHECK(imath::detail::countLeadingZeroesFallback(0ull) == 64);
    IMATHLIB_MSC_WARNING(4146);
    CHECK(imath::detail::countLeadingZeroesFallback(-1ull) == 0);
    for (int i = 0; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::countLeadingZeroesFallback(n) == 64 - i - 1);
    }
}

TEST_CASE( "CLZ32 fallback powers of 2 minus 1", "[clzf32]" ) {
    for (int i = 1; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::countLeadingZeroesFallback(n - 1) == 32 - i);
    }
}

TEST_CASE( "CLZ64 fallback powers of 2 minus 1", "[clzf64]" ) {
    for (int i = 2; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::countLeadingZeroesFallback(n - 1) == 64 - i);
    }
}

TEST_CASE( "CLZ32 powers of 2", "[clz32]" ) {
    CHECK(imath::detail::countLeadingZeroes(0u) == 32);
    IMATHLIB_MSC_WARNING(4146);
    CHECK(imath::detail::countLeadingZeroes(-1u) == 0);
    for (int i = 1; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::countLeadingZeroes(n) == 32 - i - 1);
    }
}

TEST_CASE( "CLZ64 powers of 2", "[clz64]" ) {
    CHECK(imath::detail::countLeadingZeroes(0ull) == 64);
    IMATHLIB_MSC_WARNING(4146);
    CHECK(imath::detail::countLeadingZeroes(-1ull) == 0);
    for (int i = 1; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i);
        CHECK(imath::detail::countLeadingZeroes(n) == 64 - i - 1);
    }
}

TEST_CASE( "CLZ32 powers of 2 minus 1", "[clz32]" ) {
    for (int i = 2; i < 32; ++i) {
        uint32_t n = 1u << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::countLeadingZeroes(n - 1) == 32 - i);
    }
}

TEST_CASE( "CLZ64 powers of 2 minus 1", "[clz64]" ) {
    for (int i = 2; i < 64; ++i) {
        uint64_t n = 1ull << i;
        INFO("n = 1 << " << i << " - 1");
        CHECK(imath::detail::countLeadingZeroes(n - 1) == 64 - i);
    }
}

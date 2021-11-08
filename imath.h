
//                   Copyright(c) Kamil Kaznowski 2021.
//         Distributed under the Boost Software License, Version 1.0.
//                (See accompanying file LICENSE or copy at
//                  https://www.boost.org/LICENSE_1_0.txt)

////////////////////////////////////////////////////////////////////////////////
////////////////                    imath                       ////////////////
////////////////   Library with common operations on integers   ////////////////
////////////////////////////////////////////////////////////////////////////////


#ifndef IMATHLIB_IMATH_H
#define IMATHLIB_IMATH_H

#ifdef _MSVC_LANG
#define IMATHLIB_CPP_VER _MSVC_LANG
#else
#define IMATHLIB_CPP_VER __cplusplus
#endif

static_assert(IMATHLIB_CPP_VER >= 201402L, "C++14 support is needed");
static_assert(sizeof(long long) == 8, "long long must be 8 bytes");
static_assert(-2 >> 1 == -1,
              "right shift on signed integers must be arithmetic (sar)");
static_assert(-2ll >> 1 == -1ll,
              "right shift on signed integers must be arithmetic (sar)");

#include <type_traits>
#include <utility>
#include <cmath>
#include <cstdint>

static_assert(static_cast<int32_t>(uint32_t{4294967295u}) == -1,
              "Integers must be 2's complement and static_cast uint->int "
              "must be well defined as congruent mod to 2^(8 * sizeof(...))");

#if IMATHLIB_CPP_VER >= 202002L
#include <bit>
#endif

#if defined(_MSC_VER)
#include <intrin.h>
#include <immintrin.h>
#endif

#if __cpp_lib_is_constant_evaluated
#define IMATHLIB_CONSTEXPR20 constexpr
#define IMATHLIB_HAS_CONSTEXPR20 1
#define IMATHLIB_IS_CONSTEVAL std::is_constant_evaluated()
#else
#define IMATHLIB_CONSTEXPR20 inline
#define IMATHLIB_IS_CONSTEVAL 0
#endif

#if IMATHLIB_HAS_CONSTEXPR20 || !defined(_MSC_VER)
// Support for intrinsics in constexpr or C++20 is_consteval
#define IMATHLIB_CONSTEXPR_INTR constexpr
#define IMATHLIB_HAS_CONSTEXPR_INTR 1
#else
#define IMATHLIB_CONSTEXPR_INTR inline
#endif

// In case u128 % u128 provided with your standard library is slow,
// (like in very old libc++), you may want to change this flag to 0.
// It will disable a lot of constexpr, but will use inline assembly for
// 128bit % 64bit modulo.
#define IMATHLIB_FAST_LIBRARY_MODULO 1

#if IMATHLIB_HAS_CONSTEXPR20 || \
    IMATHLIB_HAS_CONSTEXPR_INTR && defined(__SIZEOF_INT128__) &&\
    IMATHLIB_FAST_LIBRARY_MODULO
// Constexpr support for both intrinsics and __uint128_t
#define IMATHLIB_CONSTEXPR_X64 constexpr
#define IMATHLIB_HAS_CONSTEXPR_X64 1
#else
#define IMATHLIB_CONSTEXPR_X64 inline
#endif


#if !defined(IMATHLIB_ASSERT)
#if IMATHLIB_DEBUG
#include <cassert>
#define IMATHLIB_ASSERT(...) assert(__VA_ARGS__)
#else
#define IMATHLIB_ASSERT(...) void()
#endif
#endif  // !defined(IMATHLIB_ASSERT)

#if !defined(IMATHLIB_ASSUME)
#if IMATHLIB_DEBUG
#include <cassert>
#define IMATHLIB_ASSUME(...) assert(__VA_ARGS__)
#else
#if defined(__GNUG__) || defined(__clang__)
#define IMATHLIB_ASSUME(...) \
    (static_cast<bool>(__VA_ARGS__) ? void() : __builtin_unreachable())
#elif defined(_MSC_VER)
#define IMATHLIB_ASSUME(...) \
    __assume(__VA_ARGS__)
#else
#define IMATHLIB_ASSUME(...) void()
#endif
#endif
#endif  // !defined(IMATHLIB_ASSUME)

#ifdef _MSC_VER
#define IMATHLIB_MSC_WARNING(id) __pragma(warning(suppress : id))
#else
#define IMATHLIB_MSC_WARNING(id)
#endif

namespace imath {

// This is the public interface of imath library.
// While it isn't stable between releases, it should work "without surprises".

IMATHLIB_CONSTEXPR_INTR bool isPrime(uint32_t n);
IMATHLIB_CONSTEXPR_X64 bool isPrime(uint64_t n);

IMATHLIB_CONSTEXPR_INTR uint32_t nextPrimeAfter(uint32_t n);
IMATHLIB_CONSTEXPR_X64 uint64_t nextPrimeAfter(uint64_t n);

struct FactorU32;
class FactorizationResultU32;
struct FactorU64;
class FactorizationResultU64;

IMATHLIB_CONSTEXPR_INTR FactorizationResultU32 factorize(uint32_t);
IMATHLIB_CONSTEXPR_X64 FactorizationResultU64 factorize(uint64_t);

template <size_t SIZE, typename T = uint32_t>
class PrimeArray;

constexpr uint32_t pow(uint32_t n, uint32_t pow);
constexpr uint64_t pow(uint64_t n, uint64_t pow);

constexpr uint32_t mulmod(uint32_t a, uint32_t b, uint32_t mod);
IMATHLIB_CONSTEXPR_X64 uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod);
constexpr uint32_t powmod(uint32_t n, uint32_t pow, uint32_t mod);
IMATHLIB_CONSTEXPR_X64 uint64_t powmod(uint64_t n, uint64_t pow, uint64_t mod);

IMATHLIB_CONSTEXPR_INTR uint32_t gcd(uint32_t a, uint32_t b);
IMATHLIB_CONSTEXPR_INTR uint64_t gcd(uint64_t a, uint64_t b);
IMATHLIB_CONSTEXPR_INTR uint32_t lcm(uint32_t a, uint32_t b);
IMATHLIB_CONSTEXPR_INTR uint64_t lcm(uint64_t a, uint64_t b);

constexpr uint32_t roundUpToMultipleOf(uint32_t n, uint32_t mul);
constexpr uint64_t roundUpToMultipleOf(uint64_t n, uint64_t mul);
constexpr uint32_t roundDownToMultipleOf(uint32_t n, uint32_t mul);
constexpr uint64_t roundDownToMultipleOf(uint64_t n, uint64_t mul);

// class MontgomerySpaceU32;
// class MontgomeryU32;
// class MontgomerySpaceU64;
// class MontgomeryU64;

IMATHLIB_CONSTEXPR20 bool isPerfectSquare(uint32_t n);
IMATHLIB_CONSTEXPR20 bool isPerfectSquare(uint64_t n);

// End of public interface

namespace detail {

struct u128 {
    uint64_t hi;
    uint64_t lo;
};

// To avoid including entire <algorithm> header:
template<typename T>
constexpr const T& min(const T& a, const T& b)
{
    return (a < b) ? a : b;
}
template<typename T>
constexpr const T& max(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

template<typename T>
constexpr void simpleSwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
constexpr void simpleSwapIf(T& a, T& b, bool cond) {
    T temp = a;
    a = cond ? b : a;
    b = cond ? temp : b;
}

constexpr uint32_t kDeBruijn32 =
    0b00000100011001010011101011011111u;
constexpr uint64_t kDeBruijn64 =
    0b0000001000011000101000111001001011001101001111010101110110111111ull;

/**
 * Perfect 2^n -> n mapping arrays for use with De Bruijn sequences above
 * http://supertech.csail.mit.edu/papers/debruijn.pdf
 * */
constexpr uint8_t power_of_2_lookup_array_64[64] {
     0,  1,  2,  7,  3, 13,  8, 19,  4, 25, 14, 28,  9, 34, 20, 40,
     5, 17, 26, 38, 15, 46, 29, 48, 10, 31, 35, 54, 21, 50, 41, 57,
    63,  6, 12, 18, 24, 27, 33, 39, 16, 37, 45, 47, 30, 53, 49, 56,
    62, 11, 23, 32, 36, 44, 52, 55, 61, 22, 43, 51, 60, 42, 59, 58
};
constexpr uint8_t power_of_2_lookup_array_32[32] {
     0,  1,  2,  6,  3, 11,  7, 16,  4, 14, 12, 21,  8, 23, 17, 26,
     31, 5, 10, 15, 13, 20, 22, 25, 30,  9, 19, 24, 29, 18, 28, 27
};

constexpr int clzFallback(uint64_t n) {
    if (n == 0) return 64;
    n = n | (n >>  1); // Propagate leftmost
    n = n | (n >>  2); // 1-bit to the right.
    n = n | (n >>  4);
    n = n | (n >>  8);
    n = n | (n >> 16);
    n = n | (n >> 32);
    n = n - (n >> 1);  // remove all bits but the leftmost
    // This magic number has bit representation that is a de Bruijn sequence.
    // Because of that it is a perfect hash for powers of 2.
    n *= detail::kDeBruijn64;
    n >>= 58;          // top 6 bits are the perfect hash
    return 63 - detail::power_of_2_lookup_array_64[n];
}

constexpr int clzFallback(uint32_t n) {
    if (n == 0) return 32;
    n = n | (n >>  1); // Propagate leftmost
    n = n | (n >>  2); // 1-bit to the right.
    n = n | (n >>  4);
    n = n | (n >>  8);
    n = n | (n >> 16);
    n = n - (n >> 1);  // remove all bits but the leftmost
    // This magic number has bit representation that is a de Bruijn sequence.
    // Because of that it is a perfect hash for powers of 2.
    n *= detail::kDeBruijn32;
    n >>= 27;          // top 5 bits are the perfect hash
    return 31 - detail::power_of_2_lookup_array_32[n];
}

constexpr int ctzFallback(uint64_t n) {
    if (n == 0) return 64;
IMATHLIB_MSC_WARNING(4146)
    n = n & (-n); // extract rightmost bit
    // This magic number, has bit representation that is a de Bruijn sequence.
    // Because of that it is a perfect hash for powers of 2.
    n *= detail::kDeBruijn64;
    n >>= 58;          // top 6 bits are the perfect hash
    return detail::power_of_2_lookup_array_64[n];
}

constexpr int ctzFallback(uint32_t n) {
    if (n == 0) return 32;
IMATHLIB_MSC_WARNING(4146)
    n = n & (-n); // extract rightmost bit
    // This magic number has bit representation that is a de Bruijn sequence.
    // Because of that it is a perfect hash for powers of 2.
    n *= kDeBruijn32;
    n >>= 27;          // top 5 bits are the perfect hash
    return detail::power_of_2_lookup_array_32[n];
}

IMATHLIB_CONSTEXPR_INTR int clz(uint64_t n) {
    if (IMATHLIB_IS_CONSTEVAL) {
        return clzFallback(n);
    }
#if (defined(__GNUG__) || defined(__clang__))
#if INTPTR_MAX == INT64_MAX
#define IMATHLIB_FAST_CLZ64
    if (n == 0) return 64;
    return __builtin_clzll(n);
#else
    return clzFallback(n);
#endif

#elif defined(_MSC_VER)
#if defined(_M_X64) || defined(_M_ARM64)
#define IMATHLIB_FAST_CLZ64
    // There is no good way to check for LZCNT instruction availability
    if (n == 0) return 64;
    unsigned long index = 0;
    (void)_BitScanReverse64(&index, n);
    return static_cast<int>(63 - index);
#else
    return clzFallback(n);
#endif

#elif IMATHLIB_CPP_VER >= 202002L
    return std::countl_zero(n);
#else
    return clzFallback(n);
#endif
}


IMATHLIB_CONSTEXPR_INTR int clz(uint32_t n) {
    if (IMATHLIB_IS_CONSTEVAL) {
        return clzFallback(n);
    }
    // There is CLZ instruction on 32-bit ARMv5 and above architectures,
    // So use builtins whenever possible. No checks for ARMs < 5,
    // cause it would require a lot of boilerplate
#if IMATHLIB_CPP_VER >= 202002L
#define IMATHLIB_FAST_CLZ32
    return std::countl_zero(n);
#elif defined(__GNUG__) || defined(__clang__)
#define IMATHLIB_FAST_CLZ32
    if (n == 0) return 32;
    return __builtin_clz(n);
#elif defined (_MSC_VER)
#define IMATHLIB_FAST_CLZ32
    // There is no good way to check for LZCNT instruction availability
    if (n == 0) return 32;
    unsigned long index = 0;
    (void)_BitScanReverse(&index, n);
    return static_cast<int>(31 - index);
#else
    clzFallback(n);
#endif
}


IMATHLIB_CONSTEXPR_INTR int ctz(uint64_t n) {
    if (IMATHLIB_IS_CONSTEVAL) {
        return ctzFallback(n);
    }
    // This one is tricky
    // We DON'T want intrinsics on 32-bit architectures
    // cause they emit very long code or function call.
#if (defined(__GNUG__) || defined(__clang__))
#if INTPTR_MAX == INT64_MAX
#define IMATHLIB_FAST_CTZ64
    if (n == 0) return 64;
    return __builtin_ctzll(n);
#else
    return ctzFallback(n);
#endif

#elif defined(_MSC_VER)
#if defined(_M_X64) || defined(_M_ARM64)
#define IMATHLIB_FAST_CTZ64
    // There is no good way to check for TZCNT instruction availability
    if (n == 0) return 64;
    unsigned long index = 0;
    (void)_BitScanForward64(&index, n);
    return static_cast<int>(index);
#else
    return ctzFallback(n);
#endif

#elif IMATHLIB_CPP_VER >= 202002L
    return std::countr_zero(n);

#else
    return ctzFallback(n);
#endif
}

IMATHLIB_CONSTEXPR_INTR int ctz(uint32_t n) {
    if (IMATHLIB_IS_CONSTEVAL) {
        return ctzFallback(n);
    }

#if (defined(__GNUG__) || defined(__clang__))
#define IMATHLIB_FAST_CTZ32
    if (n == 0) return 32;
    return __builtin_ctz(n);
#elif IMATHLIB_CPP_VER >= 202002L
    return std::countr_zero(n);
#elif defined(_MSC_VER) && !defined(_M_ARM)
#define IMATHLIB_FAST_CTZ32
    // There is no good way to check for TZCNT instruction availibility
    if (n == 0) return 32;
    unsigned long index = 0;
    (void)_BitScanForward(&index, n);
    return static_cast<int>(index);

#else
    return ctzFallback(n);
#endif
}

constexpr uint32_t gcdModuloRecursive(uint32_t a, uint32_t b) {
    if (b == 0) return a;
    return gcdModuloRecursive(b, a % b);
}

constexpr uint64_t gcdModuloRecursive(uint64_t a, uint64_t b) {
    if (b == 0) return a;
    return gcdModuloRecursive(b, a % b);
}

template <typename T>
IMATHLIB_CONSTEXPR_INTR T gcdBinary(T a, T b) {
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "Implementation bug - GCD must operate on unsigned");

    // https://en.wikipedia.org/wiki/Binary_GCD_algorithm
    if (a == 0) return b;
    if (b == 0) return a;

    int common_tz = detail::ctz(a | b);
    b >>= common_tz;

    // Do not change it to do-while loop! Clang gets weirdly confused
    // and checks for 0 twice, conditionaly moving 32/64 to eax/rax
    // for bsf. It seems to be a bug, and using while loop
    // doesn't change codegen for other compilers.
    while (a != 0) {
        // You say it doesn't make any sense to put this here.
        // And I would agree, but otherwise MSVC checks for 0 twice,
        // and performs an extra check for architeture each loop turn...
        // Reported as a bug 10-2021.
        IMATHLIB_ASSUME(a != 0);
        a >>= detail::ctz(a);
        // why not if() swap(); ? cause GCC can't generate CMOVs then
        detail::simpleSwapIf(a, b, a < b);
        a -= b;
    }

    return b << common_tz;
}

/**
 * Pre-computed magic array for faster MR prime test of 32-bit numbers.
 * Any 32-bit number coprime to 210 is assigned a hash value,
 * and based on it - a position in the following array.
 * If number in this array from the corresponding position is used
 * as a base for Miller-Rabin test, this single test will be sufficient
 * to correctly detect if the number is a prime or composite.
 *
 * Values calculated for hash:
 *
 *   uint32_t h = n;
 *   h = ((h >> 16) ^ h) * 0x979bc64f;
 *   h = ((h >> 16) ^ h) * 0x979bc64f;
 *   h = ((h >> 16) ^ h) & 255;
 *
 * Can be confirmed by brute-checking all 32-bit numbers.
 *
 * Idea from:
 * http://ceur-ws.org/Vol-1326/020-Forisek.pdf
 * */
constexpr uint16_t bases_prime_test_u32[256] {
    4718, 496, 49848, 7899, 9378, 6345, 445, 5874, 5974, 2979, 7007, 1450,
    2810, 4529, 5367, 4371, 1938, 1817, 2230, 303, 8022, 3065, 1016, 2636,
    266, 4283, 1621, 10756, 1925, 3393, 333, 1889, 221, 2522, 408, 5453,
    7401, 13090, 272, 15, 1098, 5474, 306, 2779, 2750, 1168, 2813, 5210,
    1184, 2936, 592, 13, 26448, 1650, 7332, 1069, 1738, 239, 3804, 603,
    838, 960, 990, 1950, 353, 20014, 10077, 811, 7695, 4367, 6160, 2026,
    2913, 2093, 3977, 1776, 523, 658, 2838, 799, 2406, 6682, 429, 349,
    3419, 394, 15585, 11938, 1839, 1537, 1641, 3454, 10830, 7324, 2622,
    3643, 394, 4469, 2203, 1803, 2803, 649, 8560, 1020, 3002, 4618, 935,
    4512, 4901, 5109, 2911, 5876, 7168, 4776, 1066, 589, 2127, 3218, 11763,
    847, 19054, 4289, 1055, 22513, 2130, 923, 3869, 1766, 4299, 4777, 670,
    14780, 794, 4777, 4090, 12342, 3519, 3804, 1304, 1974, 4528, 3473, 4124,
    31802, 2139, 14323, 7514, 497, 4666, 1275, 111, 3030, 3652, 6203, 666,
    3436, 2117, 8449, 2038, 21405, 5208, 855, 981, 8756, 3268, 1732, 3463,
    935, 1882, 6816, 4400, 27093, 10614, 13098, 3560, 978, 1733, 2862, 1672,
    2951, 4686, 8641, 2549, 209, 341, 139, 1606, 894, 1266, 1941, 85, 2778, 1748,
    8605, 3270, 643, 3557, 170, 7660, 7988, 3327, 7002, 8775, 120, 718, 6343, 3054,
    745, 15047, 7717, 3796, 3484, 1032, 15349, 1514, 1029, 2925, 24747, 2783, 677,
    5048, 460, 926, 17501, 24350, 18728, 2485, 14389, 86, 1580, 1184, 1346, 628,
    6383, 603, 6540, 3430, 544, 4002, 3760, 5088, 3494, 207, 106, 4615, 25007,
    4766, 9622, 5488, 292, 9512, 740, 1431, 1238, 3934, 1216
};

/**
 * Pre-computed magic array for faster MR prime test of 64-bit numbers.
 * Any 64-bit pseudo-prime number, passing the MR test for bases 2 and 15
 * is assigned a hash value and based on it - a position in the following array.
 * Values in the following array are 3 compressed bases, in the form:
 * [10bit base1 | 11bit base2 | 11bit base3]
 * If these bases from position corresponding to the number are used
 * for Miller-Rabin test, then these 5 tests total will be sufficient
 * to correctly detect if the number is a prime or composite.
 *
 * Values calculated for hash:
 *
 *   uint64_t h = n;
 *   h = ((h >> 32) ^ h) * 0x123456789abce1b;
 *   h = ((h >> 32) ^ h) * 0x123456789abce1b;
 *   h = ((h >> 32) ^ h);
 *
 * Can be confirmed by checking every number from the list
 * of all base-2 Fermat Pseudoprimes < 2^64:
 * http://www.cecm.sfu.ca/Pseudoprimes/index-2-to-64.html
 * Every Strongly Probable Prime to some base is also a Fermat Pseudoprime
 * to the same base. Since each test start by checking base 2 and 15,
 * checking numbers from this list will be sufficient to assure correctness.
 *
 * Idea from:
 * http://ceur-ws.org/Vol-1326/020-Forisek.pdf
 * */
constexpr uint32_t bases_prime_test_u64[128] {
    30330285, 47106639, 46413094, 30597089, 32685830, 48603013, 31731201,
    30882849, 32323190, 31809401, 32460779, 31018750, 46643634, 55569395,
    47527190, 31169886, 47035014, 46757681, 29665674, 46855109, 13282987,
    48000467, 48528943, 31677635, 32448203, 46492497, 54573569, 13344755,
    33025941, 31503603, 31366501, 16605162, 30593938, 54898543, 29594978,
    13397671, 54861751, 46811666, 46579609, 30071165, 47332791, 32083377,
    29414129, 49083953, 54851781, 31387055, 47854734, 46276271, 47281801,
    47168371, 46288613, 46411673, 46436729, 46593459, 48766345, 46839019,
    29805401, 30953201, 46366101, 46757450, 47656827, 46551453, 49248002,
    46325627, 47037614, 54785737, 29649726, 29438705, 30713225, 54769591,
    29387430, 47016861, 55525167, 30403346, 54652373, 47740057, 54787410,
    46173342, 47479489, 47512554, 55069605, 54776194, 29923315, 30091730,
    54595438, 47396503, 31403643, 47681117, 54909137, 47574678, 46378343,
    47380109, 46668438, 31235999, 46191638, 31346371, 29587573, 31905379,
    54573451, 46217313, 46792355, 30585869, 48133357, 47257067, 47098841,
    54795258, 29995981, 46946026, 29718225, 30979895, 46668518, 29966245,
    46995242, 30717277, 46806505, 46493615, 30845823, 47249078, 31161653,
    55328517, 29733086, 47443201, 56011898, 47602773, 30727243, 57425821,
    54771238, 54907391
};

/**
 * Miller-Rabin probabilistic test.
 * https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
 * */
IMATHLIB_CONSTEXPR_INTR
bool isSPRP(uint32_t n, uint32_t base) {
    uint32_t d = n - 1;
    int s = ctz(d);
    d >>= s;
    uint32_t cur = powmod(base, d, n);
    if (cur == 1) return true;
    for (int r = 0; r < s; r++) {
        if (cur == n - 1) return true;
        cur = mulmod(cur, cur, n);
    }
    return false;
}

/**
 * Miller-Rabin probabilistic test.
 * https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
 * */
IMATHLIB_CONSTEXPR_X64
bool isSPRP(uint64_t n, uint64_t base) {
    uint64_t d = n - 1;
    int s = ctz(d);
    d >>= s;
    uint64_t cur = powmod(base, d, n);
    if (cur == 1) return true;
    for (int r = 0; r < s; r++) {
        if (cur == n - 1) return true;
        cur = mulmod(cur, cur, n);
    }
    return false;
}

constexpr uint32_t pollardRhoPoly(uint32_t x, uint32_t mod) {
    return static_cast<uint32_t>((uint64_t{x} * x + 1) % mod);
}

/**
 * Pollard's Rho factorization algorithm
 * Returns one of the non-trivial divisors of n, or n on failure.
 * The algorithm may fail for composite numbers.
 * Returned divisor does not have to be a prime number.
 * */
IMATHLIB_CONSTEXPR_INTR
uint32_t pollardRhoFactorization(uint32_t n, uint32_t starting_value) {
    uint32_t turtle = starting_value;
    uint32_t hare = starting_value;
    uint32_t result = 1;
    while (result == 1) {
        turtle = pollardRhoPoly(turtle, n);
        hare = pollardRhoPoly(hare, n);
        hare = pollardRhoPoly(hare, n);
        auto diff = (turtle>hare) ? (turtle-hare) : (hare-turtle);
        result = gcd(diff, n);
    }
    return result;
}

IMATHLIB_CONSTEXPR_X64 uint64_t pollardRhoPoly(uint64_t x, uint64_t mod) {
    auto r = mulmod(x, x, mod) + 1;
    return r == mod ? 0 : r;
}

/**
 * Pollard's Rho factorization algorithm
 * Returns one of the non-trivial divisors of n, or n on failure.
 * The algorithm may fail for composite numbers.
 * Returned divisor does not have to be a prime number.
 * */
IMATHLIB_CONSTEXPR_X64
uint64_t pollardRhoFactorization(uint64_t n, uint64_t starting_value) {
    uint64_t turtle = starting_value;
    uint64_t hare = starting_value;
    uint64_t result = 1;
    while (result == 1) {
        turtle = pollardRhoPoly(turtle, n);
        hare = pollardRhoPoly(hare, n);
        hare = pollardRhoPoly(hare, n);
        auto diff = (turtle>hare) ? (turtle-hare) : (hare-turtle);
        result = gcd(diff, n);
    }
    return result;
}

constexpr u128 mul64x64Fallback(uint64_t a, uint64_t b) {
    uint32_t ahi = static_cast<uint32_t>(a >> 32);
    uint32_t alo = static_cast<uint32_t>(a);
    uint32_t bhi = static_cast<uint32_t>(b >> 32);
    uint32_t blo = static_cast<uint32_t>(b);

    // B = 2 ^ 32
    // a = ahi*B + alo
    // b = bhi*B + blo
    // a * b = (ahi*B + alo) * (bhi*B + blo)
    // a * b = B * B ahi * bhi + B * (ahi * blo + bhi * alo) + alo * blo

    //      ┌─────────┐
    //      │  ah*bl  │
    //      ├─────────┤
    //      │  al*bh  │
    // ┌────┴────┬────┴────┐
    // │  ah*bh  │  al*bl  │
    // └─────────┴─────────┘

    uint64_t ahbh = uint64_t{ahi} * bhi;
    uint64_t ahbl = uint64_t{ahi} * blo;
    uint64_t albh = uint64_t{alo} * bhi;
    uint64_t albl = uint64_t{alo} * blo;

    // max 34 bit value, 2 bits of carry to result.hi
    uint64_t mid = (albl >> 32) + (ahbl & 0xffffffff) + albh;

    u128 result{};
    result.hi = ahbh + (ahbl >> 32) + (mid >> 32);
    result.lo = (mid << 32) | (albl & 0xffffffff);
    return result;
}

IMATHLIB_CONSTEXPR_X64 u128 mul64x64(uint64_t a, uint64_t b) {
#if defined(__SIZEOF_INT128__)
    __uint128_t result = (__uint128_t{a} * b);
    return u128{static_cast<uint64_t>(result >> 64),
                static_cast<uint64_t>(result & static_cast<uint64_t>(-1))};
#else
    if (IMATHLIB_IS_CONSTEVAL) {
        return mul64x64Fallback(a, b);
    }
#if defined(_M_X64) || defined(_M_ARM64) || defined(_M_IA64)
    u128 result{};
    result.lo = _umul128(a, b, &result.hi);
    return result;
#else
    return mul64x64Fallback(a, b);
#endif
#endif
}

IMATHLIB_CONSTEXPR_INTR uint64_t mod128by64Fallback(const u128 n, uint64_t mod) {
    IMATHLIB_ASSUME(0 < mod);
    IMATHLIB_ASSUME(n.hi < mod);

    // difference in bit length
    int bit_diff = detail::clz(n.hi) -
                   detail::clz(mod);

    // Due to higher bits modulo reduction,
    // 0 <= bit_diff < 64;
    // Now, split multiplication result into 2 parts:
    // higher_bits of the same bit-length as mod
    // lower_bits with the rest of the number, shifted left
    uint64_t higher_bits{};
    uint64_t lower_bits{};
    if (bit_diff == 0) {
        // to avoid Undefined Behaviour with shift by 64
        higher_bits = n.hi;
        lower_bits = n.lo;
    } else {
        higher_bits = (n.hi << bit_diff) |  (n.lo >> (64 - bit_diff));
        lower_bits = n.lo << bit_diff;
    }

    higher_bits -= mod * (higher_bits >= mod);
    bit_diff = 63 - bit_diff;

    #if defined(__clang__)
    #pragma nounroll
    #endif
    do {
        // carry:higher_bits:lower_bits = (higher_bits:lower_bits) << 1
        uint64_t carry = higher_bits >> 63;
        // shift left with carry
        higher_bits = (higher_bits << 1) | (lower_bits >> 63);
        // just shift, we don't care about the lower bits
        lower_bits = (lower_bits << 1);
        // if (carry || higher_bits >= mod) higher_bits -= mod;
        higher_bits -= mod * (carry | (higher_bits >= mod));
    } while (bit_diff--);

    return higher_bits;
}

IMATHLIB_CONSTEXPR_X64 uint64_t mod128by64(const u128 n, uint64_t mod) {
    IMATHLIB_ASSUME(0 < n.hi);
    IMATHLIB_ASSUME(0 < mod);
    IMATHLIB_ASSUME(n.hi < mod);

// We don't really want to use modulo on builtin u128,
// and x86_64 assembly is prefferable.
// We don't know if builtin optimizes for empty high bits in mod,
// and call to it likely won't be inlined.
// However, assembly is not available in constexpr context and having
// constexpr in C++14 for everything is cool.
#if defined(__SIZEOF_INT128__)
        __uint128_t p{n.hi};
        p <<= 64;
        p |= n.lo;
        return static_cast<uint64_t>(p % mod);
#else
    if (IMATHLIB_IS_CONSTEVAL) {
        return mod128by64Fallback(n, mod);
    }

#if __x86_64__
    // divq instruction
    // performs 128/64 bit division
    // rdx:rax / passed register
    // reminder stored in rdx, quotient (ignored) in rax
    // IMPORTANT - quotient must fit into 64bit register,
    // or else DE is raised (like with division by 0)
    uint64_t result;
    __asm__("divq    %[v]"
            : "=d"(result)
            : [ v ] "r"(mod), "d"(n.hi), "a"(n.lo));
    return result;
#elif defined(_M_X64)
    // inline assembly not available in MSVC x64,
    // but luckily this instruction is exposed by intrinsics
    uint64_t result;
    (void)_udiv128(n.hi, n.lo, mod, &result);
    return result;
#else
    return mod128by64Fallback(n, mod);
#endif  // __x86_64__ defined(_M_X64)
#endif  // defined(__SIZEOF_INT128__)
}

/**
 * A simple binary-search constexpr implementation of square root for integers.
 * Used by isPerfectSquare to make it constexpr for C++20
 * */
constexpr uint32_t isqrt(uint32_t n) {
    uint32_t left = 0;
    uint32_t right = detail::min(uint32_t{UINT16_MAX}, n);
    while (left != right) {
        uint32_t mid = (left + right) / 2; // no risk of overflow
        if (mid * mid == n) {
            return mid;
        } else if (mid * mid < n) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return left;
}

/**
 * A simple binary-search constexpr implementation of square root for integers.
 * Used by isPerfectSquare to make it constexpr for C++20
 * */
constexpr uint64_t isqrt(uint64_t n) {
    uint64_t left = 0;
    uint64_t right = detail::min(uint64_t{UINT32_MAX}, n);
    while (left != right) {
        uint64_t mid = (left + right) / 2; // no risk of overflow
        if (mid * mid == n) {
            return mid;
        } else if (mid * mid < n) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return left;
}

} // namespace detail

template <size_t SIZE, typename T>
class PrimeArray {
public:
    static_assert(std::is_integral<T>::value, "PrimeArray must contain integers");
    static_assert(SIZE > 0, "Empty PrimeArray is not allowed");

    constexpr
    PrimeArray() : array{} {
        constexpr const size_t sieve_size = getSieveSize();
        bool sieve[sieve_size]{}; // true if is composite
        array[0] = 2;
        size_t filled = 1;
        size_t next_prime = 1;
        while (filled < SIZE) {
            do next_prime += 2; while(sieve[next_prime / 2]);
            array[filled++] = static_cast<T>(next_prime);
            if (next_prime * next_prime > next_prime) // protect from overflow
            for (size_t i = next_prime * next_prime;
                    i / 2 < sieve_size;
                    i += 2 * next_prime)
                sieve[i / 2] = true;
        }
    }

    constexpr size_t size() const {
        return SIZE;
    }
    constexpr const T* begin() const {
        return array;
    }
    constexpr const T* end() const {
        return array + SIZE;
    }
    constexpr const T& operator[](size_t idx) const {
        return array[idx];
    }
    constexpr const T& back() const {
        return array[SIZE - 1];
    }

private:
    T array[SIZE];
    constexpr static size_t getSieveSize() {
        size_t result = sizeof(size_t) * 8
                        - detail::clzFallback(SIZE)
                        + 2;
        return result * 3 / 4 * SIZE;
    }
};

constexpr PrimeArray<64, uint16_t> kSmallPrimes;

IMATHLIB_CONSTEXPR_INTR bool isPrime(uint32_t n) {
    if (n == 2 || n == 3 || n == 5 || n == 7) return true;
    if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) return false;
    if (n < 121) return (n > 1);
    uint64_t h = n;  // important - 64 bits
    h = ((h >> 16) ^ h) * 0x979bc64f;
    h = ((h >> 16) ^ h) * 0x979bc64f;
    h = ((h >> 16) ^ h) & 255;
    return detail::isSPRP(n, detail::bases_prime_test_u32[h]);
}

IMATHLIB_CONSTEXPR_X64 bool isPrime(uint64_t n) {
    if (n < (1ull << 32)) return isPrime(static_cast<uint32_t>(n));
    if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) return false;

    if (!detail::isSPRP(n, 2)) return false;

    // Steve Worley, 2013
    if (n < 109134866497)
        return detail::isSPRP(n, 1005905886) &&
               detail::isSPRP(n, 1340600841);

    if (n < 55245642489451)
        return detail::isSPRP(n, 141889084524735) &&
               detail::isSPRP(n, 1199124725622454117) &&
               detail::isSPRP(n, 11096072698276303650u);

    uint64_t h = n;
    h = ((h >> 32) ^ h) * 0x123456789abce1b;
    h = ((h >> 32) ^ h) * 0x123456789abce1b;
    h = ((h >> 32) ^ h) & 127;

    uint32_t bs = detail::bases_prime_test_u64[h];
    // see comments above detail::bases_prime_test_u64 for details
    auto b1 = 15u;
    auto b2 = (bs >> 22) & 0x3FF;
    auto b3 = (bs >> 11) & 0x7FF;
    auto b4 = (bs >> 0) & 0x7FF;
    return detail::isSPRP(n, uint64_t{b1}) &&
           detail::isSPRP(n, uint64_t{b2}) &&
           detail::isSPRP(n, uint64_t{b3}) &&
           detail::isSPRP(n, uint64_t{b4});
}

IMATHLIB_CONSTEXPR_INTR uint32_t nextPrimeAfter(uint32_t n) {
    IMATHLIB_ASSERT(n < 4294967291u);
    if (n < 2) return 2;
    for (n = n + 1 + (n & 1); !isPrime(n); n += 2);
    return n;
}
IMATHLIB_CONSTEXPR_X64 uint64_t nextPrimeAfter(uint64_t n) {
    IMATHLIB_ASSERT(n < 18446744073709551557ull);
    if (n < 4294967291u) {  // biggest 32-bit prime
        return nextPrimeAfter(static_cast<uint32_t>(n));
    }
    for (n = n + 1 + (n & 1); !isPrime(n); n += 2);
    return n;
}

// TODO add Montgomery multiplication

struct FactorU32 {
    uint32_t prime;
    uint32_t power;
};

/**
 * Stores results of a 32-bit number factorization.
 * Results are in a form of pairs {prime, power}. Primes are in ascending order.
 * Multiplication of all pairs [i].prime ** [i].power will give the original
 * factored number.
 * */
class FactorizationResultU32 {
public:
    constexpr size_t size() const {
        return size_;
    }
    constexpr const FactorU32* begin() const {
        return factors_;
    }
    constexpr const FactorU32* end() const {
        return factors_ + size_;
    }
    constexpr const FactorU32& operator[](size_t idx) const {
        return factors_[idx];
    }
    constexpr const FactorU32& back() const {
        return factors_[size_ - 1];
    }

private:
    /**
     * Used to add prime factors found in incresing order using trialdivision
     * */
    constexpr void addFactor(FactorU32 f) {
        factors_[size_++] = f;
    }

    /**
     * Used to add prime factors found in not specified order.
     * Performs a linear search to find if the prime factor already occured.
     * Then either adds power to the existing factor, or performs insertion.
     * */
    constexpr void addUnorderedFactor(FactorU32 f) {
        size_t i = 0;
        for (; i < size_ && factors_[i].prime < f.prime; ++i);
        if (factors_[i].prime == f.prime) {
            factors_[i].power += f.power;
            return;
        }
        // if not found, perform an insertion
        FactorU32 swapper = f;
        for (; i < size_; ++i) {
            detail::simpleSwap(swapper, factors_[i]);
        }
        factors_[size_++] = swapper;
    }

    FactorU32 factors_[10]{};
    size_t size_{};
    friend IMATHLIB_CONSTEXPR_INTR FactorizationResultU32 factorize(uint32_t n);
};

struct FactorU64 {
    uint64_t prime;
    uint64_t power;
};

/**
 * Stores results of a 64-bit number factorization.
 * Results are in a form of pairs {prime, power}. Primes are in ascending order.
 * Multiplication of all pairs [i].prime ** [i].power will give the original
 * factored number.
 * */
class FactorizationResultU64 {
public:
    constexpr size_t size() const {
        return size_;
    }
    constexpr const FactorU64* begin() const {
        return factors_;
    }
    constexpr const FactorU64* end() const {
        return factors_ + size_;
    }
    constexpr const FactorU64& operator[](size_t idx) const {
        return factors_[idx];
    }
    constexpr const FactorU64& back() const {
        return factors_[size_ - 1];
    }

private:
    /**
     * Used to add prime factors found in incresing order using trialdivision
     * */
    constexpr void addFactor(FactorU64 f) {
        factors_[size_++] = f;
    }

    /**
     * Used to add prime factors found in not specified order.
     * Performs a linear search to find if the prime factor already occured.
     * Then either adds power to the existing factor, or performs insertion.
     * */
    constexpr void addUnorderedFactor(FactorU64 f) {
        size_t i = 0;
        for (; i < size_ && factors_[i].prime < f.prime; ++i);
        if (factors_[i].prime == f.prime) {
            factors_[i].power += f.power;
            return;
        }
        // if not found, perform an insertion
        FactorU64 swapper = f;
        for (; i < size_; ++i) {
            detail::simpleSwap(swapper, factors_[i]);
        }
        factors_[size_++] = swapper;
    }

    FactorU64 factors_[16]{};
    size_t size_{};
    friend IMATHLIB_CONSTEXPR_X64 FactorizationResultU64 factorize(uint64_t n);
};

IMATHLIB_CONSTEXPR_INTR FactorizationResultU32 factorize(uint32_t n) {
    constexpr const size_t kSmallPrimesTested = 16;
    static_assert(kSmallPrimesTested >= 4,
                  "2, 3, 5, 7 are used in primality test, so we need to test them here too");
    FactorizationResultU32 result{};

    for (size_t i = 0; i < kSmallPrimesTested; ++i) {
        uint32_t prime = kSmallPrimes[i];
        if (n % prime == 0) {
            FactorU32 f{};
            f.prime = prime;
            do {
                n /= prime;
                ++f.power;
            } while (n % prime == 0);
            result.addFactor(f);
        }
    }

    if (n == 1 || isPrime(n)) {
        result.addFactor({n, 1});
        return result;
    }

    // Pollard's Rho algorithm may return a composite divisor,
    // so we need to keep track of all calculated composite divisors.
    // We only add prime factors to the result
    uint32_t composite_factors[8]{n, };
    size_t composite_factors_count = 1;
    uint32_t init_value = 0x12345678;  // arbitrary non-zero

    while (composite_factors_count > 0) {
        uint32_t cf = composite_factors[--composite_factors_count];
        uint32_t f = 0;

        // Pollard's Rho algorithm might fail to find a divisor,
        // so retry with different initial values
        do {
            f = detail::pollardRhoFactorization(cf, init_value);
            // xor-shift, to get another random init_value
            // https://en.wikipedia.org/wiki/Xorshift
            init_value ^= init_value << 13;
            init_value ^= init_value >> 17;
            init_value ^= init_value << 5;
        } while (f == cf);

        if (isPrime(f)) {
            result.addUnorderedFactor({f, 1});
        } else {
            composite_factors[composite_factors_count++] = f;
        }

        cf /= f;

        if (isPrime(cf)) {
            result.addUnorderedFactor({cf, 1});
        } else {
            composite_factors[composite_factors_count++] = cf;
        }
    }

    return result;
}

IMATHLIB_CONSTEXPR_X64 FactorizationResultU64 factorize(uint64_t n) {
    constexpr const size_t kSmallPrimesTested = 16;
    static_assert(kSmallPrimesTested >= 4,
                  "2, 3, 5, 7 are used in primality test, so we need to test them here too");
    FactorizationResultU64 result{};
    if (n <= 1) return result;

    for (size_t i = 0; i < kSmallPrimesTested; ++i) {
        uint32_t prime = kSmallPrimes[i];
        if (n % prime == 0) {
            FactorU64 f{};
            f.prime = prime;
            do {
                n /= prime;
                ++f.power;
            } while (n % prime == 0);
            result.addFactor(f);
        }
    }

    if (n == 1 || isPrime(n)) {
        result.addFactor({n, 1});
        return result;
    }

    // Pollard's Rho algorithm may return a composite divisor,
    // so we need to keep track of all calculated composite divisors.
    // We only add prime factors to the result
    uint64_t composite_factors[8]{n, };
    size_t composite_factors_count = 1;
    uint64_t init_value = 0x1234567890abcdefull;  // arbitrary non-zero

    while (composite_factors_count > 0) {
        uint64_t cf = composite_factors[--composite_factors_count];
        uint64_t f = 0;

        // Pollard's Rho algorithm might fail to find a divisor,
        // so retry with different initial values
        do {
            f = detail::pollardRhoFactorization(cf, init_value);
            // xor-shift, to get another random init_value
            // https://en.wikipedia.org/wiki/Xorshift
            init_value ^= init_value >> 12;
            init_value ^= init_value << 25;
            init_value ^= init_value >> 27;
        } while (f == cf);

        if (isPrime(f)) {
            result.addUnorderedFactor({f, 1});
        } else {
            composite_factors[composite_factors_count++] = f;
        }

        cf /= f;

        if (isPrime(cf)) {
            result.addUnorderedFactor({cf, 1});
        } else {
            composite_factors[composite_factors_count++] = cf;
        }
    }

    return result;
}

constexpr uint32_t pow(uint32_t n, uint32_t pow) {
    uint32_t result = 1;
    while (pow) {
        if (pow & 1) result *= n;
        n *= n;
        pow >>= 1;
    }
    return result;
}
constexpr uint64_t pow(uint64_t n, uint64_t pow) {
    uint64_t result = 1;
    while (pow) {
        if (pow & 1) result *= n;
        n *= n;
        pow >>= 1;
    }
    return result;
}

constexpr uint32_t mulmod(uint32_t a, uint32_t b, uint32_t mod) {
    IMATHLIB_ASSERT(mod > 0);
    return static_cast<uint32_t>((uint64_t{a} * b) % mod);
}
IMATHLIB_CONSTEXPR_X64 uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod) {
    IMATHLIB_ASSERT(mod > 0);
    if ((mod & (mod - 1)) == 0) {  // is power of two
        return (a * b) & (mod - 1);
    }

    detail::u128 x = detail::mul64x64(a, b);
    if (x.hi >= mod) {
        x.hi %= mod;
    }
    if (x.hi == 0) {
        return x.lo % mod;
    }

    return detail::mod128by64(x, mod);
}

constexpr uint32_t powmod(uint32_t n, uint32_t pow, uint32_t mod) {
    IMATHLIB_ASSERT(mod > 0);
    uint32_t cur = n;
    uint32_t res = 1;
    while (pow) {
        if (pow & 1) res = mulmod(cur, res, mod);
        cur = mulmod(cur, cur, mod);
        pow >>= 1;
    }
    return res;
}
IMATHLIB_CONSTEXPR_X64 uint64_t powmod(uint64_t n, uint64_t pow, uint64_t mod) {
    IMATHLIB_ASSERT(mod > 0);
    uint64_t cur = n;
    uint64_t res = 1;
    while (pow) {
        if (pow & 1) res = mulmod(cur, res, mod);
        cur = mulmod(cur, cur, mod);
        pow >>= 1;
    }
    return res;
}

IMATHLIB_CONSTEXPR_INTR uint32_t gcd(uint32_t a, uint32_t b) {
    if (IMATHLIB_IS_CONSTEVAL) {
        return detail::gcdModuloRecursive(a, b);
    }

#if defined(IMATHLIB_FAST_CTZ32)
    return detail::gcdBinary(a, b);
#else
    return detail::gcdModuloRecursive(a, b);
#endif
}

IMATHLIB_CONSTEXPR_INTR uint64_t gcd(uint64_t a, uint64_t b) {
    if (IMATHLIB_IS_CONSTEVAL) {
        return detail::gcdModuloRecursive(a, b);
    }

#if defined(IMATHLIB_FAST_CTZ64)
    return detail::gcdBinary(a, b);
#else
    return detail::gcdModuloRecursive(a, b);
#endif
}

IMATHLIB_CONSTEXPR_INTR uint32_t lcm(uint32_t a, uint32_t b) {
    return a / gcd (a, b) * b;
}

IMATHLIB_CONSTEXPR_INTR uint64_t lcm(uint64_t a, uint64_t b) {
    return a / gcd (a, b) * b;
}

constexpr uint32_t roundUpToMultipleOf(uint32_t n, uint32_t mul) {
    IMATHLIB_ASSERT(mul);
    IMATHLIB_ASSERT((n + mul - 1) > n);
    return ((n + mul - 1) / mul) * mul;
}

constexpr uint64_t roundUpToMultipleOf(uint64_t n, uint64_t mul) {
    IMATHLIB_ASSERT(mul);
    IMATHLIB_ASSERT((n + mul - 1) > n);
    return ((n + mul - 1) / mul) * mul;
}

constexpr uint32_t roundDownToMultipleOf(uint32_t n, uint32_t mul) {
    IMATHLIB_ASSERT(mul);
    return n - n % mul;
}

constexpr uint64_t roundDownToMultipleOf(uint64_t n, uint64_t mul) {
    IMATHLIB_ASSERT(mul);
    return n - n % mul;
}

IMATHLIB_CONSTEXPR20 bool isPerfectSquare(uint32_t n) {
    // top 5 bits must be one of the following:
    // {0, 1, 4, 9, 16, 17, 25}
    // corresponding bits are lit down
    constexpr uint32_t mask =
        0b11111101111111001111110111101100u;
    if ((mask >> (n & 31)) & 1) return false;
    if (n == 0) return true;
    int trailing_zeroes = detail::ctz(n);
    if (trailing_zeroes & 1) return false;
    n >>= trailing_zeroes;
    if ((n&7) != 1) return false;
    uint32_t root{};
    if (IMATHLIB_IS_CONSTEVAL) {
        // it's slower than floating point sqrt (usually),
        // so use it only for constexpr
        root = detail::isqrt(n);
    } else {
        root = static_cast<uint32_t>(
                std::roundf(std::sqrt(static_cast<float>(n))));
    }
    return root * root == n;
}

// Note, that this function might perform relatively poorly on
// 32-bit architectures. Bit test in mask and counting trailing
// zeroes might turn out to be very difficult tasks for the processor.
// If you need performance, try replacing mask test with the
// one from the function above, or with a lookup array.
// If trailing zeroes test can't be performed quickly, consider omitting it.
IMATHLIB_CONSTEXPR20 bool isPerfectSquare(uint64_t n) {
    // top 6 bits must be one of the following:
    // {0, 1, 4, 9, 16, 17, 25, 33, 36, 41, 49, 57}
    // corresponding bits are lit down
    constexpr uint64_t mask =
        0b1111110111111101111111011110110111111101111111001111110111101100ull;
    // choose a bit in the mask, hopefully compiled to MOV + BT
    if ((mask >> (n & 63)) & 1) return false;
    if (n == 0) return true;
    int trailing_zeroes = detail::ctz(n);
    if (trailing_zeroes & 1) return false;
    n >>= trailing_zeroes;
    if ((n&7) != 1) return false;
    uint64_t root{};
    if (IMATHLIB_IS_CONSTEVAL) {
        // it's slower than floating point sqrt (usually),
        // so use it only for constexpr
        root = detail::isqrt(n);
    } else {
        root = static_cast<uint64_t>(std::round(std::sqrt(n)));
    }
    return root * root == n;
}

}  // namespace imath

// These are all the macros that can be defined by this header:
// IMATHLIB_IMATH_H
// IMATHLIB_CONSTEXPR_INTR
// IMATHLIB_CONSTEXPR_X64
// IMATHLIB_CONSTEXPR20
// IMATHLIB_HAS_CONSTEXPR_INTR
// IMATHLIB_HAS_CONSTEXPR_X64
// IMATHLIB_HAS_CONSTEXPR20
// IMATHLIB_IS_CONSTEVAL
// IMATHLIB_ASSERT
// IMATHLIB_ASSUME
// IMATHLIB_FAST_CLZ32
// IMATHLIB_FAST_CLZ64
// IMATHLIB_FAST_CTZ64
// IMATHLIB_FAST_CTZ64

#endif  // IMATHLIB_IMATH_H

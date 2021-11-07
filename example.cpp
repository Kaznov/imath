
//                   Copyright(c) Kamil Kaznowski 2021.
//         Distributed under the Boost Software License, Version 1.0.
//                (See accompanying file LICENSE or copy at
//                  https://www.boost.org/LICENSE_1_0.txt)

// This is a simple example how to use the imath library.

#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#include "imath.h"

using u32 = uint32_t;
using u64 = uint64_t;

static_assert(IMATHLIB_CPP_VER >= 201402L, "This example requires C++14");

u64 promptNumber(std::string prompt) {
    std::cout << prompt << ":\n";
    u64 num;
    while (!(std::cin >> num)) {
        if (std::cin.eof()) {
            std::cout << "Unexpected EOF, quitting...\n";
            std::exit(1);
        }
        std::cout << "Cannot read number, try again:\n";

        // clear fail bits and skip this line of input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return num;
}

void factorNumber(u64 n) {
    auto factors = imath::factorize(n);

    // By default factors are kept in a range of pairs {prime, power}.
    // This way the result is always small and there is no allocation needed.
    std::cout << n << " = ";
    // Loop like that not to show × after the last factor
    for (size_t i = 0; i < factors.size() - 1; ++i) {
        std::cout << factors[i].prime << "^" << factors[i].power << " × ";
    }
    std::cout << factors.back().prime << "^" << factors.back().power << "\n";

    // But sometimes we want some other format to display the results.
    std::cout << n << " = ";
    std::vector<u64> factors_repeated;
    for (auto&& factor : factors)
        for (size_t i = 0; i < factor.power; ++i)
            factors_repeated.push_back(factor.prime);

    for (size_t i = 0; i < factors_repeated.size() - 1; ++i) {
        std::cout << factors_repeated[i] << " × ";
    }
    std::cout << factors_repeated.back() << "\n\n";
}

void checkPrimeness(u64 n) {
    if (imath::isPrime(n)) {
        std::cout << n << " is prime\n";
    } else {
        std::cout << n << " is composite\n";
    }
}

void checkSquareness(u64 n) {
    if (imath::isPerfectSquare(n)) {
        std::cout << n << " is a square\n";
    } else {
        std::cout << n << " is not a square\n";
    }
}

void powerModulo2to32(u64 n) {
    std::cout << n << "^" << 111 << " ≡ "
              << imath::pow(static_cast<u32>(n), static_cast<u32>(111))
              << " (mod 2^32)\n";
}

void powerModulo2to64(u64 n) {
    std::cout << n << "^" << 111 << " ≡ "
              << imath::pow(u64{n}, u64{111})
              << " (mod 2^64)\n";
}

void powerModuloSomePrime(u64 n) {
    u64 mod = 1000000007;
    std::cout << n << "^" << 1111111 << " ≡ "
              << imath::powmod(n, u64{1111111}, mod)
              << " (mod " << mod <<")\n";
}

int main() {
    u64 num = promptNumber("Input a positive number below 2^64");
    factorNumber(num);
    checkPrimeness(num);
    checkSquareness(num);
    powerModulo2to32(num);
    powerModulo2to64(num);
    powerModuloSomePrime(num);
}

#if IMATHLIB_HAS_CONSTEXPR_INTR
// In C++14 for GCC, Clang, ICC and ICX most operations are constexpr!

// primes for xxhash algorithm
constexpr u32 XXPRIME32_1 = 2654435761U;
constexpr u32 XXPRIME32_2 = 2246822519U;
constexpr u32 XXPRIME32_3 = 3266489917U;
constexpr u32 XXPRIME32_4 =  668265263U;
constexpr u32 XXPRIME32_5 =  374761393U;

static_assert(imath::isPrime(XXPRIME32_1), "");
static_assert(imath::isPrime(XXPRIME32_2), "");
static_assert(imath::isPrime(XXPRIME32_3), "");
static_assert(imath::isPrime(XXPRIME32_4), "");
static_assert(imath::isPrime(XXPRIME32_5), "");

#endif

#if IMATHLIB_HAS_CONSTEXPR_X64
// Some operations require x64 target to work in constexpr (or c++20)

constexpr u64 XXPRIME64_1 = 11400714785074694791ULL;
constexpr u64 XXPRIME64_2 = 14029467366897019727ULL;
constexpr u64 XXPRIME64_3 =  1609587929392839161ULL;
constexpr u64 XXPRIME64_4 =  9650029242287828579ULL;
constexpr u64 XXPRIME64_5 =  2870177450012600261ULL;

static_assert(imath::isPrime(XXPRIME64_1), "");
static_assert(imath::isPrime(XXPRIME64_2), "");
static_assert(imath::isPrime(XXPRIME64_3), "");
static_assert(imath::isPrime(XXPRIME64_4), "");
static_assert(imath::isPrime(XXPRIME64_5), "");

#endif

#if IMATHLIB_HAS_CONSTEXPR20
// In C++20 everything is constexpr!

static_assert(imath::isSquare(1'000'000'007ull * 1'000'000'007ull));

#endif
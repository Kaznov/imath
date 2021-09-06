# imath

THIS LIBRARY IS IN A VERY EARLY PHASE OF DEVELOPMENT. DO NOT USE.

**imath** is an open-source C++14 library providing a set of fast operations on 32-bit and 64-bit integers.

Features
--------
* Fast factorization - O(∜n * polylog(n))
* Fast deterministic primality test - O(log n)
* Finding the next prime after a given number
* Integer multiplication modulo ((64bit * 64bit) % 64bit)
* Efficient integer power modulo - O(log(power))
* Class for Montgomery multiplications (in development)
* Rounding to multiples of a number
* and more!

Code should be free of warnings on all major compilers, even on high warning levels `-Wall` and `-Wextra`.

Most of the functions for 32-bit integers is `constexpr` in C++14, and all the functions, both for 32bit and 64bit integers are `constexpr` in C++20 (it requires `std::is_constant_evaluated()`).

Some operations were tuned to use intrinsics when compiled by GCC, Clang or MSVC compilers. Note, that the library was designed to work most efficiently on x86_64 architecture, but it still should work on any machine.

Examples
--------
**Test if number is a prime**

```c++
static_assert(imath::isPrime(1'000'000'007u);
```

**Factor a number**

```c++
int main() {
    auto fact_result = imath::factorize(3'851'820u);
    std::cout << "3851820 = ";
    // range-for loop also supported!
    for (size_t i = 0; i < fact_result.size() - 1; ++i) {
       auto factor = fact_result[i];
       std::cout << factor.prime << "**" << factor.power << " * ";
    }
    std::cout << fact_result.back().prime << "**"
              << fact_result.back().power;
}
```
Expected output: `3851820 = 2**2 * 3**3 * 5**1 * 7**1 * 1019**1`


**Generate a compile-time prime array**

```c++
static constexpr imath::PrimeArray<8> array_of_small_primes;

int main() {
    for (auto prime : array_of_small_primes)
        std::cout << prime << " ";
}
```
Expected output: `2 3 5 7 11 13 17 19`
Also available: imath:kSmallPrimes

**Find the Greatest Common Divisor and the Least Common Multiple ot two numbers**

```c++
int main() {
    std::cout << "GCD(24, 81) = " << imath::gcd(24u, 81u) << "\n";
    std::cout << "LCM(24, 81) = " << imath::lcm(24u, 81u) << "\n";
}
```

**Test if number is a perfect square**
```c++
int main() {
    std::cout << 1046528 << imath::isPerfectSquare(1046528u) ? "IS" : "IS NOT"
              << " a perfect square\n";
    std::cout << 1046529 << imath::isPerfectSquare(1046529u) ? "IS" : "IS NOT"
              << " a perfect square\n";
}
```

**Round a number up or down to multiple of some other number**
```c++
int main() {
    size_t number = 12345678;
    std::cout << number << " rounded up to thousands is "
              << imath::roundUpToMultipleOf(number, 1000) << "\n";
    std::cout << number << " rounded down to thousands is "
              << imath::roundDownToMultipleOf(number, 1000) << "\n"; 
}
```

**Raise a number to given power over a modulus**
```c++
int main() {
    std::cout << " (5 ** 3) % 13 =  "
              << imath::powmod(5, 3, 13) << "\n";
    // And there is no risk of overflow!
}
```
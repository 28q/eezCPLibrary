#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cstdint>
#include <iostream>

#include "../../math/modint998.hpp"

constexpr modint998 modint998_compile_time_check = modint998(-1) * modint998(2);
static_assert(modint998_compile_time_check.val() == modint998::MOD - 2);
static_assert(modint998::raw(123).val() == 123);
static_assert((modint998(3) * modint998(3).inv()).val() == 1);

int main() {
    std::int64_t a, b;
    std::cin >> a >> b;
    std::cout << a + b << '\n';
}

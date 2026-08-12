#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb_128bit"

#ifdef __linux__
#define FASTIO_SAFE_USE_POSIX 1
#endif
#include "../../IO/fastio_safe.hpp"

int main() {
    std::uint32_t test_count;
    if (!fastio_safe::scan(test_count)) return 1;

    while (test_count-- != 0) {
        __int128_t a, b;
        if (!fastio_safe::scan(a, b)) return 1;
        if (!fastio_safe::print(a + b)) return 1;
    }
}

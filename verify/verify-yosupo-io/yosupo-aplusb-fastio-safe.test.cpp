#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#ifdef __linux__
#define FASTIO_SAFE_USE_POSIX 1
#endif
#include "../../IO/fastio_safe.hpp"

int main() {
    std::int32_t a, b;
    if (!fastio_safe::scan(a, b)) return 1;
    return fastio_safe::print(a + b) ? 0 : 1;
}

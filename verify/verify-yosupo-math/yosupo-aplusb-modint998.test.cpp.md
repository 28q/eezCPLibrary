---
data:
  _extendedDependsOn:
  - icon: ':x:'
    path: math/modint998.hpp
    title: modint998
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/aplusb
    links:
    - https://judge.yosupo.jp/problem/aplusb
  bundledCode: "#line 1 \"verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/aplusb\"\n\n#include <cstdint>\n\
    #include <iostream>\n\n#line 1 \"math/modint998.hpp\"\n\n\n\r\n#include <cassert>\r\
    \n#line 7 \"math/modint998.hpp\"\n#include <type_traits>\r\n\r\nstruct modint998\
    \ {\r\n    using u32 = std::uint32_t;\r\n    using i32 = std::int32_t;\r\n   \
    \ using u64 = std::uint64_t;\r\n\r\n    static constexpr u32 MOD = 998244353u;\r\
    \n    static constexpr u32 MOD2 = MOD * 2;\r\n    static constexpr u32 primitive_root\
    \ = 3;\r\n    static constexpr int max_power_of_two = 23;\r\n\r\nprivate:\r\n\
    \    static constexpr u32 R = 3296722945u;\r\n    static constexpr u32 N2 = 932051910u;\r\
    \n\r\n    struct montgomery_tag {};\r\n\r\n    constexpr modint998(u32 x, montgomery_tag)\
    \ : a(x) {}\r\n\r\n    static constexpr u32 reduce(u64 x) {\r\n        return\
    \ static_cast<u32>(\r\n            (x + u64(static_cast<u32>(x) * u32(-R)) * MOD)\
    \ >> 32\r\n        );\r\n    }\r\n\r\npublic:\r\n    u32 a;\r\n\r\n    static_assert(MOD\
    \ < (u32(1) << 30));\r\n    static_assert((MOD & 1) != 0);\r\n    static_assert(R\
    \ * MOD == 1);\r\n\r\n    constexpr modint998() : a(0) {}\r\n\r\n    template\
    \ <class T, std::enable_if_t<std::is_integral_v<T> &&\r\n                    \
    \                    std::is_signed_v<T>, int> = 0>\r\n    constexpr modint998(T\
    \ x) : a(0) {\r\n        const std::int64_t y =\r\n            static_cast<std::int64_t>(x)\
    \ % std::int64_t(MOD) + MOD;\r\n        a = reduce(u64(y) * N2);\r\n    }\r\n\r\
    \n    template <class T, std::enable_if_t<std::is_integral_v<T> &&\r\n       \
    \                                 std::is_unsigned_v<T>, int> = 0>\r\n    constexpr\
    \ modint998(T x)\r\n        : a(reduce(((u64(x) % MOD) + MOD) * N2)) {}\r\n\r\n\
    \    static constexpr modint998 raw(u32 x) {\r\n        return modint998(reduce(u64(x)\
    \ * N2), montgomery_tag{});\r\n    }\r\n\r\n    static constexpr modint998 montgomery_raw(u32\
    \ x) {\r\n        return modint998(x, montgomery_tag{});\r\n    }\r\n\r\n    static\
    \ constexpr u32 mod() { return MOD; }\r\n    static constexpr u32 get_mod() {\
    \ return MOD; }\r\n\r\n    constexpr u32 val() const {\r\n        const u32 x\
    \ = reduce(a);\r\n        return x >= MOD ? x - MOD : x;\r\n    }\r\n\r\n    constexpr\
    \ u32 get() const { return val(); }\r\n\r\n    constexpr modint998& operator+=(const\
    \ modint998& rhs) {\r\n        a += rhs.a - MOD2;\r\n        if (i32(a) < 0) a\
    \ += MOD2;\r\n        return *this;\r\n    }\r\n\r\n    constexpr modint998& operator-=(const\
    \ modint998& rhs) {\r\n        a -= rhs.a;\r\n        if (i32(a) < 0) a += MOD2;\r\
    \n        return *this;\r\n    }\r\n\r\n    constexpr modint998& operator*=(const\
    \ modint998& rhs) {\r\n        a = reduce(u64(a) * rhs.a);\r\n        return *this;\r\
    \n    }\r\n\r\n    constexpr modint998& operator/=(const modint998& rhs) {\r\n\
    \        return *this *= rhs.inv();\r\n    }\r\n\r\n    constexpr modint998 operator+()\
    \ const { return *this; }\r\n    constexpr modint998 operator-() const { return\
    \ modint998() - *this; }\r\n\r\n    friend constexpr modint998 operator+(modint998\
    \ lhs,\r\n                                          const modint998& rhs) {\r\n\
    \        return lhs += rhs;\r\n    }\r\n\r\n    friend constexpr modint998 operator-(modint998\
    \ lhs,\r\n                                          const modint998& rhs) {\r\n\
    \        return lhs -= rhs;\r\n    }\r\n\r\n    friend constexpr modint998 operator*(modint998\
    \ lhs,\r\n                                          const modint998& rhs) {\r\n\
    \        return lhs *= rhs;\r\n    }\r\n\r\n    friend constexpr modint998 operator/(modint998\
    \ lhs,\r\n                                          const modint998& rhs) {\r\n\
    \        return lhs /= rhs;\r\n    }\r\n\r\n    friend constexpr bool operator==(const\
    \ modint998& lhs,\r\n                                     const modint998& rhs)\
    \ {\r\n        const u32 x = lhs.a >= MOD ? lhs.a - MOD : lhs.a;\r\n        const\
    \ u32 y = rhs.a >= MOD ? rhs.a - MOD : rhs.a;\r\n        return x == y;\r\n  \
    \  }\r\n\r\n    friend constexpr bool operator!=(const modint998& lhs,\r\n   \
    \                                  const modint998& rhs) {\r\n        return !(lhs\
    \ == rhs);\r\n    }\r\n\r\n    constexpr modint998& operator++() {\r\n       \
    \ return *this += raw(1);\r\n    }\r\n\r\n    constexpr modint998 operator++(int)\
    \ {\r\n        modint998 old = *this;\r\n        ++*this;\r\n        return old;\r\
    \n    }\r\n\r\n    constexpr modint998& operator--() {\r\n        return *this\
    \ -= raw(1);\r\n    }\r\n\r\n    constexpr modint998 operator--(int) {\r\n   \
    \     modint998 old = *this;\r\n        --*this;\r\n        return old;\r\n  \
    \  }\r\n\r\n    constexpr modint998 pow(u64 e) const {\r\n        if (e == 0)\
    \ return raw(1);\r\n        if (a == 0) return raw(0);\r\n    \r\n        if (e\
    \ >= MOD - 1) e %= MOD - 1;\r\n        if (e == 0) return raw(1);\r\n    \r\n\
    \        const u32 n = static_cast<u32>(e);\r\n    \r\n        if (n == 1) return\
    \ *this;\r\n    \r\n        const modint998 x = *this;\r\n        const modint998\
    \ p2 = x * x;\r\n    \r\n        if (n == 2) return p2;\r\n    \r\n        const\
    \ modint998 p3 = p2 * x;\r\n    \r\n        if (n == 3) return p3;\r\n    \r\n\
    \        const modint998 p4 = p2 * p2;\r\n        const modint998 p5 = p4 * x;\r\
    \n        const modint998 p6 = p3 * p3;\r\n        const modint998 p7 = p4 * p3;\r\
    \n    \r\n        const modint998 t[8] = {\r\n            raw(1), x, p2, p3, p4,\
    \ p5, p6, p7\r\n        };\r\n    \r\n        const unsigned s = ((std::bit_width(n)\
    \ - 1) / 3) * 3;\r\n        modint998 r = t[(n >> s) & 7];\r\n    \r\n       \
    \ auto step = [&](unsigned k) constexpr {\r\n            r *= r;\r\n         \
    \   r *= r;\r\n            r *= r;\r\n            const u32 d = (n >> k) & 7;\r\
    \n            if (d) r *= t[d];\r\n        };\r\n        switch (s) {\r\n    \
    \        case 27: step(24); [[fallthrough]];\r\n            case 24: step(21);\
    \ [[fallthrough]];\r\n            case 21: step(18); [[fallthrough]];\r\n    \
    \        case 18: step(15); [[fallthrough]];\r\n            case 15: step(12);\
    \ [[fallthrough]];\r\n            case 12: step(9);  [[fallthrough]];\r\n    \
    \        case 9:  step(6);  [[fallthrough]];\r\n            case 6:  step(3);\
    \  [[fallthrough]];\r\n            case 3:  step(0);  [[fallthrough]];\r\n   \
    \         default: break;\r\n        }\r\n        return r;\r\n    }\r\n\r\n \
    \   constexpr modint998 inv() const {\r\n        assert(val() != 0);\r\n\r\n \
    \       const modint998 x = *this;\r\n        const modint998 a2 = x * x;\r\n\
    \        const modint998 a4 = a2 * a2;\r\n        const modint998 a5 = a4 * x;\r\
    \n        const modint998 a9 = a5 * a4;\r\n        const modint998 a18 = a9 *\
    \ a9;\r\n        const modint998 a36 = a18 * a18;\r\n        const modint998 a72\
    \ = a36 * a36;\r\n        const modint998 a144 = a72 * a72;\r\n        const modint998\
    \ a288 = a144 * a144;\r\n        const modint998 a293 = a288 * a5;\r\n       \
    \ const modint998 a586 = a293 * a293;\r\n        const modint998 a879 = a586 *\
    \ a293;\r\n        const modint998 a1023 = a879 * a144;\r\n\r\n        modint998\
    \ r = a1023 * a879;  // x^1902\r\n\r\n        // x^(1902 * 2^9) = x^973824\r\n\
    \        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n\
    \        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n\
    \        r *= r;\r\n\r\n        r *= a1023;  // x^974847\r\n\r\n        // x^(974847\
    \ * 2^10) = x^998243328\r\n        r *= r;\r\n        r *= r;\r\n        r *=\
    \ r;\r\n        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n        r *=\
    \ r;\r\n        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n\r\n        return\
    \ r * a1023;  // x^(MOD - 2)\r\n    }\r\n\r\n    constexpr modint998 inverse()\
    \ const { return inv(); }\r\n\r\n    friend std::ostream& operator<<(std::ostream&\
    \ os, const modint998& x) {\r\n        return os << x.val();\r\n    }\r\n\r\n\
    \    friend std::istream& operator>>(std::istream& is, modint998& x) {\r\n   \
    \     std::int64_t value;\r\n        is >> value;\r\n        x = modint998(value);\r\
    \n        return is;\r\n    }\r\n};\r\n\r\nstatic_assert(sizeof(modint998) ==\
    \ 4);\r\nstatic_assert(std::is_trivially_copyable_v<modint998>);\r\n\r\nusing\
    \ mint998 = modint998;\r\n\r\n\n#line 7 \"verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp\"\
    \n\nconstexpr modint998 modint998_compile_time_check = modint998(-1) * modint998(2);\n\
    static_assert(modint998_compile_time_check.val() == modint998::MOD - 2);\nstatic_assert(modint998::raw(123).val()\
    \ == 123);\nstatic_assert((modint998(3) * modint998(3).inv()).val() == 1);\n\n\
    int main() {\n    std::int64_t a, b;\n    std::cin >> a >> b;\n    std::cout <<\
    \ a + b << '\\n';\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/aplusb\"\n\n#include <cstdint>\n\
    #include <iostream>\n\n#include \"../../math/modint998.hpp\"\n\nconstexpr modint998\
    \ modint998_compile_time_check = modint998(-1) * modint998(2);\nstatic_assert(modint998_compile_time_check.val()\
    \ == modint998::MOD - 2);\nstatic_assert(modint998::raw(123).val() == 123);\n\
    static_assert((modint998(3) * modint998(3).inv()).val() == 1);\n\nint main() {\n\
    \    std::int64_t a, b;\n    std::cin >> a >> b;\n    std::cout << a + b << '\\\
    n';\n}\n"
  dependsOn:
  - math/modint998.hpp
  isVerificationFile: true
  path: verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
  requiredBy: []
  timestamp: '2026-08-15 14:35:57+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
- /verify/verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp.html
title: verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
---

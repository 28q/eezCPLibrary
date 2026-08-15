---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
    title: verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 1 \"math/modint998.hpp\"\n\n\n\r\n#include <cassert>\r\n#include\
    \ <cstdint>\r\n#include <iostream>\r\n#include <type_traits>\r\n\r\nstruct modint998\
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
    \  }\r\n\r\n    constexpr modint998 pow(u64 exponent) const {\r\n        modint998\
    \ result = raw(1);\r\n        modint998 base = *this;\r\n        while (exponent\
    \ != 0) {\r\n            if (exponent & 1) result *= base;\r\n            base\
    \ *= base;\r\n            exponent >>= 1;\r\n        }\r\n        return result;\r\
    \n    }\r\n\r\n    constexpr modint998 inv() const {\n        assert(val() !=\
    \ 0);\n\n        const modint998 x = *this;\n        const modint998 a2 = x *\
    \ x;\n        const modint998 a4 = a2 * a2;\n        const modint998 a5 = a4 *\
    \ x;\n        const modint998 a9 = a5 * a4;\n        const modint998 a18 = a9\
    \ * a9;\n        const modint998 a36 = a18 * a18;\n        const modint998 a72\
    \ = a36 * a36;\n        const modint998 a144 = a72 * a72;\n        const modint998\
    \ a288 = a144 * a144;\n        const modint998 a293 = a288 * a5;\n        const\
    \ modint998 a586 = a293 * a293;\n        const modint998 a879 = a586 * a293;\n\
    \        const modint998 a1023 = a879 * a144;\n\n        modint998 r = a1023 *\
    \ a879;  // x^1902\n\n        // x^(1902 * 2^9) = x^973824\n        r *= r;\n\
    \        r *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n        r\
    \ *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n\n        r *= a1023;\
    \  // x^974847\n\n        // x^(974847 * 2^10) = x^998243328\n        r *= r;\n\
    \        r *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n        r\
    \ *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n\n\
    \        return r * a1023;  // x^(MOD - 2)\n    }\n\r\n    constexpr modint998\
    \ inverse() const { return inv(); }\r\n\r\n    friend std::ostream& operator<<(std::ostream&\
    \ os, const modint998& x) {\r\n        return os << x.val();\r\n    }\r\n\r\n\
    \    friend std::istream& operator>>(std::istream& is, modint998& x) {\r\n   \
    \     std::int64_t value;\r\n        is >> value;\r\n        x = modint998(value);\r\
    \n        return is;\r\n    }\r\n};\r\n\r\nstatic_assert(sizeof(modint998) ==\
    \ 4);\r\nstatic_assert(std::is_trivially_copyable_v<modint998>);\r\n\r\nusing\
    \ mint998 = modint998;\r\n\r\n\n"
  code: "#ifndef EEZCP_MATH_MODINT998_HPP\r\n#define EEZCP_MATH_MODINT998_HPP\r\n\r\
    \n#include <cassert>\r\n#include <cstdint>\r\n#include <iostream>\r\n#include\
    \ <type_traits>\r\n\r\nstruct modint998 {\r\n    using u32 = std::uint32_t;\r\n\
    \    using i32 = std::int32_t;\r\n    using u64 = std::uint64_t;\r\n\r\n    static\
    \ constexpr u32 MOD = 998244353u;\r\n    static constexpr u32 MOD2 = MOD * 2;\r\
    \n    static constexpr u32 primitive_root = 3;\r\n    static constexpr int max_power_of_two\
    \ = 23;\r\n\r\nprivate:\r\n    static constexpr u32 R = 3296722945u;\r\n    static\
    \ constexpr u32 N2 = 932051910u;\r\n\r\n    struct montgomery_tag {};\r\n\r\n\
    \    constexpr modint998(u32 x, montgomery_tag) : a(x) {}\r\n\r\n    static constexpr\
    \ u32 reduce(u64 x) {\r\n        return static_cast<u32>(\r\n            (x +\
    \ u64(static_cast<u32>(x) * u32(-R)) * MOD) >> 32\r\n        );\r\n    }\r\n\r\
    \npublic:\r\n    u32 a;\r\n\r\n    static_assert(MOD < (u32(1) << 30));\r\n  \
    \  static_assert((MOD & 1) != 0);\r\n    static_assert(R * MOD == 1);\r\n\r\n\
    \    constexpr modint998() : a(0) {}\r\n\r\n    template <class T, std::enable_if_t<std::is_integral_v<T>\
    \ &&\r\n                                        std::is_signed_v<T>, int> = 0>\r\
    \n    constexpr modint998(T x) : a(0) {\r\n        const std::int64_t y =\r\n\
    \            static_cast<std::int64_t>(x) % std::int64_t(MOD) + MOD;\r\n     \
    \   a = reduce(u64(y) * N2);\r\n    }\r\n\r\n    template <class T, std::enable_if_t<std::is_integral_v<T>\
    \ &&\r\n                                        std::is_unsigned_v<T>, int> =\
    \ 0>\r\n    constexpr modint998(T x)\r\n        : a(reduce(((u64(x) % MOD) + MOD)\
    \ * N2)) {}\r\n\r\n    static constexpr modint998 raw(u32 x) {\r\n        return\
    \ modint998(reduce(u64(x) * N2), montgomery_tag{});\r\n    }\r\n\r\n    static\
    \ constexpr modint998 montgomery_raw(u32 x) {\r\n        return modint998(x, montgomery_tag{});\r\
    \n    }\r\n\r\n    static constexpr u32 mod() { return MOD; }\r\n    static constexpr\
    \ u32 get_mod() { return MOD; }\r\n\r\n    constexpr u32 val() const {\r\n   \
    \     const u32 x = reduce(a);\r\n        return x >= MOD ? x - MOD : x;\r\n \
    \   }\r\n\r\n    constexpr u32 get() const { return val(); }\r\n\r\n    constexpr\
    \ modint998& operator+=(const modint998& rhs) {\r\n        a += rhs.a - MOD2;\r\
    \n        if (i32(a) < 0) a += MOD2;\r\n        return *this;\r\n    }\r\n\r\n\
    \    constexpr modint998& operator-=(const modint998& rhs) {\r\n        a -= rhs.a;\r\
    \n        if (i32(a) < 0) a += MOD2;\r\n        return *this;\r\n    }\r\n\r\n\
    \    constexpr modint998& operator*=(const modint998& rhs) {\r\n        a = reduce(u64(a)\
    \ * rhs.a);\r\n        return *this;\r\n    }\r\n\r\n    constexpr modint998&\
    \ operator/=(const modint998& rhs) {\r\n        return *this *= rhs.inv();\r\n\
    \    }\r\n\r\n    constexpr modint998 operator+() const { return *this; }\r\n\
    \    constexpr modint998 operator-() const { return modint998() - *this; }\r\n\
    \r\n    friend constexpr modint998 operator+(modint998 lhs,\r\n              \
    \                            const modint998& rhs) {\r\n        return lhs +=\
    \ rhs;\r\n    }\r\n\r\n    friend constexpr modint998 operator-(modint998 lhs,\r\
    \n                                          const modint998& rhs) {\r\n      \
    \  return lhs -= rhs;\r\n    }\r\n\r\n    friend constexpr modint998 operator*(modint998\
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
    \  }\r\n\r\n    constexpr modint998 pow(u64 exponent) const {\r\n        modint998\
    \ result = raw(1);\r\n        modint998 base = *this;\r\n        while (exponent\
    \ != 0) {\r\n            if (exponent & 1) result *= base;\r\n            base\
    \ *= base;\r\n            exponent >>= 1;\r\n        }\r\n        return result;\r\
    \n    }\r\n\r\n    constexpr modint998 inv() const {\n        assert(val() !=\
    \ 0);\n\n        const modint998 x = *this;\n        const modint998 a2 = x *\
    \ x;\n        const modint998 a4 = a2 * a2;\n        const modint998 a5 = a4 *\
    \ x;\n        const modint998 a9 = a5 * a4;\n        const modint998 a18 = a9\
    \ * a9;\n        const modint998 a36 = a18 * a18;\n        const modint998 a72\
    \ = a36 * a36;\n        const modint998 a144 = a72 * a72;\n        const modint998\
    \ a288 = a144 * a144;\n        const modint998 a293 = a288 * a5;\n        const\
    \ modint998 a586 = a293 * a293;\n        const modint998 a879 = a586 * a293;\n\
    \        const modint998 a1023 = a879 * a144;\n\n        modint998 r = a1023 *\
    \ a879;  // x^1902\n\n        // x^(1902 * 2^9) = x^973824\n        r *= r;\n\
    \        r *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n        r\
    \ *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n\n        r *= a1023;\
    \  // x^974847\n\n        // x^(974847 * 2^10) = x^998243328\n        r *= r;\n\
    \        r *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n        r\
    \ *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n        r *= r;\n\n\
    \        return r * a1023;  // x^(MOD - 2)\n    }\n\r\n    constexpr modint998\
    \ inverse() const { return inv(); }\r\n\r\n    friend std::ostream& operator<<(std::ostream&\
    \ os, const modint998& x) {\r\n        return os << x.val();\r\n    }\r\n\r\n\
    \    friend std::istream& operator>>(std::istream& is, modint998& x) {\r\n   \
    \     std::int64_t value;\r\n        is >> value;\r\n        x = modint998(value);\r\
    \n        return is;\r\n    }\r\n};\r\n\r\nstatic_assert(sizeof(modint998) ==\
    \ 4);\r\nstatic_assert(std::is_trivially_copyable_v<modint998>);\r\n\r\nusing\
    \ mint998 = modint998;\r\n\r\n#endif\r\n"
  dependsOn: []
  isVerificationFile: false
  path: math/modint998.hpp
  requiredBy: []
  timestamp: '2026-08-15 13:53:56+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
documentation_of: math/modint998.hpp
layout: document
redirect_from:
- /library/math/modint998.hpp
- /library/math/modint998.hpp.html
title: math/modint998.hpp
---

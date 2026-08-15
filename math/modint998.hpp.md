---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: convolution/ntt998.hpp
    title: ntt998
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
    \ mint998 = modint998;\r\n\r\n#endif  // EEZCP_MATH_MODINT998_HPP\r\n"
  dependsOn: []
  isVerificationFile: false
  path: math/modint998.hpp
  requiredBy:
  - convolution/ntt998.hpp
  timestamp: '2026-08-15 14:35:57+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
documentation_of: math/modint998.hpp
layout: document
title: modint998
---

## 概要

法を素数 $998244353 = 119 \cdot 2^{23} + 1$ に固定した整数型である。
値は遅延Montgomery形式で保持し、四則演算、累乗、逆元、入出力を提供する。
事前計算用のテーブルを持たず、オブジェクトの大きさは4 byteである。

C++17以降を必要とする。

## 基本的な使い方

```cpp
#include <iostream>
#include "math/modint998.hpp"

int main() {
    modint998 a, b;
    std::cin >> a >> b;

    std::cout << a + b << '\n';
    std::cout << a * b << '\n';
    std::cout << a.pow(10) << '\n';
}
```

`mint998`は`modint998`の別名である。

## コンストラクタ

```cpp
modint998();
modint998(T x);
```

- デフォルト値は$0$
- 64 bit以下の符号付き・符号なし整数から構築可能
- 入力値は自動的に$[0, 998244353)$へ正規化
- 浮動小数点数からの構築は不可
- ストリーム入力は符号付き64-bit整数の範囲に対応

## 演算

```cpp
a += b;  a -= b;  a *= b;  a /= b;
a + b;   a - b;   a * b;   a / b;
+a;      -a;
++a;     a++;
--a;     a--;
a == b;  a != b;
```

除算は右辺の逆元との乗算である。右辺が$0$でないことが前提となる。

## 値の取得

```cpp
std::uint32_t x = a.val();
std::uint32_t y = a.get();
```

`val()`と`get()`は通常表現の値を$[0, 998244353)$で返す。

メンバ`a`は特殊化したNTTなどから遅延Montgomery表現へ直接アクセスするために公開されている。
通常の整数値として扱う場合は`val()`または`get()`を用いる。

## 累乗・逆元

```cpp
modint998 y = a.pow(exponent);
modint998 z = a.inv();
modint998 w = a.inverse();
```

- `pow(exponent)`：非負整数`exponent`乗
- `inv()` / `inverse()`：乗法逆元。呼び出し時に値が$0$でないことが前提
- `inv()`は固定された加算鎖により35回のMontgomery乗算で計算

## 定数

```cpp
modint998::MOD                 // 998244353
modint998::MOD2                // 2 * MOD
modint998::primitive_root      // 3
modint998::max_power_of_two    // 23
modint998::mod()
modint998::get_mod()
```

## raw生成

```cpp
modint998 x = modint998::raw(value);
modint998 y = modint998::montgomery_raw(word);
```

- `raw(value)`：`value`が$[0, MOD)$であることを前提に通常表現から生成
- `montgomery_raw(word)`：有効な遅延Montgomery表現を直接格納

どちらも入力範囲を検査しない。前提を満たせる特殊化コードでのみ用いる。

## 計算量

- 加減乗算、比較、値の取得：$O(1)$
- 除算、逆元：$O(1)$（固定35回のMontgomery乗算）
- `pow(exponent)`：$O(\log exponent)$
- 使用メモリ：1要素あたり4 byte

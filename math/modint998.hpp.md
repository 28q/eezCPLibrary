---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 1 \"math/modint998.hpp\"\n\n\n\n#include <cassert>\n#include\
    \ <cstdint>\n#include <iostream>\n#include <type_traits>\n\nstruct modint998 {\n\
    \    using u32 = std::uint32_t;\n    using i32 = std::int32_t;\n    using u64\
    \ = std::uint64_t;\n\n    static constexpr u32 MOD = 998244353u;\n    static constexpr\
    \ u32 MOD2 = MOD * 2;\n    static constexpr u32 primitive_root = 3;\n    static\
    \ constexpr int max_power_of_two = 23;\n\nprivate:\n    static constexpr u32 R\
    \ = 3296722945u;\n    static constexpr u32 N2 = 932051910u;\n\n    struct montgomery_tag\
    \ {};\n\n    constexpr modint998(u32 x, montgomery_tag) : a(x) {}\n\n    static\
    \ constexpr u32 reduce(u64 x) {\n        return static_cast<u32>(\n          \
    \  (x + u64(static_cast<u32>(x) * u32(-R)) * MOD) >> 32\n        );\n    }\n\n\
    public:\n    u32 a;\n\n    static_assert(MOD < (u32(1) << 30));\n    static_assert((MOD\
    \ & 1) != 0);\n    static_assert(R * MOD == 1);\n\n    constexpr modint998() :\
    \ a(0) {}\n\n    template <class T, std::enable_if_t<std::is_integral_v<T> &&\n\
    \                                        std::is_signed_v<T>, int> = 0>\n    constexpr\
    \ modint998(T x) : a(0) {\n        const std::int64_t y =\n            static_cast<std::int64_t>(x)\
    \ % std::int64_t(MOD) + MOD;\n        a = reduce(u64(y) * N2);\n    }\n\n    template\
    \ <class T, std::enable_if_t<std::is_integral_v<T> &&\n                      \
    \                  std::is_unsigned_v<T>, int> = 0>\n    constexpr modint998(T\
    \ x)\n        : a(reduce(((u64(x) % MOD) + MOD) * N2)) {}\n\n    static constexpr\
    \ modint998 raw(u32 x) {\n        return modint998(reduce(u64(x) * N2), montgomery_tag{});\n\
    \    }\n\n    static constexpr modint998 montgomery_raw(u32 x) {\n        return\
    \ modint998(x, montgomery_tag{});\n    }\n\n    static constexpr u32 mod() { return\
    \ MOD; }\n    static constexpr u32 get_mod() { return MOD; }\n\n    constexpr\
    \ u32 val() const {\n        const u32 x = reduce(a);\n        return x >= MOD\
    \ ? x - MOD : x;\n    }\n\n    constexpr u32 get() const { return val(); }\n\n\
    \    constexpr modint998& operator+=(const modint998& rhs) {\n        a += rhs.a\
    \ - MOD2;\n        if (i32(a) < 0) a += MOD2;\n        return *this;\n    }\n\n\
    \    constexpr modint998& operator-=(const modint998& rhs) {\n        a -= rhs.a;\n\
    \        if (i32(a) < 0) a += MOD2;\n        return *this;\n    }\n\n    constexpr\
    \ modint998& operator*=(const modint998& rhs) {\n        a = reduce(u64(a) * rhs.a);\n\
    \        return *this;\n    }\n\n    constexpr modint998& operator/=(const modint998&\
    \ rhs) {\n        return *this *= rhs.inv();\n    }\n\n    constexpr modint998\
    \ operator+() const { return *this; }\n    constexpr modint998 operator-() const\
    \ { return modint998() - *this; }\n\n    friend constexpr modint998 operator+(modint998\
    \ lhs,\n                                          const modint998& rhs) {\n  \
    \      return lhs += rhs;\n    }\n\n    friend constexpr modint998 operator-(modint998\
    \ lhs,\n                                          const modint998& rhs) {\n  \
    \      return lhs -= rhs;\n    }\n\n    friend constexpr modint998 operator*(modint998\
    \ lhs,\n                                          const modint998& rhs) {\n  \
    \      return lhs *= rhs;\n    }\n\n    friend constexpr modint998 operator/(modint998\
    \ lhs,\n                                          const modint998& rhs) {\n  \
    \      return lhs /= rhs;\n    }\n\n    friend constexpr bool operator==(const\
    \ modint998& lhs,\n                                     const modint998& rhs)\
    \ {\n        const u32 x = lhs.a >= MOD ? lhs.a - MOD : lhs.a;\n        const\
    \ u32 y = rhs.a >= MOD ? rhs.a - MOD : rhs.a;\n        return x == y;\n    }\n\
    \n    friend constexpr bool operator!=(const modint998& lhs,\n               \
    \                      const modint998& rhs) {\n        return !(lhs == rhs);\n\
    \    }\n\n    constexpr modint998& operator++() {\n        return *this += raw(1);\n\
    \    }\n\n    constexpr modint998 operator++(int) {\n        modint998 old = *this;\n\
    \        ++*this;\n        return old;\n    }\n\n    constexpr modint998& operator--()\
    \ {\n        return *this -= raw(1);\n    }\n\n    constexpr modint998 operator--(int)\
    \ {\n        modint998 old = *this;\n        --*this;\n        return old;\n \
    \   }\n\n    constexpr modint998 pow(u64 exponent) const {\n        modint998\
    \ result = raw(1);\n        modint998 base = *this;\n        while (exponent !=\
    \ 0) {\n            if (exponent & 1) result *= base;\n            base *= base;\n\
    \            exponent >>= 1;\n        }\n        return result;\n    }\n\n   \
    \ constexpr modint998 inv() const {\n        assert(val() != 0);\n\n        const\
    \ modint998 x = *this;\n        modint998 a2 = x * x;\n        modint998 a4 =\
    \ a2 * a2;\n        modint998 a8 = a4 * a4;\n        modint998 a16 = a8 * a8;\n\
    \        modint998 a32 = a16 * a16;\n        modint998 a33 = a32 * x;\n      \
    \  modint998 a49 = a33 * a16;\n        modint998 a82 = a49 * a33;\n        modint998\
    \ a164 = a82 * a82;\n        modint998 a328 = a164 * a164;\n        modint998\
    \ r = a328 * a49;\n\n        a2 = r * r;\n        a4 = a2 * a2;\n        a8 =\
    \ a4 * a4;\n        modint998 a9 = a8 * r;\n        modint998 a18 = a9 * a9;\n\
    \        r = a18 * a9;\n\n        a2 = r * r;\n        a4 = a2 * a2;\n       \
    \ a8 = a4 * a4;\n        a16 = a8 * a8;\n        modint998 a17 = a16 * r;\n  \
    \      a33 = a17 * a16;\n        modint998 a66 = a33 * a33;\n        modint998\
    \ a132 = a66 * a66;\n        modint998 a264 = a132 * a132;\n        r = a264 *\
    \ a17;\n\n        a2 = r * r;\n        a4 = a2 * a2;\n        a8 = a4 * a4;\n\
    \        a16 = a8 * a8;\n        a32 = a16 * a16;\n        modint998 a64 = a32\
    \ * a32;\n        modint998 a65 = a64 * r;\n        modint998 a73 = a65 * a8;\n\
    \        modint998 a138 = a73 * a65;\n        modint998 a276 = a138 * a138;\n\
    \        return a276 * a73;\n    }\n\n    constexpr modint998 inverse() const\
    \ { return inv(); }\n\n    friend std::ostream& operator<<(std::ostream& os, const\
    \ modint998& x) {\n        return os << x.val();\n    }\n\n    friend std::istream&\
    \ operator>>(std::istream& is, modint998& x) {\n        std::int64_t value;\n\
    \        is >> value;\n        x = modint998(value);\n        return is;\n   \
    \ }\n};\n\nstatic_assert(sizeof(modint998) == 4);\nstatic_assert(std::is_trivially_copyable_v<modint998>);\n\
    \nusing mint998 = modint998;\n\n\n"
  code: "#ifndef EEZCP_MATH_MODINT998_HPP\n#define EEZCP_MATH_MODINT998_HPP\n\n#include\
    \ <cassert>\n#include <cstdint>\n#include <iostream>\n#include <type_traits>\n\
    \nstruct modint998 {\n    using u32 = std::uint32_t;\n    using i32 = std::int32_t;\n\
    \    using u64 = std::uint64_t;\n\n    static constexpr u32 MOD = 998244353u;\n\
    \    static constexpr u32 MOD2 = MOD * 2;\n    static constexpr u32 primitive_root\
    \ = 3;\n    static constexpr int max_power_of_two = 23;\n\nprivate:\n    static\
    \ constexpr u32 R = 3296722945u;\n    static constexpr u32 N2 = 932051910u;\n\n\
    \    struct montgomery_tag {};\n\n    constexpr modint998(u32 x, montgomery_tag)\
    \ : a(x) {}\n\n    static constexpr u32 reduce(u64 x) {\n        return static_cast<u32>(\n\
    \            (x + u64(static_cast<u32>(x) * u32(-R)) * MOD) >> 32\n        );\n\
    \    }\n\npublic:\n    u32 a;\n\n    static_assert(MOD < (u32(1) << 30));\n  \
    \  static_assert((MOD & 1) != 0);\n    static_assert(R * MOD == 1);\n\n    constexpr\
    \ modint998() : a(0) {}\n\n    template <class T, std::enable_if_t<std::is_integral_v<T>\
    \ &&\n                                        std::is_signed_v<T>, int> = 0>\n\
    \    constexpr modint998(T x) : a(0) {\n        const std::int64_t y =\n     \
    \       static_cast<std::int64_t>(x) % std::int64_t(MOD) + MOD;\n        a = reduce(u64(y)\
    \ * N2);\n    }\n\n    template <class T, std::enable_if_t<std::is_integral_v<T>\
    \ &&\n                                        std::is_unsigned_v<T>, int> = 0>\n\
    \    constexpr modint998(T x)\n        : a(reduce(((u64(x) % MOD) + MOD) * N2))\
    \ {}\n\n    static constexpr modint998 raw(u32 x) {\n        return modint998(reduce(u64(x)\
    \ * N2), montgomery_tag{});\n    }\n\n    static constexpr modint998 montgomery_raw(u32\
    \ x) {\n        return modint998(x, montgomery_tag{});\n    }\n\n    static constexpr\
    \ u32 mod() { return MOD; }\n    static constexpr u32 get_mod() { return MOD;\
    \ }\n\n    constexpr u32 val() const {\n        const u32 x = reduce(a);\n   \
    \     return x >= MOD ? x - MOD : x;\n    }\n\n    constexpr u32 get() const {\
    \ return val(); }\n\n    constexpr modint998& operator+=(const modint998& rhs)\
    \ {\n        a += rhs.a - MOD2;\n        if (i32(a) < 0) a += MOD2;\n        return\
    \ *this;\n    }\n\n    constexpr modint998& operator-=(const modint998& rhs) {\n\
    \        a -= rhs.a;\n        if (i32(a) < 0) a += MOD2;\n        return *this;\n\
    \    }\n\n    constexpr modint998& operator*=(const modint998& rhs) {\n      \
    \  a = reduce(u64(a) * rhs.a);\n        return *this;\n    }\n\n    constexpr\
    \ modint998& operator/=(const modint998& rhs) {\n        return *this *= rhs.inv();\n\
    \    }\n\n    constexpr modint998 operator+() const { return *this; }\n    constexpr\
    \ modint998 operator-() const { return modint998() - *this; }\n\n    friend constexpr\
    \ modint998 operator+(modint998 lhs,\n                                       \
    \   const modint998& rhs) {\n        return lhs += rhs;\n    }\n\n    friend constexpr\
    \ modint998 operator-(modint998 lhs,\n                                       \
    \   const modint998& rhs) {\n        return lhs -= rhs;\n    }\n\n    friend constexpr\
    \ modint998 operator*(modint998 lhs,\n                                       \
    \   const modint998& rhs) {\n        return lhs *= rhs;\n    }\n\n    friend constexpr\
    \ modint998 operator/(modint998 lhs,\n                                       \
    \   const modint998& rhs) {\n        return lhs /= rhs;\n    }\n\n    friend constexpr\
    \ bool operator==(const modint998& lhs,\n                                    \
    \ const modint998& rhs) {\n        const u32 x = lhs.a >= MOD ? lhs.a - MOD :\
    \ lhs.a;\n        const u32 y = rhs.a >= MOD ? rhs.a - MOD : rhs.a;\n        return\
    \ x == y;\n    }\n\n    friend constexpr bool operator!=(const modint998& lhs,\n\
    \                                     const modint998& rhs) {\n        return\
    \ !(lhs == rhs);\n    }\n\n    constexpr modint998& operator++() {\n        return\
    \ *this += raw(1);\n    }\n\n    constexpr modint998 operator++(int) {\n     \
    \   modint998 old = *this;\n        ++*this;\n        return old;\n    }\n\n \
    \   constexpr modint998& operator--() {\n        return *this -= raw(1);\n   \
    \ }\n\n    constexpr modint998 operator--(int) {\n        modint998 old = *this;\n\
    \        --*this;\n        return old;\n    }\n\n    constexpr modint998 pow(u64\
    \ exponent) const {\n        modint998 result = raw(1);\n        modint998 base\
    \ = *this;\n        while (exponent != 0) {\n            if (exponent & 1) result\
    \ *= base;\n            base *= base;\n            exponent >>= 1;\n        }\n\
    \        return result;\n    }\n\n    constexpr modint998 inv() const {\n    \
    \    assert(val() != 0);\n\n        const modint998 x = *this;\n        modint998\
    \ a2 = x * x;\n        modint998 a4 = a2 * a2;\n        modint998 a8 = a4 * a4;\n\
    \        modint998 a16 = a8 * a8;\n        modint998 a32 = a16 * a16;\n      \
    \  modint998 a33 = a32 * x;\n        modint998 a49 = a33 * a16;\n        modint998\
    \ a82 = a49 * a33;\n        modint998 a164 = a82 * a82;\n        modint998 a328\
    \ = a164 * a164;\n        modint998 r = a328 * a49;\n\n        a2 = r * r;\n \
    \       a4 = a2 * a2;\n        a8 = a4 * a4;\n        modint998 a9 = a8 * r;\n\
    \        modint998 a18 = a9 * a9;\n        r = a18 * a9;\n\n        a2 = r * r;\n\
    \        a4 = a2 * a2;\n        a8 = a4 * a4;\n        a16 = a8 * a8;\n      \
    \  modint998 a17 = a16 * r;\n        a33 = a17 * a16;\n        modint998 a66 =\
    \ a33 * a33;\n        modint998 a132 = a66 * a66;\n        modint998 a264 = a132\
    \ * a132;\n        r = a264 * a17;\n\n        a2 = r * r;\n        a4 = a2 * a2;\n\
    \        a8 = a4 * a4;\n        a16 = a8 * a8;\n        a32 = a16 * a16;\n   \
    \     modint998 a64 = a32 * a32;\n        modint998 a65 = a64 * r;\n        modint998\
    \ a73 = a65 * a8;\n        modint998 a138 = a73 * a65;\n        modint998 a276\
    \ = a138 * a138;\n        return a276 * a73;\n    }\n\n    constexpr modint998\
    \ inverse() const { return inv(); }\n\n    friend std::ostream& operator<<(std::ostream&\
    \ os, const modint998& x) {\n        return os << x.val();\n    }\n\n    friend\
    \ std::istream& operator>>(std::istream& is, modint998& x) {\n        std::int64_t\
    \ value;\n        is >> value;\n        x = modint998(value);\n        return\
    \ is;\n    }\n};\n\nstatic_assert(sizeof(modint998) == 4);\nstatic_assert(std::is_trivially_copyable_v<modint998>);\n\
    \nusing mint998 = modint998;\n\n#endif\n"
  dependsOn: []
  isVerificationFile: false
  path: math/modint998.hpp
  requiredBy: []
  timestamp: '2026-08-13 02:29:52+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
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
- `inv()`は固定された加算鎖により38回のMontgomery乗算で計算

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
- 除算、逆元：$O(1)$（固定38回のMontgomery乗算）
- `pow(exponent)`：$O(\log exponent)$
- 使用メモリ：1要素あたり4 byte

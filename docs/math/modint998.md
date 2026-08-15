---
title: modint998
documentation_of: ../../math/modint998.hpp
---

## 概要

法を素数 $998244353 = 119 \cdot 2^{23} + 1$ に固定した整数型である。
値は遅延Montgomery形式で保持し、四則演算、累乗、逆元、入出力を提供する。
事前計算用のテーブルを持たず、オブジェクトの大きさは4 byteである。

C++20以降を必要とする。

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

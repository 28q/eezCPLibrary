---
title: UnionFind
documentation_of: ../../data-structure/UnionFind.hpp
---

## 概要

互いに素な集合を管理するデータ構造である。
要素は$0$以上`size`未満の整数で表す。

## 基本的な使い方

```cpp
#include <cstdint>
#include <vector>
using namespace std;

#include "data-structure/UnionFind.hpp"

UnionFind uf(5);
uf.merge(0, 1);
uf.merge(1, 2);

bool connected = uf.same(0, 2);
```

このヘッダーは`vector`を修飾なしで用いるため、読み込み前に`<vector>`、`<cstdint>`、
`using namespace std;`が必要である。

## コンストラクタ

```cpp
explicit UnionFind(int n);
```

`n`個の要素を、それぞれ独立した集合として初期化する。

## merge

```cpp
bool merge(int32_t x, int32_t y) noexcept;
```

`x`を含む集合と`y`を含む集合をサイズの大きい側へ併合する。
新たに併合した場合は`true`、すでに同じ集合であった場合は`false`を返す。

## same

```cpp
bool same(int32_t x, int32_t y) noexcept;
```

`x`と`y`が同じ集合に属する場合に`true`を返す。
探索中に経路圧縮を行うため、オブジェクトの内部状態を変更する。

## 計算量

- 構築：$O(N)$
- `merge` / `same`：償却$O(\alpha(N))$
- メモリ：$O(N)$

$\alpha$は逆Ackermann関数である。
添字の範囲検査は行わない。`0 <= x, y < N`が前提となる。

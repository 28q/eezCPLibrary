---
title: fastio_safe
documentation_of: ../../IO/fastio_safe.hpp
---

## 概要

入力の終端、不正なトークン、整数の範囲外、I/Oエラーを検出できる、
C++17以降向けのバッファ付き入出力である。

整数（処理系が対応する場合は128-bit整数を含む）、浮動小数点数、文字、
文字列、`std::pair`、`std::vector`、`std::vector<bool>`に対応する。

## 基本的な使い方

```cpp
#include "IO/fastio_safe.hpp"

int main() {
    long long a, b;
    if (!fastio_safe::scan(a, b)) return 1;
    if (!fastio_safe::print(a + b)) return 1;
}
```

`scan`はすべての値を読み取れた場合、`print`は出力をバッファへ追加できた場合に
`true`を返す。`print(a, b, c)`は値を空白で区切り、末尾に改行を加える。

## 入力

```cpp
int n;
long long x;
std::string s;

if (!fastio_safe::scan(n, x, s)) return 1;
```

直前の入力結果は`fastio_safe::last_scan_error()`で確認できる。

```cpp
switch (fastio_safe::last_scan_error()) {
case fastio_safe::scan_error::none:
    break;
case fastio_safe::scan_error::end_of_file:
    break;
case fastio_safe::scan_error::invalid_input:
    break;
case fastio_safe::scan_error::out_of_range:
    break;
case fastio_safe::scan_error::io_error:
    break;
}
```

符号なし整数への負数、整数として不正なトークン、型の範囲外の値に対しては
`scan`が`false`を返す。

## 出力

```cpp
fastio_safe::print(1, 2, 3);       // "1 2 3\n"
fastio_safe::printout("answer="); // 改行・区切りを自動追加しない
fastio_safe::printflush(42);       // 出力後にstdoutをflush
fastio_safe::debug("x", 42);      // stderrへ出力してflush
```

`printout`は区切りと改行を加えず、`debug`は空白区切りと改行を加えて`stderr`へ出力する。
明示的なflushには`fastio_safe::flush_out()`または`fastio_safe::flush_err()`を用いる。
通常終了時にも自動的にflushされる。

## 128-bit整数

GCC/Clangで`__int128_t`と`__uint128_t`を利用できる場合は、そのまま
`scan`と`print`へ渡せる。

```cpp
__int128_t a, b;
fastio_safe::scan(a, b);
fastio_safe::print(a + b);
```

## POSIX I/O

既定の入出力は`fread` / `fwrite`である。Linuxなどで`read` / `write`を
用いる場合は、ヘッダーより前に次を定義する。

```cpp
#define FASTIO_SAFE_USE_POSIX 1
#include "IO/fastio_safe.hpp"
```

どちらの経路も部分書き込みを処理し、`EINTR`を再試行する。

## SIMD

x86環境では空白スキップにAVX2を利用できる。AVX2部分は関数単位で
コンパイルされ、実行時にCPU対応を確認してから呼ばれる。
ライブラリ全体への`-mavx2`指定は不要であり、非対応CPUではスカラ実装を用いる。

## 補助API

- `read_one(value)` / `write_one(value)`：1値の入出力
- `write_char(c)` / `write_char_err(c)`：1文字の出力
- `debugout(values...)`：区切りと改行を加えない`stderr`出力
- `fin(values...)`：出力とflushを行い、終了コード0で終了
- `INT`、`LL`、`STR`、`CHR`、`DBL`、`LD`：宣言と入力をまとめるマクロ
- 名前空間内の主要関数はグローバル名前空間にも導入済み

## 注意事項

- C++17以降が必要
- 内部バッファはstdinに1 MiB、stdoutとstderrに各256 KiB
- 同じストリームで`std::cin` / `std::cout`などとの混在は不可
- 内部状態を共有するため、複数スレッドからの同時呼び出しは不可
- `scan(a, b)`の途中で失敗した場合、失敗前に読み取った引数の値は更新済み

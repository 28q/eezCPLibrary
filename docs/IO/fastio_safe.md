---
title: fastio_safe
documentation_of: ../../IO/fastio_safe.hpp
---

## 概要

入力の終端、不正なトークン、整数の範囲外、I/Oエラーを検出できる、
C++17以降向けのバッファ付き入出力です。

整数（処理系が対応する場合は128-bit整数を含む）、浮動小数点数、文字、
文字列、`std::pair`、`std::vector`、`std::vector<bool>`に対応します。

## 基本的な使い方

```cpp
#include "IO/fastio_safe.hpp"

int main() {
    long long a, b;
    if (!fastio_safe::scan(a, b)) return 1;
    if (!fastio_safe::print(a + b)) return 1;
}
```

`scan`はすべての値を読み取れたとき、`print`は出力をバッファへ追加できた
ときに`true`を返します。`print(a, b, c)`は値を空白で区切り、最後に改行を
追加します。

## 入力

```cpp
int n;
long long x;
std::string s;

if (!fastio_safe::scan(n, x, s)) {
    // 入力エラー
}
```

直前の入力結果は`fastio_safe::last_scan_error()`で確認できます。

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

符号なし整数へ負数を入力した場合、数字以外を整数として読んだ場合、または
型の範囲を超えた場合は`scan`が`false`を返します。

## 出力

```cpp
fastio_safe::print(1, 2, 3);       // "1 2 3\n"
fastio_safe::printout("answer="); // 改行・区切りを自動追加しない
fastio_safe::printflush(42);       // 出力後にstdoutをflush
fastio_safe::debug("x", 42);      // stderrへ出力してflush
```

明示的にflushする場合は`fastio_safe::flush_out()`または
`fastio_safe::flush_err()`を使います。通常はプログラム終了時にもflushされます。

## 128-bit整数

GCC/Clangで`__int128_t`と`__uint128_t`が利用できる場合は、そのまま
`scan`と`print`へ渡せます。

```cpp
__int128_t a, b;
fastio_safe::scan(a, b);
fastio_safe::print(a + b);
```

## POSIX I/O

既定では`fread` / `fwrite`を使用します。Linuxなどで`read` / `write`を
使用したい場合は、ヘッダーより前に次を定義します。

```cpp
#define FASTIO_SAFE_USE_POSIX 1
#include "IO/fastio_safe.hpp"
```

どちらの経路も部分書き込みを処理し、`EINTR`を再試行します。

## SIMD

x86環境では空白スキップにAVX2を利用できます。AVX2部分は関数単位で
コンパイルされ、実行時にCPU対応を確認してから呼ばれるため、ライブラリ全体を
`-mavx2`でコンパイルする必要はありません。非対応CPUではスカラ実装を使います。

## 注意事項

- C++17以降が必要です。
- 内部バッファはstdinに1 MiB、stdoutとstderrに各256 KiBです。
- 同じストリームを`std::cin` / `std::cout`などと混在させないでください。
- 内部状態は共有されるため、同時に複数スレッドから呼ばないでください。
- `INT`、`LL`、`STR`などの入力用マクロと、従来形式のグローバル関数も提供します。

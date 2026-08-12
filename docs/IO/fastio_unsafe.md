---
title: fastio_unsafe
documentation_of: ../../IO/fastio_unsafe.hpp
---

## 用途

速度優先・入力検査なしの、32/64/128-bit整数入出力である。

## ブロック設定

`FASTIO_UNSAFE_BLOCK_LOG`の既定値は`14`である。
必要に応じて`13`～`15`の整数へ変更可能であり、ヘッダーを読み込む前に定義する。

```cpp
#define FASTIO_UNSAFE_BLOCK_LOG 13
#include "IO/fastio_unsafe.hpp"
```

## 基本的な使い方

```cpp
fastio_unsafe io;

char* in = io.input_cursor();
char* out = io.output_cursor();
char* const out_end = io.output_end();

fastio_unsafe::i32 a = io.read_i32(in);
fastio_unsafe::i64 b = io.read_i64(in);
fastio_unsafe::i128 c = io.read_i128(in);

io.write_i32(out, out_end, a);
io.write_i64(out, out_end, b);
io.write_i128(out, out_end, c);

io.finish(out);
```

## 入力関数

```text
read_u32 / read_i32
read_u64 / read_i64
read_u128 / read_i128
```

引数のカーソルは、読み取った整数の直後にある1文字の区切りまで進む。
符号付き関数だけが先頭の`-`に対応する。

## 出力関数

```text
write_u32 / write_i32
write_u64 / write_i64
write_u128 / write_i128
```

各関数は値の前に空白を1文字出力する。`finish`が先頭の余分な空白を除去し、
末尾へ改行を追加して残りのバッファを出力する。

## 型名

```text
fastio_unsafe::u32 / i32
fastio_unsafe::u64 / i64
fastio_unsafe::u128 / i128
```

## 使える条件

- GCCまたはClangのGNU C++23でコンパイルする
- コンパイル時にAVX2、BMI、BMI2、LZCNT、POPCNTを有効にする
- 対応するx86-64 CPUだけで実行する（実行時CPU判定はない）
- 入力はASCIIの10進整数だけである
- 各整数の直後に、空白または改行がちょうど1個ある
- 先頭空白、連続空白、タブ混在、`+`符号、不正文字を与えない
- `read_i32` / `read_i64` / `read_i128`だけは先頭の`-`符号に対応する
- 呼び出した関数の型範囲内の値だけを入力する
- EOF、不正入力、桁あふれは検出しない
- 出力値は空白区切りで、`finish`時にだけ改行する
- スレッドセーフではなく、通常は1プロセスにつき1個だけ作る
- 入力用メモリは終了時まで解放しない
- Linuxの通常ファイル`stdin`では`mmap`を使用する
- パイプやWindowsでは入力をすべて読み終えてから処理を開始する

これらの前提に違反した場合の動作は保証されない。

## コンパイル例

```bash
g++ -std=gnu++23 -O3 -mavx2 -mbmi -mbmi2 -mlzcnt -mpopcnt main.cpp
```

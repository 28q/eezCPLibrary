#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_large"

#ifndef FASTIO_UNSAFE_BLOCK_LOG
#define FASTIO_UNSAFE_BLOCK_LOG 12
#endif

#include <cstddef>
#include <cstdint>

#include "../../convolution/ntt998.hpp"

#if defined(__GNUC__) && !defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#elif defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute push( \
    __attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,ssse3"))), \
    apply_to = function)
#endif

#include "../../IO/fastio_unsafe.hpp"

int main() {
    fastio_unsafe io;
    char* input_cursor = io.input_cursor();
    char* output_cursor = io.output_cursor();
    char* const output_end = io.output_end();

    const std::size_t n = io.read_u32(input_cursor);
    const std::size_t m = io.read_u32(input_cursor);

    eez::ntt998::convolution_normal_io(
        n, m,
        [&]() -> std::uint32_t {
            return io.read_u32_lt1e9(input_cursor);
        },
        [&](std::uint32_t value) {
            io.write_u32_lt1e9(output_cursor, output_end, value);
        }
    );

    io.finish(output_cursor);
    return 0;
}

#if defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute pop
#endif

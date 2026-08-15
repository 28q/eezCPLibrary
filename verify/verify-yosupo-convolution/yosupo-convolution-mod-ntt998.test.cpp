#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#ifndef FASTIO_UNSAFE_BLOCK_LOG
#define FASTIO_UNSAFE_BLOCK_LOG 12
#endif

#include <cstddef>
#include <vector>

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
    std::vector<eez::ntt998::mint> a(n), b(m);

    for (auto& value : a) {
        value = eez::ntt998::mint::raw(io.read_u32(input_cursor));
    }
    for (auto& value : b) {
        value = eez::ntt998::mint::raw(io.read_u32(input_cursor));
    }

    const auto result = eez::ntt998::convolution(a, b);
    for (const auto value : result) {
        io.write_u32(output_cursor, output_end, value.val());
    }
    io.finish(output_cursor);
}

#if defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute pop
#endif

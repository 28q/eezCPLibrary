#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_large"

#ifndef FASTIO_UNSAFE_BLOCK_LOG
#define FASTIO_UNSAFE_BLOCK_LOG 12
#endif

#include <bit>
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
    const std::size_t result_size = n + m - 1;
    const std::size_t transform_size = std::bit_ceil(result_size);
    using mint = eez::ntt998::mint;

    if (transform_size < 32) {
        std::vector<mint> a(n), b(m);
        for (auto& value : a) value = mint::raw(io.read_u32(input_cursor));
        for (auto& value : b) value = mint::raw(io.read_u32(input_cursor));

        const auto result = eez::ntt998::convolution(a, b);
        for (const mint value : result) {
            io.write_u32(output_cursor, output_end, value.val());
        }
    } else {
        eez::ntt998::convolution_buffer a(transform_size), b(transform_size);
        for (std::size_t i = 0; i < n; ++i) {
            a[i] = mint::raw(io.read_u32(input_cursor));
        }
        for (std::size_t i = 0; i < m; ++i) {
            b[i] = mint::raw(io.read_u32(input_cursor));
        }

        eez::ntt998::convolution_inplace(a, b);
        for (std::size_t i = 0; i < result_size; ++i) {
            io.write_u32(output_cursor, output_end, a[i].val());
        }
    }

    io.finish(output_cursor);
}

#if defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute pop
#endif


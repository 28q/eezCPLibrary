#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb"

#if defined(__GNUC__) && !defined(__clang__) && \
    (defined(__x86_64__) || defined(__i386__))
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#endif

#ifndef FASTIO_UNSAFE_BLOCK_LOG
#define FASTIO_UNSAFE_BLOCK_LOG 12
#endif

#include "../../IO/fastio_unsafe.hpp"

int main() {
    fastio_unsafe io;
    char* input_cursor = io.in.cursor();
    char* output_cursor = io.out.begin();
    char* const output_end = io.out.end();

    const fastio_unsafe_impl::u64 test_count =
        fastio_unsafe_impl::read_u64(input_cursor);
    constexpr fastio_unsafe_impl::u64 block_size =
        1u << FASTIO_UNSAFE_BLOCK_LOG;
    alignas(64) fastio_unsafe_impl::u64 sums[block_size];

    for (fastio_unsafe_impl::u64 base = 0;
         base < test_count; base += block_size) {
        const fastio_unsafe_impl::u64 count =
            std::min(block_size, test_count - base);
        for (fastio_unsafe_impl::u64 i = 0; i < count; ++i) {
            const fastio_unsafe_impl::u64 a =
                fastio_unsafe_impl::read_u64(input_cursor);
            const fastio_unsafe_impl::u64 b =
                fastio_unsafe_impl::read_u64(input_cursor);
            sums[i] = a + b;
        }
        for (fastio_unsafe_impl::u64 i = 0; i < count; ++i) {
            fastio_unsafe_impl::write_u64(
                io.out, output_cursor, output_end, sums[i]);
        }
    }
    io.out.finish(output_cursor);
}

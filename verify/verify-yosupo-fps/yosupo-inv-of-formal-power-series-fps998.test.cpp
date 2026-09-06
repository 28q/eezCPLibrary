#define PROBLEM "https://judge.yosupo.jp/problem/inv_of_formal_power_series"

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include "IO/fastio_unsafe.hpp"
#include "fps/fps998.hpp"

int main(){
    fastio_unsafe io;
    char* in=io.in.cursor();
    char* out=io.out.begin();
    char* const out_end=io.out.end();
    using u32=fastio_unsafe_impl::u32;
    using mint=eez::fps998::mint;
    const u32 n=fastio_unsafe_impl::read_u32_lt1e9(in);
    std::vector<mint> f;
    f.reserve(n);
    for(u32 i=0;i<n;++i) f.emplace_back(mint::raw(fastio_unsafe_impl::read_u32_lt1e9(in)));
    std::vector<mint> g(n);
    eez::fps998::workspace ws;
    eez::fps998::inv_to(f,g,ws);
    for(u32 i=0;i<n;++i) fastio_unsafe_impl::write_u32_lt1e9(io.out,out,out_end,g[i].get());
    io.out.finish(out);
}

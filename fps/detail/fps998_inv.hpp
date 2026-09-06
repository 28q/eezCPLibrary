#pragma once
#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>
#include "fps998_inv_ntt.hpp"

#if defined(__GNUC__) && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma GCC push_options
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#elif defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute push(__attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,ssse3"))),apply_to=function)
#endif

namespace eez::fps998::detail{
using mint=ntt998::mint;
using usize=std::size_t;
namespace nd=ntt998::detail;

inline constexpr usize inv_naive_cutoff=64;
static_assert(std::has_single_bit(inv_naive_cutoff));
static_assert(std::is_trivially_copyable_v<mint>);
static_assert(sizeof(mint)==sizeof(std::uint32_t));

struct inv_workspace{
    nd::aligned_vector a,b;
    void reserve(usize n){a.reserve(n);b.reserve(n);}
    void ensure(usize n){if(a.size()<n)a.resize(n);if(b.size()<n)b.resize(n);}
    [[nodiscard]] usize capacity() const noexcept{return std::min(a.capacity(),b.capacity());}
};

inline void inv_naive_prefix(std::span<const mint> f,std::span<mint> g){
    if(g.empty()) return;
    const mint g0=f[0].inv();
    g[0]=g0;
    for(usize i=1;i<g.size();++i){
        mint s{};
        const usize r=std::min(i,f.size()-1);
        for(usize j=1;j<=r;++j) s+=f[j]*g[i-j];
        g[i]=-s*g0;
    }
}

inline void inv_step_3half(std::span<const mint> f,std::span<mint> out,inv_workspace& ws,usize m){
    const usize n=m<<1;
    ws.ensure(n);
    inv_ntt::step_3half(f,out,ws.a.data(),ws.b.data(),m);
}

inline void inv_to_impl(std::span<const mint> f,std::span<mint> out,inv_workspace& ws){
    const usize target=out.size();
    const usize base=std::min(target,inv_naive_cutoff);
    inv_naive_prefix(f,out.first(base));
    if(target<=inv_naive_cutoff) return;
    ws.reserve(std::bit_ceil(target));
    for(usize m=inv_naive_cutoff;m<target;m<<=1)
        inv_step_3half(f,out,ws,m);
}
}

#if defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute pop
#elif defined(__GNUC__) && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma GCC pop_options
#endif

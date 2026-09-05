#pragma once

#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstring>
#include <span>

#include "../../convolution/ntt998.hpp"

namespace eez::fps998::detail{

using mint=ntt998::mint;
using usize=std::size_t;

inline constexpr usize inv_naive_cutoff=64;
static_assert(std::has_single_bit(inv_naive_cutoff));

struct inv_workspace{
    ntt998::detail::aligned_vector a;
    ntt998::detail::aligned_vector b;

    void reserve(usize n){
        a.reserve(n);
        b.reserve(n);
    }

    void ensure(usize n){
        if(a.size()<n) a.resize(n);
        if(b.size()<n) b.resize(n);
    }

    [[nodiscard]] usize capacity() const noexcept{
        return std::min(a.capacity(),b.capacity());
    }
};

inline void zero_n(mint* p,usize n) noexcept{
    if(n) std::memset(p,0,n*sizeof(mint));
}

inline void copy_n(mint* dst,const mint* src,usize n) noexcept{
    if(n) std::memcpy(dst,src,n*sizeof(mint));
}

inline void copy_negated(mint* dst,const mint* src,usize n) noexcept{
    usize i=0;
    const auto zero=_mm256_setzero_si256();
    for(;i+8<=n;i+=8){
        const auto x=ntt998::detail::load8_aligned(src+i);
        ntt998::detail::store8(
            dst+i,
            ntt998::detail::sub8(zero,x)
        );
    }
    for(;i<n;++i) dst[i]=-src[i];
}

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

inline void inv_to_impl(
    std::span<const mint> f,
    std::span<mint> out,
    inv_workspace& ws
){
    const usize target=out.size();
    const usize base=std::min(target,inv_naive_cutoff);

    inv_naive_prefix(f,out.first(base));
    if(target<=inv_naive_cutoff) return;

    ws.reserve(std::bit_ceil(target));

    for(usize m=inv_naive_cutoff;m<target;m<<=1){
        const usize n=m<<1;
        ws.ensure(n);

        mint* const a=ws.a.data();
        mint* const b=ws.b.data();

        const usize fn=std::min(f.size(),n);

        copy_n(a,f.data(),fn);
        zero_n(a+fn,n-fn);

        copy_n(b,out.data(),m);
        zero_n(b+m,m);

        ntt998::detail::forward_dif(a,n);
        ntt998::detail::forward_dif(b,n);

        ntt998::detail::pointwise_multiply(a,b,n);

        ntt998::detail::inverse_dit(a,n);

        zero_n(a,m);

        ntt998::detail::forward_dif(a,n);

        ntt998::detail::pointwise_multiply(a,b,n);

        ntt998::detail::inverse_dit(a,n);

        const usize len=std::min(m,target-m);
        copy_negated(out.data()+m,a+m,len);
    }
}

}

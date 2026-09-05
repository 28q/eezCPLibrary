#pragma once

#include <algorithm>
#include <cstddef>
#include <span>
#include <stdexcept>
#include <vector>

#include "../convolution/ntt998.hpp"

namespace eez::fps998{

using mint=ntt998::mint;
using usize=std::size_t;
using poly=std::vector<mint>;

inline constexpr usize naive_cutoff=64;

class workspace{
public:
    workspace()=default;

private:
    friend void inv_to(std::span<const mint>,std::span<mint>,workspace&);

    ntt998::frequency_buffer a_;
    ntt998::frequency_buffer b_;
    std::vector<mint> tmp_;
};

namespace detail{

inline void inv_naive_prefix(std::span<const mint> f,std::span<mint> g){
    if(g.empty()) return;
    g[0]=f[0].inv();
    const mint g0=g[0];

    for(usize i=1;i<g.size();++i){
        mint s{};
        const usize r=std::min(i,f.size()-1);
        for(usize j=1;j<=r;++j) s+=f[j]*g[i-j];
        g[i]=-s*g0;
    }
}

}

inline void inv_to(std::span<const mint> f,std::span<mint> out,workspace& ws){
    const usize target=out.size();
    if(target==0) return;

    if(f.empty())
        throw std::invalid_argument("eez::fps998::inv_to: input is empty");

    if(f[0]==mint{})
        throw std::invalid_argument("eez::fps998::inv_to: constant term must be nonzero");

    if(target>ntt998::max_ntt_size)
        throw std::length_error("eez::fps998::inv_to: result exceeds the 2^23 NTT limit");

    const usize base=std::min(target,naive_cutoff);
    detail::inv_naive_prefix(f,out.first(base));

    if(target<=naive_cutoff) return;

    for(usize m=naive_cutoff;m<target;m<<=1){
        const usize n=m<<1;
        const usize fn=std::min(f.size(),n);

        ntt998::forward_to(f.first(fn),ws.a_,n);
        ntt998::forward_to(
            std::span<const mint>(out.data(),m),
            ws.b_,
            n
        );

        ntt998::pointwise_multiply(ws.a_,ws.b_);

        if(ws.tmp_.size()<n) ws.tmp_.resize(n);
        std::span<mint> tmp(ws.tmp_.data(),n);

        ntt998::inverse_to(ws.a_,tmp);

        std::fill_n(tmp.begin(),m,mint{});

        ntt998::forward_to(
            std::span<const mint>(tmp.data(),n),
            ws.a_,
            n
        );

        ntt998::pointwise_multiply(ws.a_,ws.b_);
        ntt998::inverse_to(ws.a_,tmp);

        const usize r=std::min(n,target);
        for(usize i=m;i<r;++i) out[i]=-tmp[i];
    }
}

inline void inv_to(std::span<const mint> f,std::span<mint> out){
    workspace ws;
    inv_to(f,out,ws);
}

inline poly inv(std::span<const mint> f,usize n,workspace& ws){
    poly result(n);
    inv_to(f,result,ws);
    return result;
}

inline poly inv(std::span<const mint> f,usize n){
    workspace ws;
    return inv(f,n,ws);
}

inline poly inv(std::span<const mint> f,workspace& ws){
    return inv(f,f.size(),ws);
}

inline poly inv(std::span<const mint> f){
    workspace ws;
    return inv(f,f.size(),ws);
}

}

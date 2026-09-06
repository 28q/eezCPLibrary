#pragma once
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include "../../convolution/ntt998.hpp"

#if defined(__GNUC__) && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma GCC push_options
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#define EEZ_FPS998_ALWAYS_INLINE inline __attribute__((always_inline))
#define EEZ_FPS998_RESTRICT __restrict__
#elif defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute push(__attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,ssse3"))),apply_to=function)
#define EEZ_FPS998_ALWAYS_INLINE inline __attribute__((always_inline))
#define EEZ_FPS998_RESTRICT __restrict__
#else
#define EEZ_FPS998_ALWAYS_INLINE inline
#define EEZ_FPS998_RESTRICT
#endif

namespace eez::fps998::detail::inv_ntt{
using mint=ntt998::mint;
using usize=std::size_t;
namespace nd=ntt998::detail;

inline constexpr std::uint32_t inv2_value=(mint::MOD+1u)>>1;
inline constexpr nd::word inv2_mont=nd::canonicalize(mint::raw(inv2_value).a);

EEZ_FPS998_ALWAYS_INLINE nd::word shrink4_scalar(nd::word x) noexcept{
    return x>=nd::mod2?x-nd::mod2:x;
}
EEZ_FPS998_ALWAYS_INLINE nd::vec neg8(nd::vec x) noexcept{
    return nd::sub8(_mm256_setzero_si256(),x);
}
EEZ_FPS998_ALWAYS_INLINE void zero_n(mint* p,usize n) noexcept{
    if(n) std::memset(p,0,n*sizeof(mint));
}
EEZ_FPS998_ALWAYS_INLINE void copy_n(mint* dst,const mint* src,usize n) noexcept{
    if(n) std::memcpy(dst,src,n*sizeof(mint));
}

inline void copy_shrink4_to2(mint* EEZ_FPS998_RESTRICT dst,const mint* EEZ_FPS998_RESTRICT src,usize n) noexcept{
    usize i=0;
    for(;i+8<=n;i+=8) nd::store8(dst+i,nd::shrink4_to_2(nd::load8(src+i)));
    for(;i<n;++i) dst[i]=nd::from_raw(shrink4_scalar(nd::raw(src[i])));
}

inline void sub_inplace(mint* EEZ_FPS998_RESTRICT a,const mint* EEZ_FPS998_RESTRICT b,usize n) noexcept{
    usize i=0;
    for(;i+8<=n;i+=8){
        const nd::vec x=nd::shrink4_to_2(nd::load8_aligned(a+i));
        const nd::vec y=nd::shrink4_to_2(nd::load8_aligned(b+i));
        nd::store8_aligned(a+i,nd::sub8(x,y));
    }
    for(;i<n;++i){
        const nd::word x=shrink4_scalar(nd::raw(a[i]));
        const nd::word y=shrink4_scalar(nd::raw(b[i]));
        a[i]=nd::from_raw(nd::sub(x,y));
    }
}

template<unsigned leaf_size,unsigned parallel_blocks>
inline void leaf_products_shifted(mint* EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize n,nd::word shift) noexcept{
    const usize blocks=n/leaf_size;
    assert(blocks%parallel_blocks==0);
    nd::word w=nd::canonicalize(shift);
    for(usize s=0;s<blocks;s+=parallel_blocks){
        std::array<nd::word,parallel_blocks> modulus{};
        for(unsigned k=0;k<parallel_blocks;++k){
            modulus[k]=w;
            const usize block=s+k;
            if(block+1<blocks)
                w=nd::canonicalize(nd::mul(w,nd::forward_rate1(nd::twiddle_index(static_cast<ntt998::u32>(block)))));
        }
        if constexpr(leaf_size==8) nd::leaf_product8x4(a,b,s,modulus);
        else{
            static_assert(leaf_size==16&&parallel_blocks==2);
            nd::leaf_product16x2_karatsuba(a,b,s,modulus);
        }
    }
}

template<unsigned leaf_size>
inline void block_twist(mint* EEZ_FPS998_RESTRICT a,usize n,nd::word ratio) noexcept{
    static_assert(leaf_size==8||leaf_size==16);
    ratio=nd::canonicalize(ratio);
    nd::word w=nd::canonicalize(nd::montgomery_one);
    const usize blocks=n/leaf_size;
    for(usize s=0;s<blocks;++s){
        const nd::word c=nd::canonicalize(w);
        const nd::vec vc=nd::broadcast(c);
        const nd::vec vcn=nd::broadcast(c*nd::montgomery_ninv);
        mint* const p=a+s*leaf_size;
        if constexpr(leaf_size==8){
            nd::store8_aligned(p,nd::mul8_fixed(nd::load8_aligned(p),vc,vcn));
        }else{
            nd::store8_aligned(p,nd::mul8_fixed(nd::load8_aligned(p),vc,vcn));
            nd::store8_aligned(p+8,nd::mul8_fixed(nd::load8_aligned(p+8),vc,vcn));
        }
        if(s+1<blocks) w=nd::canonicalize(nd::mul(w,ratio));
    }
}

template<unsigned leaf_size,unsigned parallel_blocks>
inline void cubic_leaf(mint* EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize n,nd::word shift) noexcept{
    leaf_products_shifted<leaf_size,parallel_blocks>(a,b,n,shift);
    leaf_products_shifted<leaf_size,parallel_blocks>(a,b,n,shift);
    sub_inplace(a,b,n);
}

inline void cubic_cyclic(mint* EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize n) noexcept{
    assert(n>=16);
    assert(std::has_single_bit(n));
    assert(n<=ntt998::max_convolution_size);
    const unsigned leaf_log=nd::adaptive_leaf_log(n);
    nd::forward_adaptive_pair(a,b,n,leaf_log);
    if(leaf_log==3) cubic_leaf<8,4>(a,b,n,nd::montgomery_one);
    else{
        assert(leaf_log==4);
        cubic_leaf<16,2>(a,b,n,nd::montgomery_one);
    }
    nd::inverse_adaptive(a,n,leaf_log);
}

inline void cubic_negacyclic(mint* EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize n) noexcept{
    assert(n>=16);
    assert(std::has_single_bit(n));
    assert(n<=ntt998::max_convolution_size);
    const unsigned leaf_log=nd::adaptive_leaf_log(n);
    const usize blocks=n>>leaf_log;
    const unsigned block_log=static_cast<unsigned>(std::countr_zero(blocks));
    assert(block_log+1<=nd::max_log);
    const nd::word psi=nd::canonicalize(nd::twiddles.root[block_log+1]);
    const nd::word ipsi=nd::canonicalize(nd::twiddles.iroot[block_log+1]);

    if(leaf_log==3){
        block_twist<8>(a,n,psi);
        block_twist<8>(b,n,psi);
    }else{
        assert(leaf_log==4);
        block_twist<16>(a,n,psi);
        block_twist<16>(b,n,psi);
    }

    nd::forward_adaptive_pair(a,b,n,leaf_log);
    if(leaf_log==3) cubic_leaf<8,4>(a,b,n,psi);
    else cubic_leaf<16,2>(a,b,n,psi);
    nd::inverse_adaptive(a,n,leaf_log);

    if(leaf_log==3) block_twist<8>(a,n,ipsi);
    else block_twist<16>(a,n,ipsi);
}

inline void fold_f_cyclic(std::span<const mint> f,mint* EEZ_FPS998_RESTRICT a,usize m) noexcept{
    const usize lo=std::min(f.size(),m);
    copy_n(a,f.data(),lo);
    zero_n(a+lo,m-lo);
    if(f.size()<=m) return;
    const usize hi=std::min(f.size()-m,m);
    usize i=0;
    for(;i+8<=hi;i+=8){
        const nd::vec x=nd::shrink4_to_2(nd::load8_aligned(a+i));
        const nd::vec y=nd::shrink4_to_2(nd::load8(f.data()+m+i));
        nd::store8_aligned(a+i,nd::add8(x,y));
    }
    for(;i<hi;++i){
        const nd::word x=shrink4_scalar(nd::raw(a[i]));
        const nd::word y=shrink4_scalar(nd::raw(f[m+i]));
        a[i]=nd::from_raw(nd::add(x,y));
    }
}

inline void recover_high(const mint* EEZ_FPS998_RESTRICT a,mint* out,usize m,usize len) noexcept{
    const nd::vec vhalf=nd::broadcast(inv2_mont);
    const nd::vec vhalfn=nd::broadcast(inv2_mont*nd::montgomery_ninv);
    usize i=0;
    for(;i+8<=len;i+=8){
        const nd::vec a0=nd::shrink4_to_2(nd::load8(a+i));
        const nd::vec a1=nd::shrink4_to_2(nd::load8(a+m+i));
        const nd::vec b=nd::shrink4_to_2(nd::load8(out+i));
        nd::vec x=nd::add8(a0,a1);
        x=nd::add8(x,b);
        x=nd::mul8_fixed(x,vhalf,vhalfn);
        nd::store8(out+i,neg8(x));
    }
    for(;i<len;++i){
        const nd::word a0=shrink4_scalar(nd::raw(a[i]));
        const nd::word a1=shrink4_scalar(nd::raw(a[m+i]));
        const nd::word b=shrink4_scalar(nd::raw(out[i]));
        nd::word x=nd::add(a0,a1);
        x=nd::add(x,b);
        x=nd::mul(x,inv2_mont);
        out[i]=nd::from_raw(nd::sub(0,x));
    }
}

inline void step_3half(std::span<const mint> f,std::span<mint> out,mint* EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize m) noexcept{
    const usize n=m<<1;
    assert(m>=16);
    assert(std::has_single_bit(m));
    assert(n<=ntt998::max_convolution_size);
    assert(out.size()>m);

    const usize len=std::min(m,out.size()-m);
    if(!len) return;

    fold_f_cyclic(f,a,m);
    copy_n(b,out.data(),m);
    cubic_cyclic(a,b,m);

    copy_shrink4_to2(out.data()+m,a,len);

    const usize fn=std::min(f.size(),n);
    copy_n(a,f.data(),fn);
    zero_n(a+fn,n-fn);

    copy_n(b,out.data(),m);
    zero_n(b+m,m);

    cubic_negacyclic(a,b,n);
    recover_high(a,out.data()+m,m,len);
}
}

#undef EEZ_FPS998_RESTRICT
#undef EEZ_FPS998_ALWAYS_INLINE

#if defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma clang attribute pop
#elif defined(__GNUC__) && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))
#pragma GCC pop_options
#endif

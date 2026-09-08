#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <immintrin.h>
#include <stdexcept>
#include <vector>

#include "../convolution/ntt998.hpp"

#if defined(__GNUC__)&&!defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))
#pragma GCC push_options
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#elif defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))
#pragma clang attribute push(__attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,ssse3"))),apply_to=function)
#endif

namespace eez::fps998{
using mint=eez::ntt998::mint;
using usize=std::size_t;
using u32=std::uint32_t;
using u64=std::uint64_t;

namespace detail{
namespace nt=eez::ntt998;
namespace nd=eez::ntt998::detail;

namespace bm_ntt{
using namespace eez::ntt998::detail;

struct stage_rotations{
    std::vector<word,aligned_allocator<word>> r1,r2,r3;
};
template<bool inverse>
inline const stage_rotations& cached_rotations(usize blocks){
    static std::array<stage_rotations,max_log+1> tables;
    auto& t=tables[std::countr_zero(blocks)];
    if(t.r1.empty()){
        t.r1.resize(blocks);t.r2.resize(blocks);t.r3.resize(blocks);
        word w=montgomery_one;
        for(usize s=0;s<blocks;s++){
            t.r1[s]=canonicalize(w);
            t.r2[s]=canonicalize(mul(w,w));
            t.r3[s]=canonicalize(mul(t.r2[s],w));
            if(s+1<blocks)w=mul(w,inverse?inverse_rate3(twiddle_index(u32(s))):forward_rate3(twiddle_index(u32(s))));
        }
    }
    return t;
}
inline void forward_radix4_p4(mint* __restrict__ a,usize blocks,usize first=0,usize total=0)noexcept{
    if(blocks<2){forward_radix4_scalar(a,blocks,4);return;}
    const vec imag=broadcast(canonicalize(twiddles.root[2]));
    const auto& table=cached_rotations<false>(total?total:blocks);
    for(usize s=0;s<blocks;s+=2){
        const vec w1=pack_four(table.r1[first+s],table.r1[first+s+1]);
        const vec w2=pack_four(table.r2[first+s],table.r2[first+s+1]);
        const vec w3=pack_four(table.r3[first+s],table.r3[first+s+1]);
        mint* const b0=a+s*16;
        mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1);
        const vec x1=mul8_fixed(load2x4(b0+4,b1+4),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv)));
        const vec x2=mul8_fixed(load2x4(b0+8,b1+8),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv)));
        const vec x3=mul8_fixed(load2x4(b0+12,b1+12),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv)));
        const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3);
        const vec t=mul8_fixed(lazy_sub8(x1,x3),imag,_mm256_mul_epu32(imag,broadcast(montgomery_ninv)));
        store2x4(b0,b1,add8(s02,s13));store2x4(b0+4,b1+4,sub8(s02,s13));
        store2x4(b0+8,b1+8,add8(d02,t));store2x4(b0+12,b1+12,sub8(d02,t));
    }
}
inline void inverse_radix4_p4(mint* __restrict__ a,usize blocks,usize first=0,usize total=0)noexcept{
    if(blocks<2){inverse_radix4_scalar(a,blocks,4);return;}
    const vec iimag=broadcast(canonicalize(twiddles.iroot[2]));
    const auto& table=cached_rotations<true>(total?total:blocks);
    for(usize s=0;s<blocks;s+=2){
        const vec w1=pack_four(table.r1[first+s],table.r1[first+s+1]);
        const vec w2=pack_four(table.r2[first+s],table.r2[first+s+1]);
        const vec w3=pack_four(table.r3[first+s],table.r3[first+s+1]);
        mint* const b0=a+s*16;
        mint* const b1=b0+16;
        const vec x0=load2x4(b0,b1),x1=load2x4(b0+4,b1+4);
        const vec x2=load2x4(b0+8,b1+8),x3=load2x4(b0+12,b1+12);
        const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3);
        const vec t=mul8_fixed(lazy_sub8(x2,x3),iimag,_mm256_mul_epu32(iimag,broadcast(montgomery_ninv)));
        store2x4(b0,b1,add8(s01,s23));
        store2x4(b0+4,b1+4,mul8_fixed(lazy_add8(d01,t),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv))));
        store2x4(b0+8,b1+8,mul8_fixed(lazy_sub8(s01,s23),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv))));
        store2x4(b0+12,b1+12,mul8_fixed(lazy_sub8(d01,t),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv))));
    }
}
inline void forward_radix4_p1(mint* __restrict__ a,usize blocks,usize first=0,usize total=0)noexcept{
    const vec imag=broadcast(canonicalize(twiddles.root[2]));
    const auto& table=cached_rotations<false>(total?total:blocks);
    usize s=0;
    for(;s+8<=blocks;s+=8){
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(table.r1.data()+first+s));
        const vec w2=_mm256_load_si256(reinterpret_cast<const vec*>(table.r2.data()+first+s));
        const vec w3=_mm256_load_si256(reinterpret_cast<const vec*>(table.r3.data()+first+s));
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        x1=mul8(x1,w1);x2=mul8(x2,w2);x3=mul8(x3,w3);
        const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3),t=mul8(lazy_sub8(x1,x3),imag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s02,s13),sub8(s02,s13),add8(d02,t),sub8(d02,t),v0,v1,v2,v3);
        store8(b,v0);store8(b+8,v1);store8(b+16,v2);store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word r1=table.r1[first+s],r2=table.r2[first+s],r3=table.r3[first+s];
        forward_butterfly(a+4*s,1,0,r1,r2,r3);
    }
}
inline void inverse_radix4_p1(mint* __restrict__ a,usize blocks,usize first=0,usize total=0)noexcept{
    const vec iimag=broadcast(canonicalize(twiddles.iroot[2]));
    const auto& table=cached_rotations<true>(total?total:blocks);
    usize s=0;
    for(;s+8<=blocks;s+=8){
        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(table.r1.data()+first+s));
        const vec w2=_mm256_load_si256(reinterpret_cast<const vec*>(table.r2.data()+first+s));
        const vec w3=_mm256_load_si256(reinterpret_cast<const vec*>(table.r3.data()+first+s));
        mint* const b=a+4*s;
        vec x0,x1,x2,x3;
        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);
        const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3),t=mul8(lazy_sub8(x2,x3),iimag);
        vec v0,v1,v2,v3;
        transpose_4x8_to_8x4(add8(s01,s23),mul8(lazy_add8(d01,t),w1),mul8(lazy_sub8(s01,s23),w2),mul8(lazy_sub8(d01,t),w3),v0,v1,v2,v3);
        store8(b,v0);store8(b+8,v1);store8(b+16,v2);store8(b+24,v3);
    }
    for(;s<blocks;++s){
        const word r1=table.r1[first+s],r2=table.r2[first+s],r3=table.r3[first+s];
        inverse_butterfly(a+4*s,1,0,r1,r2,r3);
    }
}
inline void forward_radix4_stage(mint* __restrict__ a,usize n,int stage)noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2),blocks=usize(1)<<stage;
    if(stride>=8)eez::ntt998::detail::forward_radix4_large(a,blocks,stride);
    else if(stride==4)forward_radix4_p4(a,blocks);
    else if(stride==1)forward_radix4_p1(a,blocks);
    else forward_radix4_scalar(a,blocks,stride);
}
inline void inverse_radix4_stage(mint* __restrict__ a,usize n,int stage)noexcept{
    const int h=static_cast<int>(std::countr_zero(n));
    assert(stage>=0&&stage+2<=h);
    const usize stride=usize(1)<<(h-stage-2),blocks=usize(1)<<stage;
    if(stride>=8)eez::ntt998::detail::inverse_radix4_large(a,blocks,stride);
    else if(stride==4)inverse_radix4_p4(a,blocks);
    else if(stride==1)inverse_radix4_p1(a,blocks);
    else inverse_radix4_scalar(a,blocks,stride);
}
inline void forward_dif(mint* __restrict__ a,usize n)noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    int stage=0;
    if(h&1){forward_radix2_first(a,n);stage=1;}
    for(;stage<h;stage+=2)forward_radix4_stage(a,n,stage);
}
inline void inverse_dit(mint* __restrict__ a,usize n)noexcept{
    if(n<=1)return;
    const int h=static_cast<int>(std::countr_zero(n));
    const word scale=mint::raw(static_cast<u32>(n)).inv().a;
    if(h&1){
        for(int stage=h-2;stage>=1;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix2_scale(a,n,scale);
    }else{
        for(int stage=h-2;stage>=2;stage-=2)inverse_radix4_stage(a,n,stage);
        final_radix4_scale(a,n,scale);
    }
}

inline constexpr usize ntt_tile=4096;

template<bool inv>
inline void stage_range(mint* a,usize stride,usize first,usize count,usize total){
    if(stride==1){
        if constexpr(inv)inverse_radix4_p1(a+first*4,count,first,total);
        else forward_radix4_p1(a+first*4,count,first,total);
    }else if(stride==4){
        if constexpr(inv)inverse_radix4_p4(a+first*16,count,first,total);
        else forward_radix4_p4(a+first*16,count,first,total);
    }else{
        const auto& t=cached_rotations<inv>(total);
        const vec imag=broadcast(inv?twiddles.iroot[2]:twiddles.root[2]);
        for(usize s=first;s<first+count;s++){
            if(s==0){
                if constexpr(inv)eez::ntt998::detail::inverse_radix4_large(a,1,stride);
                else eez::ntt998::detail::forward_radix4_large(a,1,stride);
            }else{
                if constexpr(inv)inverse_radix4_large_block(a+s*4*stride,stride,imag,t.r1[s],t.r2[s],t.r3[s]);
                else forward_radix4_large_block(a+s*4*stride,stride,imag,t.r1[s],t.r2[s],t.r3[s]);
            }
        }
    }
}
inline int tile_stage(usize n,int h){
    int stage=h&1;
    while((n>>stage)>ntt_tile)stage+=2;
    return stage;
}
inline void forward_tiled(mint* a,usize n){
    if(n<=1)return;
    const int h=int(std::countr_zero(n));
    if(n<=ntt_tile){forward_dif(a,n);return;}
    const int split=tile_stage(n,h);
    int stage=0;
    if(h&1){forward_radix2_first(a,n);stage=1;}
    for(;stage<split;stage+=2)forward_radix4_stage(a,n,stage);
    for(usize b=0;b<(usize(1)<<split);b++)for(int s=split;s<h;s+=2){
        const usize count=usize(1)<<(s-split);
        stage_range<false>(a,n>>(s+2),b*count,count,usize(1)<<s);
    }
}
inline void inverse_prefix_tiled(mint* a,usize n){
    const int h=int(std::countr_zero(n)),last=(h&1)?1:2;
    if(n<=ntt_tile){
        for(int s=h-2;s>=last;s-=2)inverse_radix4_stage(a,n,s);
        return;
    }
    const int split=tile_stage(n,h);
    for(usize b=0;b<(usize(1)<<split);b++)for(int s=h-2;s>=split;s-=2){
        const usize count=usize(1)<<(s-split);
        stage_range<true>(a,n>>(s+2),b*count,count,usize(1)<<s);
    }
    for(int s=split-2;s>=last;s-=2)inverse_radix4_stage(a,n,s);
}
inline void inverse_tiled(mint* a,usize n){
    if(n<=1)return;
    if(n<=ntt_tile){inverse_dit(a,n);return;}
    inverse_prefix_tiled(a,n);
    const word scale=mint::raw(u32(n)).inv().a;
    if(std::countr_zero(n)&1)final_radix2_scale(a,n,scale);
    else final_radix4_scale(a,n,scale);
}
inline void forward_pair(mint* a,mint* b,usize n){
    if(n<=1)return;
    forward_tiled(a,n);forward_tiled(b,n);
}
inline void inverse_pair(mint* a,mint* b,usize n){
    if(n<=1)return;
    inverse_tiled(a,n);inverse_tiled(b,n);
}
}

inline constexpr int naive_cutoff=24;

inline mint bostan_mori_naive(const std::vector<mint>& A,const std::vector<mint>& Q0,u64 N){
    int d=(int)A.size();
    std::vector<mint> Q=Q0;
    std::vector<mint> P=nt::convolution(Q,A);P.resize(d);
    std::vector<mint> Qm(d+1),R(2*d),S(2*d+1);
    while(N){
        for(int i=0;i<=d;i++)Qm[i]=(i&1)?-Q[i]:Q[i];
        std::fill(R.begin(),R.end(),mint{});
        std::fill(S.begin(),S.end(),mint{});
        for(int i=0;i<d;i++)for(int j=0;j<=d;j++)R[i+j]+=P[i]*Qm[j];
        for(int i=0;i<=d;i++)for(int j=0;j<=d;j++)S[i+j]+=Q[i]*Qm[j];
        int b=(int)(N&1);
        for(int i=0;i<d;i++)P[i]=R[2*i+b];
        for(int i=0;i<=d;i++)Q[i]=S[2*i];
        N>>=1;
    }
    return P[0];
}

inline nd::vec half8(nd::vec x)noexcept{
    x=nd::canonicalize8(x);
    const nd::vec one=_mm256_set1_epi32(1),prime=_mm256_set1_epi32((int)mint::MOD);
    nd::vec mask=_mm256_sub_epi32(_mm256_setzero_si256(),_mm256_and_si256(x,one));
    x=_mm256_add_epi32(x,_mm256_and_si256(mask,prime));
    return _mm256_srli_epi32(x,1);
}
inline void split16(nd::vec a,nd::vec b,nd::vec& e,nd::vec& o)noexcept{
    const nd::vec idx=_mm256_setr_epi32(0,2,4,6,1,3,5,7);
    a=_mm256_permutevar8x32_epi32(a,idx);b=_mm256_permutevar8x32_epi32(b,idx);
    e=_mm256_permute2x128_si256(a,b,0x20);
    o=_mm256_permute2x128_si256(a,b,0x31);
}
template<bool Odd>
inline void make_half(const mint* qf,const mint* pf,mint* qh,mint* ph,const mint* twist,usize m)noexcept{
    usize i=0;
    for(;i+8<=m;i+=8){
        nd::vec qe,qo,pe,po;
        split16(nd::load8(qf+2*i),nd::load8(qf+2*i+8),qe,qo);
        split16(nd::load8(pf+2*i),nd::load8(pf+2*i+8),pe,po);
        nd::store8(qh+i,nd::mul8(qe,qo));
        nd::vec a=nd::mul8(pe,qo),b=nd::mul8(po,qe);
        if constexpr(Odd)nd::store8(ph+i,nd::mul8(nd::sub8(a,b),nd::load8(twist+i)));
        else nd::store8(ph+i,nd::add8(a,b));
    }
    for(;i<m;i++){
        mint qe=qf[2*i],qo=qf[2*i+1],pe=pf[2*i],po=pf[2*i+1];
        qh[i]=qe*qo;
        if constexpr(Odd)ph[i]=(pe*qo-po*qe)*twist[i];
        else ph[i]=pe*qo+po*qe;
    }
}
inline mint make_final_odd_constant(const mint* qf,const mint* pf,const mint* twist,usize m)noexcept{
    nd::vec acc=_mm256_setzero_si256();
    usize i=0;
    for(;i+8<=m;i+=8){
        nd::vec qe,qo,pe,po;
        split16(nd::load8(qf+2*i),nd::load8(qf+2*i+8),qe,qo);
        split16(nd::load8(pf+2*i),nd::load8(pf+2*i+8),pe,po);
        nd::vec v=nd::canonicalize8(nd::mul8(nd::sub8(nd::mul8(pe,qo),nd::mul8(po,qe)),nd::load8(twist+i)));
        acc=nd::add8(acc,v);
    }
    alignas(32) nd::word s8[8];
    nd::store8(reinterpret_cast<mint*>(s8),acc);
    nd::word s=0;
    for(int j=0;j<8;j++)s=nd::add(s,s8[j]);
    for(;i<m;i++){
        mint v=(pf[2*i]*qf[2*i+1]-pf[2*i+1]*qf[2*i])*twist[i];
        s=nd::add(s,nd::canonicalize(nd::raw(v)));
    }
    nd::word im=mint::raw((u32)m).inv().a;
    return nd::from_raw(nd::mul(s,im));
}

struct BMPlan{usize z;bool top_alias;};

inline BMPlan bm_plan(usize plen,usize qlen){
    usize qdeg=qlen-1;
    if(qdeg&&std::has_single_bit(qdeg)&&plen<=qdeg){
        usize z=qdeg<<1;
        if(z<=nt::max_ntt_size)return {z,true};
    }
    usize need=std::max(plen+qlen-1,usize(2)*qlen-1);
    usize z=std::max(usize(2),std::bit_ceil(need));
    if(z>nt::max_ntt_size)throw std::length_error("BostanMori");
    return {z,false};
}
inline usize truncated_len(usize len,u64 N)noexcept{
    if(N>=u64(len-1))return len;
    return usize(N)+1;
}
inline usize trim_length(const mint* a,usize n)noexcept{
    while(n>1&&a[n-1]==mint{})--n;
    return n;
}

/*
half_twist[bitrev(k)] = 1/w^k
double_twist[i]       = w^i/m
w: primitive z-th root, z=2m
*/
inline void build_bm_twists(mint* half_twist,mint* double_twist,usize z)noexcept{
    usize m=z>>1;
    unsigned h=(unsigned)std::countr_zero(z);
    nd::word w=nd::twiddles.root[h],iw=nd::twiddles.iroot[h];
    nd::word fp=mint::raw((u32)m).inv().a,ip=nd::montgomery_one;
    usize rev=0;
    for(usize k=0;k<m;k++){
        double_twist[k]=nd::from_raw(fp);
        half_twist[rev]=nd::from_raw(ip);
        fp=nd::mul(fp,w);ip=nd::mul(ip,iw);
        if(k+1<m){
            usize bit=m>>1;
            while(rev&bit){rev^=bit;bit>>=1;}
            rev^=bit;
        }
    }
}
inline void inverse_twisted(mint* __restrict__ a,usize n,const mint* __restrict__ tw)noexcept{
    if(n==1){a[0]*=tw[0];return;}
    const int h=int(std::countr_zero(n));
    if(h&1){
        bm_ntt::inverse_prefix_tiled(a,n);
        const usize half=n/2;
        usize i=0;
        for(;i+8<=half;i+=8){
            const auto x=nd::load8(a+i),y=nd::load8(a+half+i);
            nd::store8(a+i,nd::mul8(nd::add8(x,y),nd::load8(tw+i)));
            nd::store8(a+half+i,nd::mul8(nd::sub8(x,y),nd::load8(tw+half+i)));
        }
        for(;i<half;i++){
            mint x=a[i],y=a[half+i];
            a[i]=(x+y)*tw[i];a[half+i]=(x-y)*tw[half+i];
        }
    }else{
        bm_ntt::inverse_prefix_tiled(a,n);
        const usize stride=n/4;
        const auto imag=nd::broadcast(nd::twiddles.iroot[2]);
        usize i=0;
        for(;i+8<=stride;i+=8){
            const auto x0=nd::load8(a+i),x1=nd::load8(a+stride+i),x2=nd::load8(a+2*stride+i),x3=nd::load8(a+3*stride+i);
            const auto s01=nd::add8(x0,x1),d01=nd::sub8(x0,x1),s23=nd::add8(x2,x3);
            const auto t=nd::mul8_fixed(nd::sub8(x2,x3),imag,_mm256_mul_epu32(imag,nd::broadcast(nd::montgomery_ninv)));
            nd::store8(a+i,nd::mul8(nd::add8(s01,s23),nd::load8(tw+i)));
            nd::store8(a+stride+i,nd::mul8(nd::add8(d01,t),nd::load8(tw+stride+i)));
            nd::store8(a+2*stride+i,nd::mul8(nd::sub8(s01,s23),nd::load8(tw+2*stride+i)));
            nd::store8(a+3*stride+i,nd::mul8(nd::sub8(d01,t),nd::load8(tw+3*stride+i)));
        }
        const mint ii=nd::from_raw(nd::twiddles.iroot[2]);
        for(;i<stride;i++){
            mint x0=a[i],x1=a[stride+i],x2=a[2*stride+i],x3=a[3*stride+i];
            mint s01=x0+x1,d01=x0-x1,s23=x2+x3,t=(x2-x3)*ii;
            a[i]=(s01+s23)*tw[i];a[stride+i]=(d01+t)*tw[stride+i];
            a[2*stride+i]=(s01-s23)*tw[2*stride+i];a[3*stride+i]=(d01-t)*tw[3*stride+i];
        }
    }
}
inline void ntt_double_pair(const mint* qh,const mint* ph,mint* qf,mint* pf,const mint* double_twist,usize m,bool top_alias,mint qtop)noexcept{
    if(qf!=qh)std::memcpy(qf,qh,m*sizeof(mint));
    if(pf!=ph)std::memcpy(pf,ph,m*sizeof(mint));
    std::memcpy(qf+m,qh,m*sizeof(mint));
    std::memcpy(pf+m,ph,m*sizeof(mint));
    inverse_twisted(qf+m,m,double_twist);
    inverse_twisted(pf+m,m,double_twist);
    if(top_alias)qf[m]-=qtop+qtop;
    bm_ntt::forward_pair(qf+m,pf+m,m);
}
inline mint bostan_mori(const std::vector<mint>& A,const std::vector<mint>& Q0,u64 N){
    usize d=A.size();
    assert(d>0&&Q0.size()==d+1);
    if(N<d)return A[usize(N)];
    if(d<=naive_cutoff)return bostan_mori_naive(A,Q0,N);

    std::vector<mint> Pv=nt::convolution(Q0,A);Pv.resize(d);
    usize plen=d,qlen=d+1;
    while(plen>1&&Pv[plen-1]==mint{})--plen;
    while(qlen>1&&Q0[qlen-1]==mint{})--qlen;

    BMPlan plan=bm_plan(plen,qlen);
    usize z=plan.z,m=z>>1;
    bool top_alias=plan.top_alias;
    const usize max_z=z,max_m=m;
    nd::aligned_vector qf(max_z),pf(max_z),qh(max_m),ph(max_m);
    nd::aligned_vector half_twist(max_m),double_twist(max_m);

    std::copy_n(Q0.begin(),qlen,qf.begin());
    std::copy_n(Pv.begin(),plen,pf.begin());
    std::fill(qf.begin()+qlen,qf.begin()+z,mint{});
    std::fill(pf.begin()+plen,pf.begin()+z,mint{});
    bm_ntt::forward_pair(qf.data(),pf.data(),z);
    build_bm_twists(half_twist.data(),double_twist.data(),z);

    mint qtop=top_alias?Q0[qlen-1]:mint{};
    mint p_inv_scale=mint::raw(1);
    const mint inv2=mint::raw(499122177);

    while(N){
        p_inv_scale*=inv2;
        if(N==1)return make_final_odd_constant(qf.data(),pf.data(),half_twist.data(),m)*p_inv_scale;

        const unsigned bit=(unsigned)(N&1);
        const usize pdeg=plen-1,qdeg=qlen-1,product_deg=pdeg+qdeg;
        usize next_plen=product_deg>=bit?(product_deg-bit)/2+1:1;
        usize next_qlen=qlen;

        if(top_alias){
            mint t=qtop*qtop;
            if(qdeg&1)t=-t;
            qtop=t;
        }

        N>>=1;
        usize cut_plen=truncated_len(next_plen,N);
        usize cut_qlen=truncated_len(next_qlen,N);
        BMPlan desired=bm_plan(cut_plen,cut_qlen);

        if(desired.z<z){
            if(bit)make_half<true>(qf.data(),pf.data(),qh.data(),ph.data(),half_twist.data(),m);
            else make_half<false>(qf.data(),pf.data(),qh.data(),ph.data(),half_twist.data(),m);
            usize old_m=m;
            bm_ntt::inverse_pair(qh.data(),ph.data(),old_m);

            if(top_alias)qh[0]-=qtop;

            usize qcopy=std::min(cut_qlen,old_m);
            std::memcpy(qf.data(),qh.data(),qcopy*sizeof(mint));
            if(top_alias&&cut_qlen>old_m)qf[old_m]=qtop;
            std::memcpy(pf.data(),ph.data(),cut_plen*sizeof(mint));

            plen=trim_length(pf.data(),cut_plen);
            qlen=trim_length(qf.data(),cut_qlen);

            plan=bm_plan(plen,qlen);
            z=plan.z;m=z>>1;
            top_alias=plan.top_alias;
            qtop=top_alias?qf[qlen-1]:mint{};

            std::fill(qf.begin()+qlen,qf.begin()+z,mint{});
            std::fill(pf.begin()+plen,pf.begin()+z,mint{});
            bm_ntt::forward_pair(qf.data(),pf.data(),z);
            build_bm_twists(half_twist.data(),double_twist.data(),z);
        }else{
            plen=next_plen;qlen=next_qlen;
            if(bit)make_half<true>(qf.data(),pf.data(),qf.data(),pf.data(),half_twist.data(),m);
            else make_half<false>(qf.data(),pf.data(),qf.data(),pf.data(),half_twist.data(),m);
            ntt_double_pair(qf.data(),pf.data(),qf.data(),pf.data(),double_twist.data(),m,top_alias,qtop);
        }
    }
    return {};
}
}

inline mint BostanMori(const std::vector<mint>& A,const std::vector<mint>& Q,std::uint64_t N){
    return detail::bostan_mori(A,Q,N);
}

}

#if defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))
#pragma clang attribute pop
#elif defined(__GNUC__)&&!defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))
#pragma GCC pop_options
#endif

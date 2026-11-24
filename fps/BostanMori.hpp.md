---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: convolution/ntt998.hpp
    title: ntt998
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/verify-yosupo-fps/yosupo-kth-term-of-linearly-recurrent-sequence-BostanMori.test.cpp
    title: verify/verify-yosupo-fps/yosupo-kth-term-of-linearly-recurrent-sequence-BostanMori.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links:
    - https://github.com/28q/eezCPLibrary
  bundledCode: "Traceback (most recent call last):\n  File \"/home/runner/.local/lib/python3.14/site-packages/onlinejudge_verify/documentation/build.py\"\
    , line 71, in _render_source_code_stat\n    bundled_code = language.bundle(stat.path,\
    \ basedir=basedir, options={'include_paths': [basedir]}).decode()\n          \
    \         ~~~~~~~~~~~~~~~^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\
    \  File \"/home/runner/.local/lib/python3.14/site-packages/onlinejudge_verify/languages/cplusplus.py\"\
    , line 187, in bundle\n    bundler.update(path)\n    ~~~~~~~~~~~~~~^^^^^^\n  File\
    \ \"/home/runner/.local/lib/python3.14/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 312, in update\n    raise BundleErrorAt(path, i + 1, \"#pragma once found\
    \ in a non-first line\")\nonlinejudge_verify.languages.cplusplus_bundle.BundleErrorAt:\
    \ fps/BostanMori.hpp: line 2: #pragma once found in a non-first line\n"
  code: "// eez CP Library: https://github.com/28q/eezCPLibrary\n#pragma once\n\n\
    #include <algorithm>\n#include <array>\n#include <bit>\n#include <cassert>\n#include\
    \ <cstddef>\n#include <cstdint>\n#include <cstring>\n#include <immintrin.h>\n\
    #include <stdexcept>\n#include <vector>\n\n#include \"../convolution/ntt998.hpp\"\
    \n\n#if defined(__GNUC__)&&!defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))\n\
    #pragma GCC push_options\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC\
    \ target(\"avx2,bmi,bmi2,lzcnt,popcnt\")\n#elif defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))\n\
    #pragma clang attribute push(__attribute__((target(\"avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"\
    ))),apply_to=function)\n#endif\n\nnamespace eez::fps998{\nusing mint=eez::ntt998::mint;\n\
    using usize=std::size_t;\nusing u32=std::uint32_t;\nusing u64=std::uint64_t;\n\
    \nnamespace detail{\nnamespace nt=eez::ntt998;\nnamespace nd=eez::ntt998::detail;\n\
    \nnamespace bm_ntt{\nusing namespace eez::ntt998::detail;\n\nstruct stage_rotations{\n\
    \    std::vector<word,aligned_allocator<word>> r1,r2,r3;\n};\ntemplate<bool inverse>\n\
    inline const stage_rotations& cached_rotations(usize blocks){\n    static std::array<stage_rotations,max_log+1>\
    \ tables;\n    auto& t=tables[std::countr_zero(blocks)];\n    if(t.r1.empty()){\n\
    \        t.r1.resize(blocks);t.r2.resize(blocks);t.r3.resize(blocks);\n      \
    \  word w=montgomery_one;\n        for(usize s=0;s<blocks;s++){\n            t.r1[s]=canonicalize(w);\n\
    \            t.r2[s]=canonicalize(mul(w,w));\n            t.r3[s]=canonicalize(mul(t.r2[s],w));\n\
    \            if(s+1<blocks)w=mul(w,inverse?inverse_rate3(twiddle_index(u32(s))):forward_rate3(twiddle_index(u32(s))));\n\
    \        }\n    }\n    return t;\n}\ninline void forward_radix4_p4(mint* __restrict__\
    \ a,usize blocks,usize first=0,usize total=0)noexcept{\n    if(blocks<2){forward_radix4_scalar(a,blocks,4);return;}\n\
    \    const vec imag=broadcast(canonicalize(twiddles.root[2]));\n    const auto&\
    \ table=cached_rotations<false>(total?total:blocks);\n    for(usize s=0;s<blocks;s+=2){\n\
    \        const vec w1=pack_four(table.r1[first+s],table.r1[first+s+1]);\n    \
    \    const vec w2=pack_four(table.r2[first+s],table.r2[first+s+1]);\n        const\
    \ vec w3=pack_four(table.r3[first+s],table.r3[first+s+1]);\n        mint* const\
    \ b0=a+s*16;\n        mint* const b1=b0+16;\n        const vec x0=load2x4(b0,b1);\n\
    \        const vec x1=mul8_fixed(load2x4(b0+4,b1+4),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv)));\n\
    \        const vec x2=mul8_fixed(load2x4(b0+8,b1+8),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv)));\n\
    \        const vec x3=mul8_fixed(load2x4(b0+12,b1+12),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv)));\n\
    \        const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3);\n        const\
    \ vec t=mul8_fixed(lazy_sub8(x1,x3),imag,_mm256_mul_epu32(imag,broadcast(montgomery_ninv)));\n\
    \        store2x4(b0,b1,add8(s02,s13));store2x4(b0+4,b1+4,sub8(s02,s13));\n  \
    \      store2x4(b0+8,b1+8,add8(d02,t));store2x4(b0+12,b1+12,sub8(d02,t));\n  \
    \  }\n}\ninline void inverse_radix4_p4(mint* __restrict__ a,usize blocks,usize\
    \ first=0,usize total=0)noexcept{\n    if(blocks<2){inverse_radix4_scalar(a,blocks,4);return;}\n\
    \    const vec iimag=broadcast(canonicalize(twiddles.iroot[2]));\n    const auto&\
    \ table=cached_rotations<true>(total?total:blocks);\n    for(usize s=0;s<blocks;s+=2){\n\
    \        const vec w1=pack_four(table.r1[first+s],table.r1[first+s+1]);\n    \
    \    const vec w2=pack_four(table.r2[first+s],table.r2[first+s+1]);\n        const\
    \ vec w3=pack_four(table.r3[first+s],table.r3[first+s+1]);\n        mint* const\
    \ b0=a+s*16;\n        mint* const b1=b0+16;\n        const vec x0=load2x4(b0,b1),x1=load2x4(b0+4,b1+4);\n\
    \        const vec x2=load2x4(b0+8,b1+8),x3=load2x4(b0+12,b1+12);\n        const\
    \ vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3);\n        const vec t=mul8_fixed(lazy_sub8(x2,x3),iimag,_mm256_mul_epu32(iimag,broadcast(montgomery_ninv)));\n\
    \        store2x4(b0,b1,add8(s01,s23));\n        store2x4(b0+4,b1+4,mul8_fixed(lazy_add8(d01,t),w1,_mm256_mul_epu32(w1,broadcast(montgomery_ninv))));\n\
    \        store2x4(b0+8,b1+8,mul8_fixed(lazy_sub8(s01,s23),w2,_mm256_mul_epu32(w2,broadcast(montgomery_ninv))));\n\
    \        store2x4(b0+12,b1+12,mul8_fixed(lazy_sub8(d01,t),w3,_mm256_mul_epu32(w3,broadcast(montgomery_ninv))));\n\
    \    }\n}\ninline void forward_radix4_p1(mint* __restrict__ a,usize blocks,usize\
    \ first=0,usize total=0)noexcept{\n    const vec imag=broadcast(canonicalize(twiddles.root[2]));\n\
    \    const auto& table=cached_rotations<false>(total?total:blocks);\n    usize\
    \ s=0;\n    for(;s+8<=blocks;s+=8){\n        const vec w1=_mm256_load_si256(reinterpret_cast<const\
    \ vec*>(table.r1.data()+first+s));\n        const vec w2=_mm256_load_si256(reinterpret_cast<const\
    \ vec*>(table.r2.data()+first+s));\n        const vec w3=_mm256_load_si256(reinterpret_cast<const\
    \ vec*>(table.r3.data()+first+s));\n        mint* const b=a+4*s;\n        vec\
    \ x0,x1,x2,x3;\n        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);\n\
    \        x1=mul8(x1,w1);x2=mul8(x2,w2);x3=mul8(x3,w3);\n        const vec s02=add8(x0,x2),d02=sub8(x0,x2),s13=add8(x1,x3),t=mul8(lazy_sub8(x1,x3),imag);\n\
    \        vec v0,v1,v2,v3;\n        transpose_4x8_to_8x4(add8(s02,s13),sub8(s02,s13),add8(d02,t),sub8(d02,t),v0,v1,v2,v3);\n\
    \        store8(b,v0);store8(b+8,v1);store8(b+16,v2);store8(b+24,v3);\n    }\n\
    \    for(;s<blocks;++s){\n        const word r1=table.r1[first+s],r2=table.r2[first+s],r3=table.r3[first+s];\n\
    \        forward_butterfly(a+4*s,1,0,r1,r2,r3);\n    }\n}\ninline void inverse_radix4_p1(mint*\
    \ __restrict__ a,usize blocks,usize first=0,usize total=0)noexcept{\n    const\
    \ vec iimag=broadcast(canonicalize(twiddles.iroot[2]));\n    const auto& table=cached_rotations<true>(total?total:blocks);\n\
    \    usize s=0;\n    for(;s+8<=blocks;s+=8){\n        const vec w1=_mm256_load_si256(reinterpret_cast<const\
    \ vec*>(table.r1.data()+first+s));\n        const vec w2=_mm256_load_si256(reinterpret_cast<const\
    \ vec*>(table.r2.data()+first+s));\n        const vec w3=_mm256_load_si256(reinterpret_cast<const\
    \ vec*>(table.r3.data()+first+s));\n        mint* const b=a+4*s;\n        vec\
    \ x0,x1,x2,x3;\n        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);\n\
    \        const vec s01=add8(x0,x1),d01=sub8(x0,x1),s23=add8(x2,x3),t=mul8(lazy_sub8(x2,x3),iimag);\n\
    \        vec v0,v1,v2,v3;\n        transpose_4x8_to_8x4(add8(s01,s23),mul8(lazy_add8(d01,t),w1),mul8(lazy_sub8(s01,s23),w2),mul8(lazy_sub8(d01,t),w3),v0,v1,v2,v3);\n\
    \        store8(b,v0);store8(b+8,v1);store8(b+16,v2);store8(b+24,v3);\n    }\n\
    \    for(;s<blocks;++s){\n        const word r1=table.r1[first+s],r2=table.r2[first+s],r3=table.r3[first+s];\n\
    \        inverse_butterfly(a+4*s,1,0,r1,r2,r3);\n    }\n}\ninline void forward_radix4_stage(mint*\
    \ __restrict__ a,usize n,int stage)noexcept{\n    const int h=static_cast<int>(std::countr_zero(n));\n\
    \    assert(stage>=0&&stage+2<=h);\n    const usize stride=usize(1)<<(h-stage-2),blocks=usize(1)<<stage;\n\
    \    if(stride>=8)eez::ntt998::detail::forward_radix4_large(a,blocks,stride);\n\
    \    else if(stride==4)forward_radix4_p4(a,blocks);\n    else if(stride==1)forward_radix4_p1(a,blocks);\n\
    \    else forward_radix4_scalar(a,blocks,stride);\n}\ninline void inverse_radix4_stage(mint*\
    \ __restrict__ a,usize n,int stage)noexcept{\n    const int h=static_cast<int>(std::countr_zero(n));\n\
    \    assert(stage>=0&&stage+2<=h);\n    const usize stride=usize(1)<<(h-stage-2),blocks=usize(1)<<stage;\n\
    \    if(stride>=8)eez::ntt998::detail::inverse_radix4_large(a,blocks,stride);\n\
    \    else if(stride==4)inverse_radix4_p4(a,blocks);\n    else if(stride==1)inverse_radix4_p1(a,blocks);\n\
    \    else inverse_radix4_scalar(a,blocks,stride);\n}\ninline void forward_dif(mint*\
    \ __restrict__ a,usize n)noexcept{\n    if(n<=1)return;\n    const int h=static_cast<int>(std::countr_zero(n));\n\
    \    int stage=0;\n    if(h&1){forward_radix2_first(a,n);stage=1;}\n    for(;stage<h;stage+=2)forward_radix4_stage(a,n,stage);\n\
    }\ninline void inverse_dit(mint* __restrict__ a,usize n)noexcept{\n    if(n<=1)return;\n\
    \    const int h=static_cast<int>(std::countr_zero(n));\n    const word scale=mint::raw(static_cast<u32>(n)).inv().a;\n\
    \    if(h&1){\n        for(int stage=h-2;stage>=1;stage-=2)inverse_radix4_stage(a,n,stage);\n\
    \        final_radix2_scale(a,n,scale);\n    }else{\n        for(int stage=h-2;stage>=2;stage-=2)inverse_radix4_stage(a,n,stage);\n\
    \        final_radix4_scale(a,n,scale);\n    }\n}\n\ninline constexpr usize ntt_tile=4096;\n\
    \ntemplate<bool inv>\ninline void stage_range(mint* a,usize stride,usize first,usize\
    \ count,usize total){\n    if(stride==1){\n        if constexpr(inv)inverse_radix4_p1(a+first*4,count,first,total);\n\
    \        else forward_radix4_p1(a+first*4,count,first,total);\n    }else if(stride==4){\n\
    \        if constexpr(inv)inverse_radix4_p4(a+first*16,count,first,total);\n \
    \       else forward_radix4_p4(a+first*16,count,first,total);\n    }else{\n  \
    \      const auto& t=cached_rotations<inv>(total);\n        const vec imag=broadcast(inv?twiddles.iroot[2]:twiddles.root[2]);\n\
    \        for(usize s=first;s<first+count;s++){\n            if(s==0){\n      \
    \          if constexpr(inv)eez::ntt998::detail::inverse_radix4_large(a,1,stride);\n\
    \                else eez::ntt998::detail::forward_radix4_large(a,1,stride);\n\
    \            }else{\n                if constexpr(inv)inverse_radix4_large_block(a+s*4*stride,stride,imag,t.r1[s],t.r2[s],t.r3[s]);\n\
    \                else forward_radix4_large_block(a+s*4*stride,stride,imag,t.r1[s],t.r2[s],t.r3[s]);\n\
    \            }\n        }\n    }\n}\ninline int tile_stage(usize n,int h){\n \
    \   int stage=h&1;\n    while((n>>stage)>ntt_tile)stage+=2;\n    return stage;\n\
    }\ninline void forward_tiled(mint* a,usize n){\n    if(n<=1)return;\n    const\
    \ int h=int(std::countr_zero(n));\n    if(n<=ntt_tile){forward_dif(a,n);return;}\n\
    \    const int split=tile_stage(n,h);\n    int stage=0;\n    if(h&1){forward_radix2_first(a,n);stage=1;}\n\
    \    for(;stage<split;stage+=2)forward_radix4_stage(a,n,stage);\n    for(usize\
    \ b=0;b<(usize(1)<<split);b++)for(int s=split;s<h;s+=2){\n        const usize\
    \ count=usize(1)<<(s-split);\n        stage_range<false>(a,n>>(s+2),b*count,count,usize(1)<<s);\n\
    \    }\n}\ninline void inverse_prefix_tiled(mint* a,usize n){\n    const int h=int(std::countr_zero(n)),last=(h&1)?1:2;\n\
    \    if(n<=ntt_tile){\n        for(int s=h-2;s>=last;s-=2)inverse_radix4_stage(a,n,s);\n\
    \        return;\n    }\n    const int split=tile_stage(n,h);\n    for(usize b=0;b<(usize(1)<<split);b++)for(int\
    \ s=h-2;s>=split;s-=2){\n        const usize count=usize(1)<<(s-split);\n    \
    \    stage_range<true>(a,n>>(s+2),b*count,count,usize(1)<<s);\n    }\n    for(int\
    \ s=split-2;s>=last;s-=2)inverse_radix4_stage(a,n,s);\n}\ninline void inverse_tiled(mint*\
    \ a,usize n){\n    if(n<=1)return;\n    if(n<=ntt_tile){inverse_dit(a,n);return;}\n\
    \    inverse_prefix_tiled(a,n);\n    const word scale=mint::raw(u32(n)).inv().a;\n\
    \    if(std::countr_zero(n)&1)final_radix2_scale(a,n,scale);\n    else final_radix4_scale(a,n,scale);\n\
    }\ninline void forward_pair(mint* a,mint* b,usize n){\n    if(n<=1)return;\n \
    \   forward_tiled(a,n);forward_tiled(b,n);\n}\ninline void inverse_pair(mint*\
    \ a,mint* b,usize n){\n    if(n<=1)return;\n    inverse_tiled(a,n);inverse_tiled(b,n);\n\
    }\n}\n\ninline constexpr int naive_cutoff=24;\n\ninline mint bostan_mori_naive(const\
    \ std::vector<mint>& A,const std::vector<mint>& Q0,u64 N){\n    int d=(int)A.size();\n\
    \    std::vector<mint> Q=Q0;\n    std::vector<mint> P=nt::convolution(Q,A);P.resize(d);\n\
    \    std::vector<mint> Qm(d+1),R(2*d),S(2*d+1);\n    while(N){\n        for(int\
    \ i=0;i<=d;i++)Qm[i]=(i&1)?-Q[i]:Q[i];\n        std::fill(R.begin(),R.end(),mint{});\n\
    \        std::fill(S.begin(),S.end(),mint{});\n        for(int i=0;i<d;i++)for(int\
    \ j=0;j<=d;j++)R[i+j]+=P[i]*Qm[j];\n        for(int i=0;i<=d;i++)for(int j=0;j<=d;j++)S[i+j]+=Q[i]*Qm[j];\n\
    \        int b=(int)(N&1);\n        for(int i=0;i<d;i++)P[i]=R[2*i+b];\n     \
    \   for(int i=0;i<=d;i++)Q[i]=S[2*i];\n        N>>=1;\n    }\n    return P[0];\n\
    }\n\ninline nd::vec half8(nd::vec x)noexcept{\n    x=nd::canonicalize8(x);\n \
    \   const nd::vec one=_mm256_set1_epi32(1),prime=_mm256_set1_epi32((int)mint::MOD);\n\
    \    nd::vec mask=_mm256_sub_epi32(_mm256_setzero_si256(),_mm256_and_si256(x,one));\n\
    \    x=_mm256_add_epi32(x,_mm256_and_si256(mask,prime));\n    return _mm256_srli_epi32(x,1);\n\
    }\ninline void split16(nd::vec a,nd::vec b,nd::vec& e,nd::vec& o)noexcept{\n \
    \   const nd::vec idx=_mm256_setr_epi32(0,2,4,6,1,3,5,7);\n    a=_mm256_permutevar8x32_epi32(a,idx);b=_mm256_permutevar8x32_epi32(b,idx);\n\
    \    e=_mm256_permute2x128_si256(a,b,0x20);\n    o=_mm256_permute2x128_si256(a,b,0x31);\n\
    }\ntemplate<bool Odd>\ninline void make_half(const mint* qf,const mint* pf,mint*\
    \ qh,mint* ph,const mint* twist,usize m)noexcept{\n    usize i=0;\n    for(;i+8<=m;i+=8){\n\
    \        nd::vec qe,qo,pe,po;\n        split16(nd::load8(qf+2*i),nd::load8(qf+2*i+8),qe,qo);\n\
    \        split16(nd::load8(pf+2*i),nd::load8(pf+2*i+8),pe,po);\n        nd::store8(qh+i,nd::mul8(qe,qo));\n\
    \        nd::vec a=nd::mul8(pe,qo),b=nd::mul8(po,qe);\n        if constexpr(Odd)nd::store8(ph+i,nd::mul8(nd::sub8(a,b),nd::load8(twist+i)));\n\
    \        else nd::store8(ph+i,nd::add8(a,b));\n    }\n    for(;i<m;i++){\n   \
    \     mint qe=qf[2*i],qo=qf[2*i+1],pe=pf[2*i],po=pf[2*i+1];\n        qh[i]=qe*qo;\n\
    \        if constexpr(Odd)ph[i]=(pe*qo-po*qe)*twist[i];\n        else ph[i]=pe*qo+po*qe;\n\
    \    }\n}\ninline mint make_final_odd_constant(const mint* qf,const mint* pf,const\
    \ mint* twist,usize m)noexcept{\n    nd::vec acc=_mm256_setzero_si256();\n   \
    \ usize i=0;\n    for(;i+8<=m;i+=8){\n        nd::vec qe,qo,pe,po;\n        split16(nd::load8(qf+2*i),nd::load8(qf+2*i+8),qe,qo);\n\
    \        split16(nd::load8(pf+2*i),nd::load8(pf+2*i+8),pe,po);\n        nd::vec\
    \ v=nd::canonicalize8(nd::mul8(nd::sub8(nd::mul8(pe,qo),nd::mul8(po,qe)),nd::load8(twist+i)));\n\
    \        acc=nd::add8(acc,v);\n    }\n    alignas(32) nd::word s8[8];\n    nd::store8(reinterpret_cast<mint*>(s8),acc);\n\
    \    nd::word s=0;\n    for(int j=0;j<8;j++)s=nd::add(s,s8[j]);\n    for(;i<m;i++){\n\
    \        mint v=(pf[2*i]*qf[2*i+1]-pf[2*i+1]*qf[2*i])*twist[i];\n        s=nd::add(s,nd::canonicalize(nd::raw(v)));\n\
    \    }\n    nd::word im=mint::raw((u32)m).inv().a;\n    return nd::from_raw(nd::mul(s,im));\n\
    }\n\nstruct BMPlan{usize z;bool top_alias;};\n\ninline BMPlan bm_plan(usize plen,usize\
    \ qlen){\n    usize qdeg=qlen-1;\n    if(qdeg&&std::has_single_bit(qdeg)&&plen<=qdeg){\n\
    \        usize z=qdeg<<1;\n        if(z<=nt::max_ntt_size)return {z,true};\n \
    \   }\n    usize need=std::max(plen+qlen-1,usize(2)*qlen-1);\n    usize z=std::max(usize(2),std::bit_ceil(need));\n\
    \    if(z>nt::max_ntt_size)throw std::length_error(\"BostanMori\");\n    return\
    \ {z,false};\n}\ninline usize truncated_len(usize len,u64 N)noexcept{\n    if(N>=u64(len-1))return\
    \ len;\n    return usize(N)+1;\n}\ninline usize trim_length(const mint* a,usize\
    \ n)noexcept{\n    while(n>1&&a[n-1]==mint{})--n;\n    return n;\n}\n\n/*\nhalf_twist[bitrev(k)]\
    \ = 1/w^k\ndouble_twist[i]       = w^i/m\nw: primitive z-th root, z=2m\n*/\ninline\
    \ void build_bm_twists(mint* half_twist,mint* double_twist,usize z)noexcept{\n\
    \    usize m=z>>1;\n    unsigned h=(unsigned)std::countr_zero(z);\n    nd::word\
    \ w=nd::twiddles.root[h],iw=nd::twiddles.iroot[h];\n    nd::word fp=mint::raw((u32)m).inv().a,ip=nd::montgomery_one;\n\
    \    usize rev=0;\n    for(usize k=0;k<m;k++){\n        double_twist[k]=nd::from_raw(fp);\n\
    \        half_twist[rev]=nd::from_raw(ip);\n        fp=nd::mul(fp,w);ip=nd::mul(ip,iw);\n\
    \        if(k+1<m){\n            usize bit=m>>1;\n            while(rev&bit){rev^=bit;bit>>=1;}\n\
    \            rev^=bit;\n        }\n    }\n}\ninline void inverse_twisted(mint*\
    \ __restrict__ a,usize n,const mint* __restrict__ tw)noexcept{\n    if(n==1){a[0]*=tw[0];return;}\n\
    \    const int h=int(std::countr_zero(n));\n    if(h&1){\n        bm_ntt::inverse_prefix_tiled(a,n);\n\
    \        const usize half=n/2;\n        usize i=0;\n        for(;i+8<=half;i+=8){\n\
    \            const auto x=nd::load8(a+i),y=nd::load8(a+half+i);\n            nd::store8(a+i,nd::mul8(nd::add8(x,y),nd::load8(tw+i)));\n\
    \            nd::store8(a+half+i,nd::mul8(nd::sub8(x,y),nd::load8(tw+half+i)));\n\
    \        }\n        for(;i<half;i++){\n            mint x=a[i],y=a[half+i];\n\
    \            a[i]=(x+y)*tw[i];a[half+i]=(x-y)*tw[half+i];\n        }\n    }else{\n\
    \        bm_ntt::inverse_prefix_tiled(a,n);\n        const usize stride=n/4;\n\
    \        const auto imag=nd::broadcast(nd::twiddles.iroot[2]);\n        usize\
    \ i=0;\n        for(;i+8<=stride;i+=8){\n            const auto x0=nd::load8(a+i),x1=nd::load8(a+stride+i),x2=nd::load8(a+2*stride+i),x3=nd::load8(a+3*stride+i);\n\
    \            const auto s01=nd::add8(x0,x1),d01=nd::sub8(x0,x1),s23=nd::add8(x2,x3);\n\
    \            const auto t=nd::mul8_fixed(nd::sub8(x2,x3),imag,_mm256_mul_epu32(imag,nd::broadcast(nd::montgomery_ninv)));\n\
    \            nd::store8(a+i,nd::mul8(nd::add8(s01,s23),nd::load8(tw+i)));\n  \
    \          nd::store8(a+stride+i,nd::mul8(nd::add8(d01,t),nd::load8(tw+stride+i)));\n\
    \            nd::store8(a+2*stride+i,nd::mul8(nd::sub8(s01,s23),nd::load8(tw+2*stride+i)));\n\
    \            nd::store8(a+3*stride+i,nd::mul8(nd::sub8(d01,t),nd::load8(tw+3*stride+i)));\n\
    \        }\n        const mint ii=nd::from_raw(nd::twiddles.iroot[2]);\n     \
    \   for(;i<stride;i++){\n            mint x0=a[i],x1=a[stride+i],x2=a[2*stride+i],x3=a[3*stride+i];\n\
    \            mint s01=x0+x1,d01=x0-x1,s23=x2+x3,t=(x2-x3)*ii;\n            a[i]=(s01+s23)*tw[i];a[stride+i]=(d01+t)*tw[stride+i];\n\
    \            a[2*stride+i]=(s01-s23)*tw[2*stride+i];a[3*stride+i]=(d01-t)*tw[3*stride+i];\n\
    \        }\n    }\n}\ninline void ntt_double_pair(const mint* qh,const mint* ph,mint*\
    \ qf,mint* pf,const mint* double_twist,usize m,bool top_alias,mint qtop)noexcept{\n\
    \    if(qf!=qh)std::memcpy(qf,qh,m*sizeof(mint));\n    if(pf!=ph)std::memcpy(pf,ph,m*sizeof(mint));\n\
    \    std::memcpy(qf+m,qh,m*sizeof(mint));\n    std::memcpy(pf+m,ph,m*sizeof(mint));\n\
    \    inverse_twisted(qf+m,m,double_twist);\n    inverse_twisted(pf+m,m,double_twist);\n\
    \    if(top_alias)qf[m]-=qtop+qtop;\n    bm_ntt::forward_pair(qf+m,pf+m,m);\n\
    }\ninline mint bostan_mori(const std::vector<mint>& A,const std::vector<mint>&\
    \ Q0,u64 N){\n    usize d=A.size();\n    assert(d>0&&Q0.size()==d+1);\n    if(N<d)return\
    \ A[usize(N)];\n    if(d<=naive_cutoff)return bostan_mori_naive(A,Q0,N);\n\n \
    \   std::vector<mint> Pv=nt::convolution(Q0,A);Pv.resize(d);\n    usize plen=d,qlen=d+1;\n\
    \    while(plen>1&&Pv[plen-1]==mint{})--plen;\n    while(qlen>1&&Q0[qlen-1]==mint{})--qlen;\n\
    \n    BMPlan plan=bm_plan(plen,qlen);\n    usize z=plan.z,m=z>>1;\n    bool top_alias=plan.top_alias;\n\
    \    const usize max_z=z,max_m=m;\n    nd::aligned_vector qf(max_z),pf(max_z),qh(max_m),ph(max_m);\n\
    \    nd::aligned_vector half_twist(max_m),double_twist(max_m);\n\n    std::copy_n(Q0.begin(),qlen,qf.begin());\n\
    \    std::copy_n(Pv.begin(),plen,pf.begin());\n    std::fill(qf.begin()+qlen,qf.begin()+z,mint{});\n\
    \    std::fill(pf.begin()+plen,pf.begin()+z,mint{});\n    bm_ntt::forward_pair(qf.data(),pf.data(),z);\n\
    \    build_bm_twists(half_twist.data(),double_twist.data(),z);\n\n    mint qtop=top_alias?Q0[qlen-1]:mint{};\n\
    \    mint p_inv_scale=mint::raw(1);\n    const mint inv2=mint::raw(499122177);\n\
    \n    while(N){\n        p_inv_scale*=inv2;\n        if(N==1)return make_final_odd_constant(qf.data(),pf.data(),half_twist.data(),m)*p_inv_scale;\n\
    \n        const unsigned bit=(unsigned)(N&1);\n        const usize pdeg=plen-1,qdeg=qlen-1,product_deg=pdeg+qdeg;\n\
    \        usize next_plen=product_deg>=bit?(product_deg-bit)/2+1:1;\n        usize\
    \ next_qlen=qlen;\n\n        if(top_alias){\n            mint t=qtop*qtop;\n \
    \           if(qdeg&1)t=-t;\n            qtop=t;\n        }\n\n        N>>=1;\n\
    \        usize cut_plen=truncated_len(next_plen,N);\n        usize cut_qlen=truncated_len(next_qlen,N);\n\
    \        BMPlan desired=bm_plan(cut_plen,cut_qlen);\n\n        if(desired.z<z){\n\
    \            if(bit)make_half<true>(qf.data(),pf.data(),qh.data(),ph.data(),half_twist.data(),m);\n\
    \            else make_half<false>(qf.data(),pf.data(),qh.data(),ph.data(),half_twist.data(),m);\n\
    \            usize old_m=m;\n            bm_ntt::inverse_pair(qh.data(),ph.data(),old_m);\n\
    \n            if(top_alias)qh[0]-=qtop;\n\n            usize qcopy=std::min(cut_qlen,old_m);\n\
    \            std::memcpy(qf.data(),qh.data(),qcopy*sizeof(mint));\n          \
    \  if(top_alias&&cut_qlen>old_m)qf[old_m]=qtop;\n            std::memcpy(pf.data(),ph.data(),cut_plen*sizeof(mint));\n\
    \n            plen=trim_length(pf.data(),cut_plen);\n            qlen=trim_length(qf.data(),cut_qlen);\n\
    \n            plan=bm_plan(plen,qlen);\n            z=plan.z;m=z>>1;\n       \
    \     top_alias=plan.top_alias;\n            qtop=top_alias?qf[qlen-1]:mint{};\n\
    \n            std::fill(qf.begin()+qlen,qf.begin()+z,mint{});\n            std::fill(pf.begin()+plen,pf.begin()+z,mint{});\n\
    \            bm_ntt::forward_pair(qf.data(),pf.data(),z);\n            build_bm_twists(half_twist.data(),double_twist.data(),z);\n\
    \        }else{\n            plen=next_plen;qlen=next_qlen;\n            if(bit)make_half<true>(qf.data(),pf.data(),qf.data(),pf.data(),half_twist.data(),m);\n\
    \            else make_half<false>(qf.data(),pf.data(),qf.data(),pf.data(),half_twist.data(),m);\n\
    \            ntt_double_pair(qf.data(),pf.data(),qf.data(),pf.data(),double_twist.data(),m,top_alias,qtop);\n\
    \        }\n    }\n    return {};\n}\n}\n\ninline mint BostanMori(const std::vector<mint>&\
    \ A,const std::vector<mint>& Q,std::uint64_t N){\n    return detail::bostan_mori(A,Q,N);\n\
    }\n\n}\n\n#if defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))\n#pragma\
    \ clang attribute pop\n#elif defined(__GNUC__)&&!defined(__clang__)&&(defined(__x86_64__)||defined(__i386__))\n\
    #pragma GCC pop_options\n#endif\n"
  dependsOn:
  - convolution/ntt998.hpp
  isVerificationFile: false
  path: fps/BostanMori.hpp
  requiredBy: []
  timestamp: '2026-11-24 15:26:04+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/verify-yosupo-fps/yosupo-kth-term-of-linearly-recurrent-sequence-BostanMori.test.cpp
documentation_of: fps/BostanMori.hpp
layout: document
redirect_from:
- /library/fps/BostanMori.hpp
- /library/fps/BostanMori.hpp.html
title: fps/BostanMori.hpp
---

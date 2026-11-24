---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: convolution/ntt998.hpp
    title: ntt998
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: fps/detail/fps998_inv.hpp
    title: fps/detail/fps998_inv.hpp
  - icon: ':heavy_check_mark:'
    path: fps/fps998.hpp
    title: fps/fps998.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
    title: verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
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
    \ fps/detail/fps998_inv_ntt.hpp: line 2: #pragma once found in a non-first line\n"
  code: "// eez CP Library: https://github.com/28q/eezCPLibrary\n#pragma once\n#include\
    \ <algorithm>\n#include <array>\n#include <bit>\n#include <cassert>\n#include\
    \ <cstddef>\n#include <cstdint>\n#include <cstring>\n#include <span>\n#include\
    \ \"../../convolution/ntt998.hpp\"\n\n#if defined(__GNUC__) && !defined(__clang__)\
    \ && (defined(__x86_64__) || defined(__i386__))\n#pragma GCC push_options\n#pragma\
    \ GCC optimize(\"O3,unroll-loops\")\n#pragma GCC target(\"avx2,bmi,bmi2,lzcnt,popcnt\"\
    )\n#define EEZ_FPS998_ALWAYS_INLINE inline __attribute__((always_inline))\n#define\
    \ EEZ_FPS998_RESTRICT __restrict__\n#elif defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute push(__attribute__((target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))),apply_to=function)\n#define EEZ_FPS998_ALWAYS_INLINE\
    \ inline __attribute__((always_inline))\n#define EEZ_FPS998_RESTRICT __restrict__\n\
    #else\n#define EEZ_FPS998_ALWAYS_INLINE inline\n#define EEZ_FPS998_RESTRICT\n\
    #endif\n\nnamespace eez::fps998::detail::inv_ntt{\nusing mint=ntt998::mint;\n\
    using usize=std::size_t;\nnamespace nd=ntt998::detail;\n\ninline constexpr std::uint32_t\
    \ inv2_value=(mint::MOD+1u)>>1;\ninline constexpr nd::word inv2_mont=nd::canonicalize(mint::raw(inv2_value).a);\n\
    \nEEZ_FPS998_ALWAYS_INLINE nd::word shrink4_scalar(nd::word x) noexcept{\n   \
    \ return x>=nd::mod2?x-nd::mod2:x;\n}\nEEZ_FPS998_ALWAYS_INLINE nd::vec neg8(nd::vec\
    \ x) noexcept{\n    return nd::sub8(_mm256_setzero_si256(),x);\n}\nEEZ_FPS998_ALWAYS_INLINE\
    \ void zero_n(mint* p,usize n) noexcept{\n    if(n) std::memset(p,0,n*sizeof(mint));\n\
    }\nEEZ_FPS998_ALWAYS_INLINE void copy_n(mint* dst,const mint* src,usize n) noexcept{\n\
    \    if(n) std::memcpy(dst,src,n*sizeof(mint));\n}\n\ninline void copy_shrink4_to2(mint*\
    \ EEZ_FPS998_RESTRICT dst,const mint* EEZ_FPS998_RESTRICT src,usize n) noexcept{\n\
    \    usize i=0;\n    for(;i+8<=n;i+=8) nd::store8(dst+i,nd::shrink4_to_2(nd::load8(src+i)));\n\
    \    for(;i<n;++i) dst[i]=nd::from_raw(shrink4_scalar(nd::raw(src[i])));\n}\n\n\
    inline void sub_inplace(mint* EEZ_FPS998_RESTRICT a,const mint* EEZ_FPS998_RESTRICT\
    \ b,usize n) noexcept{\n    usize i=0;\n    for(;i+8<=n;i+=8){\n        const\
    \ nd::vec x=nd::shrink4_to_2(nd::load8_aligned(a+i));\n        const nd::vec y=nd::shrink4_to_2(nd::load8_aligned(b+i));\n\
    \        nd::store8_aligned(a+i,nd::sub8(x,y));\n    }\n    for(;i<n;++i){\n \
    \       const nd::word x=shrink4_scalar(nd::raw(a[i]));\n        const nd::word\
    \ y=shrink4_scalar(nd::raw(b[i]));\n        a[i]=nd::from_raw(nd::sub(x,y));\n\
    \    }\n}\n\ntemplate<unsigned leaf_size,unsigned parallel_blocks>\ninline void\
    \ leaf_products_shifted(mint* EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT\
    \ b,usize n,nd::word shift) noexcept{\n    const usize blocks=n/leaf_size;\n \
    \   assert(blocks%parallel_blocks==0);\n    nd::word w=nd::canonicalize(shift);\n\
    \    for(usize s=0;s<blocks;s+=parallel_blocks){\n        std::array<nd::word,parallel_blocks>\
    \ modulus{};\n        for(unsigned k=0;k<parallel_blocks;++k){\n            modulus[k]=w;\n\
    \            const usize block=s+k;\n            if(block+1<blocks)\n        \
    \        w=nd::canonicalize(nd::mul(w,nd::forward_rate1(nd::twiddle_index(static_cast<ntt998::u32>(block)))));\n\
    \        }\n        if constexpr(leaf_size==8) nd::leaf_product8x4(a,b,s,modulus);\n\
    \        else{\n            static_assert(leaf_size==16&&parallel_blocks==2);\n\
    \            nd::leaf_product16x2_karatsuba(a,b,s,modulus);\n        }\n    }\n\
    }\n\ntemplate<unsigned leaf_size>\ninline void block_twist(mint* EEZ_FPS998_RESTRICT\
    \ a,usize n,nd::word ratio) noexcept{\n    static_assert(leaf_size==8||leaf_size==16);\n\
    \    ratio=nd::canonicalize(ratio);\n    nd::word w=nd::canonicalize(nd::montgomery_one);\n\
    \    const usize blocks=n/leaf_size;\n    for(usize s=0;s<blocks;++s){\n     \
    \   const nd::word c=nd::canonicalize(w);\n        const nd::vec vc=nd::broadcast(c);\n\
    \        const nd::vec vcn=nd::broadcast(c*nd::montgomery_ninv);\n        mint*\
    \ const p=a+s*leaf_size;\n        if constexpr(leaf_size==8){\n            nd::store8_aligned(p,nd::mul8_fixed(nd::load8_aligned(p),vc,vcn));\n\
    \        }else{\n            nd::store8_aligned(p,nd::mul8_fixed(nd::load8_aligned(p),vc,vcn));\n\
    \            nd::store8_aligned(p+8,nd::mul8_fixed(nd::load8_aligned(p+8),vc,vcn));\n\
    \        }\n        if(s+1<blocks) w=nd::canonicalize(nd::mul(w,ratio));\n   \
    \ }\n}\n\ntemplate<unsigned leaf_size,unsigned parallel_blocks>\ninline void cubic_leaf(mint*\
    \ EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize n,nd::word shift) noexcept{\n\
    \    leaf_products_shifted<leaf_size,parallel_blocks>(a,b,n,shift);\n    leaf_products_shifted<leaf_size,parallel_blocks>(a,b,n,shift);\n\
    \    sub_inplace(a,b,n);\n}\n\ninline void cubic_cyclic(mint* EEZ_FPS998_RESTRICT\
    \ a,mint* EEZ_FPS998_RESTRICT b,usize n) noexcept{\n    assert(n>=16);\n    assert(std::has_single_bit(n));\n\
    \    assert(n<=ntt998::max_convolution_size);\n    const unsigned leaf_log=nd::adaptive_leaf_log(n);\n\
    \    nd::forward_adaptive_pair(a,b,n,leaf_log);\n    if(leaf_log==3) cubic_leaf<8,4>(a,b,n,nd::montgomery_one);\n\
    \    else{\n        assert(leaf_log==4);\n        cubic_leaf<16,2>(a,b,n,nd::montgomery_one);\n\
    \    }\n    nd::inverse_adaptive(a,n,leaf_log);\n}\n\ninline void cubic_negacyclic(mint*\
    \ EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize n) noexcept{\n    assert(n>=16);\n\
    \    assert(std::has_single_bit(n));\n    assert(n<=ntt998::max_convolution_size);\n\
    \    const unsigned leaf_log=nd::adaptive_leaf_log(n);\n    const usize blocks=n>>leaf_log;\n\
    \    const unsigned block_log=static_cast<unsigned>(std::countr_zero(blocks));\n\
    \    assert(block_log+1<=nd::max_log);\n    const nd::word psi=nd::canonicalize(nd::twiddles.root[block_log+1]);\n\
    \    const nd::word ipsi=nd::canonicalize(nd::twiddles.iroot[block_log+1]);\n\n\
    \    if(leaf_log==3){\n        block_twist<8>(a,n,psi);\n        block_twist<8>(b,n,psi);\n\
    \    }else{\n        assert(leaf_log==4);\n        block_twist<16>(a,n,psi);\n\
    \        block_twist<16>(b,n,psi);\n    }\n\n    nd::forward_adaptive_pair(a,b,n,leaf_log);\n\
    \    if(leaf_log==3) cubic_leaf<8,4>(a,b,n,psi);\n    else cubic_leaf<16,2>(a,b,n,psi);\n\
    \    nd::inverse_adaptive(a,n,leaf_log);\n\n    if(leaf_log==3) block_twist<8>(a,n,ipsi);\n\
    \    else block_twist<16>(a,n,ipsi);\n}\n\ninline void fold_f_cyclic(std::span<const\
    \ mint> f,mint* EEZ_FPS998_RESTRICT a,usize m) noexcept{\n    const usize lo=std::min(f.size(),m);\n\
    \    copy_n(a,f.data(),lo);\n    zero_n(a+lo,m-lo);\n    if(f.size()<=m) return;\n\
    \    const usize hi=std::min(f.size()-m,m);\n    usize i=0;\n    for(;i+8<=hi;i+=8){\n\
    \        const nd::vec x=nd::shrink4_to_2(nd::load8_aligned(a+i));\n        const\
    \ nd::vec y=nd::shrink4_to_2(nd::load8(f.data()+m+i));\n        nd::store8_aligned(a+i,nd::add8(x,y));\n\
    \    }\n    for(;i<hi;++i){\n        const nd::word x=shrink4_scalar(nd::raw(a[i]));\n\
    \        const nd::word y=shrink4_scalar(nd::raw(f[m+i]));\n        a[i]=nd::from_raw(nd::add(x,y));\n\
    \    }\n}\n\ninline void recover_high(const mint* EEZ_FPS998_RESTRICT a,mint*\
    \ out,usize m,usize len) noexcept{\n    const nd::vec vhalf=nd::broadcast(inv2_mont);\n\
    \    const nd::vec vhalfn=nd::broadcast(inv2_mont*nd::montgomery_ninv);\n    usize\
    \ i=0;\n    for(;i+8<=len;i+=8){\n        const nd::vec a0=nd::shrink4_to_2(nd::load8(a+i));\n\
    \        const nd::vec a1=nd::shrink4_to_2(nd::load8(a+m+i));\n        const nd::vec\
    \ b=nd::shrink4_to_2(nd::load8(out+i));\n        nd::vec x=nd::add8(a0,a1);\n\
    \        x=nd::add8(x,b);\n        x=nd::mul8_fixed(x,vhalf,vhalfn);\n       \
    \ nd::store8(out+i,neg8(x));\n    }\n    for(;i<len;++i){\n        const nd::word\
    \ a0=shrink4_scalar(nd::raw(a[i]));\n        const nd::word a1=shrink4_scalar(nd::raw(a[m+i]));\n\
    \        const nd::word b=shrink4_scalar(nd::raw(out[i]));\n        nd::word x=nd::add(a0,a1);\n\
    \        x=nd::add(x,b);\n        x=nd::mul(x,inv2_mont);\n        out[i]=nd::from_raw(nd::sub(0,x));\n\
    \    }\n}\n\ninline void step_3half(std::span<const mint> f,std::span<mint> out,mint*\
    \ EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize m) noexcept{\n    const\
    \ usize n=m<<1;\n    assert(m>=16);\n    assert(std::has_single_bit(m));\n   \
    \ assert(n<=ntt998::max_convolution_size);\n    assert(out.size()>m);\n\n    const\
    \ usize len=std::min(m,out.size()-m);\n    if(!len) return;\n\n    fold_f_cyclic(f,a,m);\n\
    \    copy_n(b,out.data(),m);\n    cubic_cyclic(a,b,m);\n\n    copy_shrink4_to2(out.data()+m,a,len);\n\
    \n    const usize fn=std::min(f.size(),n);\n    copy_n(a,f.data(),fn);\n    zero_n(a+fn,n-fn);\n\
    \n    copy_n(b,out.data(),m);\n    zero_n(b+m,m);\n\n    cubic_negacyclic(a,b,n);\n\
    \    recover_high(a,out.data()+m,m,len);\n}\n}\n\n#undef EEZ_FPS998_RESTRICT\n\
    #undef EEZ_FPS998_ALWAYS_INLINE\n\n#if defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute pop\n#elif defined(__GNUC__)\
    \ && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))\n#pragma\
    \ GCC pop_options\n#endif\n"
  dependsOn:
  - convolution/ntt998.hpp
  isVerificationFile: false
  path: fps/detail/fps998_inv_ntt.hpp
  requiredBy:
  - fps/detail/fps998_inv.hpp
  - fps/fps998.hpp
  timestamp: '2026-11-24 15:26:04+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
documentation_of: fps/detail/fps998_inv_ntt.hpp
layout: document
redirect_from:
- /library/fps/detail/fps998_inv_ntt.hpp
- /library/fps/detail/fps998_inv_ntt.hpp.html
title: fps/detail/fps998_inv_ntt.hpp
---

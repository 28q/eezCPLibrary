---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: convolution/ntt998.hpp
    title: ntt998
  - icon: ':heavy_check_mark:'
    path: fps/detail/fps998_inv_ntt.hpp
    title: fps/detail/fps998_inv_ntt.hpp
  _extendedRequiredBy:
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
    \ fps/detail/fps998_inv.hpp: line 2: #pragma once found in a non-first line\n"
  code: "// eez CP Library: https://github.com/28q/eezCPLibrary\n#pragma once\n#include\
    \ <algorithm>\n#include <bit>\n#include <cstddef>\n#include <cstdint>\n#include\
    \ <span>\n#include <type_traits>\n#include \"fps998_inv_ntt.hpp\"\n\n#if defined(__GNUC__)\
    \ && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))\n#pragma\
    \ GCC push_options\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt\")\n#elif defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute push(__attribute__((target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))),apply_to=function)\n#endif\n\nnamespace\
    \ eez::fps998::detail{\nusing mint=ntt998::mint;\nusing usize=std::size_t;\nnamespace\
    \ nd=ntt998::detail;\n\ninline constexpr usize inv_naive_cutoff=64;\nstatic_assert(std::has_single_bit(inv_naive_cutoff));\n\
    static_assert(std::is_trivially_copyable_v<mint>);\nstatic_assert(sizeof(mint)==sizeof(std::uint32_t));\n\
    \nstruct inv_workspace{\n    nd::aligned_vector a,b;\n    void reserve(usize n){a.reserve(n);b.reserve(n);}\n\
    \    void ensure(usize n){if(a.size()<n)a.resize(n);if(b.size()<n)b.resize(n);}\n\
    \    [[nodiscard]] usize capacity() const noexcept{return std::min(a.capacity(),b.capacity());}\n\
    };\n\ninline void inv_naive_prefix(std::span<const mint> f,std::span<mint> g){\n\
    \    if(g.empty()) return;\n    const mint g0=f[0].inv();\n    g[0]=g0;\n    for(usize\
    \ i=1;i<g.size();++i){\n        mint s{};\n        const usize r=std::min(i,f.size()-1);\n\
    \        for(usize j=1;j<=r;++j) s+=f[j]*g[i-j];\n        g[i]=-s*g0;\n    }\n\
    }\n\ninline void inv_step_3half(std::span<const mint> f,std::span<mint> out,inv_workspace&\
    \ ws,usize m){\n    const usize n=m<<1;\n    ws.ensure(n);\n    inv_ntt::step_3half(f,out,ws.a.data(),ws.b.data(),m);\n\
    }\n\ninline void inv_to_impl(std::span<const mint> f,std::span<mint> out,inv_workspace&\
    \ ws){\n    const usize target=out.size();\n    const usize base=std::min(target,inv_naive_cutoff);\n\
    \    inv_naive_prefix(f,out.first(base));\n    if(target<=inv_naive_cutoff) return;\n\
    \    ws.reserve(std::bit_ceil(target));\n    for(usize m=inv_naive_cutoff;m<target;m<<=1)\n\
    \        inv_step_3half(f,out,ws,m);\n}\n}\n\n#if defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute pop\n#elif defined(__GNUC__)\
    \ && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))\n#pragma\
    \ GCC pop_options\n#endif\n"
  dependsOn:
  - fps/detail/fps998_inv_ntt.hpp
  - convolution/ntt998.hpp
  isVerificationFile: false
  path: fps/detail/fps998_inv.hpp
  requiredBy:
  - fps/fps998.hpp
  timestamp: '2026-11-24 15:26:04+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
documentation_of: fps/detail/fps998_inv.hpp
layout: document
redirect_from:
- /library/fps/detail/fps998_inv.hpp
- /library/fps/detail/fps998_inv.hpp.html
title: fps/detail/fps998_inv.hpp
---

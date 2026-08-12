---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
    title: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"data-structure/UnionFind.hpp\"\n\nstruct UnionFind{\n\t\
    private:\n\tvector<int32_t> p;\n\tpublic:\n\texplicit UnionFind(int siz) : p(siz){\n\
    \t\tint32_t i=0;\n\t\tfor (;i+1<siz;i+=2){\n\t\t\tconst uint64_t v= uint64_t(uint32_t(i))|(uint64_t(uint32_t(i+1))<<32);\n\
    \t\t\t*reinterpret_cast<uint64_t*>(p.data()+i)=v;\n\t\t}\n\t\tif(i<siz) p[i]=i;\n\
    \t}\n\t[[gnu::always_inline]]\n\tinline void merge(int32_t x,int32_t y) noexcept{\n\
    \t\tint32_t* const __restrict__ pp=p.data();\n\t\tint32_t px=pp[x];\n\t\tint32_t\
    \ py=pp[y];\n\t\twhile(px!=py)[[likely]]{\n\t\t\tif(px<py)[[likely]]{\n\t\t\t\t\
    if(x==px){\n\t\t\t\t\tpp[x]=py;\n\t\t\t\t\treturn;\n\t\t\t\t}\n\t\t\t\tpp[x]=py;\n\
    \t\t\t\tx=px;\n\t\t\t\tpx=pp[x];\n\t\t\t}\n\t\t\telse{\n\t\t\t\tif (y == py){\n\
    \t\t\t\t\tpp[y]=px;\n\t\t\t\t\treturn;\n\t\t\t\t}\n\t\t\t\tpp[y] = px;\n\t\t\t\
    \ty=py;\n\t\t\t\tpy=pp[y];\n\t\t\t}\n\t\t}\n\t\treturn;\n\t}\n\t[[gnu::always_inline]]\n\
    \tinline bool same(int32_t x,int32_t y) noexcept{\n\t\tint32_t* const __restrict__\
    \ pp=p.data();\n\t\twhile(pp[x]!=x){\n\t\t\tpp[x]=pp[pp[x]];\n\t\t\tx=pp[x];\n\
    \t\t}\n\t\twhile(pp[y]!=y){\n\t\t\tpp[y]=pp[pp[y]];\n\t\t\ty=pp[y];\n\t\t}\n\t\
    \treturn x==y;\n\t}\n};\n"
  code: "#pragma once\n\nstruct UnionFind{\n\tprivate:\n\tvector<int32_t> p;\n\tpublic:\n\
    \texplicit UnionFind(int siz) : p(siz){\n\t\tint32_t i=0;\n\t\tfor (;i+1<siz;i+=2){\n\
    \t\t\tconst uint64_t v= uint64_t(uint32_t(i))|(uint64_t(uint32_t(i+1))<<32);\n\
    \t\t\t*reinterpret_cast<uint64_t*>(p.data()+i)=v;\n\t\t}\n\t\tif(i<siz) p[i]=i;\n\
    \t}\n\t[[gnu::always_inline]]\n\tinline void merge(int32_t x,int32_t y) noexcept{\n\
    \t\tint32_t* const __restrict__ pp=p.data();\n\t\tint32_t px=pp[x];\n\t\tint32_t\
    \ py=pp[y];\n\t\twhile(px!=py)[[likely]]{\n\t\t\tif(px<py)[[likely]]{\n\t\t\t\t\
    if(x==px){\n\t\t\t\t\tpp[x]=py;\n\t\t\t\t\treturn;\n\t\t\t\t}\n\t\t\t\tpp[x]=py;\n\
    \t\t\t\tx=px;\n\t\t\t\tpx=pp[x];\n\t\t\t}\n\t\t\telse{\n\t\t\t\tif (y == py){\n\
    \t\t\t\t\tpp[y]=px;\n\t\t\t\t\treturn;\n\t\t\t\t}\n\t\t\t\tpp[y] = px;\n\t\t\t\
    \ty=py;\n\t\t\t\tpy=pp[y];\n\t\t\t}\n\t\t}\n\t\treturn;\n\t}\n\t[[gnu::always_inline]]\n\
    \tinline bool same(int32_t x,int32_t y) noexcept{\n\t\tint32_t* const __restrict__\
    \ pp=p.data();\n\t\twhile(pp[x]!=x){\n\t\t\tpp[x]=pp[pp[x]];\n\t\t\tx=pp[x];\n\
    \t\t}\n\t\twhile(pp[y]!=y){\n\t\t\tpp[y]=pp[pp[y]];\n\t\t\ty=pp[y];\n\t\t}\n\t\
    \treturn x==y;\n\t}\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: data-structure/UnionFind.hpp
  requiredBy: []
  timestamp: '2026-08-12 18:26:02+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
documentation_of: data-structure/UnionFind.hpp
layout: document
title: UnionFind
---

## 使い方

## 計算量

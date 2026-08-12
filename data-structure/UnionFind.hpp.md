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
  bundledCode: "#line 2 \"data-structure/UnionFind.hpp\"\n\nstruct UnionFind {\n\t\
    private:\n\tvector<int32_t> p;\n\tpublic:\n\texplicit UnionFind(int n) : p(n,-1){}\n\
    \t[[gnu::always_inline]]\n\tinline bool same(int32_t x,int32_t y) noexcept{\n\t\
    int32_t* const __restrict__ p=this->p.data();\n\t\tfor(;;){\n\t\t\tconst int32_t\
    \ px=p[x];\n\t\t\tif(px<0) break;\n\t\t\tconst int32_t ppx=p[px];\n\t\t\tif(ppx<0){\n\
    \t\t\t\tx=px;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t\tp[x]=ppx;\n\t\t\tx=ppx;\n\t\t}\n\
    \t\tfor(;;){\n\t\t\tconst int32_t py=p[y];\n\t\t\tif(py<0) break;\n\t\t\tconst\
    \ int32_t ppy=p[py];\n\t\t\tif(ppy<0){\n\t\t\t\ty=py;\n\t\t\t\tbreak;\n\t\t\t\
    }\n\t\t\tp[y]=ppy;\n\t\t\ty=ppy;\n\t\t}\n\t\treturn x==y;\n\t}\n\t[[gnu::always_inline]]\n\
    \tinline bool merge(int32_t x,int32_t y)noexcept {\n\t\tint32_t* const __restrict__\
    \ p=this->p.data();\n\t\tfor(;;){\n\t\t\tconst int32_t px=p[x];\n\t\t\tif(px<0)\
    \ break;\n\t\t\tconst int32_t ppx=p[px];\n\t\t\tif(ppx<0){\n\t\t\t\tx=px;\n\t\t\
    \t\tbreak;\n\t\t\t}\n\t\t\tp[x]=ppx;\n\t\t\tx=ppx;\n\t\t}\n\t\tfor(;;){\n\t\t\t\
    const int32_t py=p[y];\n\t\t\tif(py<0) break;\n\t\t\tconst int32_t ppy=p[py];\n\
    \t\t\tif(ppy<0){\n\t\t\t\ty=py;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t\tp[y]=ppy;\n\t\t\
    \ty=ppy;\n\t\t}\n\t\tif(x==y) return false;\n\t\tif(p[x]>p[y]){\n\t\t\tconst int32_t\
    \ t=x;\n\t\t\tx=y;\n\t\t\ty=t;\n\t\t}\n\t\tp[x]+=p[y];\n\t\tp[y]=x;\n\t\treturn\
    \ true;\n\t}\n};\n"
  code: "#pragma once\n\nstruct UnionFind {\n\tprivate:\n\tvector<int32_t> p;\n\t\
    public:\n\texplicit UnionFind(int n) : p(n,-1){}\n\t[[gnu::always_inline]]\n\t\
    inline bool same(int32_t x,int32_t y) noexcept{\n\tint32_t* const __restrict__\
    \ p=this->p.data();\n\t\tfor(;;){\n\t\t\tconst int32_t px=p[x];\n\t\t\tif(px<0)\
    \ break;\n\t\t\tconst int32_t ppx=p[px];\n\t\t\tif(ppx<0){\n\t\t\t\tx=px;\n\t\t\
    \t\tbreak;\n\t\t\t}\n\t\t\tp[x]=ppx;\n\t\t\tx=ppx;\n\t\t}\n\t\tfor(;;){\n\t\t\t\
    const int32_t py=p[y];\n\t\t\tif(py<0) break;\n\t\t\tconst int32_t ppy=p[py];\n\
    \t\t\tif(ppy<0){\n\t\t\t\ty=py;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t\tp[y]=ppy;\n\t\t\
    \ty=ppy;\n\t\t}\n\t\treturn x==y;\n\t}\n\t[[gnu::always_inline]]\n\tinline bool\
    \ merge(int32_t x,int32_t y)noexcept {\n\t\tint32_t* const __restrict__ p=this->p.data();\n\
    \t\tfor(;;){\n\t\t\tconst int32_t px=p[x];\n\t\t\tif(px<0) break;\n\t\t\tconst\
    \ int32_t ppx=p[px];\n\t\t\tif(ppx<0){\n\t\t\t\tx=px;\n\t\t\t\tbreak;\n\t\t\t\
    }\n\t\t\tp[x]=ppx;\n\t\t\tx=ppx;\n\t\t}\n\t\tfor(;;){\n\t\t\tconst int32_t py=p[y];\n\
    \t\t\tif(py<0) break;\n\t\t\tconst int32_t ppy=p[py];\n\t\t\tif(ppy<0){\n\t\t\t\
    \ty=py;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t\tp[y]=ppy;\n\t\t\ty=ppy;\n\t\t}\n\t\tif(x==y)\
    \ return false;\n\t\tif(p[x]>p[y]){\n\t\t\tconst int32_t t=x;\n\t\t\tx=y;\n\t\t\
    \ty=t;\n\t\t}\n\t\tp[x]+=p[y];\n\t\tp[y]=x;\n\t\treturn true;\n\t}\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: data-structure/UnionFind.hpp
  requiredBy: []
  timestamp: '2026-08-12 23:53:26+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
documentation_of: data-structure/UnionFind.hpp
layout: document
title: UnionFind
---

## 使い方

## 計算量

---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: data-structure/UnionFind.hpp
    title: UnionFind
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/unionfind
    links:
    - https://judge.yosupo.jp/problem/unionfind
  bundledCode: "#line 1 \"verify/verify-yosupo-ds/yosupo-unionfind.test.cpp\"\n#define\
    \ PROBLEM \"https://judge.yosupo.jp/problem/unionfind\"\n#include<bits/stdc++.h>\n\
    using namespace std;\n\n#line 2 \"data-structure/UnionFind.hpp\"\n\nstruct UnionFind\
    \ {\n\tprivate:\n\tvector<int32_t> p;\n\tpublic:\n\texplicit UnionFind(int n)\
    \ : p(n,-1){}\n\t[[gnu::always_inline]]\n\tinline bool same(int32_t x,int32_t\
    \ y) noexcept{\n\tint32_t* const __restrict__ p=this->p.data();\n\t\tfor(;;){\n\
    \t\t\tconst int32_t px=p[x];\n\t\t\tif(px<0) break;\n\t\t\tconst int32_t ppx=p[px];\n\
    \t\t\tif(ppx<0){\n\t\t\t\tx=px;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t\tp[x]=ppx;\n\t\t\
    \tx=ppx;\n\t\t}\n\t\tfor(;;){\n\t\t\tconst int32_t py=p[y];\n\t\t\tif(py<0) break;\n\
    \t\t\tconst int32_t ppy=p[py];\n\t\t\tif(ppy<0){\n\t\t\t\ty=py;\n\t\t\t\tbreak;\n\
    \t\t\t}\n\t\t\tp[y]=ppy;\n\t\t\ty=ppy;\n\t\t}\n\t\treturn x==y;\n\t}\n\t[[gnu::always_inline]]\n\
    \tinline bool merge(int32_t x,int32_t y)noexcept {\n\t\tint32_t* const __restrict__\
    \ p=this->p.data();\n\t\tfor(;;){\n\t\t\tconst int32_t px=p[x];\n\t\t\tif(px<0)\
    \ break;\n\t\t\tconst int32_t ppx=p[px];\n\t\t\tif(ppx<0){\n\t\t\t\tx=px;\n\t\t\
    \t\tbreak;\n\t\t\t}\n\t\t\tp[x]=ppx;\n\t\t\tx=ppx;\n\t\t}\n\t\tfor(;;){\n\t\t\t\
    const int32_t py=p[y];\n\t\t\tif(py<0) break;\n\t\t\tconst int32_t ppy=p[py];\n\
    \t\t\tif(ppy<0){\n\t\t\t\ty=py;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t\tp[y]=ppy;\n\t\t\
    \ty=ppy;\n\t\t}\n\t\tif(x==y) return false;\n\t\tif(p[x]>p[y]){\n\t\t\tconst int32_t\
    \ t=x;\n\t\t\tx=y;\n\t\t\ty=t;\n\t\t}\n\t\tp[x]+=p[y];\n\t\tp[y]=x;\n\t\treturn\
    \ true;\n\t}\n};\n#line 6 \"verify/verify-yosupo-ds/yosupo-unionfind.test.cpp\"\
    \n\nint main(){\n\tcin.tie(0)->ios::sync_with_stdio(0);\n\tint N,Q;\n\tcin >>\
    \ N >> Q;\n\tUnionFind uf(N);\n\tfor(;Q--;){\n\t\tint t,u,v;\n\t\tcin >> t >>\
    \ u >> v;\n\t\tif(t==0) uf.merge(u,v);\n\t\telse cout << uf.same(u,v) << '\\n';\n\
    \t}\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/unionfind\"\n#include<bits/stdc++.h>\n\
    using namespace std;\n\n#include\"../../data-structure/UnionFind.hpp\"\n\nint\
    \ main(){\n\tcin.tie(0)->ios::sync_with_stdio(0);\n\tint N,Q;\n\tcin >> N >> Q;\n\
    \tUnionFind uf(N);\n\tfor(;Q--;){\n\t\tint t,u,v;\n\t\tcin >> t >> u >> v;\n\t\
    \tif(t==0) uf.merge(u,v);\n\t\telse cout << uf.same(u,v) << '\\n';\n\t}\n}\n"
  dependsOn:
  - data-structure/UnionFind.hpp
  isVerificationFile: true
  path: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
  requiredBy: []
  timestamp: '2026-08-12 23:53:26+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp.html
title: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
---

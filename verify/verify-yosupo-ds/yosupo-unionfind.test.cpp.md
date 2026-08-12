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
    using namespace std;\n\n#line 2 \"data-structure/UnionFind.hpp\"\n\nstruct UnionFind{\n\
    \tprivate:\n\tvector<int32_t> p;\n\tpublic:\n\texplicit UnionFind(int siz) : p(siz){\n\
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
    \treturn x==y;\n\t}\n};\n#line 6 \"verify/verify-yosupo-ds/yosupo-unionfind.test.cpp\"\
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
  timestamp: '2026-08-12 18:26:02+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp.html
title: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
---

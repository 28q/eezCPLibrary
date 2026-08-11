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
    \tprivate:\n\tint N;\n\tvector<int> par;\n\tint root(int x){\n\t\tint x0=x;\n\t\
    \twhile(par[x]>=0){\n\t\t\tx=par[x];\n\t\t}\n\t\twhile(par[x0]>=0){\n\t\t\tint\
    \ x1=par[x0];\n\t\t\tpar[x0]=x;\n\t\t\tswap(x0,x1);\n\t\t}\n\t\treturn x;\n\t\
    }\n\tpublic:\n\tUnionFind(int siz) : N(siz),par(siz,-1){}\n\tvoid merge(int a,int\
    \ b){\n\t\ta=root(a),b=root(b);\n\t\tif(a==b) return;\n\t\tif(-par[a]<-par[b])\
    \ swap(a,b);\n\t\tpar[a]+=par[b];\n\t\tpar[b]=a;\n\t}\n\tbool same(int a,int b){\n\
    \t\treturn root(a)==root(b);\n\t}\n};\n#line 6 \"verify/verify-yosupo-ds/yosupo-unionfind.test.cpp\"\
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
  timestamp: '2026-08-12 01:37:02+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp.html
title: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
---

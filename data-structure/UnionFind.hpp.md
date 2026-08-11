---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"data-structure/UnionFind.hpp\"\n\nstruct UnionFind{\n\t\
    private:\n\tint N;\n\tvector<int> par;\n\tint root(int x){\n\t\tint x0=x;\n\t\t\
    while(par[x]>=0){\n\t\t\tx=par[x];\n\t\t}\n\t\twhile(par[x0]>=0){\n\t\t\tint x1=par[x0];\n\
    \t\t\tpar[x0]=x;\n\t\t\tswap(x0,x1);\n\t\t}\n\t\treturn x;\n\t}\n\tpublic:\n\t\
    UnionFind(int siz) : N(siz),par(siz,-1){}\n\tvoid merge(int a,int b){\n\t\ta=root(a),b=root(b);\n\
    \t\tif(a==b) return;\n\t\tif(-par[a]<-par[b]) swap(a,b);\n\t\tpar[a]+=par[b];\n\
    \t\tpar[b]=a;\n\t}\n\tbool same(int a,int b){\n\t\treturn root(a)==root(b);\n\t\
    }\n};\n"
  code: "#pragma once\n\nstruct UnionFind{\n\tprivate:\n\tint N;\n\tvector<int> par;\n\
    \tint root(int x){\n\t\tint x0=x;\n\t\twhile(par[x]>=0){\n\t\t\tx=par[x];\n\t\t\
    }\n\t\twhile(par[x0]>=0){\n\t\t\tint x1=par[x0];\n\t\t\tpar[x0]=x;\n\t\t\tswap(x0,x1);\n\
    \t\t}\n\t\treturn x;\n\t}\n\tpublic:\n\tUnionFind(int siz) : N(siz),par(siz,-1){}\n\
    \tvoid merge(int a,int b){\n\t\ta=root(a),b=root(b);\n\t\tif(a==b) return;\n\t\
    \tif(-par[a]<-par[b]) swap(a,b);\n\t\tpar[a]+=par[b];\n\t\tpar[b]=a;\n\t}\n\t\
    bool same(int a,int b){\n\t\treturn root(a)==root(b);\n\t}\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: data-structure/UnionFind.hpp
  requiredBy: []
  timestamp: '2026-08-12 01:04:02+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: data-structure/UnionFind.hpp
layout: document
redirect_from:
- /library/data-structure/UnionFind.hpp
- /library/data-structure/UnionFind.hpp.html
title: data-structure/UnionFind.hpp
---

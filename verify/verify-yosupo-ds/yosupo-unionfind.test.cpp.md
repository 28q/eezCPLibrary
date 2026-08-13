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
    using namespace std;\n\n#line 2 \"data-structure/UnionFind.hpp\"\n\nusing namespace\
    \ std;\n#line 8 \"data-structure/UnionFind.hpp\"\n\n#if defined(__linux__)\n#include<sys/mman.h>\n\
    #define UF_USE_HUGEPAGE 1\n#endif\n\n#if UF_USE_HUGEPAGE\nnamespace uf_detail{\n\
    \tconstexpr size_t kAlign=size_t(1)<<21;\n\tinline void* huge_alloc(size_t bytes)\
    \ {\n\t\tsize_t len=(bytes+kAlign-1)&~(kAlign-1);\n\t\tif(len==0)len=kAlign;\n\
    \t\tvoid* ptr=mmap(nullptr,len,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);\n\
    \t\tif(ptr==MAP_FAILED) throw bad_alloc();\n\t\tmadvise(ptr,len,MADV_HUGEPAGE);\n\
    \t\treturn ptr;\n\t}\n\tinline void huge_free(void* ptr,size_t bytes) noexcept{\n\
    \t\tsize_t len=(bytes+kAlign-1)&~(kAlign-1);\n\t\tif(len==0) len=kAlign;\n\t\t\
    munmap(ptr,len);\n\t}\n}\n\ntemplate<class T>\nstruct HugePageAllocator{\n\tusing\
    \ value_type=T;\n\tHugePageAllocator()noexcept=default;\n\ttemplate<class U> HugePageAllocator(const\
    \ HugePageAllocator<U>&) noexcept{}\n\tT* allocate(size_t n){\n\t\treturn static_cast<T*>(uf_detail::huge_alloc(n*sizeof(T)));\n\
    \t}\n\tvoid deallocate(T* p,size_t n) noexcept{\n\t\tuf_detail::huge_free(p,n*sizeof(T));\n\
    \t}\n\ttemplate<class U> bool operator==(const HugePageAllocator<U>&) const noexcept{return\
    \ true;}\n\ttemplate<class U> bool operator!=(const HugePageAllocator<U>&) const\
    \ noexcept{return false;}\n};\n\ntemplate<class T>\nusing UFAllocator = HugePageAllocator<T>;\n\
    #else\ntemplate<class T>\nusing UFAllocator = std::allocator<T>;\n#endif\n\nstruct\
    \ UnionFind{\n\tprivate:\n\tstd::vector<int32_t,UFAllocator<int32_t>> p;\n\tpublic:\n\
    \texplicit UnionFind(int n) : p(n,-1){}\n\t[[gnu::always_inline]]\n\tinline bool\
    \ same(int32_t x,int32_t y) noexcept{\n\t\tint32_t* const __restrict__ p=this->p.data();\n\
    \t\tfor(;;){\n\t\t\tconst int32_t px=p[x];\n\t\t\tif(px<0) break;\n\t\t\tconst\
    \ int32_t ppx=p[px];\n\t\t\tif(ppx<0){\n\t\t\t\tx=px;\n\t\t\t\tbreak;\n\t\t\t\
    }\n\t\t\tp[x]=ppx;\n\t\t\tx=ppx;\n\t\t}\n\t\tfor(;;){\n\t\t\tconst int32_t py=p[y];\n\
    \t\t\tif(py<0) break;\n\t\t\tconst int32_t ppy=p[py];\n\t\t\tif(ppy<0){\n\t\t\t\
    \ty=py;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t\tp[y]=ppy;\n\t\t\ty=ppy;\n\t\t}\n\t\treturn\
    \ x==y;\n\t}\n\n\t[[gnu::always_inline]]\n\tinline bool merge(int32_t x,int32_t\
    \ y)noexcept {\n\t\tint32_t* const __restrict__ p=this->p.data();\n\t\tfor(;;){\n\
    \t\t\tconst int32_t px=p[x];\n\t\t\tif(px<0) break;\n\t\t\tconst int32_t ppx=p[px];\n\
    \t\t\tif(ppx<0){\n\t\t\t\tx=px;\n\t\t\t\tbreak;\n\t\t\t}\n\t\t\tp[x]=ppx;\n\t\t\
    \tx=ppx;\n\t\t}\n\t\tfor(;;){\n\t\t\tconst int32_t py=p[y];\n\t\t\tif(py<0) break;\n\
    \t\t\tconst int32_t ppy=p[py];\n\t\t\tif(ppy<0){\n\t\t\t\ty=py;\n\t\t\t\tbreak;\n\
    \t\t\t}\n\t\t\tp[y]=ppy;\n\t\t\ty=ppy;\n\t\t}\n\t\tif(x==y) return false;\n\t\t\
    if(p[x]>p[y]){\n\t\t\tconst int32_t t=x;\n\t\t\tx=y;\n\t\t\ty=t;\n\t\t}\n\t\t\
    p[x]+=p[y];\n\t\tp[y]=x;\n\t\treturn true;\n\t}\n};\n#line 6 \"verify/verify-yosupo-ds/yosupo-unionfind.test.cpp\"\
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
  timestamp: '2026-08-13 23:58:41+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp.html
title: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
---

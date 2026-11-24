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
  bundledCode: "Traceback (most recent call last):\n  File \"/home/runner/.local/lib/python3.14/site-packages/onlinejudge_verify/documentation/build.py\"\
    , line 71, in _render_source_code_stat\n    bundled_code = language.bundle(stat.path,\
    \ basedir=basedir, options={'include_paths': [basedir]}).decode()\n          \
    \         ~~~~~~~~~~~~~~~^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\
    \  File \"/home/runner/.local/lib/python3.14/site-packages/onlinejudge_verify/languages/cplusplus.py\"\
    , line 187, in bundle\n    bundler.update(path)\n    ~~~~~~~~~~~~~~^^^^^^\n  File\
    \ \"/home/runner/.local/lib/python3.14/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 401, in update\n    self.update(self._resolve(pathlib.Path(included), included_from=path))\n\
    \    ~~~~~~~~~~~^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\
    \  File \"/home/runner/.local/lib/python3.14/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 312, in update\n    raise BundleErrorAt(path, i + 1, \"#pragma once found\
    \ in a non-first line\")\nonlinejudge_verify.languages.cplusplus_bundle.BundleErrorAt:\
    \ data-structure/UnionFind.hpp: line 2: #pragma once found in a non-first line\n"
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
  timestamp: '2026-11-24 15:26:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
- /verify/verify/verify-yosupo-ds/yosupo-unionfind.test.cpp.html
title: verify/verify-yosupo-ds/yosupo-unionfind.test.cpp
---

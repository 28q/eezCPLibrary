---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: convolution/ntt998.hpp
    title: ntt998
  - icon: ':heavy_check_mark:'
    path: fps/BostanMori.hpp
    title: fps/BostanMori.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence
    links:
    - https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence
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
    \ fps/BostanMori.hpp: line 2: #pragma once found in a non-first line\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence\"\
    \n\n#include <cstddef>\n#include <cstdint>\n#include <iostream>\n#include <vector>\n\
    \n#include \"../../fps/BostanMori.hpp\"\n\nint main(){\n    std::cin.tie(nullptr);\n\
    \    std::ios::sync_with_stdio(false);\n\n    std::size_t d;\n    std::uint64_t\
    \ k;\n    std::cin>>d>>k;\n\n    using mint=eez::fps998::mint;\n    std::vector<mint>\
    \ a(d),q(d+1);\n    for(auto& x:a)std::cin>>x;\n\n    q[0]=mint::raw(1);\n   \
    \ for(std::size_t i=0;i<d;i++){\n        mint c;\n        std::cin>>c;\n     \
    \   q[i+1]=-c;\n    }\n\n    std::cout<<eez::fps998::BostanMori(a,q,k)<<'\\n';\n\
    }\n"
  dependsOn:
  - fps/BostanMori.hpp
  - convolution/ntt998.hpp
  isVerificationFile: true
  path: verify/verify-yosupo-fps/yosupo-kth-term-of-linearly-recurrent-sequence-BostanMori.test.cpp
  requiredBy: []
  timestamp: '2026-11-24 15:26:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-fps/yosupo-kth-term-of-linearly-recurrent-sequence-BostanMori.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-fps/yosupo-kth-term-of-linearly-recurrent-sequence-BostanMori.test.cpp
- /verify/verify/verify-yosupo-fps/yosupo-kth-term-of-linearly-recurrent-sequence-BostanMori.test.cpp.html
title: verify/verify-yosupo-fps/yosupo-kth-term-of-linearly-recurrent-sequence-BostanMori.test.cpp
---

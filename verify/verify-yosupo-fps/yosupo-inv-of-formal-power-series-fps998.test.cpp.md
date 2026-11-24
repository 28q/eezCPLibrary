---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: convolution/ntt998.hpp
    title: ntt998
  - icon: ':heavy_check_mark:'
    path: fps/detail/fps998_inv.hpp
    title: fps/detail/fps998_inv.hpp
  - icon: ':heavy_check_mark:'
    path: fps/detail/fps998_inv_ntt.hpp
    title: fps/detail/fps998_inv_ntt.hpp
  - icon: ':heavy_check_mark:'
    path: fps/fps998.hpp
    title: fps/fps998.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/inv_of_formal_power_series
    links:
    - https://judge.yosupo.jp/problem/inv_of_formal_power_series
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
    \ fps/fps998.hpp: line 2: #pragma once found in a non-first line\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/inv_of_formal_power_series\"\
    \n\n#include <iostream>\n#include <vector>\n\n#include \"../../fps/fps998.hpp\"\
    \n\n\nint main(){\n    std::cin.tie(nullptr);\n    std::ios::sync_with_stdio(false);\n\
    \n    std::size_t n;\n    std::cin>>n;\n\n    using mint=eez::fps998::mint;\n\n\
    \    std::vector<mint> f(n);\n    for(auto& x:f) std::cin>>x;\n\n    const auto\
    \ g=eez::fps998::inv(f);\n\n    for(std::size_t i=0;i<n;++i){\n        if(i) std::cout<<'\
    \ ';\n        std::cout<<g[i];\n    }\n    std::cout<<'\\n';\n}\n"
  dependsOn:
  - fps/fps998.hpp
  - fps/detail/fps998_inv.hpp
  - fps/detail/fps998_inv_ntt.hpp
  - convolution/ntt998.hpp
  isVerificationFile: true
  path: verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
  requiredBy: []
  timestamp: '2026-11-24 15:26:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
- /verify/verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp.html
title: verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
---

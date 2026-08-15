---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes: {}
  bundledCode: "Traceback (most recent call last):\n  File \"/home/runner/.local/lib/python3.12/site-packages/onlinejudge_verify/documentation/build.py\"\
    , line 71, in _render_source_code_stat\n    bundled_code = language.bundle(stat.path,\
    \ basedir=basedir, options={'include_paths': [basedir]}).decode()\n          \
    \         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\
    \  File \"/home/runner/.local/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus.py\"\
    , line 187, in bundle\n    bundler.update(path)\n  File \"/home/runner/.local/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 401, in update\n    self.update(self._resolve(pathlib.Path(included), included_from=path))\n\
    \  File \"/home/runner/.local/lib/python3.12/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py\"\
    , line 304, in update\n    raise BundleErrorAt(path, i + 1, \"unmatched #endif\"\
    )\nonlinejudge_verify.languages.cplusplus_bundle.BundleErrorAt: math/modint998.hpp:\
    \ line 228: unmatched #endif\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/aplusb\"\n\n#include <cstdint>\n\
    #include <iostream>\n\n#include \"../../math/modint998.hpp\"\n\nconstexpr modint998\
    \ modint998_compile_time_check = modint998(-1) * modint998(2);\nstatic_assert(modint998_compile_time_check.val()\
    \ == modint998::MOD - 2);\nstatic_assert(modint998::raw(123).val() == 123);\n\
    static_assert((modint998(3) * modint998(3).inv()).val() == 1);\n\nint main() {\n\
    \    std::int64_t a, b;\n    std::cin >> a >> b;\n    std::cout << a + b << '\\\
    n';\n}\n"
  dependsOn: []
  isVerificationFile: true
  path: verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
- /verify/verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp.html
title: verify/verify-yosupo-math/yosupo-aplusb-modint998.test.cpp
---

---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: IO/fastio_unsafe.hpp
    title: fastio_unsafe
  - icon: ':heavy_check_mark:'
    path: convolution/ntt998.hpp
    title: ntt998
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/convolution_mod_large
    links:
    - https://judge.yosupo.jp/problem/convolution_mod_large
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
    \ convolution/ntt998.hpp: line 2: #pragma once found in a non-first line\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/convolution_mod_large\"\
    \n\n#ifndef FASTIO_UNSAFE_BLOCK_LOG\n#define FASTIO_UNSAFE_BLOCK_LOG 12\n#endif\n\
    \n#include <cstddef>\n#include <cstdint>\n\n#include \"../../convolution/ntt998.hpp\"\
    \n\n#if defined(__GNUC__) && !defined(__clang__) && \\\n    (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC\
    \ target(\"avx2,bmi,bmi2,lzcnt,popcnt\")\n#elif defined(__clang__) && \\\n   \
    \ (defined(__x86_64__) || defined(__i386__))\n#pragma clang attribute push( \\\
    \n    __attribute__((target(\"avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))), \\\n    apply_to\
    \ = function)\n#endif\n\n#include \"../../IO/fastio_unsafe.hpp\"\n\nint main()\
    \ {\n    fastio_unsafe io;\n    char* input_cursor = io.input_cursor();\n    char*\
    \ output_cursor = io.output_cursor();\n    char* const output_end = io.output_end();\n\
    \n    const std::size_t n = io.read_u32(input_cursor);\n    const std::size_t\
    \ m = io.read_u32(input_cursor);\n\n    eez::ntt998::convolution_normal_io(\n\
    \        n, m,\n        [&]() -> std::uint32_t {\n            return io.read_u32_lt1e9(input_cursor);\n\
    \        },\n        [&](std::uint32_t value) {\n            io.write_u32_lt1e9(output_cursor,\
    \ output_end, value);\n        }\n    );\n\n    io.finish(output_cursor);\n  \
    \  return 0;\n}\n\n#if defined(__clang__) && \\\n    (defined(__x86_64__) || defined(__i386__))\n\
    #pragma clang attribute pop\n#endif\n"
  dependsOn:
  - convolution/ntt998.hpp
  - IO/fastio_unsafe.hpp
  isVerificationFile: true
  path: verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp
  requiredBy: []
  timestamp: '2026-11-24 15:26:04+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp
- /verify/verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp.html
title: verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp
---

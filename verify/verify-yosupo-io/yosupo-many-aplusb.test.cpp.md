---
data:
  _extendedDependsOn:
  - icon: ':question:'
    path: IO/fastio_unsafe.hpp
    title: fastio_unsafe
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/many_aplusb
    links:
    - https://judge.yosupo.jp/problem/many_aplusb
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
    \ IO/fastio_unsafe.hpp: line 2: #pragma once found in a non-first line\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/many_aplusb\"\n\n#if defined(__GNUC__)\
    \ && !defined(__clang__) && \\\n    (defined(__x86_64__) || defined(__i386__))\n\
    #pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC target(\"avx2,bmi,bmi2,lzcnt,popcnt\"\
    )\n#elif defined(__clang__) && \\\n    (defined(__x86_64__) || defined(__i386__))\n\
    #pragma clang attribute push( \\\n    __attribute__((target(\"avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"\
    ))), \\\n    apply_to = function)\n#endif\n\n#ifndef FASTIO_UNSAFE_BLOCK_LOG\n\
    #define FASTIO_UNSAFE_BLOCK_LOG 12\n#endif\n\n#include \"../../IO/fastio_unsafe.hpp\"\
    \n\nint main() {\n    fastio_unsafe io;\n    char* input_cursor = io.in.cursor();\n\
    \    char* output_cursor = io.out.begin();\n    char* const output_end = io.out.end();\n\
    \n    const fastio_unsafe_impl::u64 test_count =\n        fastio_unsafe_impl::read_u64(input_cursor);\n\
    \    constexpr fastio_unsafe_impl::u64 block_size =\n        1ULL << FASTIO_UNSAFE_BLOCK_LOG;\n\
    \    alignas(64) fastio_unsafe_impl::u64 sums[block_size];\n\n    for (fastio_unsafe_impl::u64\
    \ base = 0;\n         base < test_count; base += block_size) {\n        const\
    \ fastio_unsafe_impl::u64 count =\n            std::min(block_size, test_count\
    \ - base);\n        for (fastio_unsafe_impl::u64 i = 0; i < count; ++i) {\n  \
    \          const fastio_unsafe_impl::u64 a =\n                fastio_unsafe_impl::read_u64(input_cursor);\n\
    \            const fastio_unsafe_impl::u64 b =\n                fastio_unsafe_impl::read_u64(input_cursor);\n\
    \            sums[i] = a + b;\n        }\n        for (fastio_unsafe_impl::u64\
    \ i = 0; i < count; ++i) {\n            fastio_unsafe_impl::write_u64(\n     \
    \           io.out, output_cursor, output_end, sums[i]);\n        }\n    }\n \
    \   io.out.finish(output_cursor);\n}\n\n#if defined(__clang__) && \\\n    (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute pop\n#endif\n"
  dependsOn:
  - IO/fastio_unsafe.hpp
  isVerificationFile: true
  path: verify/verify-yosupo-io/yosupo-many-aplusb.test.cpp
  requiredBy: []
  timestamp: '2026-11-24 15:26:04+09:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: verify/verify-yosupo-io/yosupo-many-aplusb.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-io/yosupo-many-aplusb.test.cpp
- /verify/verify/verify-yosupo-io/yosupo-many-aplusb.test.cpp.html
title: verify/verify-yosupo-io/yosupo-many-aplusb.test.cpp
---

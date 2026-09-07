---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: convolution/ntt998.hpp
    title: ntt998
  - icon: ':heavy_check_mark:'
    path: fps/detail/fps998_inv_ntt.hpp
    title: fps/detail/fps998_inv_ntt.hpp
  - icon: ':heavy_check_mark:'
    path: math/modint998.hpp
    title: modint998
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: fps/fps998.hpp
    title: fps/fps998.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
    title: verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"fps/detail/fps998_inv.hpp\"\n#include <algorithm>\n#include\
    \ <bit>\n#include <cstddef>\n#include <cstdint>\n#include <span>\n#include <type_traits>\n\
    #line 3 \"fps/detail/fps998_inv_ntt.hpp\"\n#include <array>\n#line 5 \"fps/detail/fps998_inv_ntt.hpp\"\
    \n#include <cassert>\n#line 8 \"fps/detail/fps998_inv_ntt.hpp\"\n#include <cstring>\n\
    #line 2 \"convolution/ntt998.hpp\"\n\r\n#if ((defined(__GNUC__) || defined(__clang__))\
    \ && \\\r\n     (defined(__x86_64__) || defined(__i386__))) || \\\r\n    defined(_M_AVX2)\r\
    \n#define EEZ_NTT998_USE_AVX2 1\r\n#else\r\n#error \"eez::ntt998 requires an x86\
    \ target with AVX2 support\"\r\n#endif\r\n\r\n#line 17 \"convolution/ntt998.hpp\"\
    \n#include <immintrin.h>\r\n#include <new>\r\n#line 20 \"convolution/ntt998.hpp\"\
    \n#include <stdexcept>\r\n#line 22 \"convolution/ntt998.hpp\"\n#include <vector>\r\
    \n\r\n#line 1 \"math/modint998.hpp\"\n\n\n\r\n#line 6 \"math/modint998.hpp\"\n\
    #include <iostream>\r\n#line 8 \"math/modint998.hpp\"\n\r\nstruct modint998 {\r\
    \n    using u32 = std::uint32_t;\r\n    using i32 = std::int32_t;\r\n    using\
    \ u64 = std::uint64_t;\r\n\r\n    static constexpr u32 MOD = 998244353u;\r\n \
    \   static constexpr u32 MOD2 = MOD * 2;\r\n    static constexpr u32 primitive_root\
    \ = 3;\r\n    static constexpr int max_power_of_two = 23;\r\n\r\nprivate:\r\n\
    \    static constexpr u32 R = 3296722945u;\r\n    static constexpr u32 N2 = 932051910u;\r\
    \n\r\n    struct montgomery_tag {};\r\n\r\n    constexpr modint998(u32 x, montgomery_tag)\
    \ : a(x) {}\r\n\r\n    static constexpr u32 reduce(u64 x) {\r\n        return\
    \ static_cast<u32>(\r\n            (x + u64(static_cast<u32>(x) * u32(-R)) * MOD)\
    \ >> 32\r\n        );\r\n    }\r\n\r\npublic:\r\n    u32 a;\r\n\r\n    static_assert(MOD\
    \ < (u32(1) << 30));\r\n    static_assert((MOD & 1) != 0);\r\n    static_assert(R\
    \ * MOD == 1);\r\n\r\n    constexpr modint998() : a(0) {}\r\n\r\n    template\
    \ <class T, std::enable_if_t<std::is_integral_v<T> &&\r\n                    \
    \                    std::is_signed_v<T>, int> = 0>\r\n    constexpr modint998(T\
    \ x) : a(0) {\r\n        const std::int64_t y =\r\n            static_cast<std::int64_t>(x)\
    \ % std::int64_t(MOD) + MOD;\r\n        a = reduce(u64(y) * N2);\r\n    }\r\n\r\
    \n    template <class T, std::enable_if_t<std::is_integral_v<T> &&\r\n       \
    \                                 std::is_unsigned_v<T>, int> = 0>\r\n    constexpr\
    \ modint998(T x)\r\n        : a(reduce(((u64(x) % MOD) + MOD) * N2)) {}\r\n\r\n\
    \    static constexpr modint998 raw(u32 x) {\r\n        return modint998(reduce(u64(x)\
    \ * N2), montgomery_tag{});\r\n    }\r\n\r\n    static constexpr modint998 montgomery_raw(u32\
    \ x) {\r\n        return modint998(x, montgomery_tag{});\r\n    }\r\n\r\n    static\
    \ constexpr u32 mod() { return MOD; }\r\n    static constexpr u32 get_mod() {\
    \ return MOD; }\r\n\r\n    constexpr u32 val() const {\r\n        const u32 x\
    \ = reduce(a);\r\n        return x >= MOD ? x - MOD : x;\r\n    }\r\n\r\n    constexpr\
    \ u32 get() const { return val(); }\r\n\r\n    constexpr modint998& operator+=(const\
    \ modint998& rhs) {\r\n        a += rhs.a - MOD2;\r\n        if (i32(a) < 0) a\
    \ += MOD2;\r\n        return *this;\r\n    }\r\n\r\n    constexpr modint998& operator-=(const\
    \ modint998& rhs) {\r\n        a -= rhs.a;\r\n        if (i32(a) < 0) a += MOD2;\r\
    \n        return *this;\r\n    }\r\n\r\n    constexpr modint998& operator*=(const\
    \ modint998& rhs) {\r\n        a = reduce(u64(a) * rhs.a);\r\n        return *this;\r\
    \n    }\r\n\r\n    constexpr modint998& operator/=(const modint998& rhs) {\r\n\
    \        return *this *= rhs.inv();\r\n    }\r\n\r\n    constexpr modint998 operator+()\
    \ const { return *this; }\r\n    constexpr modint998 operator-() const { return\
    \ modint998() - *this; }\r\n\r\n    friend constexpr modint998 operator+(modint998\
    \ lhs,\r\n                                          const modint998& rhs) {\r\n\
    \        return lhs += rhs;\r\n    }\r\n\r\n    friend constexpr modint998 operator-(modint998\
    \ lhs,\r\n                                          const modint998& rhs) {\r\n\
    \        return lhs -= rhs;\r\n    }\r\n\r\n    friend constexpr modint998 operator*(modint998\
    \ lhs,\r\n                                          const modint998& rhs) {\r\n\
    \        return lhs *= rhs;\r\n    }\r\n\r\n    friend constexpr modint998 operator/(modint998\
    \ lhs,\r\n                                          const modint998& rhs) {\r\n\
    \        return lhs /= rhs;\r\n    }\r\n\r\n    friend constexpr bool operator==(const\
    \ modint998& lhs,\r\n                                     const modint998& rhs)\
    \ {\r\n        const u32 x = lhs.a >= MOD ? lhs.a - MOD : lhs.a;\r\n        const\
    \ u32 y = rhs.a >= MOD ? rhs.a - MOD : rhs.a;\r\n        return x == y;\r\n  \
    \  }\r\n\r\n    friend constexpr bool operator!=(const modint998& lhs,\r\n   \
    \                                  const modint998& rhs) {\r\n        return !(lhs\
    \ == rhs);\r\n    }\r\n\r\n    constexpr modint998& operator++() {\r\n       \
    \ return *this += raw(1);\r\n    }\r\n\r\n    constexpr modint998 operator++(int)\
    \ {\r\n        modint998 old = *this;\r\n        ++*this;\r\n        return old;\r\
    \n    }\r\n\r\n    constexpr modint998& operator--() {\r\n        return *this\
    \ -= raw(1);\r\n    }\r\n\r\n    constexpr modint998 operator--(int) {\r\n   \
    \     modint998 old = *this;\r\n        --*this;\r\n        return old;\r\n  \
    \  }\r\n\r\n    constexpr modint998 pow(u64 e) const {\r\n        if (e == 0)\
    \ return raw(1);\r\n        if (a == 0) return raw(0);\r\n    \r\n        if (e\
    \ >= MOD - 1) e %= MOD - 1;\r\n        if (e == 0) return raw(1);\r\n    \r\n\
    \        const u32 n = static_cast<u32>(e);\r\n    \r\n        if (n == 1) return\
    \ *this;\r\n    \r\n        const modint998 x = *this;\r\n        const modint998\
    \ p2 = x * x;\r\n    \r\n        if (n == 2) return p2;\r\n    \r\n        const\
    \ modint998 p3 = p2 * x;\r\n    \r\n        if (n == 3) return p3;\r\n    \r\n\
    \        const modint998 p4 = p2 * p2;\r\n        const modint998 p5 = p4 * x;\r\
    \n        const modint998 p6 = p3 * p3;\r\n        const modint998 p7 = p4 * p3;\r\
    \n    \r\n        const modint998 t[8] = {\r\n            raw(1), x, p2, p3, p4,\
    \ p5, p6, p7\r\n        };\r\n    \r\n        const unsigned s = ((std::bit_width(n)\
    \ - 1) / 3) * 3;\r\n        modint998 r = t[(n >> s) & 7];\r\n    \r\n       \
    \ auto step = [&](unsigned k) constexpr {\r\n            r *= r;\r\n         \
    \   r *= r;\r\n            r *= r;\r\n            const u32 d = (n >> k) & 7;\r\
    \n            if (d) r *= t[d];\r\n        };\r\n        switch (s) {\r\n    \
    \        case 27: step(24); [[fallthrough]];\r\n            case 24: step(21);\
    \ [[fallthrough]];\r\n            case 21: step(18); [[fallthrough]];\r\n    \
    \        case 18: step(15); [[fallthrough]];\r\n            case 15: step(12);\
    \ [[fallthrough]];\r\n            case 12: step(9);  [[fallthrough]];\r\n    \
    \        case 9:  step(6);  [[fallthrough]];\r\n            case 6:  step(3);\
    \  [[fallthrough]];\r\n            case 3:  step(0);  [[fallthrough]];\r\n   \
    \         default: break;\r\n        }\r\n        return r;\r\n    }\r\n\r\n \
    \   constexpr modint998 inv() const {\r\n        assert(val() != 0);\r\n\r\n \
    \       const modint998 x = *this;\r\n        const modint998 a2 = x * x;\r\n\
    \        const modint998 a4 = a2 * a2;\r\n        const modint998 a5 = a4 * x;\r\
    \n        const modint998 a9 = a5 * a4;\r\n        const modint998 a18 = a9 *\
    \ a9;\r\n        const modint998 a36 = a18 * a18;\r\n        const modint998 a72\
    \ = a36 * a36;\r\n        const modint998 a144 = a72 * a72;\r\n        const modint998\
    \ a288 = a144 * a144;\r\n        const modint998 a293 = a288 * a5;\r\n       \
    \ const modint998 a586 = a293 * a293;\r\n        const modint998 a879 = a586 *\
    \ a293;\r\n        const modint998 a1023 = a879 * a144;\r\n\r\n        modint998\
    \ r = a1023 * a879;  // x^1902\r\n\r\n        // x^(1902 * 2^9) = x^973824\r\n\
    \        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n\
    \        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n\
    \        r *= r;\r\n\r\n        r *= a1023;  // x^974847\r\n\r\n        // x^(974847\
    \ * 2^10) = x^998243328\r\n        r *= r;\r\n        r *= r;\r\n        r *=\
    \ r;\r\n        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n        r *=\
    \ r;\r\n        r *= r;\r\n        r *= r;\r\n        r *= r;\r\n\r\n        return\
    \ r * a1023;  // x^(MOD - 2)\r\n    }\r\n\r\n    constexpr modint998 inverse()\
    \ const { return inv(); }\r\n\r\n    friend std::ostream& operator<<(std::ostream&\
    \ os, const modint998& x) {\r\n        return os << x.val();\r\n    }\r\n\r\n\
    \    friend std::istream& operator>>(std::istream& is, modint998& x) {\r\n   \
    \     std::int64_t value;\r\n        is >> value;\r\n        x = modint998(value);\r\
    \n        return is;\r\n    }\r\n};\r\n\r\nstatic_assert(sizeof(modint998) ==\
    \ 4);\r\nstatic_assert(std::is_trivially_copyable_v<modint998>);\r\n\r\nusing\
    \ mint998 = modint998;\r\n\r\n\n#line 25 \"convolution/ntt998.hpp\"\n\r\n#if defined(__GNUC__)\
    \ && !defined(__clang__) && \\\r\n    (defined(__x86_64__) || defined(__i386__))\r\
    \n#pragma GCC push_options\r\n#pragma GCC optimize(\"O3,unroll-loops\")\r\n#pragma\
    \ GCC target(\"avx2,bmi,bmi2,lzcnt,popcnt\")\r\n#elif defined(__clang__) && \\\
    \r\n    (defined(__x86_64__) || defined(__i386__))\r\n#pragma clang attribute\
    \ push( \\\r\n    __attribute__((target(\"avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))),\
    \ \\\r\n    apply_to = function)\r\n#endif\r\n\r\n#if defined(_MSC_VER)\r\n#define\
    \ EEZ_NTT998_ALWAYS_INLINE __forceinline\r\n#define EEZ_NTT998_RESTRICT __restrict\r\
    \n#elif defined(__GNUC__) || defined(__clang__)\r\n#define EEZ_NTT998_ALWAYS_INLINE\
    \ inline __attribute__((always_inline))\r\n#define EEZ_NTT998_RESTRICT __restrict__\r\
    \n#else\r\n#define EEZ_NTT998_ALWAYS_INLINE inline\r\n#define EEZ_NTT998_RESTRICT\r\
    \n#endif\r\n\r\nnamespace eez::ntt998{\r\n\r\nusing mint = modint998;\r\nusing\
    \ u32 = std::uint32_t;\r\nusing usize = std::size_t;\r\n\r\ninline constexpr u32\
    \ mod = mint::MOD;\r\ninline constexpr usize max_ntt_size = usize(1) << 23;\r\n\
    inline constexpr usize max_convolution_size = usize(1) << 25;\r\ninline constexpr\
    \ usize max_size = max_ntt_size;\r\ninline constexpr usize naive_cutoff = 48;\r\
    \n\r\ninline void forward(std::span<mint> a) noexcept;\r\ninline void inverse(std::span<mint>\
    \ a) noexcept;\r\ninline std::vector<mint> convolution(std::span<const mint> a,\
    \ std::span<const mint> b);\r\ninline std::vector<mint> square(std::span<const\
    \ mint> a);\r\n\r\ninline std::vector<mint> convolution(const std::vector<mint>&\
    \ a, const std::vector<mint>& b){\r\n    return convolution(std::span<const mint>(a.data(),\
    \ a.size()), std::span<const mint>(b.data(), b.size()));\r\n}\r\n\r\ninline std::vector<mint>\
    \ square(const std::vector<mint>& a){\r\n    return square(std::span<const mint>(a.data(),\
    \ a.size()));\r\n}\r\n\r\nnamespace detail{\r\n\r\ntemplate<class T>\r\nclass\
    \ aligned_allocator{\r\npublic:\r\n    using value_type = T;\r\n    using is_always_equal\
    \ = std::true_type;\r\n\r\n    aligned_allocator() noexcept = default;\r\n   \
    \ template<class U> constexpr aligned_allocator(const aligned_allocator<U>&) noexcept\
    \ {}\r\n\r\n    [[nodiscard]] T* allocate(usize n){\r\n        return static_cast<T*>(::operator\
    \ new(n * sizeof(T), std::align_val_t{64}));\r\n    }\r\n\r\n    void deallocate(T*\
    \ p, usize) noexcept {\r\n        ::operator delete(p, std::align_val_t{64});\r\
    \n    }\r\n\r\n    template<class U>\r\n    struct rebind{\r\n        using other\
    \ = aligned_allocator<U>;\r\n    };\r\n};\r\n\r\ntemplate<class T, class U>\r\n\
    constexpr bool operator==(const aligned_allocator<T>&, const aligned_allocator<U>&)\
    \ noexcept {\r\n    return true;\r\n}\r\n\r\ntemplate<class T, class U>\r\nconstexpr\
    \ bool operator!=(const aligned_allocator<T>&, const aligned_allocator<U>&) noexcept\
    \ {\r\n    return false;\r\n}\r\n\r\nusing aligned_vector = std::vector<mint,\
    \ aligned_allocator<mint>>;\r\n\r\n}\r\n\r\nclass workspace{\r\npublic:\r\n  \
    \  workspace() = default;\r\n    explicit workspace(usize n){ reserve(n); }\r\n\
    \r\n    void reserve(usize n){\r\n        if(a_.size() < n) a_.resize(n);\r\n\
    \        if(b_.size() < n) b_.resize(n);\r\n    }\r\n\r\n    [[nodiscard]] usize\
    \ capacity() const noexcept {\r\n        return std::min(a_.size(), b_.size());\r\
    \n    }\r\n\r\nprivate:\r\n    friend void convolution_to(std::span<const mint>,\
    \ std::span<const mint>, std::span<mint>, workspace&);\r\n    friend void square_to(std::span<const\
    \ mint>, std::span<mint>, workspace&);\r\n\r\n    detail::aligned_vector a_;\r\
    \n    detail::aligned_vector b_;\r\n};\r\n\r\ninline void convolution_to(std::span<const\
    \ mint> a, std::span<const mint> b, std::span<mint> out, workspace& ws);\r\ninline\
    \ void square_to(std::span<const mint> a, std::span<mint> out, workspace& ws);\r\
    \n\r\nclass frequency_buffer{\r\npublic:\r\n    frequency_buffer() = default;\r\
    \n\r\n    [[nodiscard]] usize size() const noexcept {\r\n        return data_.size();\r\
    \n    }\r\n\r\nprivate:\r\n    friend void forward_to(std::span<const mint>, frequency_buffer&,\
    \ usize);\r\n    friend void pointwise_multiply(frequency_buffer&, const frequency_buffer&);\r\
    \n    friend void pointwise_square(frequency_buffer&);\r\n    friend void inverse_to(frequency_buffer&,\
    \ std::span<mint>);\r\n\r\n    std::vector<mint> data_;\r\n};\r\n\r\ninline void\
    \ forward_to(std::span<const mint> src, frequency_buffer& dst, usize n);\r\ninline\
    \ void pointwise_multiply(frequency_buffer& lhs, const frequency_buffer& rhs);\r\
    \ninline void pointwise_square(frequency_buffer& a);\r\ninline void inverse_to(frequency_buffer&\
    \ src, std::span<mint> out);\r\n\r\nconstexpr usize convolution_size(usize n,\
    \ usize m) noexcept {\r\n    return n && m ? n + m - 1 : 0;\r\n}\r\n\r\nconstexpr\
    \ usize transform_size(usize n, usize m) noexcept {\r\n    if(!n || !m) return\
    \ 0;\r\n    if(n > max_ntt_size || m > max_ntt_size) return 0;\r\n    if(n > max_ntt_size\
    \ - m + 1) return 0;\r\n    const usize z = n + m - 1;\r\n    usize x = 1;\r\n\
    \    while(x < z) x <<= 1;\r\n    return x;\r\n}\r\n\r\nconstexpr usize convolution_transform_size(usize\
    \ n, usize m) noexcept {\r\n    if(!n || !m) return 0;\r\n    if(n > max_convolution_size\
    \ || m > max_convolution_size) return 0;\r\n    if(n > max_convolution_size -\
    \ m + 1) return 0;\r\n    const usize z = n + m - 1;\r\n    usize x = 1;\r\n \
    \   while(x < z) x <<= 1;\r\n    return x;\r\n}\r\n\r\nconstexpr bool valid_ntt_size(usize\
    \ n) noexcept {\r\n    return n != 0 && (n & (n - 1)) == 0 && n <= max_ntt_size;\r\
    \n}\r\n\r\nconstexpr bool valid_convolution_transform_size(usize n) noexcept {\r\
    \n    return n >= 32 && (n & (n - 1)) == 0 && n <= max_convolution_size;\r\n}\r\
    \n\r\nnamespace detail{\r\n\r\nusing word = u32;\r\nusing u64 = std::uint64_t;\r\
    \n\r\ninline constexpr word mod = mint::MOD;\r\ninline constexpr word mod2 = 2\
    \ * mod;\r\ninline constexpr unsigned max_log = 23;\r\ninline constexpr word montgomery_ninv\
    \ = 998244351u;\r\ninline constexpr word montgomery_one = mint::raw(1).a;\r\n\r\
    \nstatic_assert(mod < (word(1) << 30));\r\nstatic_assert(word(mod * montgomery_ninv)\
    \ == ~word(0));\r\nstatic_assert(sizeof(mint) == sizeof(word));\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ constexpr word raw(const mint& x) noexcept {\r\n    return x.a;\r\n}\r\n\r\n\
    EEZ_NTT998_ALWAYS_INLINE constexpr mint from_raw(word x) noexcept {\r\n    return\
    \ mint::montgomery_raw(x);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE constexpr word\
    \ mul(word a, word b) noexcept {\r\n    const u64 x = u64(a) * b;\r\n    const\
    \ word q = static_cast<word>(x) * montgomery_ninv;\r\n    return static_cast<word>((x\
    \ + u64(q) * mod) >> 32);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE constexpr word\
    \ add(word a, word b) noexcept {\r\n    const word x = a + b;\r\n    return x\
    \ >= mod2 ? x - mod2 : x;\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE constexpr word\
    \ sub(word a, word b) noexcept {\r\n    return a >= b ? a - b : a + mod2 - b;\r\
    \n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE constexpr word canonicalize(word a) noexcept\
    \ {\r\n    return a >= mod ? a - mod : a;\r\n}\r\n\r\nstruct twiddle_table{\r\n\
    \    std::array<word, max_log + 1> root{};\r\n    std::array<word, max_log + 1>\
    \ iroot{};\r\n    std::array<word, max_log + 1> rate1{};\r\n    std::array<word,\
    \ max_log + 1> rate3{};\r\n    std::array<word, max_log + 1> irate3{};\r\n\r\n\
    \    constexpr twiddle_table(){\r\n        root[max_log] = mint::raw(mint::primitive_root).pow((mod\
    \ - 1) >> max_log).a;\r\n        iroot[max_log] = mint::montgomery_raw(root[max_log]).inv().a;\r\
    \n        for(int i = int(max_log) - 1; i >= 0; --i){\r\n            root[usize(i)]\
    \ = mul(root[usize(i + 1)], root[usize(i + 1)]);\r\n            iroot[usize(i)]\
    \ = mul(iroot[usize(i + 1)], iroot[usize(i + 1)]);\r\n        }\r\n\r\n      \
    \  word prod = montgomery_one;\r\n        for(unsigned i = 0; i + 1 <= max_log;\
    \ ++i){\r\n            rate1[i] = mul(root[i + 1], prod);\r\n            prod\
    \ = mul(prod, iroot[i + 1]);\r\n        }\r\n\r\n        prod = montgomery_one;\r\
    \n        word iprod = montgomery_one;\r\n        for(unsigned i = 0; i + 3 <=\
    \ max_log; ++i){\r\n            rate3[i] = mul(root[i + 3], prod);\r\n       \
    \     irate3[i] = mul(iroot[i + 3], iprod);\r\n            prod = mul(prod, iroot[i\
    \ + 3]);\r\n            iprod = mul(iprod, root[i + 3]);\r\n        }\r\n    }\r\
    \n};\r\n\r\ninline constexpr twiddle_table twiddles{};\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ word forward_rate1(unsigned i) noexcept {\r\n    return twiddles.rate1[i];\r\
    \n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE word forward_rate3(unsigned i) noexcept {\r\
    \n    return twiddles.rate3[i];\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE word inverse_rate3(unsigned\
    \ i) noexcept {\r\n    return twiddles.irate3[i];\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ unsigned twiddle_index(u32 block) noexcept {\r\n    return static_cast<unsigned>(std::countr_zero(~block));\r\
    \n}\r\n\r\n#if EEZ_NTT998_USE_AVX2\r\n\r\nusing vec = __m256i;\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ vec load8(const mint* p) noexcept {\r\n    return _mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(static_cast<const void*>(p)));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ void store8(mint* p, vec x) noexcept {\r\n    _mm256_storeu_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),\
    \ x);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec broadcast(word x) noexcept {\r\n\
    \    return _mm256_set1_epi32(static_cast<int>(x));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ vec add8(vec a, vec b) noexcept {\r\n    const vec two_p = broadcast(mod2);\r\
    \n    vec x = _mm256_sub_epi32(_mm256_add_epi32(a, b), two_p);\r\n    return _mm256_add_epi32(x,\
    \ _mm256_and_si256(_mm256_srai_epi32(x, 31), two_p));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ vec sub8(vec a, vec b) noexcept {\r\n    const vec two_p = broadcast(mod2);\r\
    \n    vec x = _mm256_sub_epi32(a, b);\r\n    return _mm256_add_epi32(x, _mm256_and_si256(_mm256_srai_epi32(x,\
    \ 31), two_p));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec mul8(vec a, vec b) noexcept\
    \ {\r\n    const vec ninv = broadcast(montgomery_ninv);\r\n    const vec prime\
    \ = broadcast(mod);\r\n    const vec product_even = _mm256_mul_epu32(a, b);\r\n\
    \    const vec product_odd = _mm256_mul_epu32(_mm256_bsrli_epi128(a, 4), _mm256_bsrli_epi128(b,\
    \ 4));\r\n    const vec q_even = _mm256_mul_epu32(product_even, ninv);\r\n   \
    \ const vec q_odd = _mm256_mul_epu32(product_odd, ninv);\r\n    const vec reduced_even\
    \ = _mm256_add_epi64(product_even, _mm256_mul_epu32(q_even, prime));\r\n    const\
    \ vec reduced_odd = _mm256_add_epi64(product_odd, _mm256_mul_epu32(q_odd, prime));\r\
    \n    return _mm256_or_si256(_mm256_bsrli_epi128(reduced_even, 4), reduced_odd);\r\
    \n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec mul8_fixed(vec a, vec b, vec bninv) noexcept\
    \ {\r\n    const vec prime = broadcast(mod);\r\n    const vec odd_a = _mm256_bsrli_epi128(a,\
    \ 4);\r\n    const vec product_even = _mm256_mul_epu32(a, b);\r\n    const vec\
    \ product_odd = _mm256_mul_epu32(odd_a, b);\r\n    const vec q_even = _mm256_mul_epu32(a,\
    \ bninv);\r\n    const vec q_odd = _mm256_mul_epu32(odd_a, bninv);\r\n    const\
    \ vec reduced_even = _mm256_add_epi64(product_even, _mm256_mul_epu32(q_even, prime));\r\
    \n    const vec reduced_odd = _mm256_add_epi64(product_odd, _mm256_mul_epu32(q_odd,\
    \ prime));\r\n    return _mm256_or_si256(_mm256_bsrli_epi128(reduced_even, 4),\
    \ reduced_odd);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec canonicalize8(vec x)\
    \ noexcept {\r\n    const vec prime = broadcast(mod);\r\n    vec y = _mm256_sub_epi32(x,\
    \ prime);\r\n    return _mm256_add_epi32(y, _mm256_and_si256(_mm256_srai_epi32(y,\
    \ 31), prime));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec pack_four(word x0, word\
    \ x1) noexcept {\r\n    return _mm256_setr_epi32(static_cast<int>(x0), static_cast<int>(x0),\
    \ static_cast<int>(x0), static_cast<int>(x0),\r\n                            \
    \ static_cast<int>(x1), static_cast<int>(x1), static_cast<int>(x1), static_cast<int>(x1));\r\
    \n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec load2x4(const mint* p0, const mint* p1)\
    \ noexcept {\r\n    const __m128i lo = _mm_loadu_si128(reinterpret_cast<const\
    \ __m128i*>(static_cast<const void*>(p0)));\r\n    const __m128i hi = _mm_loadu_si128(reinterpret_cast<const\
    \ __m128i*>(static_cast<const void*>(p1)));\r\n    return _mm256_set_m128i(hi,\
    \ lo);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void store2x4(mint* p0, mint* p1,\
    \ vec x) noexcept {\r\n    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p0)),\
    \ _mm256_castsi256_si128(x));\r\n    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p1)),\
    \ _mm256_extracti128_si256(x, 1));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void transpose_8x4_to_4x8(vec\
    \ v0, vec v1, vec v2, vec v3, vec& x0, vec& x1, vec& x2, vec& x3) noexcept {\r\
    \n    const vec t0 = _mm256_unpacklo_epi32(v0, v1);\r\n    const vec t1 = _mm256_unpackhi_epi32(v0,\
    \ v1);\r\n    const vec t2 = _mm256_unpacklo_epi32(v2, v3);\r\n    const vec t3\
    \ = _mm256_unpackhi_epi32(v2, v3);\r\n    const vec perm = _mm256_setr_epi32(0,\
    \ 4, 1, 5, 2, 6, 3, 7);\r\n    x0 = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t0,\
    \ t2), perm);\r\n    x1 = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t0,\
    \ t2), perm);\r\n    x2 = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t1,\
    \ t3), perm);\r\n    x3 = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t1,\
    \ t3), perm);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void transpose_4x8_to_8x4(vec\
    \ x0, vec x1, vec x2, vec x3, vec& v0, vec& v1, vec& v2, vec& v3) noexcept {\r\
    \n    const vec perm = _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7);\r\n    const\
    \ vec q0 = _mm256_permutevar8x32_epi32(x0, perm);\r\n    const vec q1 = _mm256_permutevar8x32_epi32(x1,\
    \ perm);\r\n    const vec q2 = _mm256_permutevar8x32_epi32(x2, perm);\r\n    const\
    \ vec q3 = _mm256_permutevar8x32_epi32(x3, perm);\r\n    const vec t0 = _mm256_unpacklo_epi64(q0,\
    \ q1);\r\n    const vec t2 = _mm256_unpackhi_epi64(q0, q1);\r\n    const vec t1\
    \ = _mm256_unpacklo_epi64(q2, q3);\r\n    const vec t3 = _mm256_unpackhi_epi64(q2,\
    \ q3);\r\n    v0 = _mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),\
    \ _mm256_castsi256_ps(t1), _MM_SHUFFLE(2, 0, 2, 0)));\r\n    v1 = _mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),\
    \ _mm256_castsi256_ps(t1), _MM_SHUFFLE(3, 1, 3, 1)));\r\n    v2 = _mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),\
    \ _mm256_castsi256_ps(t3), _MM_SHUFFLE(2, 0, 2, 0)));\r\n    v3 = _mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),\
    \ _mm256_castsi256_ps(t3), _MM_SHUFFLE(3, 1, 3, 1)));\r\n}\r\n\r\n#endif\r\n\r\
    \nEEZ_NTT998_ALWAYS_INLINE void forward_butterfly(mint* b, usize stride, usize\
    \ i, word r1, word r2, word r3) noexcept {\r\n    const word x0 = raw(b[i]);\r\
    \n    const word x1 = mul(raw(b[stride + i]), r1);\r\n    const word x2 = mul(raw(b[2\
    \ * stride + i]), r2);\r\n    const word x3 = mul(raw(b[3 * stride + i]), r3);\r\
    \n    const word s02 = add(x0, x2);\r\n    const word d02 = sub(x0, x2);\r\n \
    \   const word s13 = add(x1, x3);\r\n    const word t = mul(sub(x1, x3), twiddles.root[2]);\r\
    \n    b[i] = from_raw(add(s02, s13));\r\n    b[stride + i] = from_raw(sub(s02,\
    \ s13));\r\n    b[2 * stride + i] = from_raw(add(d02, t));\r\n    b[3 * stride\
    \ + i] = from_raw(sub(d02, t));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void inverse_butterfly(mint*\
    \ b, usize stride, usize i, word r1, word r2, word r3) noexcept {\r\n    const\
    \ word x0 = raw(b[i]);\r\n    const word x1 = raw(b[stride + i]);\r\n    const\
    \ word x2 = raw(b[2 * stride + i]);\r\n    const word x3 = raw(b[3 * stride +\
    \ i]);\r\n    const word s01 = add(x0, x1);\r\n    const word d01 = sub(x0, x1);\r\
    \n    const word s23 = add(x2, x3);\r\n    const word t = mul(sub(x2, x3), twiddles.iroot[2]);\r\
    \n    b[i] = from_raw(add(s01, s23));\r\n    b[stride + i] = from_raw(mul(add(d01,\
    \ t), r1));\r\n    b[2 * stride + i] = from_raw(mul(sub(s01, s23), r2));\r\n \
    \   b[3 * stride + i] = from_raw(mul(sub(d01, t), r3));\r\n}\r\n\r\ninline void\
    \ forward_radix4_scalar(mint* EEZ_NTT998_RESTRICT a, usize blocks, usize stride)\
    \ noexcept {\r\n    {\r\n        mint* const b = a;\r\n        for(usize i = 0;\
    \ i < stride; ++i){\r\n            const word x0 = raw(b[i]);\r\n            const\
    \ word x1 = raw(b[stride + i]);\r\n            const word x2 = raw(b[2 * stride\
    \ + i]);\r\n            const word x3 = raw(b[3 * stride + i]);\r\n          \
    \  const word s02 = add(x0, x2);\r\n            const word d02 = sub(x0, x2);\r\
    \n            const word s13 = add(x1, x3);\r\n            const word t = mul(sub(x1,\
    \ x3), twiddles.root[2]);\r\n            b[i] = from_raw(add(s02, s13));\r\n \
    \           b[stride + i] = from_raw(sub(s02, s13));\r\n            b[2 * stride\
    \ + i] = from_raw(add(d02, t));\r\n            b[3 * stride + i] = from_raw(sub(d02,\
    \ t));\r\n        }\r\n    }\r\n    if(blocks == 1) return;\r\n    word rot =\
    \ forward_rate3(0);\r\n    for(usize s = 1; s < blocks; ++s){\r\n        const\
    \ word rot2 = mul(rot, rot);\r\n        const word rot3 = mul(rot2, rot);\r\n\
    \        mint* const b = a + s * 4 * stride;\r\n        for(usize i = 0; i < stride;\
    \ ++i) forward_butterfly(b, stride, i, rot, rot2, rot3);\r\n        if(s + 1 <\
    \ blocks) rot = mul(rot, forward_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\n\r\ninline void inverse_radix4_scalar(mint* EEZ_NTT998_RESTRICT\
    \ a, usize blocks, usize stride) noexcept {\r\n    {\r\n        mint* const b\
    \ = a;\r\n        for(usize i = 0; i < stride; ++i){\r\n            const word\
    \ x0 = raw(b[i]);\r\n            const word x1 = raw(b[stride + i]);\r\n     \
    \       const word x2 = raw(b[2 * stride + i]);\r\n            const word x3 =\
    \ raw(b[3 * stride + i]);\r\n            const word s01 = add(x0, x1);\r\n   \
    \         const word d01 = sub(x0, x1);\r\n            const word s23 = add(x2,\
    \ x3);\r\n            const word t = mul(sub(x2, x3), twiddles.iroot[2]);\r\n\
    \            b[i] = from_raw(add(s01, s23));\r\n            b[stride + i] = from_raw(add(d01,\
    \ t));\r\n            b[2 * stride + i] = from_raw(sub(s01, s23));\r\n       \
    \     b[3 * stride + i] = from_raw(sub(d01, t));\r\n        }\r\n    }\r\n   \
    \ if(blocks == 1) return;\r\n    word rot = inverse_rate3(0);\r\n    for(usize\
    \ s = 1; s < blocks; ++s){\r\n        const word rot2 = mul(rot, rot);\r\n   \
    \     const word rot3 = mul(rot2, rot);\r\n        mint* const b = a + s * 4 *\
    \ stride;\r\n        for(usize i = 0; i < stride; ++i) inverse_butterfly(b, stride,\
    \ i, rot, rot2, rot3);\r\n        if(s + 1 < blocks) rot = mul(rot, inverse_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\n\r\n#if EEZ_NTT998_USE_AVX2\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void\
    \ forward_radix4_large_block(mint* EEZ_NTT998_RESTRICT b, usize stride, vec imag,\
    \ word r1, word r2, word r3) noexcept {\r\n    const vec w1 = broadcast(r1);\r\
    \n    const vec w2 = broadcast(r2);\r\n    const vec w3 = broadcast(r3);\r\n \
    \   for(usize i = 0; i < stride; i += 8){\r\n        const vec x0 = load8(b +\
    \ i);\r\n        const vec x1 = mul8(load8(b + stride + i), w1);\r\n        const\
    \ vec x2 = mul8(load8(b + 2 * stride + i), w2);\r\n        const vec x3 = mul8(load8(b\
    \ + 3 * stride + i), w3);\r\n        const vec s02 = add8(x0, x2);\r\n       \
    \ const vec d02 = sub8(x0, x2);\r\n        const vec s13 = add8(x1, x3);\r\n \
    \       const vec t = mul8(sub8(x1, x3), imag);\r\n        store8(b + i, add8(s02,\
    \ s13));\r\n        store8(b + stride + i, sub8(s02, s13));\r\n        store8(b\
    \ + 2 * stride + i, add8(d02, t));\r\n        store8(b + 3 * stride + i, sub8(d02,\
    \ t));\r\n    }\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void inverse_radix4_large_block(mint*\
    \ EEZ_NTT998_RESTRICT b, usize stride, vec iimag, word r1, word r2, word r3) noexcept\
    \ {\r\n    const vec w1 = broadcast(r1);\r\n    const vec w2 = broadcast(r2);\r\
    \n    const vec w3 = broadcast(r3);\r\n    for(usize i = 0; i < stride; i += 8){\r\
    \n        const vec x0 = load8(b + i);\r\n        const vec x1 = load8(b + stride\
    \ + i);\r\n        const vec x2 = load8(b + 2 * stride + i);\r\n        const\
    \ vec x3 = load8(b + 3 * stride + i);\r\n        const vec s01 = add8(x0, x1);\r\
    \n        const vec d01 = sub8(x0, x1);\r\n        const vec s23 = add8(x2, x3);\r\
    \n        const vec t = mul8(sub8(x2, x3), iimag);\r\n        store8(b + i, add8(s01,\
    \ s23));\r\n        store8(b + stride + i, mul8(add8(d01, t), w1));\r\n      \
    \  store8(b + 2 * stride + i, mul8(sub8(s01, s23), w2));\r\n        store8(b +\
    \ 3 * stride + i, mul8(sub8(d01, t), w3));\r\n    }\r\n}\r\n\r\ninline void forward_radix4_large(mint*\
    \ EEZ_NTT998_RESTRICT a, usize blocks, usize stride) noexcept {\r\n    const vec\
    \ imag = broadcast(twiddles.root[2]);\r\n    {\r\n        mint* const b = a;\r\
    \n        for(usize i = 0; i < stride; i += 8){\r\n            const vec x0 =\
    \ load8(b + i);\r\n            const vec x1 = load8(b + stride + i);\r\n     \
    \       const vec x2 = load8(b + 2 * stride + i);\r\n            const vec x3\
    \ = load8(b + 3 * stride + i);\r\n            const vec s02 = add8(x0, x2);\r\n\
    \            const vec d02 = sub8(x0, x2);\r\n            const vec s13 = add8(x1,\
    \ x3);\r\n            const vec t = mul8(sub8(x1, x3), imag);\r\n            store8(b\
    \ + i, add8(s02, s13));\r\n            store8(b + stride + i, sub8(s02, s13));\r\
    \n            store8(b + 2 * stride + i, add8(d02, t));\r\n            store8(b\
    \ + 3 * stride + i, sub8(d02, t));\r\n        }\r\n    }\r\n    if(blocks == 1)\
    \ return;\r\n    word rot = forward_rate3(0);\r\n    usize s = 1;\r\n    for(;\
    \ s + 8 <= blocks; s += 8){\r\n        alignas(32) word r1[8], r2[8], r3[8];\r\
    \n        for(unsigned lane = 0; lane < 8; ++lane){\r\n            r1[lane] =\
    \ rot;\r\n            if(s + lane + 1 < blocks) rot = mul(rot, forward_rate3(twiddle_index(static_cast<u32>(s\
    \ + lane))));\r\n        }\r\n        const vec w1 = _mm256_load_si256(reinterpret_cast<const\
    \ vec*>(r1));\r\n        const vec w2 = mul8(w1, w1);\r\n        const vec w3\
    \ = mul8(w2, w1);\r\n        _mm256_store_si256(reinterpret_cast<vec*>(r2), w2);\r\
    \n        _mm256_store_si256(reinterpret_cast<vec*>(r3), w3);\r\n        for(unsigned\
    \ lane = 0; lane < 8; ++lane)\r\n            forward_radix4_large_block(a + (s\
    \ + lane) * 4 * stride, stride, imag, r1[lane], r2[lane], r3[lane]);\r\n    }\r\
    \n    for(; s < blocks; ++s){\r\n        const word rot2 = mul(rot, rot);\r\n\
    \        const word rot3 = mul(rot2, rot);\r\n        forward_radix4_large_block(a\
    \ + s * 4 * stride, stride, imag, rot, rot2, rot3);\r\n        if(s + 1 < blocks)\
    \ rot = mul(rot, forward_rate3(twiddle_index(static_cast<u32>(s))));\r\n    }\r\
    \n}\r\n\r\ninline void inverse_radix4_large(mint* EEZ_NTT998_RESTRICT a, usize\
    \ blocks, usize stride) noexcept {\r\n    const vec iimag = broadcast(twiddles.iroot[2]);\r\
    \n    {\r\n        mint* const b = a;\r\n        for(usize i = 0; i < stride;\
    \ i += 8){\r\n            const vec x0 = load8(b + i);\r\n            const vec\
    \ x1 = load8(b + stride + i);\r\n            const vec x2 = load8(b + 2 * stride\
    \ + i);\r\n            const vec x3 = load8(b + 3 * stride + i);\r\n         \
    \   const vec s01 = add8(x0, x1);\r\n            const vec d01 = sub8(x0, x1);\r\
    \n            const vec s23 = add8(x2, x3);\r\n            const vec t = mul8(sub8(x2,\
    \ x3), iimag);\r\n            store8(b + i, add8(s01, s23));\r\n            store8(b\
    \ + stride + i, add8(d01, t));\r\n            store8(b + 2 * stride + i, sub8(s01,\
    \ s23));\r\n            store8(b + 3 * stride + i, sub8(d01, t));\r\n        }\r\
    \n    }\r\n    if(blocks == 1) return;\r\n    word rot = inverse_rate3(0);\r\n\
    \    usize s = 1;\r\n    for(; s + 8 <= blocks; s += 8){\r\n        alignas(32)\
    \ word r1[8], r2[8], r3[8];\r\n        for(unsigned lane = 0; lane < 8; ++lane){\r\
    \n            r1[lane] = rot;\r\n            if(s + lane + 1 < blocks) rot = mul(rot,\
    \ inverse_rate3(twiddle_index(static_cast<u32>(s + lane))));\r\n        }\r\n\
    \        const vec w1 = _mm256_load_si256(reinterpret_cast<const vec*>(r1));\r\
    \n        const vec w2 = mul8(w1, w1);\r\n        const vec w3 = mul8(w2, w1);\r\
    \n        _mm256_store_si256(reinterpret_cast<vec*>(r2), w2);\r\n        _mm256_store_si256(reinterpret_cast<vec*>(r3),\
    \ w3);\r\n        for(unsigned lane = 0; lane < 8; ++lane)\r\n            inverse_radix4_large_block(a\
    \ + (s + lane) * 4 * stride, stride, iimag, r1[lane], r2[lane], r3[lane]);\r\n\
    \    }\r\n    for(; s < blocks; ++s){\r\n        const word rot2 = mul(rot, rot);\r\
    \n        const word rot3 = mul(rot2, rot);\r\n        inverse_radix4_large_block(a\
    \ + s * 4 * stride, stride, iimag, rot, rot2, rot3);\r\n        if(s + 1 < blocks)\
    \ rot = mul(rot, inverse_rate3(twiddle_index(static_cast<u32>(s))));\r\n    }\r\
    \n}\r\n\r\ninline void forward_radix4_p4(mint* EEZ_NTT998_RESTRICT a, usize blocks)\
    \ noexcept {\r\n    if(blocks < 2){\r\n        forward_radix4_scalar(a, blocks,\
    \ 4);\r\n        return;\r\n    }\r\n    const vec imag = broadcast(twiddles.root[2]);\r\
    \n    word rot = montgomery_one;\r\n    for(usize s = 0; s < blocks; s += 2){\r\
    \n        const word r10 = rot;\r\n        rot = mul(rot, forward_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n        const word r11 = rot;\r\n        const vec w1 = pack_four(r10, r11);\r\
    \n        const vec w2 = mul8(w1, w1);\r\n        const vec w3 = mul8(w2, w1);\r\
    \n        mint* const b0 = a + s * 16;\r\n        mint* const b1 = b0 + 16;\r\n\
    \        const vec x0 = load2x4(b0, b1);\r\n        const vec x1 = mul8(load2x4(b0\
    \ + 4, b1 + 4), w1);\r\n        const vec x2 = mul8(load2x4(b0 + 8, b1 + 8), w2);\r\
    \n        const vec x3 = mul8(load2x4(b0 + 12, b1 + 12), w3);\r\n        const\
    \ vec s02 = add8(x0, x2);\r\n        const vec d02 = sub8(x0, x2);\r\n       \
    \ const vec s13 = add8(x1, x3);\r\n        const vec t = mul8(sub8(x1, x3), imag);\r\
    \n        store2x4(b0, b1, add8(s02, s13));\r\n        store2x4(b0 + 4, b1 + 4,\
    \ sub8(s02, s13));\r\n        store2x4(b0 + 8, b1 + 8, add8(d02, t));\r\n    \
    \    store2x4(b0 + 12, b1 + 12, sub8(d02, t));\r\n        if(s + 2 < blocks) rot\
    \ = mul(rot, forward_rate3(twiddle_index(static_cast<u32>(s + 1))));\r\n    }\r\
    \n}\r\n\r\ninline void inverse_radix4_p4(mint* EEZ_NTT998_RESTRICT a, usize blocks)\
    \ noexcept {\r\n    if(blocks < 2){\r\n        inverse_radix4_scalar(a, blocks,\
    \ 4);\r\n        return;\r\n    }\r\n    const vec iimag = broadcast(twiddles.iroot[2]);\r\
    \n    word rot = montgomery_one;\r\n    for(usize s = 0; s < blocks; s += 2){\r\
    \n        const word r10 = rot;\r\n        rot = mul(rot, inverse_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n        const word r11 = rot;\r\n        const vec w1 = pack_four(r10, r11);\r\
    \n        const vec w2 = mul8(w1, w1);\r\n        const vec w3 = mul8(w2, w1);\r\
    \n        mint* const b0 = a + s * 16;\r\n        mint* const b1 = b0 + 16;\r\n\
    \        const vec x0 = load2x4(b0, b1);\r\n        const vec x1 = load2x4(b0\
    \ + 4, b1 + 4);\r\n        const vec x2 = load2x4(b0 + 8, b1 + 8);\r\n       \
    \ const vec x3 = load2x4(b0 + 12, b1 + 12);\r\n        const vec s01 = add8(x0,\
    \ x1);\r\n        const vec d01 = sub8(x0, x1);\r\n        const vec s23 = add8(x2,\
    \ x3);\r\n        const vec t = mul8(sub8(x2, x3), iimag);\r\n        store2x4(b0,\
    \ b1, add8(s01, s23));\r\n        store2x4(b0 + 4, b1 + 4, mul8(add8(d01, t),\
    \ w1));\r\n        store2x4(b0 + 8, b1 + 8, mul8(sub8(s01, s23), w2));\r\n   \
    \     store2x4(b0 + 12, b1 + 12, mul8(sub8(d01, t), w3));\r\n        if(s + 2\
    \ < blocks) rot = mul(rot, inverse_rate3(twiddle_index(static_cast<u32>(s + 1))));\r\
    \n    }\r\n}\r\n\r\ninline void forward_radix4_p1(mint* EEZ_NTT998_RESTRICT a,\
    \ usize blocks) noexcept {\r\n    const vec imag = broadcast(twiddles.root[2]);\r\
    \n    word rot = montgomery_one;\r\n    usize s = 0;\r\n    for(; s + 8 <= blocks;\
    \ s += 8){\r\n        alignas(32) word r1[8];\r\n        for(unsigned lane = 0;\
    \ lane < 8; ++lane){\r\n            r1[lane] = rot;\r\n            if(s + lane\
    \ + 1 < blocks) rot = mul(rot, forward_rate3(twiddle_index(static_cast<u32>(s\
    \ + lane))));\r\n        }\r\n        const vec w1 = _mm256_load_si256(reinterpret_cast<const\
    \ vec*>(r1));\r\n        const vec w2 = mul8(w1, w1);\r\n        const vec w3\
    \ = mul8(w2, w1);\r\n        mint* const b = a + 4 * s;\r\n        vec x0, x1,\
    \ x2, x3;\r\n        transpose_8x4_to_4x8(load8(b), load8(b + 8), load8(b + 16),\
    \ load8(b + 24), x0, x1, x2, x3);\r\n        x1 = mul8(x1, w1);\r\n        x2\
    \ = mul8(x2, w2);\r\n        x3 = mul8(x3, w3);\r\n        const vec s02 = add8(x0,\
    \ x2);\r\n        const vec d02 = sub8(x0, x2);\r\n        const vec s13 = add8(x1,\
    \ x3);\r\n        const vec t = mul8(sub8(x1, x3), imag);\r\n        vec v0, v1,\
    \ v2, v3;\r\n        transpose_4x8_to_8x4(add8(s02, s13), sub8(s02, s13), add8(d02,\
    \ t), sub8(d02, t), v0, v1, v2, v3);\r\n        store8(b, v0);\r\n        store8(b\
    \ + 8, v1);\r\n        store8(b + 16, v2);\r\n        store8(b + 24, v3);\r\n\
    \    }\r\n    for(; s < blocks; ++s){\r\n        const word rot2 = mul(rot, rot);\r\
    \n        const word rot3 = mul(rot2, rot);\r\n        forward_butterfly(a + 4\
    \ * s, 1, 0, rot, rot2, rot3);\r\n        if(s + 1 < blocks) rot = mul(rot, forward_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\n\r\ninline void inverse_radix4_p1(mint* EEZ_NTT998_RESTRICT a,\
    \ usize blocks) noexcept {\r\n    const vec iimag = broadcast(twiddles.iroot[2]);\r\
    \n    word rot = montgomery_one;\r\n    usize s = 0;\r\n    for(; s + 8 <= blocks;\
    \ s += 8){\r\n        alignas(32) word r1[8];\r\n        for(unsigned lane = 0;\
    \ lane < 8; ++lane){\r\n            r1[lane] = rot;\r\n            if(s + lane\
    \ + 1 < blocks) rot = mul(rot, inverse_rate3(twiddle_index(static_cast<u32>(s\
    \ + lane))));\r\n        }\r\n        const vec w1 = _mm256_load_si256(reinterpret_cast<const\
    \ vec*>(r1));\r\n        const vec w2 = mul8(w1, w1);\r\n        const vec w3\
    \ = mul8(w2, w1);\r\n        mint* const b = a + 4 * s;\r\n        vec x0, x1,\
    \ x2, x3;\r\n        transpose_8x4_to_4x8(load8(b), load8(b + 8), load8(b + 16),\
    \ load8(b + 24), x0, x1, x2, x3);\r\n        const vec s01 = add8(x0, x1);\r\n\
    \        const vec d01 = sub8(x0, x1);\r\n        const vec s23 = add8(x2, x3);\r\
    \n        const vec t = mul8(sub8(x2, x3), iimag);\r\n        vec v0, v1, v2,\
    \ v3;\r\n        transpose_4x8_to_8x4(add8(s01, s23), mul8(add8(d01, t), w1),\
    \ mul8(sub8(s01, s23), w2), mul8(sub8(d01, t), w3), v0, v1, v2, v3);\r\n     \
    \   store8(b, v0);\r\n        store8(b + 8, v1);\r\n        store8(b + 16, v2);\r\
    \n        store8(b + 24, v3);\r\n    }\r\n    for(; s < blocks; ++s){\r\n    \
    \    const word rot2 = mul(rot, rot);\r\n        const word rot3 = mul(rot2, rot);\r\
    \n        inverse_butterfly(a + 4 * s, 1, 0, rot, rot2, rot3);\r\n        if(s\
    \ + 1 < blocks) rot = mul(rot, inverse_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\n\r\n#endif\r\n\r\ninline void forward_radix2_first(mint* EEZ_NTT998_RESTRICT\
    \ a, usize n) noexcept {\r\n    const usize half = n >> 1;\r\n    usize i = 0;\r\
    \n#if EEZ_NTT998_USE_AVX2\r\n    for(; i + 8 <= half; i += 8){\r\n        const\
    \ vec x = load8(a + i);\r\n        const vec y = load8(a + half + i);\r\n    \
    \    store8(a + i, add8(x, y));\r\n        store8(a + half + i, sub8(x, y));\r\
    \n    }\r\n#endif\r\n    for(; i < half; ++i){\r\n        const word x = raw(a[i]);\r\
    \n        const word y = raw(a[half + i]);\r\n        a[i] = from_raw(add(x, y));\r\
    \n        a[half + i] = from_raw(sub(x, y));\r\n    }\r\n}\r\n\r\ninline void\
    \ forward_radix4_stage(mint* EEZ_NTT998_RESTRICT a, usize n, int stage) noexcept\
    \ {\r\n    const int h = static_cast<int>(std::countr_zero(n));\r\n    assert(stage\
    \ >= 0 && stage + 2 <= h);\r\n    const usize stride = usize(1) << (h - stage\
    \ - 2);\r\n    const usize blocks = usize(1) << stage;\r\n#if EEZ_NTT998_USE_AVX2\r\
    \n    if(stride >= 8) forward_radix4_large(a, blocks, stride);\r\n    else if(stride\
    \ == 4) forward_radix4_p4(a, blocks);\r\n    else if(stride == 1) forward_radix4_p1(a,\
    \ blocks);\r\n    else forward_radix4_scalar(a, blocks, stride);\r\n#else\r\n\
    \    forward_radix4_scalar(a, blocks, stride);\r\n#endif\r\n}\r\n\r\ninline void\
    \ inverse_radix4_stage(mint* EEZ_NTT998_RESTRICT a, usize n, int stage) noexcept\
    \ {\r\n    const int h = static_cast<int>(std::countr_zero(n));\r\n    assert(stage\
    \ >= 0 && stage + 2 <= h);\r\n    const usize stride = usize(1) << (h - stage\
    \ - 2);\r\n    const usize blocks = usize(1) << stage;\r\n#if EEZ_NTT998_USE_AVX2\r\
    \n    if(stride >= 8) inverse_radix4_large(a, blocks, stride);\r\n    else if(stride\
    \ == 4) inverse_radix4_p4(a, blocks);\r\n    else if(stride == 1) inverse_radix4_p1(a,\
    \ blocks);\r\n    else inverse_radix4_scalar(a, blocks, stride);\r\n#else\r\n\
    \    inverse_radix4_scalar(a, blocks, stride);\r\n#endif\r\n}\r\n\r\ninline void\
    \ final_radix2_scale(mint* EEZ_NTT998_RESTRICT a, usize n, word scale_mont) noexcept\
    \ {\r\n    const usize half = n >> 1;\r\n    usize i = 0;\r\n#if EEZ_NTT998_USE_AVX2\r\
    \n    const vec scale = broadcast(scale_mont);\r\n    for(; i + 8 <= half; i +=\
    \ 8){\r\n        const vec x = load8(a + i);\r\n        const vec y = load8(a\
    \ + half + i);\r\n        store8(a + i, mul8(add8(x, y), scale));\r\n        store8(a\
    \ + half + i, mul8(sub8(x, y), scale));\r\n    }\r\n#endif\r\n    for(; i < half;\
    \ ++i){\r\n        const word x = raw(a[i]);\r\n        const word y = raw(a[half\
    \ + i]);\r\n        a[i] = from_raw(mul(add(x, y), scale_mont));\r\n        a[half\
    \ + i] = from_raw(mul(sub(x, y), scale_mont));\r\n    }\r\n}\r\n\r\ninline void\
    \ final_radix4_scale(mint* EEZ_NTT998_RESTRICT a, usize n, word scale_mont) noexcept\
    \ {\r\n    const usize stride = n >> 2;\r\n    usize i = 0;\r\n#if EEZ_NTT998_USE_AVX2\r\
    \n    const vec iimag = broadcast(twiddles.iroot[2]);\r\n    const vec scale =\
    \ broadcast(scale_mont);\r\n    for(; i + 8 <= stride; i += 8){\r\n        const\
    \ vec x0 = load8(a + i);\r\n        const vec x1 = load8(a + stride + i);\r\n\
    \        const vec x2 = load8(a + 2 * stride + i);\r\n        const vec x3 = load8(a\
    \ + 3 * stride + i);\r\n        const vec s01 = add8(x0, x1);\r\n        const\
    \ vec d01 = sub8(x0, x1);\r\n        const vec s23 = add8(x2, x3);\r\n       \
    \ const vec t = mul8(sub8(x2, x3), iimag);\r\n        store8(a + i, mul8(add8(s01,\
    \ s23), scale));\r\n        store8(a + stride + i, mul8(add8(d01, t), scale));\r\
    \n        store8(a + 2 * stride + i, mul8(sub8(s01, s23), scale));\r\n       \
    \ store8(a + 3 * stride + i, mul8(sub8(d01, t), scale));\r\n    }\r\n#endif\r\n\
    \    for(; i < stride; ++i){\r\n        const word x0 = raw(a[i]);\r\n       \
    \ const word x1 = raw(a[stride + i]);\r\n        const word x2 = raw(a[2 * stride\
    \ + i]);\r\n        const word x3 = raw(a[3 * stride + i]);\r\n        const word\
    \ s01 = add(x0, x1);\r\n        const word d01 = sub(x0, x1);\r\n        const\
    \ word s23 = add(x2, x3);\r\n        const word t = mul(sub(x2, x3), twiddles.iroot[2]);\r\
    \n        a[i] = from_raw(mul(add(s01, s23), scale_mont));\r\n        a[stride\
    \ + i] = from_raw(mul(add(d01, t), scale_mont));\r\n        a[2 * stride + i]\
    \ = from_raw(mul(sub(s01, s23), scale_mont));\r\n        a[3 * stride + i] = from_raw(mul(sub(d01,\
    \ t), scale_mont));\r\n    }\r\n}\r\n\r\ninline void forward_dif(mint* EEZ_NTT998_RESTRICT\
    \ a, usize n) noexcept {\r\n    if(n <= 1) return;\r\n    const int h = static_cast<int>(std::countr_zero(n));\r\
    \n    int stage = 0;\r\n    if(h & 1){\r\n        forward_radix2_first(a, n);\r\
    \n        stage = 1;\r\n    }\r\n    for(; stage < h; stage += 2) forward_radix4_stage(a,\
    \ n, stage);\r\n}\r\n\r\ninline void inverse_dit(mint* EEZ_NTT998_RESTRICT a,\
    \ usize n) noexcept {\r\n    if(n <= 1) return;\r\n    const int h = static_cast<int>(std::countr_zero(n));\r\
    \n    const word scale = mint::raw(static_cast<u32>(n)).inv().a;\r\n    if(h &\
    \ 1){\r\n        for(int stage = h - 2; stage >= 1; stage -= 2) inverse_radix4_stage(a,\
    \ n, stage);\r\n        final_radix2_scale(a, n, scale);\r\n    }else{\r\n   \
    \     for(int stage = h - 2; stage >= 2; stage -= 2) inverse_radix4_stage(a, n,\
    \ stage);\r\n        final_radix4_scale(a, n, scale);\r\n    }\r\n}\r\n\r\n#if\
    \ EEZ_NTT998_USE_AVX2\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec load8_aligned(const\
    \ mint* p) noexcept {\r\n    return _mm256_load_si256(reinterpret_cast<const __m256i*>(static_cast<const\
    \ void*>(p)));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void store8_aligned(mint*\
    \ p, vec x) noexcept {\r\n    _mm256_store_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),\
    \ x);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec shrink4_to_2(vec x) noexcept {\r\
    \n    return _mm256_min_epu32(x, _mm256_sub_epi32(x, broadcast(mod2)));\r\n}\r\
    \n\r\nEEZ_NTT998_ALWAYS_INLINE vec lazy_add8(vec a, vec b) noexcept {\r\n    return\
    \ _mm256_add_epi32(a, b);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec lazy_sub8(vec\
    \ a, vec b) noexcept {\r\n    return _mm256_add_epi32(a, _mm256_sub_epi32(broadcast(mod2),\
    \ b));\r\n}\r\n\r\ntemplate<bool trivial_twiddle>\r\ninline void forward_radix4_block_lazy(mint*\
    \ b, usize stride, word r1) noexcept {\r\n    const word imag = canonicalize(twiddles.root[2]);\r\
    \n    const vec vimag = broadcast(imag);\r\n    const vec vimag_ninv = broadcast(imag\
    \ * montgomery_ninv);\r\n    const vec vr1 = broadcast(r1);\r\n    const vec vr1_ninv\
    \ = broadcast(r1 * montgomery_ninv);\r\n    const word r2 = canonicalize(mul(r1,\
    \ r1));\r\n    const vec vr2 = broadcast(r2);\r\n    const vec vr2_ninv = broadcast(r2\
    \ * montgomery_ninv);\r\n    const word r3 = canonicalize(mul(r2, r1));\r\n  \
    \  const vec vr3 = broadcast(r3);\r\n    const vec vr3_ninv = broadcast(r3 * montgomery_ninv);\r\
    \n\r\n    for(usize i = 0; i < stride; i += 8){\r\n        vec x0 = shrink4_to_2(load8_aligned(b\
    \ + i));\r\n        vec x1 = load8_aligned(b + stride + i);\r\n        vec x2\
    \ = load8_aligned(b + 2 * stride + i);\r\n        vec x3 = load8_aligned(b + 3\
    \ * stride + i);\r\n        if constexpr(!trivial_twiddle){\r\n            x1\
    \ = mul8_fixed(x1, vr1, vr1_ninv);\r\n            x2 = mul8_fixed(x2, vr2, vr2_ninv);\r\
    \n            x3 = mul8_fixed(x3, vr3, vr3_ninv);\r\n        }else{\r\n      \
    \      x1 = shrink4_to_2(x1);\r\n            x2 = shrink4_to_2(x2);\r\n      \
    \      x3 = shrink4_to_2(x3);\r\n        }\r\n\r\n        vec s02 = lazy_add8(x0,\
    \ x2);\r\n        vec d02 = lazy_sub8(x0, x2);\r\n        vec s13 = lazy_add8(x1,\
    \ x3);\r\n        const vec t = mul8_fixed(lazy_sub8(x1, x3), vimag, vimag_ninv);\r\
    \n        s02 = shrink4_to_2(s02);\r\n        d02 = shrink4_to_2(d02);\r\n   \
    \     s13 = shrink4_to_2(s13);\r\n\r\n        store8_aligned(b + i, lazy_add8(s02,\
    \ s13));\r\n        store8_aligned(b + stride + i, lazy_sub8(s02, s13));\r\n \
    \       store8_aligned(b + 2 * stride + i, lazy_add8(d02, t));\r\n        store8_aligned(b\
    \ + 3 * stride + i, lazy_sub8(d02, t));\r\n    }\r\n}\r\n\r\ntemplate<bool trivial_twiddle>\r\
    \nEEZ_NTT998_ALWAYS_INLINE void forward_radix4_block_pair_lazy(mint* EEZ_NTT998_RESTRICT\
    \ a, mint* EEZ_NTT998_RESTRICT b, usize stride, word r1) noexcept {\r\n    forward_radix4_block_lazy<trivial_twiddle>(a,\
    \ stride, r1);\r\n    forward_radix4_block_lazy<trivial_twiddle>(b, stride, r1);\r\
    \n}\r\n\r\ntemplate<bool trivial_twiddle, bool apply_scale>\r\ninline void inverse_radix4_block_lazy(mint*\
    \ b, usize stride, word r1, word scale) noexcept {\r\n    const word iimag = canonicalize(twiddles.iroot[2]);\r\
    \n    const vec viimag = broadcast(iimag);\r\n    const vec viimag_ninv = broadcast(iimag\
    \ * montgomery_ninv);\r\n    const vec vr1 = broadcast(r1);\r\n    const vec vr1_ninv\
    \ = broadcast(r1 * montgomery_ninv);\r\n    const word r2 = canonicalize(mul(r1,\
    \ r1));\r\n    const vec vr2 = broadcast(r2);\r\n    const vec vr2_ninv = broadcast(r2\
    \ * montgomery_ninv);\r\n    const word r3 = canonicalize(mul(r2, r1));\r\n  \
    \  const vec vr3 = broadcast(r3);\r\n    const vec vr3_ninv = broadcast(r3 * montgomery_ninv);\r\
    \n    const word scale_canonical = canonicalize(scale);\r\n\r\n    for(usize i\
    \ = 0; i < stride; i += 8){\r\n        const vec x0 = shrink4_to_2(load8_aligned(b\
    \ + i));\r\n        const vec x1 = shrink4_to_2(load8_aligned(b + stride + i));\r\
    \n        const vec x2 = shrink4_to_2(load8_aligned(b + 2 * stride + i));\r\n\
    \        const vec x3 = shrink4_to_2(load8_aligned(b + 3 * stride + i));\r\n\r\
    \n        vec s01 = lazy_add8(x0, x1);\r\n        vec d01 = lazy_sub8(x0, x1);\r\
    \n        vec s23 = lazy_add8(x2, x3);\r\n        const vec t = mul8_fixed(lazy_sub8(x2,\
    \ x3), viimag, viimag_ninv);\r\n        s01 = shrink4_to_2(s01);\r\n        d01\
    \ = shrink4_to_2(d01);\r\n        s23 = shrink4_to_2(s23);\r\n\r\n        vec\
    \ y0 = lazy_add8(s01, s23);\r\n        vec y1 = lazy_add8(d01, t);\r\n       \
    \ vec y2 = lazy_sub8(s01, s23);\r\n        vec y3 = lazy_sub8(d01, t);\r\n\r\n\
    \        if constexpr(apply_scale){\r\n            const word s0 = scale_canonical;\r\
    \n            const word s1 = trivial_twiddle ? s0 : canonicalize(mul(s0, r1));\r\
    \n            const word s2 = trivial_twiddle ? s0 : canonicalize(mul(s0, r2));\r\
    \n            const word s3 = trivial_twiddle ? s0 : canonicalize(mul(s0, r3));\r\
    \n            y0 = mul8_fixed(y0, broadcast(s0), broadcast(s0 * montgomery_ninv));\r\
    \n            y1 = mul8_fixed(y1, broadcast(s1), broadcast(s1 * montgomery_ninv));\r\
    \n            y2 = mul8_fixed(y2, broadcast(s2), broadcast(s2 * montgomery_ninv));\r\
    \n            y3 = mul8_fixed(y3, broadcast(s3), broadcast(s3 * montgomery_ninv));\r\
    \n        }else if constexpr(!trivial_twiddle){\r\n            y1 = mul8_fixed(y1,\
    \ vr1, vr1_ninv);\r\n            y2 = mul8_fixed(y2, vr2, vr2_ninv);\r\n     \
    \       y3 = mul8_fixed(y3, vr3, vr3_ninv);\r\n        }\r\n\r\n        store8_aligned(b\
    \ + i, y0);\r\n        store8_aligned(b + stride + i, y1);\r\n        store8_aligned(b\
    \ + 2 * stride + i, y2);\r\n        store8_aligned(b + 3 * stride + i, y3);\r\n\
    \    }\r\n}\r\n\r\ninline unsigned adaptive_leaf_log(usize n) noexcept {\r\n \
    \   const unsigned h = static_cast<unsigned>(std::countr_zero(n));\r\n    return\
    \ (h & 1u) ? 3u : 4u;\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void forward_cache_node(mint*\
    \ EEZ_NTT998_RESTRICT base, usize block_size, unsigned layer, usize block, usize\
    \ blocks_at_layer, std::array<word, max_log / 2 + 1>& rotation) noexcept {\r\n\
    \    const usize stride = block_size >> 2;\r\n    if(block == 0) forward_radix4_block_lazy<true>(base,\
    \ stride, montgomery_one);\r\n    else forward_radix4_block_lazy<false>(base,\
    \ stride, rotation[layer]);\r\n    if(block + 1 < blocks_at_layer) rotation[layer]\
    \ = canonicalize(mul(rotation[layer], forward_rate3(twiddle_index(static_cast<u32>(block)))));\r\
    \n}\r\n\r\ninline void forward_cache_block(mint* EEZ_NTT998_RESTRICT base, usize\
    \ block_size, unsigned layer, usize block, usize blocks_at_layer, std::array<word,\
    \ max_log / 2 + 1>& rotation) noexcept {\r\n    forward_cache_node(base, block_size,\
    \ layer, block, blocks_at_layer, rotation);\r\n    const usize child_size = block_size\
    \ >> 2;\r\n    for(usize child = 0; child < 4; ++child){\r\n        mint* const\
    \ child_base = base + child * child_size;\r\n        const usize child_block =\
    \ block * 4 + child;\r\n        forward_cache_node(child_base, child_size, layer\
    \ + 1, child_block, blocks_at_layer * 4, rotation);\r\n        const usize grandchild_size\
    \ = child_size >> 2;\r\n        for(usize grandchild = 0; grandchild < 4; ++grandchild)\r\
    \n            forward_cache_node(child_base + grandchild * grandchild_size, grandchild_size,\
    \ layer + 2, child_block * 4 + grandchild, blocks_at_layer * 16, rotation);\r\n\
    \    }\r\n}\r\n\r\ninline void forward_cache_dfs(mint* EEZ_NTT998_RESTRICT base,\
    \ usize block_size, usize leaf_size, unsigned layer, usize block, usize blocks_at_layer,\
    \ std::array<word, max_log / 2 + 1>& rotation) noexcept {\r\n    if(block_size\
    \ == leaf_size * 64){\r\n        forward_cache_block(base, block_size, layer,\
    \ block, blocks_at_layer, rotation);\r\n        return;\r\n    }\r\n    const\
    \ usize stride = block_size >> 2;\r\n    if(block == 0) forward_radix4_block_lazy<true>(base,\
    \ stride, montgomery_one);\r\n    else forward_radix4_block_lazy<false>(base,\
    \ stride, rotation[layer]);\r\n    if(block + 1 < blocks_at_layer) rotation[layer]\
    \ = canonicalize(mul(rotation[layer], forward_rate3(twiddle_index(static_cast<u32>(block)))));\r\
    \n    const usize child_size = block_size >> 2;\r\n    if(child_size == leaf_size)\
    \ return;\r\n    for(usize child = 0; child < 4; ++child)\r\n        forward_cache_dfs(base\
    \ + child * child_size, child_size, leaf_size, layer + 1, block * 4 + child, blocks_at_layer\
    \ * 4, rotation);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void forward_cache_pair_node(mint*\
    \ EEZ_NTT998_RESTRICT a, mint* EEZ_NTT998_RESTRICT b, usize block_size, unsigned\
    \ layer, usize block, usize blocks_at_layer, std::array<word, max_log / 2 + 1>&\
    \ rotation) noexcept {\r\n    const usize stride = block_size >> 2;\r\n    if(block\
    \ == 0) forward_radix4_block_pair_lazy<true>(a, b, stride, montgomery_one);\r\n\
    \    else forward_radix4_block_pair_lazy<false>(a, b, stride, rotation[layer]);\r\
    \n    if(block + 1 < blocks_at_layer) rotation[layer] = canonicalize(mul(rotation[layer],\
    \ forward_rate3(twiddle_index(static_cast<u32>(block)))));\r\n}\r\n\r\ninline\
    \ void forward_cache_pair_block(mint* EEZ_NTT998_RESTRICT a, mint* EEZ_NTT998_RESTRICT\
    \ b, usize block_size, unsigned layer, usize block, usize blocks_at_layer, std::array<word,\
    \ max_log / 2 + 1>& rotation) noexcept {\r\n    forward_cache_pair_node(a, b,\
    \ block_size, layer, block, blocks_at_layer, rotation);\r\n    const usize child_size\
    \ = block_size >> 2;\r\n    for(usize child = 0; child < 4; ++child){\r\n    \
    \    mint* const child_a = a + child * child_size;\r\n        mint* const child_b\
    \ = b + child * child_size;\r\n        const usize child_block = block * 4 + child;\r\
    \n        forward_cache_pair_node(child_a, child_b, child_size, layer + 1, child_block,\
    \ blocks_at_layer * 4, rotation);\r\n        const usize grandchild_size = child_size\
    \ >> 2;\r\n        for(usize grandchild = 0; grandchild < 4; ++grandchild)\r\n\
    \            forward_cache_pair_node(child_a + grandchild * grandchild_size, child_b\
    \ + grandchild * grandchild_size, grandchild_size, layer + 2, child_block * 4\
    \ + grandchild, blocks_at_layer * 16, rotation);\r\n    }\r\n}\r\n\r\ninline void\
    \ forward_cache_pair_dfs(mint* EEZ_NTT998_RESTRICT a, mint* EEZ_NTT998_RESTRICT\
    \ b, usize block_size, usize leaf_size, unsigned layer, usize block, usize blocks_at_layer,\
    \ std::array<word, max_log / 2 + 1>& rotation) noexcept {\r\n    if(block_size\
    \ == leaf_size * 64){\r\n        forward_cache_pair_block(a, b, block_size, layer,\
    \ block, blocks_at_layer, rotation);\r\n        return;\r\n    }\r\n    forward_cache_pair_node(a,\
    \ b, block_size, layer, block, blocks_at_layer, rotation);\r\n    const usize\
    \ child_size = block_size >> 2;\r\n    if(child_size == leaf_size) return;\r\n\
    \    for(usize child = 0; child < 4; ++child)\r\n        forward_cache_pair_dfs(a\
    \ + child * child_size, b + child * child_size, child_size, leaf_size, layer +\
    \ 1, block * 4 + child, blocks_at_layer * 4, rotation);\r\n}\r\n\r\ntemplate<bool\
    \ apply_scale>\r\nEEZ_NTT998_ALWAYS_INLINE void inverse_cache_node(mint* EEZ_NTT998_RESTRICT\
    \ base, usize block_size, unsigned layer, usize block, usize blocks_at_layer,\
    \ word scale, std::array<word, max_log / 2 + 1>& rotation) noexcept {\r\n    const\
    \ usize stride = block_size >> 2;\r\n    if(block == 0) inverse_radix4_block_lazy<true,\
    \ apply_scale>(base, stride, montgomery_one, scale);\r\n    else inverse_radix4_block_lazy<false,\
    \ apply_scale>(base, stride, rotation[layer], scale);\r\n    if(block + 1 < blocks_at_layer)\
    \ rotation[layer] = canonicalize(mul(rotation[layer], inverse_rate3(twiddle_index(static_cast<u32>(block)))));\r\
    \n}\r\n\r\ntemplate<bool scale_leaf>\r\ninline void inverse_cache_block(mint*\
    \ EEZ_NTT998_RESTRICT base, usize block_size, unsigned layer, usize block, usize\
    \ blocks_at_layer, word scale, std::array<word, max_log / 2 + 1>& rotation) noexcept\
    \ {\r\n    const usize child_size = block_size >> 2;\r\n    const usize grandchild_size\
    \ = child_size >> 2;\r\n    for(usize child = 0; child < 4; ++child){\r\n    \
    \    mint* const child_base = base + child * child_size;\r\n        const usize\
    \ child_block = block * 4 + child;\r\n        for(usize grandchild = 0; grandchild\
    \ < 4; ++grandchild)\r\n            inverse_cache_node<scale_leaf>(child_base\
    \ + grandchild * grandchild_size, grandchild_size, layer + 2, child_block * 4\
    \ + grandchild, blocks_at_layer * 16, scale, rotation);\r\n        inverse_cache_node<false>(child_base,\
    \ child_size, layer + 1, child_block, blocks_at_layer * 4, scale, rotation);\r\
    \n    }\r\n    inverse_cache_node<false>(base, block_size, layer, block, blocks_at_layer,\
    \ scale, rotation);\r\n}\r\n\r\ntemplate<bool scale_leaf>\r\ninline void inverse_cache_dfs(mint*\
    \ EEZ_NTT998_RESTRICT base, usize block_size, usize leaf_size, unsigned layer,\
    \ usize block, usize blocks_at_layer, word scale, std::array<word, max_log / 2\
    \ + 1>& rotation) noexcept {\r\n    if(block_size == leaf_size * 64){\r\n    \
    \    inverse_cache_block<scale_leaf>(base, block_size, layer, block, blocks_at_layer,\
    \ scale, rotation);\r\n        return;\r\n    }\r\n    const usize child_size\
    \ = block_size >> 2;\r\n    if(child_size != leaf_size){\r\n        for(usize\
    \ child = 0; child < 4; ++child)\r\n            inverse_cache_dfs<scale_leaf>(base\
    \ + child * child_size, child_size, leaf_size, layer + 1, block * 4 + child, blocks_at_layer\
    \ * 4, scale, rotation);\r\n    }\r\n\r\n    const usize stride = block_size >>\
    \ 2;\r\n    if constexpr(scale_leaf){\r\n        if(child_size == leaf_size){\r\
    \n            if(block == 0) inverse_radix4_block_lazy<true, true>(base, stride,\
    \ montgomery_one, scale);\r\n            else inverse_radix4_block_lazy<false,\
    \ true>(base, stride, rotation[layer], scale);\r\n        }else if(block == 0)\
    \ inverse_radix4_block_lazy<true, false>(base, stride, montgomery_one, scale);\r\
    \n        else inverse_radix4_block_lazy<false, false>(base, stride, rotation[layer],\
    \ scale);\r\n    }else if(block == 0) inverse_radix4_block_lazy<true, false>(base,\
    \ stride, montgomery_one, scale);\r\n    else inverse_radix4_block_lazy<false,\
    \ false>(base, stride, rotation[layer], scale);\r\n\r\n    if(block + 1 < blocks_at_layer)\
    \ rotation[layer] = canonicalize(mul(rotation[layer], inverse_rate3(twiddle_index(static_cast<u32>(block)))));\r\
    \n}\r\n\r\ninline void forward_adaptive(mint* a, usize n, unsigned leaf_log) noexcept\
    \ {\r\n    const usize leaf_size = usize(1) << leaf_log;\r\n    if(n == leaf_size)\
    \ return;\r\n    std::array<word, max_log / 2 + 1> rotation{};\r\n    rotation.fill(canonicalize(montgomery_one));\r\
    \n    if((std::countr_zero(n) - leaf_log) & 1u){\r\n        forward_radix2_first(a,\
    \ n);\r\n        forward_cache_dfs(a, n >> 1, leaf_size, 0, 0, 2, rotation);\r\
    \n        forward_cache_dfs(a + (n >> 1), n >> 1, leaf_size, 0, 1, 2, rotation);\r\
    \n        return;\r\n    }\r\n    forward_cache_dfs(a, n, leaf_size, 0, 0, 1,\
    \ rotation);\r\n}\r\n\r\ninline void forward_adaptive_pair(mint* EEZ_NTT998_RESTRICT\
    \ a, mint* EEZ_NTT998_RESTRICT b, usize n, unsigned leaf_log) noexcept {\r\n \
    \   const usize leaf_size = usize(1) << leaf_log;\r\n    if(n == leaf_size) return;\r\
    \n    std::array<word, max_log / 2 + 1> rotation{};\r\n    rotation.fill(canonicalize(montgomery_one));\r\
    \n    if((std::countr_zero(n) - leaf_log) & 1u){\r\n        forward_radix2_first(a,\
    \ n);\r\n        forward_radix2_first(b, n);\r\n        forward_cache_pair_dfs(a,\
    \ b, n >> 1, leaf_size, 0, 0, 2, rotation);\r\n        forward_cache_pair_dfs(a\
    \ + (n >> 1), b + (n >> 1), n >> 1, leaf_size, 0, 1, 2, rotation);\r\n       \
    \ return;\r\n    }\r\n    forward_cache_pair_dfs(a, b, n, leaf_size, 0, 0, 1,\
    \ rotation);\r\n}\r\n\r\ninline void inverse_adaptive(mint* a, usize n, unsigned\
    \ leaf_log) noexcept {\r\n    const usize leaf_size = usize(1) << leaf_log;\r\n\
    \    if(n == leaf_size) return;\r\n    const word scale = mint::raw(static_cast<word>(n\
    \ >> leaf_log)).inv().a;\r\n    std::array<word, max_log / 2 + 1> rotation{};\r\
    \n    rotation.fill(canonicalize(montgomery_one));\r\n    if((std::countr_zero(n)\
    \ - leaf_log) & 1u){\r\n        inverse_cache_dfs<true>(a, n >> 1, leaf_size,\
    \ 0, 0, 2, scale, rotation);\r\n        inverse_cache_dfs<true>(a + (n >> 1),\
    \ n >> 1, leaf_size, 0, 1, 2, scale, rotation);\r\n        const usize half =\
    \ n >> 1;\r\n        for(usize i = 0; i < half; i += 8){\r\n            const\
    \ vec x = shrink4_to_2(load8_aligned(a + i));\r\n            const vec y = shrink4_to_2(load8_aligned(a\
    \ + half + i));\r\n            store8_aligned(a + i, lazy_add8(x, y));\r\n   \
    \         store8_aligned(a + half + i, lazy_sub8(x, y));\r\n        }\r\n    \
    \    return;\r\n    }\r\n    inverse_cache_dfs<true>(a, n, leaf_size, 0, 0, 1,\
    \ scale, rotation);\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE __m128i reduce_four_accumulators(vec\
    \ x) noexcept {\r\n    const vec ninv = broadcast(montgomery_ninv);\r\n    const\
    \ vec prime = broadcast(mod);\r\n    const vec q = _mm256_mul_epu32(x, ninv);\r\
    \n    const vec sum = _mm256_add_epi64(x, _mm256_mul_epu32(q, prime));\r\n   \
    \ const vec high = _mm256_bsrli_epi128(sum, 4);\r\n    const vec packed = _mm256_permutevar8x32_epi32(high,\
    \ _mm256_setr_epi32(0, 2, 4, 6, 0, 0, 0, 0));\r\n    return _mm256_castsi256_si128(packed);\r\
    \n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE vec reduce_eight_accumulators(vec even, vec\
    \ odd) noexcept {\r\n    const vec ninv = broadcast(montgomery_ninv);\r\n    const\
    \ vec prime = broadcast(mod);\r\n    const vec q_even = _mm256_mul_epu32(even,\
    \ ninv);\r\n    const vec q_odd = _mm256_mul_epu32(odd, ninv);\r\n    const vec\
    \ reduced_even = _mm256_add_epi64(even, _mm256_mul_epu32(q_even, prime));\r\n\
    \    const vec reduced_odd = _mm256_add_epi64(odd, _mm256_mul_epu32(q_odd, prime));\r\
    \n    return shrink4_to_2(_mm256_or_si256(_mm256_bsrli_epi128(reduced_even, 4),\
    \ reduced_odd));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void leaf_product8x4(mint*\
    \ EEZ_NTT998_RESTRICT a, mint* EEZ_NTT998_RESTRICT b, usize first_block, const\
    \ std::array<word, 4>& modulus) noexcept {\r\n    alignas(64) word lhs[4][16];\r\
    \n    alignas(64) vec even[4]{};\r\n    alignas(64) vec odd[4]{};\r\n\r\n    for(unsigned\
    \ k = 0; k < 4; ++k){\r\n        const usize offset = (first_block + k) * 8;\r\
    \n        const vec x = canonicalize8(shrink4_to_2(load8_aligned(a + offset)));\r\
    \n        const vec y = canonicalize8(shrink4_to_2(load8_aligned(b + offset)));\r\
    \n        const word w = canonicalize(modulus[k]);\r\n        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]),\
    \ mul8_fixed(x, broadcast(w), broadcast(w * montgomery_ninv)));\r\n        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]\
    \ + 8), x);\r\n        store8_aligned(b + offset, y);\r\n    }\r\n\r\n    for(unsigned\
    \ i = 0; i < 8; ++i){\r\n        for(unsigned k = 0; k < 4; ++k){\r\n        \
    \    const usize offset = (first_block + k) * 8;\r\n            const vec y =\
    \ broadcast(raw(b[offset + i]));\r\n            const vec x = _mm256_loadu_si256(reinterpret_cast<const\
    \ vec*>(lhs[k] + 8 - i));\r\n            even[k] = _mm256_add_epi64(even[k], _mm256_mul_epu32(y,\
    \ x));\r\n            odd[k] = _mm256_add_epi64(odd[k], _mm256_mul_epu32(y, _mm256_bsrli_epi128(x,\
    \ 4)));\r\n        }\r\n    }\r\n\r\n    for(unsigned k = 0; k < 4; ++k)\r\n \
    \       store8_aligned(a + (first_block + k) * 8, reduce_eight_accumulators(even[k],\
    \ odd[k]));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void leaf_product16x2_karatsuba(mint*\
    \ EEZ_NTT998_RESTRICT a, const mint* EEZ_NTT998_RESTRICT b, usize first_block,\
    \ const std::array<word, 2>& modulus) noexcept {\r\n    const vec split = _mm256_setr_epi32(0,\
    \ 2, 4, 6, 1, 3, 5, 7);\r\n    alignas(64) word lhs[6][16];\r\n    alignas(64)\
    \ word rhs[6][8];\r\n    alignas(64) vec even[6]{};\r\n    alignas(64) vec odd[6]{};\r\
    \n    word w[2];\r\n\r\n    for(unsigned k = 0; k < 2; ++k){\r\n        const\
    \ usize offset = (first_block + k) * 16;\r\n        const vec ax0 = _mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a\
    \ + offset))), split);\r\n        const vec ax1 = _mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a\
    \ + offset + 8))), split);\r\n        const vec bx0 = _mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b\
    \ + offset))), split);\r\n        const vec bx1 = _mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b\
    \ + offset + 8))), split);\r\n        const vec ae = _mm256_permute2x128_si256(ax0,\
    \ ax1, 0x20);\r\n        const vec ao = _mm256_permute2x128_si256(ax0, ax1, 0x31);\r\
    \n        const vec be = _mm256_permute2x128_si256(bx0, bx1, 0x20);\r\n      \
    \  const vec bo = _mm256_permute2x128_si256(bx0, bx1, 0x31);\r\n        const\
    \ vec lx[3]{ae, ao, canonicalize8(add8(ae, ao))};\r\n        const vec ry[3]{be,\
    \ bo, canonicalize8(add8(be, bo))};\r\n        w[k] = canonicalize(modulus[k]);\r\
    \n        const vec vw = broadcast(w[k]);\r\n        const vec vwninv = broadcast(w[k]\
    \ * montgomery_ninv);\r\n        for(unsigned j = 0; j < 3; ++j){\r\n        \
    \    const unsigned p = k * 3 + j;\r\n            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]),\
    \ mul8_fixed(lx[j], vw, vwninv));\r\n            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]\
    \ + 8), lx[j]);\r\n            _mm256_store_si256(reinterpret_cast<vec*>(rhs[p]),\
    \ ry[j]);\r\n        }\r\n    }\r\n\r\n    for(unsigned i = 0; i < 8; ++i){\r\n\
    \        for(unsigned p = 0; p < 6; ++p){\r\n            const vec y = broadcast(rhs[p][i]);\r\
    \n            const vec x = _mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[p]\
    \ + 8 - i));\r\n            even[p] = _mm256_add_epi64(even[p], _mm256_mul_epu32(y,\
    \ x));\r\n            odd[p] = _mm256_add_epi64(odd[p], _mm256_mul_epu32(y, _mm256_bsrli_epi128(x,\
    \ 4)));\r\n        }\r\n    }\r\n\r\n    for(unsigned k = 0; k < 2; ++k){\r\n\
    \        const vec p0 = reduce_eight_accumulators(even[k * 3], odd[k * 3]);\r\n\
    \        const vec p1 = reduce_eight_accumulators(even[k * 3 + 1], odd[k * 3 +\
    \ 1]);\r\n        const vec p2 = reduce_eight_accumulators(even[k * 3 + 2], odd[k\
    \ * 3 + 2]);\r\n        vec yp1 = _mm256_permutevar8x32_epi32(p1, _mm256_setr_epi32(7,\
    \ 0, 1, 2, 3, 4, 5, 6));\r\n        yp1 = _mm256_insert_epi32(yp1, canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,\
    \ 7)), w[k])), 0);\r\n        const vec ce = add8(p0, yp1);\r\n        const vec\
    \ co = sub8(sub8(p2, p0), p1);\r\n        const vec lo = _mm256_unpacklo_epi32(ce,\
    \ co);\r\n        const vec hi = _mm256_unpackhi_epi32(ce, co);\r\n        const\
    \ usize offset = (first_block + k) * 16;\r\n        store8_aligned(a + offset,\
    \ _mm256_permute2x128_si256(lo, hi, 0x20));\r\n        store8_aligned(a + offset\
    \ + 8, _mm256_permute2x128_si256(lo, hi, 0x31));\r\n    }\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ word twice(word x) noexcept {\r\n    return x + x;\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ vec pack4_u32(word x0, word x1, word x2, word x3) noexcept {\r\n    const __m128i\
    \ x = _mm_setr_epi32(static_cast<int>(x0), static_cast<int>(x1), static_cast<int>(x2),\
    \ static_cast<int>(x3));\r\n    return _mm256_cvtepu32_epi64(x);\r\n}\r\n\r\n\
    EEZ_NTT998_ALWAYS_INLINE vec mul4_u32(word a0, word b0, word a1, word b1, word\
    \ a2, word b2, word a3, word b3) noexcept {\r\n    return _mm256_mul_epu32(pack4_u32(a0,\
    \ a1, a2, a3), pack4_u32(b0, b1, b2, b3));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ u64 hsum4_u64(vec x) noexcept {\r\n    __m128i s = _mm_add_epi64(_mm256_castsi256_si128(x),\
    \ _mm256_extracti128_si256(x, 1));\r\n    s = _mm_add_epi64(s, _mm_srli_si128(s,\
    \ 8));\r\n    return static_cast<u64>(_mm_cvtsi128_si64(s));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ vec square8_packed(vec vx, word w) noexcept {\r\n    alignas(32) word x[8],\
    \ xw[8];\r\n    vx = canonicalize8(shrink4_to_2(vx));\r\n    w = canonicalize(w);\r\
    \n    _mm256_store_si256(reinterpret_cast<vec*>(x), vx);\r\n    _mm256_store_si256(reinterpret_cast<vec*>(xw),\
    \ mul8_fixed(vx, broadcast(w), broadcast(w * montgomery_ninv)));\r\n\r\n    u64\
    \ a0 = hsum4_u64(mul4_u32(\r\n        x[0], x[0],\r\n        twice(xw[1]), x[7],\r\
    \n        twice(xw[2]), x[6],\r\n        twice(xw[3]), x[5]\r\n    ));\r\n   \
    \ const u64 a1 = hsum4_u64(mul4_u32(\r\n        twice(x[0]), x[1],\r\n       \
    \ twice(xw[2]), x[7],\r\n        twice(xw[3]), x[6],\r\n        twice(xw[4]),\
    \ x[5]\r\n    ));\r\n    u64 a2 = hsum4_u64(mul4_u32(\r\n        twice(x[0]),\
    \ x[2],\r\n        x[1], x[1],\r\n        twice(xw[3]), x[7],\r\n        twice(xw[4]),\
    \ x[6]\r\n    ));\r\n    const u64 a3 = hsum4_u64(mul4_u32(\r\n        twice(x[0]),\
    \ x[3],\r\n        twice(x[1]), x[2],\r\n        twice(xw[4]), x[7],\r\n     \
    \   twice(xw[5]), x[6]\r\n    ));\r\n    u64 a4 = hsum4_u64(mul4_u32(\r\n    \
    \    twice(x[0]), x[4],\r\n        twice(x[1]), x[3],\r\n        x[2], x[2],\r\
    \n        twice(xw[5]), x[7]\r\n    ));\r\n    const u64 a5 = hsum4_u64(mul4_u32(\r\
    \n        twice(x[0]), x[5],\r\n        twice(x[1]), x[4],\r\n        twice(x[2]),\
    \ x[3],\r\n        twice(xw[6]), x[7]\r\n    ));\r\n    u64 a6 = hsum4_u64(mul4_u32(\r\
    \n        twice(x[0]), x[6],\r\n        twice(x[1]), x[5],\r\n        twice(x[2]),\
    \ x[4],\r\n        x[3], x[3]\r\n    ));\r\n    const u64 a7 = hsum4_u64(mul4_u32(\r\
    \n        twice(x[0]), x[7],\r\n        twice(x[1]), x[6],\r\n        twice(x[2]),\
    \ x[5],\r\n        twice(x[3]), x[4]\r\n    ));\r\n\r\n    const vec extra = mul4_u32(\r\
    \n        xw[4], x[4],\r\n        xw[5], x[5],\r\n        xw[6], x[6],\r\n   \
    \     xw[7], x[7]\r\n    );\r\n    alignas(32) u64 e[4];\r\n    _mm256_store_si256(reinterpret_cast<vec*>(e),\
    \ extra);\r\n    a0 += e[0];\r\n    a2 += e[1];\r\n    a4 += e[2];\r\n    a6 +=\
    \ e[3];\r\n\r\n    const vec lo = _mm256_setr_epi64x(static_cast<long long>(a0),\
    \ static_cast<long long>(a1),\r\n                                      static_cast<long\
    \ long>(a2), static_cast<long long>(a3));\r\n    const vec hi = _mm256_setr_epi64x(static_cast<long\
    \ long>(a4), static_cast<long long>(a5),\r\n                                 \
    \     static_cast<long long>(a6), static_cast<long long>(a7));\r\n    return shrink4_to_2(_mm256_set_m128i(reduce_four_accumulators(hi),\
    \ reduce_four_accumulators(lo)));\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void leaf_square8x4(mint*\
    \ EEZ_NTT998_RESTRICT a, usize first_block, const std::array<word, 4>& modulus)\
    \ noexcept {\r\n    for(unsigned k = 0; k < 4; ++k){\r\n        const usize offset\
    \ = (first_block + k) * 8;\r\n        store8_aligned(a + offset, square8_packed(load8_aligned(a\
    \ + offset), modulus[k]));\r\n    }\r\n}\r\n\r\nEEZ_NTT998_ALWAYS_INLINE void\
    \ leaf_square16x2_karatsuba(mint* EEZ_NTT998_RESTRICT a, usize first_block, const\
    \ std::array<word, 2>& modulus) noexcept {\r\n    const vec split = _mm256_setr_epi32(0,\
    \ 2, 4, 6, 1, 3, 5, 7);\r\n\r\n    for(unsigned k = 0; k < 2; ++k){\r\n      \
    \  const usize offset = (first_block + k) * 16;\r\n        const vec x0 = _mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a\
    \ + offset))), split);\r\n        const vec x1 = _mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a\
    \ + offset + 8))), split);\r\n        const vec xe = _mm256_permute2x128_si256(x0,\
    \ x1, 0x20);\r\n        const vec xo = _mm256_permute2x128_si256(x0, x1, 0x31);\r\
    \n        const vec xs = canonicalize8(add8(xe, xo));\r\n        const word w\
    \ = canonicalize(modulus[k]);\r\n\r\n        const vec p0 = square8_packed(xe,\
    \ w);\r\n        const vec p1 = square8_packed(xo, w);\r\n        const vec p2\
    \ = square8_packed(xs, w);\r\n\r\n        vec yp1 = _mm256_permutevar8x32_epi32(p1,\
    \ _mm256_setr_epi32(7, 0, 1, 2, 3, 4, 5, 6));\r\n        yp1 = _mm256_insert_epi32(yp1,\
    \ canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1, 7)), w)), 0);\r\n\
    \        const vec ce = add8(p0, yp1);\r\n        const vec co = sub8(sub8(p2,\
    \ p0), p1);\r\n        const vec lo = _mm256_unpacklo_epi32(ce, co);\r\n     \
    \   const vec hi = _mm256_unpackhi_epi32(ce, co);\r\n\r\n        store8_aligned(a\
    \ + offset, _mm256_permute2x128_si256(lo, hi, 0x20));\r\n        store8_aligned(a\
    \ + offset + 8, _mm256_permute2x128_si256(lo, hi, 0x31));\r\n    }\r\n}\r\n\r\n\
    template<unsigned leaf_size, unsigned parallel_blocks>\r\ninline void leaf_products(mint*\
    \ a, mint* b, usize n) noexcept {\r\n    const usize blocks = n / leaf_size;\r\
    \n    word w = montgomery_one;\r\n    for(usize s = 0; s < blocks; s += parallel_blocks){\r\
    \n        std::array<word, parallel_blocks> modulus{};\r\n        for(unsigned\
    \ k = 0; k < parallel_blocks; ++k){\r\n            modulus[k] = w;\r\n       \
    \     const usize block = s + k;\r\n            if(block + 1 < blocks) w = mul(w,\
    \ forward_rate1(twiddle_index(static_cast<u32>(block))));\r\n        }\r\n   \
    \     if constexpr(leaf_size == 8) leaf_product8x4(a, b, s, modulus);\r\n    \
    \    else leaf_product16x2_karatsuba(a, b, s, modulus);\r\n    }\r\n}\r\n\r\n\
    template<unsigned leaf_size, unsigned parallel_blocks>\r\ninline void leaf_squares(mint*\
    \ a, usize n) noexcept {\r\n    const usize blocks = n / leaf_size;\r\n    word\
    \ w = montgomery_one;\r\n    for(usize s = 0; s < blocks; s += parallel_blocks){\r\
    \n        std::array<word, parallel_blocks> modulus{};\r\n        for(unsigned\
    \ k = 0; k < parallel_blocks; ++k){\r\n            modulus[k] = w;\r\n       \
    \     const usize block = s + k;\r\n            if(block + 1 < blocks) w = mul(w,\
    \ forward_rate1(twiddle_index(static_cast<u32>(block))));\r\n        }\r\n   \
    \     if constexpr(leaf_size == 8) leaf_square8x4(a, s, modulus);\r\n        else\
    \ leaf_square16x2_karatsuba(a, s, modulus);\r\n    }\r\n}\r\n\r\ninline void convolution_adaptive_inplace(mint*\
    \ a, mint* b, usize n) noexcept {\r\n    const unsigned leaf_log = adaptive_leaf_log(n);\r\
    \n    forward_adaptive_pair(a, b, n, leaf_log);\r\n    if(leaf_log == 3) leaf_products<8,\
    \ 4>(a, b, n);\r\n    else leaf_products<16, 2>(a, b, n);\r\n    inverse_adaptive(a,\
    \ n, leaf_log);\r\n}\r\n\r\ninline void square_adaptive_inplace(mint* a, usize\
    \ n) noexcept {\r\n    const unsigned leaf_log = adaptive_leaf_log(n);\r\n   \
    \ forward_adaptive(a, n, leaf_log);\r\n    if(leaf_log == 3) leaf_squares<8, 4>(a,\
    \ n);\r\n    else leaf_squares<16, 2>(a, n);\r\n    inverse_adaptive(a, n, leaf_log);\r\
    \n}\r\n\r\n#endif\r\n\r\ninline void pointwise_multiply(mint* EEZ_NTT998_RESTRICT\
    \ a, const mint* EEZ_NTT998_RESTRICT b, usize n) noexcept {\r\n    usize i = 0;\r\
    \n#if EEZ_NTT998_USE_AVX2\r\n    for(; i + 8 <= n; i += 8) store8(a + i, mul8(load8(a\
    \ + i), load8(b + i)));\r\n#endif\r\n    for(; i < n; ++i) a[i] = from_raw(mul(raw(a[i]),\
    \ raw(b[i])));\r\n}\r\n\r\ninline void pointwise_square(mint* EEZ_NTT998_RESTRICT\
    \ a, usize n) noexcept {\r\n    usize i = 0;\r\n#if EEZ_NTT998_USE_AVX2\r\n  \
    \  for(; i + 8 <= n; i += 8){\r\n        const vec x = load8(a + i);\r\n     \
    \   store8(a + i, mul8(x, x));\r\n    }\r\n#endif\r\n    for(; i < n; ++i) a[i]\
    \ = from_raw(mul(raw(a[i]), raw(a[i])));\r\n}\r\n\r\n}\r\n\r\ninline void forward(std::span<mint>\
    \ a) noexcept {\r\n    if(a.size() <= 1) return;\r\n    assert(valid_ntt_size(a.size()));\r\
    \n    detail::forward_dif(a.data(), a.size());\r\n}\r\n\r\ninline void inverse(std::span<mint>\
    \ a) noexcept {\r\n    if(a.size() <= 1) return;\r\n    assert(valid_ntt_size(a.size()));\r\
    \n    detail::inverse_dit(a.data(), a.size());\r\n}\r\n\r\nnamespace detail{\r\
    \n\r\ninline std::vector<mint> convolution_naive(std::span<const mint> a, std::span<const\
    \ mint> b){\r\n    std::vector<mint> result(convolution_size(a.size(), b.size()));\r\
    \n    for(usize i = 0; i < a.size(); ++i)\r\n        for(usize j = 0; j < b.size();\
    \ ++j)\r\n            result[i + j] += a[i] * b[j];\r\n    return result;\r\n\
    }\r\n\r\ninline std::vector<mint> square_naive(std::span<const mint> a){\r\n \
    \   std::vector<mint> result(convolution_size(a.size(), a.size()));\r\n    for(usize\
    \ i = 0; i < a.size(); ++i){\r\n        result[2 * i] += a[i] * a[i];\r\n    \
    \    for(usize j = i + 1; j < a.size(); ++j){\r\n            const mint product\
    \ = a[i] * a[j];\r\n            result[i + j] += product + product;\r\n      \
    \  }\r\n    }\r\n    return result;\r\n}\r\n\r\ninline usize checked_transform_size(usize\
    \ n, usize m){\r\n    const usize result = convolution_transform_size(n, m);\r\
    \n    if(n && m && !result)\r\n        throw std::length_error(\"eez::ntt998:\
    \ convolution exceeds the 2^25 transform limit\");\r\n    return result;\r\n}\r\
    \n\r\ninline void require_ntt_size(usize n){\r\n    if(!valid_ntt_size(n))\r\n\
    \        throw std::invalid_argument(\"eez::ntt998: transform length must be a\
    \ power of two in [1, 2^23]\");\r\n}\r\n\r\n}\r\n\r\n#if EEZ_NTT998_USE_AVX2\r\
    \n\r\nusing convolution_buffer = detail::aligned_vector;\r\n\r\ninline void convolution_inplace(convolution_buffer&\
    \ a, convolution_buffer& b){\r\n    if(a.size() != b.size() || !valid_convolution_transform_size(a.size()))\r\
    \n        throw std::invalid_argument(\"eez::ntt998::convolution_inplace: buffer\
    \ sizes must match and be a power of two in [32, 2^25]\");\r\n    detail::convolution_adaptive_inplace(a.data(),\
    \ b.data(), a.size());\r\n}\r\n\r\n#endif\r\n\r\ninline std::vector<mint> convolution(std::span<const\
    \ mint> a, std::span<const mint> b){\r\n    if(a.empty() || b.empty()) return\
    \ {};\r\n    if(a.data() == b.data() && a.size() == b.size()) return square(a);\r\
    \n    if(std::min(a.size(), b.size()) <= naive_cutoff) return detail::convolution_naive(a,\
    \ b);\r\n\r\n    const usize result_size = convolution_size(a.size(), b.size());\r\
    \n    const usize n = detail::checked_transform_size(a.size(), b.size());\r\n\
    \    detail::aligned_vector fa(n), fb(n);\r\n    std::copy(a.begin(), a.end(),\
    \ fa.begin());\r\n    std::copy(b.begin(), b.end(), fb.begin());\r\n\r\n    detail::convolution_adaptive_inplace(fa.data(),\
    \ fb.data(), n);\r\n\r\n    std::vector<mint> result(result_size);\r\n    std::copy_n(fa.data(),\
    \ result_size, result.data());\r\n    return result;\r\n}\r\n\r\ninline std::vector<mint>\
    \ square(std::span<const mint> a){\r\n    if(a.empty()) return {};\r\n    if(a.size()\
    \ <= naive_cutoff) return detail::square_naive(a);\r\n\r\n    const usize result_size\
    \ = convolution_size(a.size(), a.size());\r\n    const usize n = detail::checked_transform_size(a.size(),\
    \ a.size());\r\n    detail::aligned_vector fa(n);\r\n    std::copy(a.begin(),\
    \ a.end(), fa.begin());\r\n\r\n    detail::square_adaptive_inplace(fa.data(),\
    \ n);\r\n\r\n    std::vector<mint> result(result_size);\r\n    std::copy_n(fa.data(),\
    \ result_size, result.data());\r\n    return result;\r\n}\r\n\r\ninline void convolution_to(std::span<const\
    \ mint> a, std::span<const mint> b, std::span<mint> out, workspace& ws){\r\n \
    \   if(a.empty() || b.empty()) return;\r\n    const usize result_size = convolution_size(a.size(),\
    \ b.size());\r\n    if(out.size() < result_size)\r\n        throw std::invalid_argument(\"\
    eez::ntt998::convolution_to: output span is too small\");\r\n\r\n    if(a.data()\
    \ == b.data() && a.size() == b.size()){\r\n        square_to(a, out, ws);\r\n\
    \        return;\r\n    }\r\n\r\n    if(std::min(a.size(), b.size()) <= naive_cutoff){\r\
    \n        std::fill_n(out.begin(), result_size, mint{});\r\n        for(usize\
    \ i = 0; i < a.size(); ++i)\r\n            for(usize j = 0; j < b.size(); ++j)\r\
    \n                out[i + j] += a[i] * b[j];\r\n        return;\r\n    }\r\n\r\
    \n    const usize n = detail::checked_transform_size(a.size(), b.size());\r\n\
    \    ws.reserve(n);\r\n    std::fill_n(ws.a_.begin(), n, mint{});\r\n    std::fill_n(ws.b_.begin(),\
    \ n, mint{});\r\n    std::copy(a.begin(), a.end(), ws.a_.begin());\r\n    std::copy(b.begin(),\
    \ b.end(), ws.b_.begin());\r\n\r\n    detail::convolution_adaptive_inplace(ws.a_.data(),\
    \ ws.b_.data(), n);\r\n    std::copy_n(ws.a_.begin(), result_size, out.begin());\r\
    \n}\r\n\r\ninline void square_to(std::span<const mint> a, std::span<mint> out,\
    \ workspace& ws){\r\n    if(a.empty()) return;\r\n    const usize result_size\
    \ = convolution_size(a.size(), a.size());\r\n    if(out.size() < result_size)\r\
    \n        throw std::invalid_argument(\"eez::ntt998::square_to: output span is\
    \ too small\");\r\n\r\n    if(a.size() <= naive_cutoff){\r\n        std::fill_n(out.begin(),\
    \ result_size, mint{});\r\n        for(usize i = 0; i < a.size(); ++i){\r\n  \
    \          out[2 * i] += a[i] * a[i];\r\n            for(usize j = i + 1; j <\
    \ a.size(); ++j){\r\n                const mint product = a[i] * a[j];\r\n   \
    \             out[i + j] += product + product;\r\n            }\r\n        }\r\
    \n        return;\r\n    }\r\n\r\n    const usize n = detail::checked_transform_size(a.size(),\
    \ a.size());\r\n    ws.reserve(n);\r\n    std::fill_n(ws.a_.begin(), n, mint{});\r\
    \n    std::copy(a.begin(), a.end(), ws.a_.begin());\r\n\r\n    detail::square_adaptive_inplace(ws.a_.data(),\
    \ n);\r\n    std::copy_n(ws.a_.begin(), result_size, out.begin());\r\n}\r\n\r\n\
    inline void forward_to(std::span<const mint> src, frequency_buffer& dst, usize\
    \ n){\r\n    detail::require_ntt_size(n);\r\n    if(src.size() > n)\r\n      \
    \  throw std::invalid_argument(\"eez::ntt998::forward_to: source is longer than\
    \ transform\");\r\n\r\n    dst.data_.assign(n, mint{});\r\n    std::copy(src.begin(),\
    \ src.end(), dst.data_.begin());\r\n    detail::forward_dif(dst.data_.data(),\
    \ n);\r\n}\r\n\r\ninline void pointwise_multiply(frequency_buffer& lhs, const\
    \ frequency_buffer& rhs){\r\n    if(lhs.size() != rhs.size())\r\n        throw\
    \ std::invalid_argument(\"eez::ntt998::pointwise_multiply: transform sizes differ\"\
    );\r\n    detail::pointwise_multiply(lhs.data_.data(), rhs.data_.data(), lhs.data_.size());\r\
    \n}\r\n\r\ninline void pointwise_square(frequency_buffer& a){\r\n    detail::pointwise_square(a.data_.data(),\
    \ a.data_.size());\r\n}\r\n\r\ninline void inverse_to(frequency_buffer& src, std::span<mint>\
    \ out){\r\n    if(src.data_.empty()){\r\n        if(!out.empty())\r\n        \
    \    throw std::invalid_argument(\"eez::ntt998::inverse_to: empty transform\"\
    );\r\n        return;\r\n    }\r\n\r\n    if(out.size() > src.data_.size())\r\n\
    \        throw std::invalid_argument(\"eez::ntt998::inverse_to: output is longer\
    \ than transform\");\r\n\r\n    detail::inverse_dit(src.data_.data(), src.data_.size());\r\
    \n    std::copy_n(src.data_.begin(), out.size(), out.begin());\r\n}\r\n\r\n}\r\
    \n\r\n#undef EEZ_NTT998_ALWAYS_INLINE\r\n#undef EEZ_NTT998_RESTRICT\r\n#undef\
    \ EEZ_NTT998_USE_AVX2\r\n\r\n#if defined(__clang__) && \\\r\n    (defined(__x86_64__)\
    \ || defined(__i386__))\r\n#pragma clang attribute pop\r\n#elif defined(__GNUC__)\
    \ && \\\r\n    (defined(__x86_64__) || defined(__i386__))\r\n#pragma GCC pop_options\r\
    \n#endif\r\n#line 11 \"fps/detail/fps998_inv_ntt.hpp\"\n\n#if defined(__GNUC__)\
    \ && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))\n#pragma\
    \ GCC push_options\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt\")\n#define EEZ_FPS998_ALWAYS_INLINE inline __attribute__((always_inline))\n\
    #define EEZ_FPS998_RESTRICT __restrict__\n#elif defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute push(__attribute__((target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))),apply_to=function)\n#define EEZ_FPS998_ALWAYS_INLINE\
    \ inline __attribute__((always_inline))\n#define EEZ_FPS998_RESTRICT __restrict__\n\
    #else\n#define EEZ_FPS998_ALWAYS_INLINE inline\n#define EEZ_FPS998_RESTRICT\n\
    #endif\n\nnamespace eez::fps998::detail::inv_ntt{\nusing mint=ntt998::mint;\n\
    using usize=std::size_t;\nnamespace nd=ntt998::detail;\n\ninline constexpr std::uint32_t\
    \ inv2_value=(mint::MOD+1u)>>1;\ninline constexpr nd::word inv2_mont=nd::canonicalize(mint::raw(inv2_value).a);\n\
    \nEEZ_FPS998_ALWAYS_INLINE nd::word shrink4_scalar(nd::word x) noexcept{\n   \
    \ return x>=nd::mod2?x-nd::mod2:x;\n}\nEEZ_FPS998_ALWAYS_INLINE nd::vec neg8(nd::vec\
    \ x) noexcept{\n    return nd::sub8(_mm256_setzero_si256(),x);\n}\nEEZ_FPS998_ALWAYS_INLINE\
    \ void zero_n(mint* p,usize n) noexcept{\n    if(n) std::memset(p,0,n*sizeof(mint));\n\
    }\nEEZ_FPS998_ALWAYS_INLINE void copy_n(mint* dst,const mint* src,usize n) noexcept{\n\
    \    if(n) std::memcpy(dst,src,n*sizeof(mint));\n}\n\ninline void copy_shrink4_to2(mint*\
    \ EEZ_FPS998_RESTRICT dst,const mint* EEZ_FPS998_RESTRICT src,usize n) noexcept{\n\
    \    usize i=0;\n    for(;i+8<=n;i+=8) nd::store8(dst+i,nd::shrink4_to_2(nd::load8(src+i)));\n\
    \    for(;i<n;++i) dst[i]=nd::from_raw(shrink4_scalar(nd::raw(src[i])));\n}\n\n\
    inline void sub_inplace(mint* EEZ_FPS998_RESTRICT a,const mint* EEZ_FPS998_RESTRICT\
    \ b,usize n) noexcept{\n    usize i=0;\n    for(;i+8<=n;i+=8){\n        const\
    \ nd::vec x=nd::shrink4_to_2(nd::load8_aligned(a+i));\n        const nd::vec y=nd::shrink4_to_2(nd::load8_aligned(b+i));\n\
    \        nd::store8_aligned(a+i,nd::sub8(x,y));\n    }\n    for(;i<n;++i){\n \
    \       const nd::word x=shrink4_scalar(nd::raw(a[i]));\n        const nd::word\
    \ y=shrink4_scalar(nd::raw(b[i]));\n        a[i]=nd::from_raw(nd::sub(x,y));\n\
    \    }\n}\n\ntemplate<unsigned leaf_size,unsigned parallel_blocks>\ninline void\
    \ leaf_products_shifted(mint* EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT\
    \ b,usize n,nd::word shift) noexcept{\n    const usize blocks=n/leaf_size;\n \
    \   assert(blocks%parallel_blocks==0);\n    nd::word w=nd::canonicalize(shift);\n\
    \    for(usize s=0;s<blocks;s+=parallel_blocks){\n        std::array<nd::word,parallel_blocks>\
    \ modulus{};\n        for(unsigned k=0;k<parallel_blocks;++k){\n            modulus[k]=w;\n\
    \            const usize block=s+k;\n            if(block+1<blocks)\n        \
    \        w=nd::canonicalize(nd::mul(w,nd::forward_rate1(nd::twiddle_index(static_cast<ntt998::u32>(block)))));\n\
    \        }\n        if constexpr(leaf_size==8) nd::leaf_product8x4(a,b,s,modulus);\n\
    \        else{\n            static_assert(leaf_size==16&&parallel_blocks==2);\n\
    \            nd::leaf_product16x2_karatsuba(a,b,s,modulus);\n        }\n    }\n\
    }\n\ntemplate<unsigned leaf_size>\ninline void block_twist(mint* EEZ_FPS998_RESTRICT\
    \ a,usize n,nd::word ratio) noexcept{\n    static_assert(leaf_size==8||leaf_size==16);\n\
    \    ratio=nd::canonicalize(ratio);\n    nd::word w=nd::canonicalize(nd::montgomery_one);\n\
    \    const usize blocks=n/leaf_size;\n    for(usize s=0;s<blocks;++s){\n     \
    \   const nd::word c=nd::canonicalize(w);\n        const nd::vec vc=nd::broadcast(c);\n\
    \        const nd::vec vcn=nd::broadcast(c*nd::montgomery_ninv);\n        mint*\
    \ const p=a+s*leaf_size;\n        if constexpr(leaf_size==8){\n            nd::store8_aligned(p,nd::mul8_fixed(nd::load8_aligned(p),vc,vcn));\n\
    \        }else{\n            nd::store8_aligned(p,nd::mul8_fixed(nd::load8_aligned(p),vc,vcn));\n\
    \            nd::store8_aligned(p+8,nd::mul8_fixed(nd::load8_aligned(p+8),vc,vcn));\n\
    \        }\n        if(s+1<blocks) w=nd::canonicalize(nd::mul(w,ratio));\n   \
    \ }\n}\n\ntemplate<unsigned leaf_size,unsigned parallel_blocks>\ninline void cubic_leaf(mint*\
    \ EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize n,nd::word shift) noexcept{\n\
    \    leaf_products_shifted<leaf_size,parallel_blocks>(a,b,n,shift);\n    leaf_products_shifted<leaf_size,parallel_blocks>(a,b,n,shift);\n\
    \    sub_inplace(a,b,n);\n}\n\ninline void cubic_cyclic(mint* EEZ_FPS998_RESTRICT\
    \ a,mint* EEZ_FPS998_RESTRICT b,usize n) noexcept{\n    assert(n>=16);\n    assert(std::has_single_bit(n));\n\
    \    assert(n<=ntt998::max_convolution_size);\n    const unsigned leaf_log=nd::adaptive_leaf_log(n);\n\
    \    nd::forward_adaptive_pair(a,b,n,leaf_log);\n    if(leaf_log==3) cubic_leaf<8,4>(a,b,n,nd::montgomery_one);\n\
    \    else{\n        assert(leaf_log==4);\n        cubic_leaf<16,2>(a,b,n,nd::montgomery_one);\n\
    \    }\n    nd::inverse_adaptive(a,n,leaf_log);\n}\n\ninline void cubic_negacyclic(mint*\
    \ EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize n) noexcept{\n    assert(n>=16);\n\
    \    assert(std::has_single_bit(n));\n    assert(n<=ntt998::max_convolution_size);\n\
    \    const unsigned leaf_log=nd::adaptive_leaf_log(n);\n    const usize blocks=n>>leaf_log;\n\
    \    const unsigned block_log=static_cast<unsigned>(std::countr_zero(blocks));\n\
    \    assert(block_log+1<=nd::max_log);\n    const nd::word psi=nd::canonicalize(nd::twiddles.root[block_log+1]);\n\
    \    const nd::word ipsi=nd::canonicalize(nd::twiddles.iroot[block_log+1]);\n\n\
    \    if(leaf_log==3){\n        block_twist<8>(a,n,psi);\n        block_twist<8>(b,n,psi);\n\
    \    }else{\n        assert(leaf_log==4);\n        block_twist<16>(a,n,psi);\n\
    \        block_twist<16>(b,n,psi);\n    }\n\n    nd::forward_adaptive_pair(a,b,n,leaf_log);\n\
    \    if(leaf_log==3) cubic_leaf<8,4>(a,b,n,psi);\n    else cubic_leaf<16,2>(a,b,n,psi);\n\
    \    nd::inverse_adaptive(a,n,leaf_log);\n\n    if(leaf_log==3) block_twist<8>(a,n,ipsi);\n\
    \    else block_twist<16>(a,n,ipsi);\n}\n\ninline void fold_f_cyclic(std::span<const\
    \ mint> f,mint* EEZ_FPS998_RESTRICT a,usize m) noexcept{\n    const usize lo=std::min(f.size(),m);\n\
    \    copy_n(a,f.data(),lo);\n    zero_n(a+lo,m-lo);\n    if(f.size()<=m) return;\n\
    \    const usize hi=std::min(f.size()-m,m);\n    usize i=0;\n    for(;i+8<=hi;i+=8){\n\
    \        const nd::vec x=nd::shrink4_to_2(nd::load8_aligned(a+i));\n        const\
    \ nd::vec y=nd::shrink4_to_2(nd::load8(f.data()+m+i));\n        nd::store8_aligned(a+i,nd::add8(x,y));\n\
    \    }\n    for(;i<hi;++i){\n        const nd::word x=shrink4_scalar(nd::raw(a[i]));\n\
    \        const nd::word y=shrink4_scalar(nd::raw(f[m+i]));\n        a[i]=nd::from_raw(nd::add(x,y));\n\
    \    }\n}\n\ninline void recover_high(const mint* EEZ_FPS998_RESTRICT a,mint*\
    \ out,usize m,usize len) noexcept{\n    const nd::vec vhalf=nd::broadcast(inv2_mont);\n\
    \    const nd::vec vhalfn=nd::broadcast(inv2_mont*nd::montgomery_ninv);\n    usize\
    \ i=0;\n    for(;i+8<=len;i+=8){\n        const nd::vec a0=nd::shrink4_to_2(nd::load8(a+i));\n\
    \        const nd::vec a1=nd::shrink4_to_2(nd::load8(a+m+i));\n        const nd::vec\
    \ b=nd::shrink4_to_2(nd::load8(out+i));\n        nd::vec x=nd::add8(a0,a1);\n\
    \        x=nd::add8(x,b);\n        x=nd::mul8_fixed(x,vhalf,vhalfn);\n       \
    \ nd::store8(out+i,neg8(x));\n    }\n    for(;i<len;++i){\n        const nd::word\
    \ a0=shrink4_scalar(nd::raw(a[i]));\n        const nd::word a1=shrink4_scalar(nd::raw(a[m+i]));\n\
    \        const nd::word b=shrink4_scalar(nd::raw(out[i]));\n        nd::word x=nd::add(a0,a1);\n\
    \        x=nd::add(x,b);\n        x=nd::mul(x,inv2_mont);\n        out[i]=nd::from_raw(nd::sub(0,x));\n\
    \    }\n}\n\ninline void step_3half(std::span<const mint> f,std::span<mint> out,mint*\
    \ EEZ_FPS998_RESTRICT a,mint* EEZ_FPS998_RESTRICT b,usize m) noexcept{\n    const\
    \ usize n=m<<1;\n    assert(m>=16);\n    assert(std::has_single_bit(m));\n   \
    \ assert(n<=ntt998::max_convolution_size);\n    assert(out.size()>m);\n\n    const\
    \ usize len=std::min(m,out.size()-m);\n    if(!len) return;\n\n    fold_f_cyclic(f,a,m);\n\
    \    copy_n(b,out.data(),m);\n    cubic_cyclic(a,b,m);\n\n    copy_shrink4_to2(out.data()+m,a,len);\n\
    \n    const usize fn=std::min(f.size(),n);\n    copy_n(a,f.data(),fn);\n    zero_n(a+fn,n-fn);\n\
    \n    copy_n(b,out.data(),m);\n    zero_n(b+m,m);\n\n    cubic_negacyclic(a,b,n);\n\
    \    recover_high(a,out.data()+m,m,len);\n}\n}\n\n#undef EEZ_FPS998_RESTRICT\n\
    #undef EEZ_FPS998_ALWAYS_INLINE\n\n#if defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute pop\n#elif defined(__GNUC__)\
    \ && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))\n#pragma\
    \ GCC pop_options\n#endif\n#line 9 \"fps/detail/fps998_inv.hpp\"\n\n#if defined(__GNUC__)\
    \ && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))\n#pragma\
    \ GCC push_options\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt\")\n#elif defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute push(__attribute__((target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))),apply_to=function)\n#endif\n\nnamespace\
    \ eez::fps998::detail{\nusing mint=ntt998::mint;\nusing usize=std::size_t;\nnamespace\
    \ nd=ntt998::detail;\n\ninline constexpr usize inv_naive_cutoff=64;\nstatic_assert(std::has_single_bit(inv_naive_cutoff));\n\
    static_assert(std::is_trivially_copyable_v<mint>);\nstatic_assert(sizeof(mint)==sizeof(std::uint32_t));\n\
    \nstruct inv_workspace{\n    nd::aligned_vector a,b;\n    void reserve(usize n){a.reserve(n);b.reserve(n);}\n\
    \    void ensure(usize n){if(a.size()<n)a.resize(n);if(b.size()<n)b.resize(n);}\n\
    \    [[nodiscard]] usize capacity() const noexcept{return std::min(a.capacity(),b.capacity());}\n\
    };\n\ninline void inv_naive_prefix(std::span<const mint> f,std::span<mint> g){\n\
    \    if(g.empty()) return;\n    const mint g0=f[0].inv();\n    g[0]=g0;\n    for(usize\
    \ i=1;i<g.size();++i){\n        mint s{};\n        const usize r=std::min(i,f.size()-1);\n\
    \        for(usize j=1;j<=r;++j) s+=f[j]*g[i-j];\n        g[i]=-s*g0;\n    }\n\
    }\n\ninline void inv_step_3half(std::span<const mint> f,std::span<mint> out,inv_workspace&\
    \ ws,usize m){\n    const usize n=m<<1;\n    ws.ensure(n);\n    inv_ntt::step_3half(f,out,ws.a.data(),ws.b.data(),m);\n\
    }\n\ninline void inv_to_impl(std::span<const mint> f,std::span<mint> out,inv_workspace&\
    \ ws){\n    const usize target=out.size();\n    const usize base=std::min(target,inv_naive_cutoff);\n\
    \    inv_naive_prefix(f,out.first(base));\n    if(target<=inv_naive_cutoff) return;\n\
    \    ws.reserve(std::bit_ceil(target));\n    for(usize m=inv_naive_cutoff;m<target;m<<=1)\n\
    \        inv_step_3half(f,out,ws,m);\n}\n}\n\n#if defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute pop\n#elif defined(__GNUC__)\
    \ && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))\n#pragma\
    \ GCC pop_options\n#endif\n"
  code: "#pragma once\n#include <algorithm>\n#include <bit>\n#include <cstddef>\n\
    #include <cstdint>\n#include <span>\n#include <type_traits>\n#include \"fps998_inv_ntt.hpp\"\
    \n\n#if defined(__GNUC__) && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))\n\
    #pragma GCC push_options\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC\
    \ target(\"avx2,bmi,bmi2,lzcnt,popcnt\")\n#elif defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute push(__attribute__((target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))),apply_to=function)\n#endif\n\nnamespace\
    \ eez::fps998::detail{\nusing mint=ntt998::mint;\nusing usize=std::size_t;\nnamespace\
    \ nd=ntt998::detail;\n\ninline constexpr usize inv_naive_cutoff=64;\nstatic_assert(std::has_single_bit(inv_naive_cutoff));\n\
    static_assert(std::is_trivially_copyable_v<mint>);\nstatic_assert(sizeof(mint)==sizeof(std::uint32_t));\n\
    \nstruct inv_workspace{\n    nd::aligned_vector a,b;\n    void reserve(usize n){a.reserve(n);b.reserve(n);}\n\
    \    void ensure(usize n){if(a.size()<n)a.resize(n);if(b.size()<n)b.resize(n);}\n\
    \    [[nodiscard]] usize capacity() const noexcept{return std::min(a.capacity(),b.capacity());}\n\
    };\n\ninline void inv_naive_prefix(std::span<const mint> f,std::span<mint> g){\n\
    \    if(g.empty()) return;\n    const mint g0=f[0].inv();\n    g[0]=g0;\n    for(usize\
    \ i=1;i<g.size();++i){\n        mint s{};\n        const usize r=std::min(i,f.size()-1);\n\
    \        for(usize j=1;j<=r;++j) s+=f[j]*g[i-j];\n        g[i]=-s*g0;\n    }\n\
    }\n\ninline void inv_step_3half(std::span<const mint> f,std::span<mint> out,inv_workspace&\
    \ ws,usize m){\n    const usize n=m<<1;\n    ws.ensure(n);\n    inv_ntt::step_3half(f,out,ws.a.data(),ws.b.data(),m);\n\
    }\n\ninline void inv_to_impl(std::span<const mint> f,std::span<mint> out,inv_workspace&\
    \ ws){\n    const usize target=out.size();\n    const usize base=std::min(target,inv_naive_cutoff);\n\
    \    inv_naive_prefix(f,out.first(base));\n    if(target<=inv_naive_cutoff) return;\n\
    \    ws.reserve(std::bit_ceil(target));\n    for(usize m=inv_naive_cutoff;m<target;m<<=1)\n\
    \        inv_step_3half(f,out,ws,m);\n}\n}\n\n#if defined(__clang__) && (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute pop\n#elif defined(__GNUC__)\
    \ && !defined(__clang__) && (defined(__x86_64__) || defined(__i386__))\n#pragma\
    \ GCC pop_options\n#endif\n"
  dependsOn:
  - fps/detail/fps998_inv_ntt.hpp
  - convolution/ntt998.hpp
  - math/modint998.hpp
  isVerificationFile: false
  path: fps/detail/fps998_inv.hpp
  requiredBy:
  - fps/fps998.hpp
  timestamp: '2026-09-07 02:49:04+09:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/verify-yosupo-fps/yosupo-inv-of-formal-power-series-fps998.test.cpp
documentation_of: fps/detail/fps998_inv.hpp
layout: document
redirect_from:
- /library/fps/detail/fps998_inv.hpp
- /library/fps/detail/fps998_inv.hpp.html
title: fps/detail/fps998_inv.hpp
---

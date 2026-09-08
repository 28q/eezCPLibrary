---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: true
  _pathExtension: cpp
  _verificationStatusIcon: ':x:'
  attributes: {}
  bundledCode: "#line 1 \"verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/convolution_mod_large\"\n\n\
    #ifndef FASTIO_UNSAFE_BLOCK_LOG\n#define FASTIO_UNSAFE_BLOCK_LOG 12\n#endif\n\n\
    #include <bit>\n#include <cstddef>\n#include <vector>\n\n#line 2 \"convolution/ntt998.hpp\"\
    \n#if !defined(__AVX2__) && !defined(_M_AVX2)\r\n#error \"Compile with -mavx2\
    \ (see README.md).\"\r\n#endif\r\n#line 2 \"ntt998.hpp\"\r\n#if defined(__GNUC__)\
    \ && !defined(__clang__) && \\\r\n    (defined(__x86_64__) || defined(__i386__))\r\
    \n#pragma GCC optimize(\"O3,unroll-loops\")\r\n#pragma GCC target(\"avx2,bmi,bmi2,lzcnt,popcnt\"\
    )\r\n#elif defined(__clang__) && \\\r\n    (defined(__x86_64__) || defined(__i386__))\r\
    \n#pragma clang attribute push( \\\r\n    __attribute__((target(\"avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"\
    ))), \\\r\n    apply_to = function)\r\n#endif\r\n#include <bits/stdc++.h>\r\n\
    #include <immintrin.h>\r\n#line 1 \"math/modint998.hpp\"\r\n#include <type_traits>\r\
    \nstruct modint998 {\r\n    using u32 = std::uint32_t;\r\n    using i32 = std::int32_t;\r\
    \n    using u64 = std::uint64_t;\r\n    static constexpr u32 MOD = 998244353u;\r\
    \n    static constexpr u32 MOD2 = MOD * 2;\r\n    static constexpr u32 primitive_root\
    \ = 3;\r\n    static constexpr int max_power_of_two = 23;\r\nprivate:\r\n    static\
    \ constexpr u32 R = 3296722945u;\r\n    static constexpr u32 N2 = 932051910u;\r\
    \n    struct montgomery_tag {};\r\n    constexpr modint998(u32 x, montgomery_tag)\
    \ : a(x) {}\r\n    static constexpr u32 reduce(u64 x) {\r\n        return static_cast<u32>(\r\
    \n            (x + u64(static_cast<u32>(x) * u32(-R)) * MOD) >> 32\r\n       \
    \ );\r\n    }\r\npublic:\r\n    u32 a;\r\n    static_assert(MOD < (u32(1) << 30));\r\
    \n    static_assert((MOD & 1) != 0);\r\n    static_assert(R * MOD == 1);\r\n \
    \   constexpr modint998() : a(0) {}\r\n    template <class T, std::enable_if_t<std::is_integral_v<T>\
    \ &&\r\n                                        std::is_signed_v<T>, int> = 0>\r\
    \n    constexpr modint998(T x) : a(0) {\r\n        const std::int64_t y =\r\n\
    \            static_cast<std::int64_t>(x) % std::int64_t(MOD) + MOD;\r\n     \
    \   a = reduce(u64(y) * N2);\r\n    }\r\n    template <class T, std::enable_if_t<std::is_integral_v<T>\
    \ &&\r\n                                        std::is_unsigned_v<T>, int> =\
    \ 0>\r\n    constexpr modint998(T x)\r\n        : a(reduce(((u64(x) % MOD) + MOD)\
    \ * N2)) {}\r\n    static constexpr modint998 raw(u32 x) {\r\n        return modint998(reduce(u64(x)\
    \ * N2), montgomery_tag{});\r\n    }\r\n    static constexpr modint998 montgomery_raw(u32\
    \ x) {\r\n        return modint998(x, montgomery_tag{});\r\n    }\r\n    static\
    \ constexpr u32 mod() { return MOD; }\r\n    static constexpr u32 get_mod() {\
    \ return MOD; }\r\n    constexpr u32 val() const {\r\n        const u32 x = reduce(a);\r\
    \n        return x >= MOD ? x - MOD : x;\r\n    }\r\n    constexpr u32 get() const\
    \ { return val(); }\r\n    constexpr modint998& operator+=(const modint998& rhs)\
    \ {\r\n        a += rhs.a - MOD2;\r\n        if (i32(a) < 0) a += MOD2;\r\n  \
    \      return *this;\r\n    }\r\n    constexpr modint998& operator-=(const modint998&\
    \ rhs) {\r\n        a -= rhs.a;\r\n        if (i32(a) < 0) a += MOD2;\r\n    \
    \    return *this;\r\n    }\r\n    constexpr modint998& operator*=(const modint998&\
    \ rhs) {\r\n        a = reduce(u64(a) * rhs.a);\r\n        return *this;\r\n \
    \   }\r\n    constexpr modint998& operator/=(const modint998& rhs) {\r\n     \
    \   return *this *= rhs.inv();\r\n    }\r\n    constexpr modint998 operator+()\
    \ const { return *this; }\r\n    constexpr modint998 operator-() const { return\
    \ modint998() - *this; }\r\n    friend constexpr modint998 operator+(modint998\
    \ lhs, const modint998& rhs) {\r\n        return lhs += rhs;\r\n    }\r\n    friend\
    \ constexpr modint998 operator-(modint998 lhs, const modint998& rhs) {\r\n   \
    \     return lhs -= rhs;\r\n    }\r\n    friend constexpr modint998 operator*(modint998\
    \ lhs, const modint998& rhs) {\r\n        return lhs *= rhs;\r\n    }\r\n    friend\
    \ constexpr modint998 operator/(modint998 lhs, const modint998& rhs) {\r\n   \
    \     return lhs /= rhs;\r\n    }\r\n    friend constexpr bool operator==(const\
    \ modint998& lhs, const modint998& rhs) {\r\n        const u32 x = lhs.a >= MOD\
    \ ? lhs.a - MOD : lhs.a;\r\n        const u32 y = rhs.a >= MOD ? rhs.a - MOD :\
    \ rhs.a;\r\n        return x == y;\r\n    }\r\n    friend constexpr bool operator!=(const\
    \ modint998& lhs, const modint998& rhs) {\r\n        return !(lhs == rhs);\r\n\
    \    }\r\n    constexpr modint998& operator++() {\r\n        return *this += raw(1);\r\
    \n    }\r\n    constexpr modint998 operator++(int) {\r\n        modint998 old\
    \ = *this;\r\n        ++*this;\r\n        return old;\r\n    }\r\n    constexpr\
    \ modint998& operator--() {\r\n        return *this -= raw(1);\r\n    }\r\n  \
    \  constexpr modint998 operator--(int) {\r\n        modint998 old = *this;\r\n\
    \        --*this;\r\n        return old;\r\n    }\r\n    constexpr modint998 pow(u64\
    \ exponent) const {\r\n        modint998 result = raw(1);\r\n        modint998\
    \ base = *this;\r\n        while (exponent != 0) {\r\n            if (exponent\
    \ & 1) result *= base;\r\n            base *= base;\r\n            exponent >>=\
    \ 1;\r\n        }\r\n        return result;\r\n    }\r\n    constexpr modint998\
    \ inv() const {\r\n        assert(val() != 0);\r\n        const modint998 x =\
    \ *this;\r\n        modint998 a2 = x * x;\r\n        modint998 a4 = a2 * a2;\r\
    \n        modint998 a8 = a4 * a4;\r\n        modint998 a16 = a8 * a8;\r\n    \
    \    modint998 a32 = a16 * a16;\r\n        modint998 a33 = a32 * x;\r\n      \
    \  modint998 a49 = a33 * a16;\r\n        modint998 a82 = a49 * a33;\r\n      \
    \  modint998 a164 = a82 * a82;\r\n        modint998 a328 = a164 * a164;\r\n  \
    \      modint998 r = a328 * a49;\r\n        a2 = r * r;\r\n        a4 = a2 * a2;\r\
    \n        a8 = a4 * a4;\r\n        modint998 a9 = a8 * r;\r\n        modint998\
    \ a18 = a9 * a9;\r\n        r = a18 * a9;\r\n        a2 = r * r;\r\n        a4\
    \ = a2 * a2;\r\n        a8 = a4 * a4;\r\n        a16 = a8 * a8;\r\n        modint998\
    \ a17 = a16 * r;\r\n        a33 = a17 * a16;\r\n        modint998 a66 = a33 *\
    \ a33;\r\n        modint998 a132 = a66 * a66;\r\n        modint998 a264 = a132\
    \ * a132;\r\n        r = a264 * a17;\r\n        a2 = r * r;\r\n        a4 = a2\
    \ * a2;\r\n        a8 = a4 * a4;\r\n        a16 = a8 * a8;\r\n        a32 = a16\
    \ * a16;\r\n        modint998 a64 = a32 * a32;\r\n        modint998 a65 = a64\
    \ * r;\r\n        modint998 a73 = a65 * a8;\r\n        modint998 a138 = a73 *\
    \ a65;\r\n        modint998 a276 = a138 * a138;\r\n        return a276 * a73;\r\
    \n    }\r\n    constexpr modint998 inverse() const { return inv(); }\r\n    friend\
    \ std::ostream& operator<<(std::ostream& os, const modint998& x) {\r\n       \
    \ return os << x.val();\r\n    }\r\n    friend std::istream& operator>>(std::istream&\
    \ is, modint998& x) {\r\n        std::int64_t value;\r\n        is >> value;\r\
    \n        x = modint998(value);\r\n        return is;\r\n    }\r\n};\r\nstatic_assert(sizeof(modint998)\
    \ == 4);\r\nstatic_assert(std::is_trivially_copyable_v<modint998>);\r\nusing mint998\
    \ = modint998;\r\n#line 18 \"ntt998.hpp\"\r\n#if defined(_MSC_VER)\r\n#define\
    \ EEZ_NTT998_ALWAYS_INLINE __forceinline\r\n#define EEZ_NTT998_RESTRICT __restrict\r\
    \n#elif defined(__GNUC__) || defined(__clang__)\r\n#define EEZ_NTT998_ALWAYS_INLINE\
    \ inline __attribute__((always_inline))\r\n#define EEZ_NTT998_RESTRICT __restrict__\r\
    \n#else\r\n#define EEZ_NTT998_ALWAYS_INLINE inline\r\n#define EEZ_NTT998_RESTRICT\r\
    \n#endif\r\nnamespace eez::ntt998{\r\nusing mint=modint998;\r\nusing u32=std::uint32_t;\r\
    \nusing usize=std::size_t;\r\ninline constexpr u32 mod=mint::MOD;\r\ninline constexpr\
    \ usize max_ntt_size=usize(1)<<23;\r\ninline constexpr usize max_convolution_size=usize(1)<<25;\r\
    \ninline constexpr usize max_size=max_ntt_size;\r\ninline constexpr usize naive_cutoff=60;\r\
    \ninline void forward(std::span<mint> a) noexcept;\r\ninline void inverse(std::span<mint>\
    \ a) noexcept;\r\ninline std::vector<mint> convolution(std::span<const mint> a,std::span<const\
    \ mint> b);\r\ninline std::vector<mint> square(std::span<const mint> a);\r\ninline\
    \ std::vector<mint> convolution(const std::vector<mint>& a,const std::vector<mint>&\
    \ b){\r\n    return convolution(std::span<const mint>(a.data(),a.size()),std::span<const\
    \ mint>(b.data(),b.size()));\r\n}\r\ninline std::vector<mint> square(const std::vector<mint>&\
    \ a){\r\n    return square(std::span<const mint>(a.data(),a.size()));\r\n}\r\n\
    namespace detail{\r\ntemplate<class T>\r\nclass aligned_allocator{\r\npublic:\r\
    \n    using value_type=T;\r\n    using is_always_equal=std::true_type;\r\n   \
    \ aligned_allocator() noexcept=default;\r\n    template<class U> constexpr aligned_allocator(const\
    \ aligned_allocator<U>&) noexcept{}\r\n    [[nodiscard]] T* allocate(usize n){\r\
    \n        return static_cast<T*>(::operator new(n*sizeof(T),std::align_val_t{64}));\r\
    \n    }\r\n    void deallocate(T* p,usize) noexcept{\r\n        ::operator delete(p,std::align_val_t{64});\r\
    \n    }\r\n    template<class U> struct rebind{using other=aligned_allocator<U>;};\r\
    \n};\r\ntemplate<class T,class U>\r\nconstexpr bool operator==(const aligned_allocator<T>&,const\
    \ aligned_allocator<U>&) noexcept{return true;}\r\ntemplate<class T,class U>\r\
    \nconstexpr bool operator!=(const aligned_allocator<T>&,const aligned_allocator<U>&)\
    \ noexcept{return false;}\r\nusing aligned_vector=std::vector<mint,aligned_allocator<mint>>;\r\
    \n}\r\nclass workspace{\r\npublic:\r\n    workspace()=default;\r\n    explicit\
    \ workspace(usize n){reserve(n);}\r\n    void reserve(usize n){\r\n        if(a_.size()<n)a_.resize(n);\r\
    \n        if(b_.size()<n)b_.resize(n);\r\n    }\r\n    [[nodiscard]] usize capacity()const\
    \ noexcept{return std::min(a_.size(),b_.size());}\r\nprivate:\r\n    friend void\
    \ convolution_to(std::span<const mint>,std::span<const mint>,std::span<mint>,workspace&);\r\
    \n    friend void square_to(std::span<const mint>,std::span<mint>,workspace&);\r\
    \n    detail::aligned_vector a_;\r\n    detail::aligned_vector b_;\r\n};\r\ninline\
    \ void convolution_to(std::span<const mint> a,std::span<const mint> b,std::span<mint>\
    \ out,workspace& ws);\r\ninline void square_to(std::span<const mint> a,std::span<mint>\
    \ out,workspace& ws);\r\nclass frequency_buffer{\r\npublic:\r\n    frequency_buffer()=default;\r\
    \n    [[nodiscard]] usize size()const noexcept{return data_.size();}\r\nprivate:\r\
    \n    friend void forward_to(std::span<const mint>,frequency_buffer&,usize);\r\
    \n    friend void pointwise_multiply(frequency_buffer&,const frequency_buffer&);\r\
    \n    friend void pointwise_square(frequency_buffer&);\r\n    friend void inverse_to(frequency_buffer&,std::span<mint>);\r\
    \n    std::vector<mint> data_;\r\n};\r\ninline void forward_to(std::span<const\
    \ mint> src,frequency_buffer& dst,usize n);\r\ninline void pointwise_multiply(frequency_buffer&\
    \ lhs,const frequency_buffer& rhs);\r\ninline void pointwise_square(frequency_buffer&\
    \ a);\r\ninline void inverse_to(frequency_buffer& src,std::span<mint> out);\r\n\
    constexpr usize convolution_size(usize n,usize m) noexcept{\r\n    return n&&m?n+m-1:0;\r\
    \n}\r\nconstexpr usize transform_size(usize n,usize m) noexcept{\r\n    if(!n||!m)return\
    \ 0;\r\n    if(n>max_ntt_size||m>max_ntt_size)return 0;\r\n    if(n>max_ntt_size-m+1)return\
    \ 0;\r\n    const usize z=n+m-1;\r\n    usize x=1;\r\n    while(x<z)x<<=1;\r\n\
    \    return x;\r\n}\r\nconstexpr usize convolution_transform_size(usize n,usize\
    \ m) noexcept{\r\n    if(!n||!m)return 0;\r\n    if(n>max_convolution_size||m>max_convolution_size)return\
    \ 0;\r\n    if(n>max_convolution_size-m+1)return 0;\r\n    const usize z=n+m-1;\r\
    \n    usize x=1;\r\n    while(x<z)x<<=1;\r\n    return x;\r\n}\r\nconstexpr bool\
    \ valid_ntt_size(usize n) noexcept{\r\n    return n!=0&&(n&(n-1))==0&&n<=max_ntt_size;\r\
    \n}\r\nconstexpr bool valid_convolution_transform_size(usize n) noexcept{\r\n\
    \    return n>=32&&(n&(n-1))==0&&n<=max_convolution_size;\r\n}\r\nnamespace detail{\r\
    \nusing word=u32;\r\nusing u64=std::uint64_t;\r\ninline constexpr word mod=mint::MOD;\r\
    \ninline constexpr word mod2=2*mod;\r\ninline constexpr unsigned max_log=23;\r\
    \ninline constexpr word montgomery_ninv=998244351u;\r\ninline constexpr word montgomery_one=mint::raw(1).a;\r\
    \nstatic_assert(mod<(word(1)<<30));\r\nstatic_assert(word(mod*montgomery_ninv)==~word(0));\r\
    \nstatic_assert(sizeof(mint)==sizeof(word));\r\nEEZ_NTT998_ALWAYS_INLINE constexpr\
    \ word raw(const mint& x) noexcept{return x.a;}\r\nEEZ_NTT998_ALWAYS_INLINE constexpr\
    \ mint from_raw(word x) noexcept{return mint::montgomery_raw(x);}\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ constexpr word mul(word a,word b) noexcept{\r\n    const u64 x=u64(a)*b;\r\n\
    \    const word q=static_cast<word>(x)*montgomery_ninv;\r\n    return static_cast<word>((x+u64(q)*mod)>>32);\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE constexpr word add(word a,word b) noexcept{\r\n\
    \    const word x=a+b;\r\n    return x>=mod2?x-mod2:x;\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ constexpr word sub(word a,word b) noexcept{\r\n    return a>=b?a-b:a+mod2-b;\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE constexpr word canonicalize(word a) noexcept{\r\
    \n    return a>=mod?a-mod:a;\r\n}\r\nstruct twiddle_table{\r\n    std::array<word,max_log+1>\
    \ root{};\r\n    std::array<word,max_log+1> iroot{};\r\n    std::array<word,max_log+1>\
    \ rate1{};\r\n    std::array<word,max_log+1> rate3{};\r\n    std::array<word,max_log+1>\
    \ irate3{};\r\n    constexpr twiddle_table(){\r\n        root[max_log]=mint::raw(mint::primitive_root).pow((mod-1)>>max_log).a;\r\
    \n        iroot[max_log]=mint::montgomery_raw(root[max_log]).inv().a;\r\n    \
    \    for(int i=int(max_log)-1;i>=0;--i){\r\n            root[usize(i)]=mul(root[usize(i+1)],root[usize(i+1)]);\r\
    \n            iroot[usize(i)]=mul(iroot[usize(i+1)],iroot[usize(i+1)]);\r\n  \
    \      }\r\n        word prod=montgomery_one;\r\n        for(unsigned i=0;i+1<=max_log;++i){\r\
    \n            rate1[i]=mul(root[i+1],prod);\r\n            prod=mul(prod,iroot[i+1]);\r\
    \n        }\r\n        prod=montgomery_one;\r\n        word iprod=montgomery_one;\r\
    \n        for(unsigned i=0;i+3<=max_log;++i){\r\n            rate3[i]=mul(root[i+3],prod);\r\
    \n            irate3[i]=mul(iroot[i+3],iprod);\r\n            prod=mul(prod,iroot[i+3]);\r\
    \n            iprod=mul(iprod,root[i+3]);\r\n        }\r\n    }\r\n};\r\ninline\
    \ constexpr twiddle_table twiddles{};\r\nEEZ_NTT998_ALWAYS_INLINE word forward_rate1(unsigned\
    \ i) noexcept{return twiddles.rate1[i];}\r\nEEZ_NTT998_ALWAYS_INLINE word forward_rate3(unsigned\
    \ i) noexcept{return twiddles.rate3[i];}\r\nEEZ_NTT998_ALWAYS_INLINE word inverse_rate3(unsigned\
    \ i) noexcept{return twiddles.irate3[i];}\r\nEEZ_NTT998_ALWAYS_INLINE unsigned\
    \ twiddle_index(u32 block) noexcept{\r\n    return static_cast<unsigned>(std::countr_zero(~block));\r\
    \n}\r\n#if defined(__AVX2__) || defined(_M_AVX2)\r\nusing vec=__m256i;\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ vec load8(const mint* p) noexcept{\r\n    return _mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(static_cast<const void*>(p)));\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE void\
    \ store8(mint* p,vec x) noexcept{\r\n    _mm256_storeu_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE vec broadcast(word x) noexcept{\r\n    return\
    \ _mm256_set1_epi32(static_cast<int>(x));\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE vec\
    \ add8(vec a,vec b) noexcept{\r\n    const vec two_p=broadcast(mod2);\r\n    vec\
    \ x=_mm256_sub_epi32(_mm256_add_epi32(a,b),two_p);\r\n    return _mm256_add_epi32(x,_mm256_and_si256(_mm256_srai_epi32(x,31),two_p));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE vec sub8(vec a,vec b) noexcept{\r\n    const vec\
    \ two_p=broadcast(mod2);\r\n    vec x=_mm256_sub_epi32(a,b);\r\n    return _mm256_add_epi32(x,_mm256_and_si256(_mm256_srai_epi32(x,31),two_p));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE vec mul8(vec a,vec b) noexcept{\r\n    const vec\
    \ ninv=broadcast(montgomery_ninv);\r\n    const vec prime=broadcast(mod);\r\n\
    \    const vec pe=_mm256_mul_epu32(a,b);\r\n    const vec po=_mm256_mul_epu32(_mm256_bsrli_epi128(a,4),_mm256_bsrli_epi128(b,4));\r\
    \n    const vec qe=_mm256_mul_epu32(pe,ninv);\r\n    const vec qo=_mm256_mul_epu32(po,ninv);\r\
    \n    const vec re=_mm256_add_epi64(pe,_mm256_mul_epu32(qe,prime));\r\n    const\
    \ vec ro=_mm256_add_epi64(po,_mm256_mul_epu32(qo,prime));\r\n    return _mm256_or_si256(_mm256_bsrli_epi128(re,4),ro);\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE vec mul8_fixed(vec a,vec b,vec bninv) noexcept{\r\
    \n    const vec prime=broadcast(mod);\r\n    const vec oa=_mm256_bsrli_epi128(a,4);\r\
    \n    const vec pe=_mm256_mul_epu32(a,b);\r\n    const vec po=_mm256_mul_epu32(oa,b);\r\
    \n    const vec qe=_mm256_mul_epu32(a,bninv);\r\n    const vec qo=_mm256_mul_epu32(oa,bninv);\r\
    \n    const vec re=_mm256_add_epi64(pe,_mm256_mul_epu32(qe,prime));\r\n    const\
    \ vec ro=_mm256_add_epi64(po,_mm256_mul_epu32(qo,prime));\r\n    return _mm256_or_si256(_mm256_bsrli_epi128(re,4),ro);\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE vec canonicalize8(vec x) noexcept{\r\n    const\
    \ vec prime=broadcast(mod);\r\n    vec y=_mm256_sub_epi32(x,prime);\r\n    return\
    \ _mm256_add_epi32(y,_mm256_and_si256(_mm256_srai_epi32(y,31),prime));\r\n}\r\n\
    EEZ_NTT998_ALWAYS_INLINE vec pack_four(word x0,word x1) noexcept{\r\n    return\
    \ _mm256_setr_epi32(\r\n        static_cast<int>(x0),static_cast<int>(x0),static_cast<int>(x0),static_cast<int>(x0),\r\
    \n        static_cast<int>(x1),static_cast<int>(x1),static_cast<int>(x1),static_cast<int>(x1)\r\
    \n    );\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE vec load2x4(const mint* p0,const mint*\
    \ p1) noexcept{\r\n    const __m128i lo=_mm_loadu_si128(reinterpret_cast<const\
    \ __m128i*>(static_cast<const void*>(p0)));\r\n    const __m128i hi=_mm_loadu_si128(reinterpret_cast<const\
    \ __m128i*>(static_cast<const void*>(p1)));\r\n    return _mm256_set_m128i(hi,lo);\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE void store2x4(mint* p0,mint* p1,vec x) noexcept{\r\
    \n    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p0)),_mm256_castsi256_si128(x));\r\
    \n    _mm_storeu_si128(reinterpret_cast<__m128i*>(static_cast<void*>(p1)),_mm256_extracti128_si256(x,1));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE void transpose_8x4_to_4x8(vec v0,vec v1,vec v2,vec\
    \ v3,vec& x0,vec& x1,vec& x2,vec& x3) noexcept{\r\n    const vec t0=_mm256_unpacklo_epi32(v0,v1);\r\
    \n    const vec t1=_mm256_unpackhi_epi32(v0,v1);\r\n    const vec t2=_mm256_unpacklo_epi32(v2,v3);\r\
    \n    const vec t3=_mm256_unpackhi_epi32(v2,v3);\r\n    const vec perm=_mm256_setr_epi32(0,4,1,5,2,6,3,7);\r\
    \n    x0=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t0,t2),perm);\r\n \
    \   x1=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t0,t2),perm);\r\n   \
    \ x2=_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(t1,t3),perm);\r\n    x3=_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(t1,t3),perm);\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE void transpose_4x8_to_8x4(vec x0,vec x1,vec x2,vec\
    \ x3,vec& v0,vec& v1,vec& v2,vec& v3) noexcept{\r\n    const vec perm=_mm256_setr_epi32(0,2,4,6,1,3,5,7);\r\
    \n    const vec q0=_mm256_permutevar8x32_epi32(x0,perm);\r\n    const vec q1=_mm256_permutevar8x32_epi32(x1,perm);\r\
    \n    const vec q2=_mm256_permutevar8x32_epi32(x2,perm);\r\n    const vec q3=_mm256_permutevar8x32_epi32(x3,perm);\r\
    \n    const vec t0=_mm256_unpacklo_epi64(q0,q1);\r\n    const vec t2=_mm256_unpackhi_epi64(q0,q1);\r\
    \n    const vec t1=_mm256_unpacklo_epi64(q2,q3);\r\n    const vec t3=_mm256_unpackhi_epi64(q2,q3);\r\
    \n    v0=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(2,0,2,0)));\r\
    \n    v1=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t0),_mm256_castsi256_ps(t1),_MM_SHUFFLE(3,1,3,1)));\r\
    \n    v2=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(2,0,2,0)));\r\
    \n    v3=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(t2),_mm256_castsi256_ps(t3),_MM_SHUFFLE(3,1,3,1)));\r\
    \n}\r\n#endif\r\nEEZ_NTT998_ALWAYS_INLINE void forward_butterfly(mint* b,usize\
    \ stride,usize i,word r1,word r2,word r3) noexcept{\r\n    const word x0=raw(b[i]);\r\
    \n    const word x1=mul(raw(b[stride+i]),r1);\r\n    const word x2=mul(raw(b[2*stride+i]),r2);\r\
    \n    const word x3=mul(raw(b[3*stride+i]),r3);\r\n    const word s02=add(x0,x2);\r\
    \n    const word d02=sub(x0,x2);\r\n    const word s13=add(x1,x3);\r\n    const\
    \ word t=mul(sub(x1,x3),twiddles.root[2]);\r\n    b[i]=from_raw(add(s02,s13));\r\
    \n    b[stride+i]=from_raw(sub(s02,s13));\r\n    b[2*stride+i]=from_raw(add(d02,t));\r\
    \n    b[3*stride+i]=from_raw(sub(d02,t));\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE void\
    \ inverse_butterfly(mint* b,usize stride,usize i,word r1,word r2,word r3) noexcept{\r\
    \n    const word x0=raw(b[i]);\r\n    const word x1=raw(b[stride+i]);\r\n    const\
    \ word x2=raw(b[2*stride+i]);\r\n    const word x3=raw(b[3*stride+i]);\r\n   \
    \ const word s01=add(x0,x1);\r\n    const word d01=sub(x0,x1);\r\n    const word\
    \ s23=add(x2,x3);\r\n    const word t=mul(sub(x2,x3),twiddles.iroot[2]);\r\n \
    \   b[i]=from_raw(add(s01,s23));\r\n    b[stride+i]=from_raw(mul(add(d01,t),r1));\r\
    \n    b[2*stride+i]=from_raw(mul(sub(s01,s23),r2));\r\n    b[3*stride+i]=from_raw(mul(sub(d01,t),r3));\r\
    \n}\r\ninline void forward_radix4_scalar(mint* EEZ_NTT998_RESTRICT a,usize blocks,usize\
    \ stride) noexcept{\r\n    {\r\n        mint* const b=a;\r\n        for(usize\
    \ i=0;i<stride;++i){\r\n            const word x0=raw(b[i]);\r\n            const\
    \ word x1=raw(b[stride+i]);\r\n            const word x2=raw(b[2*stride+i]);\r\
    \n            const word x3=raw(b[3*stride+i]);\r\n            const word s02=add(x0,x2);\r\
    \n            const word d02=sub(x0,x2);\r\n            const word s13=add(x1,x3);\r\
    \n            const word t=mul(sub(x1,x3),twiddles.root[2]);\r\n            b[i]=from_raw(add(s02,s13));\r\
    \n            b[stride+i]=from_raw(sub(s02,s13));\r\n            b[2*stride+i]=from_raw(add(d02,t));\r\
    \n            b[3*stride+i]=from_raw(sub(d02,t));\r\n        }\r\n    }\r\n  \
    \  if(blocks==1)return;\r\n    word rot=forward_rate3(0);\r\n    for(usize s=1;s<blocks;++s){\r\
    \n        const word rot2=mul(rot,rot);\r\n        const word rot3=mul(rot2,rot);\r\
    \n        mint* const b=a+s*4*stride;\r\n        for(usize i=0;i<stride;++i)forward_butterfly(b,stride,i,rot,rot2,rot3);\r\
    \n        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\ninline void inverse_radix4_scalar(mint* EEZ_NTT998_RESTRICT a,usize\
    \ blocks,usize stride) noexcept{\r\n    {\r\n        mint* const b=a;\r\n    \
    \    for(usize i=0;i<stride;++i){\r\n            const word x0=raw(b[i]);\r\n\
    \            const word x1=raw(b[stride+i]);\r\n            const word x2=raw(b[2*stride+i]);\r\
    \n            const word x3=raw(b[3*stride+i]);\r\n            const word s01=add(x0,x1);\r\
    \n            const word d01=sub(x0,x1);\r\n            const word s23=add(x2,x3);\r\
    \n            const word t=mul(sub(x2,x3),twiddles.iroot[2]);\r\n            b[i]=from_raw(add(s01,s23));\r\
    \n            b[stride+i]=from_raw(add(d01,t));\r\n            b[2*stride+i]=from_raw(sub(s01,s23));\r\
    \n            b[3*stride+i]=from_raw(sub(d01,t));\r\n        }\r\n    }\r\n  \
    \  if(blocks==1)return;\r\n    word rot=inverse_rate3(0);\r\n    for(usize s=1;s<blocks;++s){\r\
    \n        const word rot2=mul(rot,rot);\r\n        const word rot3=mul(rot2,rot);\r\
    \n        mint* const b=a+s*4*stride;\r\n        for(usize i=0;i<stride;++i)inverse_butterfly(b,stride,i,rot,rot2,rot3);\r\
    \n        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\n#if defined(__AVX2__) || defined(_M_AVX2)\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ void forward_radix4_large_block(mint* EEZ_NTT998_RESTRICT b,usize stride,vec\
    \ imag,word r1,word r2,word r3) noexcept{\r\n    const vec w1=broadcast(r1);\r\
    \n    const vec w2=broadcast(r2);\r\n    const vec w3=broadcast(r3);\r\n    for(usize\
    \ i=0;i<stride;i+=8){\r\n        const vec x0=load8(b+i);\r\n        const vec\
    \ x1=mul8(load8(b+stride+i),w1);\r\n        const vec x2=mul8(load8(b+2*stride+i),w2);\r\
    \n        const vec x3=mul8(load8(b+3*stride+i),w3);\r\n        const vec s02=add8(x0,x2);\r\
    \n        const vec d02=sub8(x0,x2);\r\n        const vec s13=add8(x1,x3);\r\n\
    \        const vec t=mul8(sub8(x1,x3),imag);\r\n        store8(b+i,add8(s02,s13));\r\
    \n        store8(b+stride+i,sub8(s02,s13));\r\n        store8(b+2*stride+i,add8(d02,t));\r\
    \n        store8(b+3*stride+i,sub8(d02,t));\r\n    }\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ void inverse_radix4_large_block(mint* EEZ_NTT998_RESTRICT b,usize stride,vec\
    \ iimag,word r1,word r2,word r3) noexcept{\r\n    const vec w1=broadcast(r1);\r\
    \n    const vec w2=broadcast(r2);\r\n    const vec w3=broadcast(r3);\r\n    for(usize\
    \ i=0;i<stride;i+=8){\r\n        const vec x0=load8(b+i);\r\n        const vec\
    \ x1=load8(b+stride+i);\r\n        const vec x2=load8(b+2*stride+i);\r\n     \
    \   const vec x3=load8(b+3*stride+i);\r\n        const vec s01=add8(x0,x1);\r\n\
    \        const vec d01=sub8(x0,x1);\r\n        const vec s23=add8(x2,x3);\r\n\
    \        const vec t=mul8(sub8(x2,x3),iimag);\r\n        store8(b+i,add8(s01,s23));\r\
    \n        store8(b+stride+i,mul8(add8(d01,t),w1));\r\n        store8(b+2*stride+i,mul8(sub8(s01,s23),w2));\r\
    \n        store8(b+3*stride+i,mul8(sub8(d01,t),w3));\r\n    }\r\n}\r\ninline void\
    \ forward_radix4_large(mint* EEZ_NTT998_RESTRICT a,usize blocks,usize stride)\
    \ noexcept{\r\n    const vec imag=broadcast(twiddles.root[2]);\r\n    {\r\n  \
    \      mint* const b=a;\r\n        for(usize i=0;i<stride;i+=8){\r\n         \
    \   const vec x0=load8(b+i);\r\n            const vec x1=load8(b+stride+i);\r\n\
    \            const vec x2=load8(b+2*stride+i);\r\n            const vec x3=load8(b+3*stride+i);\r\
    \n            const vec s02=add8(x0,x2);\r\n            const vec d02=sub8(x0,x2);\r\
    \n            const vec s13=add8(x1,x3);\r\n            const vec t=mul8(sub8(x1,x3),imag);\r\
    \n            store8(b+i,add8(s02,s13));\r\n            store8(b+stride+i,sub8(s02,s13));\r\
    \n            store8(b+2*stride+i,add8(d02,t));\r\n            store8(b+3*stride+i,sub8(d02,t));\r\
    \n        }\r\n    }\r\n    if(blocks==1)return;\r\n    word rot=forward_rate3(0);\r\
    \n    usize s=1;\r\n    for(;s+8<=blocks;s+=8){\r\n        alignas(32) word r1[8],r2[8],r3[8];\r\
    \n        for(unsigned lane=0;lane<8;++lane){\r\n            r1[lane]=rot;\r\n\
    \            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));\r\
    \n        }\r\n        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));\r\
    \n        const vec w2=mul8(w1,w1);\r\n        const vec w3=mul8(w2,w1);\r\n \
    \       _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);\r\n        _mm256_store_si256(reinterpret_cast<vec*>(r3),w3);\r\
    \n        for(unsigned lane=0;lane<8;++lane)forward_radix4_large_block(a+(s+lane)*4*stride,stride,imag,r1[lane],r2[lane],r3[lane]);\r\
    \n    }\r\n    for(;s<blocks;++s){\r\n        const word rot2=mul(rot,rot);\r\n\
    \        const word rot3=mul(rot2,rot);\r\n        forward_radix4_large_block(a+s*4*stride,stride,imag,rot,rot2,rot3);\r\
    \n        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\ninline void inverse_radix4_large(mint* EEZ_NTT998_RESTRICT a,usize\
    \ blocks,usize stride) noexcept{\r\n    const vec iimag=broadcast(twiddles.iroot[2]);\r\
    \n    {\r\n        mint* const b=a;\r\n        for(usize i=0;i<stride;i+=8){\r\
    \n            const vec x0=load8(b+i);\r\n            const vec x1=load8(b+stride+i);\r\
    \n            const vec x2=load8(b+2*stride+i);\r\n            const vec x3=load8(b+3*stride+i);\r\
    \n            const vec s01=add8(x0,x1);\r\n            const vec d01=sub8(x0,x1);\r\
    \n            const vec s23=add8(x2,x3);\r\n            const vec t=mul8(sub8(x2,x3),iimag);\r\
    \n            store8(b+i,add8(s01,s23));\r\n            store8(b+stride+i,add8(d01,t));\r\
    \n            store8(b+2*stride+i,sub8(s01,s23));\r\n            store8(b+3*stride+i,sub8(d01,t));\r\
    \n        }\r\n    }\r\n    if(blocks==1)return;\r\n    word rot=inverse_rate3(0);\r\
    \n    usize s=1;\r\n    for(;s+8<=blocks;s+=8){\r\n        alignas(32) word r1[8],r2[8],r3[8];\r\
    \n        for(unsigned lane=0;lane<8;++lane){\r\n            r1[lane]=rot;\r\n\
    \            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));\r\
    \n        }\r\n        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));\r\
    \n        const vec w2=mul8(w1,w1);\r\n        const vec w3=mul8(w2,w1);\r\n \
    \       _mm256_store_si256(reinterpret_cast<vec*>(r2),w2);\r\n        _mm256_store_si256(reinterpret_cast<vec*>(r3),w3);\r\
    \n        for(unsigned lane=0;lane<8;++lane)inverse_radix4_large_block(a+(s+lane)*4*stride,stride,iimag,r1[lane],r2[lane],r3[lane]);\r\
    \n    }\r\n    for(;s<blocks;++s){\r\n        const word rot2=mul(rot,rot);\r\n\
    \        const word rot3=mul(rot2,rot);\r\n        inverse_radix4_large_block(a+s*4*stride,stride,iimag,rot,rot2,rot3);\r\
    \n        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\ninline void forward_radix4_p4(mint* EEZ_NTT998_RESTRICT a,usize\
    \ blocks) noexcept{\r\n    if(blocks<2){\r\n        forward_radix4_scalar(a,blocks,4);\r\
    \n        return;\r\n    }\r\n    const vec imag=broadcast(twiddles.root[2]);\r\
    \n    word rot=montgomery_one;\r\n    for(usize s=0;s<blocks;s+=2){\r\n      \
    \  const word r10=rot;\r\n        rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n        const word r11=rot;\r\n        const vec w1=pack_four(r10,r11);\r\n\
    \        const vec w2=mul8(w1,w1);\r\n        const vec w3=mul8(w2,w1);\r\n  \
    \      mint* const b0=a+s*16;\r\n        mint* const b1=b0+16;\r\n        const\
    \ vec x0=load2x4(b0,b1);\r\n        const vec x1=mul8(load2x4(b0+4,b1+4),w1);\r\
    \n        const vec x2=mul8(load2x4(b0+8,b1+8),w2);\r\n        const vec x3=mul8(load2x4(b0+12,b1+12),w3);\r\
    \n        const vec s02=add8(x0,x2);\r\n        const vec d02=sub8(x0,x2);\r\n\
    \        const vec s13=add8(x1,x3);\r\n        const vec t=mul8(sub8(x1,x3),imag);\r\
    \n        store2x4(b0,b1,add8(s02,s13));\r\n        store2x4(b0+4,b1+4,sub8(s02,s13));\r\
    \n        store2x4(b0+8,b1+8,add8(d02,t));\r\n        store2x4(b0+12,b1+12,sub8(d02,t));\r\
    \n        if(s+2<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+1))));\r\
    \n    }\r\n}\r\ninline void inverse_radix4_p4(mint* EEZ_NTT998_RESTRICT a,usize\
    \ blocks) noexcept{\r\n    if(blocks<2){\r\n        inverse_radix4_scalar(a,blocks,4);\r\
    \n        return;\r\n    }\r\n    const vec iimag=broadcast(twiddles.iroot[2]);\r\
    \n    word rot=montgomery_one;\r\n    for(usize s=0;s<blocks;s+=2){\r\n      \
    \  const word r10=rot;\r\n        rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n        const word r11=rot;\r\n        const vec w1=pack_four(r10,r11);\r\n\
    \        const vec w2=mul8(w1,w1);\r\n        const vec w3=mul8(w2,w1);\r\n  \
    \      mint* const b0=a+s*16;\r\n        mint* const b1=b0+16;\r\n        const\
    \ vec x0=load2x4(b0,b1);\r\n        const vec x1=load2x4(b0+4,b1+4);\r\n     \
    \   const vec x2=load2x4(b0+8,b1+8);\r\n        const vec x3=load2x4(b0+12,b1+12);\r\
    \n        const vec s01=add8(x0,x1);\r\n        const vec d01=sub8(x0,x1);\r\n\
    \        const vec s23=add8(x2,x3);\r\n        const vec t=mul8(sub8(x2,x3),iimag);\r\
    \n        store2x4(b0,b1,add8(s01,s23));\r\n        store2x4(b0+4,b1+4,mul8(add8(d01,t),w1));\r\
    \n        store2x4(b0+8,b1+8,mul8(sub8(s01,s23),w2));\r\n        store2x4(b0+12,b1+12,mul8(sub8(d01,t),w3));\r\
    \n        if(s+2<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+1))));\r\
    \n    }\r\n}\r\ninline void forward_radix4_p1(mint* EEZ_NTT998_RESTRICT a,usize\
    \ blocks) noexcept{\r\n    const vec imag=broadcast(twiddles.root[2]);\r\n   \
    \ word rot=montgomery_one;\r\n    usize s=0;\r\n    for(;s+8<=blocks;s+=8){\r\n\
    \        alignas(32) word r1[8];\r\n        for(unsigned lane=0;lane<8;++lane){\r\
    \n            r1[lane]=rot;\r\n            if(s+lane+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s+lane))));\r\
    \n        }\r\n        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));\r\
    \n        const vec w2=mul8(w1,w1);\r\n        const vec w3=mul8(w2,w1);\r\n \
    \       mint* const b=a+4*s;\r\n        vec x0,x1,x2,x3;\r\n        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);\r\
    \n        x1=mul8(x1,w1);\r\n        x2=mul8(x2,w2);\r\n        x3=mul8(x3,w3);\r\
    \n        const vec s02=add8(x0,x2);\r\n        const vec d02=sub8(x0,x2);\r\n\
    \        const vec s13=add8(x1,x3);\r\n        const vec t=mul8(sub8(x1,x3),imag);\r\
    \n        vec v0,v1,v2,v3;\r\n        transpose_4x8_to_8x4(add8(s02,s13),sub8(s02,s13),add8(d02,t),sub8(d02,t),v0,v1,v2,v3);\r\
    \n        store8(b,v0);\r\n        store8(b+8,v1);\r\n        store8(b+16,v2);\r\
    \n        store8(b+24,v3);\r\n    }\r\n    for(;s<blocks;++s){\r\n        const\
    \ word rot2=mul(rot,rot);\r\n        const word rot3=mul(rot2,rot);\r\n      \
    \  forward_butterfly(a+4*s,1,0,rot,rot2,rot3);\r\n        if(s+1<blocks)rot=mul(rot,forward_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\ninline void inverse_radix4_p1(mint* EEZ_NTT998_RESTRICT a,usize\
    \ blocks) noexcept{\r\n    const vec iimag=broadcast(twiddles.iroot[2]);\r\n \
    \   word rot=montgomery_one;\r\n    usize s=0;\r\n    for(;s+8<=blocks;s+=8){\r\
    \n        alignas(32) word r1[8];\r\n        for(unsigned lane=0;lane<8;++lane){\r\
    \n            r1[lane]=rot;\r\n            if(s+lane+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s+lane))));\r\
    \n        }\r\n        const vec w1=_mm256_load_si256(reinterpret_cast<const vec*>(r1));\r\
    \n        const vec w2=mul8(w1,w1);\r\n        const vec w3=mul8(w2,w1);\r\n \
    \       mint* const b=a+4*s;\r\n        vec x0,x1,x2,x3;\r\n        transpose_8x4_to_4x8(load8(b),load8(b+8),load8(b+16),load8(b+24),x0,x1,x2,x3);\r\
    \n        const vec s01=add8(x0,x1);\r\n        const vec d01=sub8(x0,x1);\r\n\
    \        const vec s23=add8(x2,x3);\r\n        const vec t=mul8(sub8(x2,x3),iimag);\r\
    \n        vec v0,v1,v2,v3;\r\n        transpose_4x8_to_8x4(add8(s01,s23),mul8(add8(d01,t),w1),mul8(sub8(s01,s23),w2),mul8(sub8(d01,t),w3),v0,v1,v2,v3);\r\
    \n        store8(b,v0);\r\n        store8(b+8,v1);\r\n        store8(b+16,v2);\r\
    \n        store8(b+24,v3);\r\n    }\r\n    for(;s<blocks;++s){\r\n        const\
    \ word rot2=mul(rot,rot);\r\n        const word rot3=mul(rot2,rot);\r\n      \
    \  inverse_butterfly(a+4*s,1,0,rot,rot2,rot3);\r\n        if(s+1<blocks)rot=mul(rot,inverse_rate3(twiddle_index(static_cast<u32>(s))));\r\
    \n    }\r\n}\r\n#endif\r\ninline void forward_radix2_first(mint* EEZ_NTT998_RESTRICT\
    \ a,usize n) noexcept{\r\n    const usize half=n>>1;\r\n    usize i=0;\r\n#if\
    \ defined(__AVX2__) || defined(_M_AVX2)\r\n    for(;i+8<=half;i+=8){\r\n     \
    \   const vec x=load8(a+i);\r\n        const vec y=load8(a+half+i);\r\n      \
    \  store8(a+i,add8(x,y));\r\n        store8(a+half+i,sub8(x,y));\r\n    }\r\n\
    #endif\r\n    for(;i<half;++i){\r\n        const word x=raw(a[i]);\r\n       \
    \ const word y=raw(a[half+i]);\r\n        a[i]=from_raw(add(x,y));\r\n       \
    \ a[half+i]=from_raw(sub(x,y));\r\n    }\r\n}\r\ninline void forward_radix4_stage(mint*\
    \ EEZ_NTT998_RESTRICT a,usize n,int stage) noexcept{\r\n    const int h=static_cast<int>(std::countr_zero(n));\r\
    \n    assert(stage>=0&&stage+2<=h);\r\n    const usize stride=usize(1)<<(h-stage-2);\r\
    \n    const usize blocks=usize(1)<<stage;\r\n#if defined(__AVX2__) || defined(_M_AVX2)\r\
    \n    if(stride>=8)forward_radix4_large(a,blocks,stride);\r\n    else if(stride==4)forward_radix4_p4(a,blocks);\r\
    \n    else if(stride==1)forward_radix4_p1(a,blocks);\r\n    else forward_radix4_scalar(a,blocks,stride);\r\
    \n#else\r\n    forward_radix4_scalar(a,blocks,stride);\r\n#endif\r\n}\r\ninline\
    \ void inverse_radix4_stage(mint* EEZ_NTT998_RESTRICT a,usize n,int stage) noexcept{\r\
    \n    const int h=static_cast<int>(std::countr_zero(n));\r\n    assert(stage>=0&&stage+2<=h);\r\
    \n    const usize stride=usize(1)<<(h-stage-2);\r\n    const usize blocks=usize(1)<<stage;\r\
    \n#if defined(__AVX2__) || defined(_M_AVX2)\r\n    if(stride>=8)inverse_radix4_large(a,blocks,stride);\r\
    \n    else if(stride==4)inverse_radix4_p4(a,blocks);\r\n    else if(stride==1)inverse_radix4_p1(a,blocks);\r\
    \n    else inverse_radix4_scalar(a,blocks,stride);\r\n#else\r\n    inverse_radix4_scalar(a,blocks,stride);\r\
    \n#endif\r\n}\r\ninline void final_radix2_scale(mint* EEZ_NTT998_RESTRICT a,usize\
    \ n,word scale_mont) noexcept{\r\n    const usize half=n>>1;\r\n    usize i=0;\r\
    \n#if defined(__AVX2__) || defined(_M_AVX2)\r\n    const vec scale=broadcast(scale_mont);\r\
    \n    for(;i+8<=half;i+=8){\r\n        const vec x=load8(a+i);\r\n        const\
    \ vec y=load8(a+half+i);\r\n        store8(a+i,mul8(add8(x,y),scale));\r\n   \
    \     store8(a+half+i,mul8(sub8(x,y),scale));\r\n    }\r\n#endif\r\n    for(;i<half;++i){\r\
    \n        const word x=raw(a[i]);\r\n        const word y=raw(a[half+i]);\r\n\
    \        a[i]=from_raw(mul(add(x,y),scale_mont));\r\n        a[half+i]=from_raw(mul(sub(x,y),scale_mont));\r\
    \n    }\r\n}\r\ninline void final_radix4_scale(mint* EEZ_NTT998_RESTRICT a,usize\
    \ n,word scale_mont) noexcept{\r\n    const usize stride=n>>2;\r\n    usize i=0;\r\
    \n#if defined(__AVX2__) || defined(_M_AVX2)\r\n    const vec iimag=broadcast(twiddles.iroot[2]);\r\
    \n    const vec scale=broadcast(scale_mont);\r\n    for(;i+8<=stride;i+=8){\r\n\
    \        const vec x0=load8(a+i);\r\n        const vec x1=load8(a+stride+i);\r\
    \n        const vec x2=load8(a+2*stride+i);\r\n        const vec x3=load8(a+3*stride+i);\r\
    \n        const vec s01=add8(x0,x1);\r\n        const vec d01=sub8(x0,x1);\r\n\
    \        const vec s23=add8(x2,x3);\r\n        const vec t=mul8(sub8(x2,x3),iimag);\r\
    \n        store8(a+i,mul8(add8(s01,s23),scale));\r\n        store8(a+stride+i,mul8(add8(d01,t),scale));\r\
    \n        store8(a+2*stride+i,mul8(sub8(s01,s23),scale));\r\n        store8(a+3*stride+i,mul8(sub8(d01,t),scale));\r\
    \n    }\r\n#endif\r\n    for(;i<stride;++i){\r\n        const word x0=raw(a[i]);\r\
    \n        const word x1=raw(a[stride+i]);\r\n        const word x2=raw(a[2*stride+i]);\r\
    \n        const word x3=raw(a[3*stride+i]);\r\n        const word s01=add(x0,x1);\r\
    \n        const word d01=sub(x0,x1);\r\n        const word s23=add(x2,x3);\r\n\
    \        const word t=mul(sub(x2,x3),twiddles.iroot[2]);\r\n        a[i]=from_raw(mul(add(s01,s23),scale_mont));\r\
    \n        a[stride+i]=from_raw(mul(add(d01,t),scale_mont));\r\n        a[2*stride+i]=from_raw(mul(sub(s01,s23),scale_mont));\r\
    \n        a[3*stride+i]=from_raw(mul(sub(d01,t),scale_mont));\r\n    }\r\n}\r\n\
    inline void forward_dif(mint* EEZ_NTT998_RESTRICT a,usize n) noexcept{\r\n   \
    \ if(n<=1)return;\r\n    const int h=static_cast<int>(std::countr_zero(n));\r\n\
    \    int stage=0;\r\n    if(h&1){\r\n        forward_radix2_first(a,n);\r\n  \
    \      stage=1;\r\n    }\r\n    for(;stage<h;stage+=2)forward_radix4_stage(a,n,stage);\r\
    \n}\r\ninline void inverse_dit(mint* EEZ_NTT998_RESTRICT a,usize n) noexcept{\r\
    \n    if(n<=1)return;\r\n    const int h=static_cast<int>(std::countr_zero(n));\r\
    \n    const word scale=mint::raw(static_cast<u32>(n)).inv().a;\r\n    if(h&1){\r\
    \n        for(int stage=h-2;stage>=1;stage-=2)inverse_radix4_stage(a,n,stage);\r\
    \n        final_radix2_scale(a,n,scale);\r\n    }else{\r\n        for(int stage=h-2;stage>=2;stage-=2)inverse_radix4_stage(a,n,stage);\r\
    \n        final_radix4_scale(a,n,scale);\r\n    }\r\n}\r\n#if defined(__AVX2__)\
    \ || defined(_M_AVX2)\r\nclass aligned_uninitialized_buffer{\r\n    static_assert(std::is_trivially_destructible_v<mint>);\r\
    \n    mint* data_=nullptr;\r\npublic:\r\n    explicit aligned_uninitialized_buffer(usize\
    \ n)\r\n        :data_(static_cast<mint*>(::operator new[](n*sizeof(mint),std::align_val_t{64}))){}\r\
    \n    aligned_uninitialized_buffer(const aligned_uninitialized_buffer&)=delete;\r\
    \n    aligned_uninitialized_buffer& operator=(const aligned_uninitialized_buffer&)=delete;\r\
    \n    ~aligned_uninitialized_buffer(){\r\n        ::operator delete[](data_,std::align_val_t{64});\r\
    \n    }\r\n    mint* data() noexcept{return data_;}\r\n    const mint* data()const\
    \ noexcept{return data_;}\r\n};\r\nEEZ_NTT998_ALWAYS_INLINE vec load8_aligned(const\
    \ mint* p) noexcept{\r\n    return _mm256_load_si256(reinterpret_cast<const __m256i*>(static_cast<const\
    \ void*>(p)));\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE void store8_aligned(mint* p,vec\
    \ x) noexcept{\r\n    _mm256_store_si256(reinterpret_cast<__m256i*>(static_cast<void*>(p)),x);\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE vec shrink4_to_2(vec x) noexcept{\r\n    return\
    \ _mm256_min_epu32(x,_mm256_sub_epi32(x,broadcast(mod2)));\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ vec lazy_add8(vec a,vec b) noexcept{return _mm256_add_epi32(a,b);}\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ vec lazy_sub8(vec a,vec b) noexcept{\r\n    return _mm256_add_epi32(a,_mm256_sub_epi32(broadcast(mod2),b));\r\
    \n}\r\ntemplate<bool trivial_twiddle,bool convert_input=false>\r\ninline void\
    \ forward_radix4_block_lazy(mint* b,usize stride,word r1) noexcept{\r\n    const\
    \ word imag=canonicalize(twiddles.root[2]);\r\n    const vec vimag=broadcast(imag);\r\
    \n    const vec vimag_ninv=broadcast(imag*montgomery_ninv);\r\n    const vec vr1=broadcast(r1);\r\
    \n    const vec vr1_ninv=broadcast(r1*montgomery_ninv);\r\n    const word r2=canonicalize(mul(r1,r1));\r\
    \n    const vec vr2=broadcast(r2);\r\n    const vec vr2_ninv=broadcast(r2*montgomery_ninv);\r\
    \n    const word r3=canonicalize(mul(r2,r1));\r\n    const vec vr3=broadcast(r3);\r\
    \n    const vec vr3_ninv=broadcast(r3*montgomery_ninv);\r\n    for(usize i=0;i<stride;i+=8){\r\
    \n        vec x0=shrink4_to_2(load8_aligned(b+i));\r\n        vec x1=load8_aligned(b+stride+i);\r\
    \n        vec x2=load8_aligned(b+2*stride+i);\r\n        vec x3=load8_aligned(b+3*stride+i);\r\
    \n        if constexpr(!trivial_twiddle){\r\n            x1=mul8_fixed(x1,vr1,vr1_ninv);\r\
    \n            x2=mul8_fixed(x2,vr2,vr2_ninv);\r\n            x3=mul8_fixed(x3,vr3,vr3_ninv);\r\
    \n        }else{\r\n            x1=shrink4_to_2(x1);\r\n            x2=shrink4_to_2(x2);\r\
    \n            x3=shrink4_to_2(x3);\r\n        }\r\n        vec s02=lazy_add8(x0,x2);\r\
    \n        vec d02=lazy_sub8(x0,x2);\r\n        vec s13=lazy_add8(x1,x3);\r\n \
    \       const vec t=mul8_fixed(lazy_sub8(x1,x3),vimag,vimag_ninv);\r\n       \
    \ s02=shrink4_to_2(s02);\r\n        d02=shrink4_to_2(d02);\r\n        s13=shrink4_to_2(s13);\r\
    \n        vec y0=lazy_add8(s02,s13);\r\n        vec y1=lazy_sub8(s02,s13);\r\n\
    \        vec y2=lazy_add8(d02,t);\r\n        vec y3=lazy_sub8(d02,t);\r\n    \
    \    if constexpr(convert_input){\r\n            constexpr word r2c=932051910u;\r\
    \n            const vec vr=broadcast(r2c);\r\n            const vec vn=broadcast(r2c*montgomery_ninv);\r\
    \n            y0=mul8_fixed(y0,vr,vn);\r\n            y1=mul8_fixed(y1,vr,vn);\r\
    \n            y2=mul8_fixed(y2,vr,vn);\r\n            y3=mul8_fixed(y3,vr,vn);\r\
    \n        }\r\n        store8_aligned(b+i,y0);\r\n        store8_aligned(b+stride+i,y1);\r\
    \n        store8_aligned(b+2*stride+i,y2);\r\n        store8_aligned(b+3*stride+i,y3);\r\
    \n    }\r\n}\r\ntemplate<bool trivial_twiddle,bool convert_input=false>\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ void forward_radix4_block_pair_lazy(mint* EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT\
    \ b,usize stride,word r1) noexcept{\r\n    forward_radix4_block_lazy<trivial_twiddle,convert_input>(a,stride,r1);\r\
    \n    forward_radix4_block_lazy<trivial_twiddle,convert_input>(b,stride,r1);\r\
    \n}\r\ntemplate<bool trivial_twiddle,bool apply_scale,bool convert_output=false,bool\
    \ direct_output=false>\r\ninline void inverse_radix4_block_lazy(mint* b,usize\
    \ stride,word r1,word scale) noexcept{\r\n    const word iimag=canonicalize(twiddles.iroot[2]);\r\
    \n    const vec viimag=broadcast(iimag);\r\n    const vec viimag_ninv=broadcast(iimag*montgomery_ninv);\r\
    \n    const vec vr1=broadcast(r1);\r\n    const vec vr1_ninv=broadcast(r1*montgomery_ninv);\r\
    \n    const word r2=canonicalize(mul(r1,r1));\r\n    const vec vr2=broadcast(r2);\r\
    \n    const vec vr2_ninv=broadcast(r2*montgomery_ninv);\r\n    const word r3=canonicalize(mul(r2,r1));\r\
    \n    const vec vr3=broadcast(r3);\r\n    const vec vr3_ninv=broadcast(r3*montgomery_ninv);\r\
    \n    const word sc=canonicalize(scale);\r\n    for(usize i=0;i<stride;i+=8){\r\
    \n        const vec x0=shrink4_to_2(load8_aligned(b+i));\r\n        const vec\
    \ x1=shrink4_to_2(load8_aligned(b+stride+i));\r\n        const vec x2=shrink4_to_2(load8_aligned(b+2*stride+i));\r\
    \n        const vec x3=shrink4_to_2(load8_aligned(b+3*stride+i));\r\n        vec\
    \ s01=lazy_add8(x0,x1);\r\n        vec d01=lazy_sub8(x0,x1);\r\n        vec s23=lazy_add8(x2,x3);\r\
    \n        const vec t=mul8_fixed(lazy_sub8(x2,x3),viimag,viimag_ninv);\r\n   \
    \     s01=shrink4_to_2(s01);\r\n        d01=shrink4_to_2(d01);\r\n        s23=shrink4_to_2(s23);\r\
    \n        vec y0=lazy_add8(s01,s23);\r\n        vec y1=lazy_add8(d01,t);\r\n \
    \       vec y2=lazy_sub8(s01,s23);\r\n        vec y3=lazy_sub8(d01,t);\r\n   \
    \     if constexpr(apply_scale){\r\n            const word s0=sc;\r\n        \
    \    const word s1=trivial_twiddle?s0:canonicalize(mul(s0,r1));\r\n          \
    \  const word s2=trivial_twiddle?s0:canonicalize(mul(s0,r2));\r\n            const\
    \ word s3=trivial_twiddle?s0:canonicalize(mul(s0,r3));\r\n            y0=mul8_fixed(y0,broadcast(s0),broadcast(s0*montgomery_ninv));\r\
    \n            y1=mul8_fixed(y1,broadcast(s1),broadcast(s1*montgomery_ninv));\r\
    \n            y2=mul8_fixed(y2,broadcast(s2),broadcast(s2*montgomery_ninv));\r\
    \n            y3=mul8_fixed(y3,broadcast(s3),broadcast(s3*montgomery_ninv));\r\
    \n        }else if constexpr(!trivial_twiddle){\r\n            y1=mul8_fixed(y1,vr1,vr1_ninv);\r\
    \n            y2=mul8_fixed(y2,vr2,vr2_ninv);\r\n            y3=mul8_fixed(y3,vr3,vr3_ninv);\r\
    \n        }\r\n        if constexpr(convert_output){\r\n            const vec\
    \ one=broadcast(1);\r\n            const vec ninv=broadcast(montgomery_ninv);\r\
    \n            y0=canonicalize8(mul8_fixed(y0,one,ninv));\r\n            y1=canonicalize8(mul8_fixed(y1,one,ninv));\r\
    \n            y2=canonicalize8(mul8_fixed(y2,one,ninv));\r\n            y3=canonicalize8(mul8_fixed(y3,one,ninv));\r\
    \n        }else if constexpr(direct_output){\r\n            y0=canonicalize8(shrink4_to_2(y0));\r\
    \n            y1=canonicalize8(shrink4_to_2(y1));\r\n            y2=canonicalize8(shrink4_to_2(y2));\r\
    \n            y3=canonicalize8(shrink4_to_2(y3));\r\n        }\r\n        store8_aligned(b+i,y0);\r\
    \n        store8_aligned(b+stride+i,y1);\r\n        store8_aligned(b+2*stride+i,y2);\r\
    \n        store8_aligned(b+3*stride+i,y3);\r\n    }\r\n}\r\ninline unsigned adaptive_leaf_log(usize\
    \ n) noexcept{\r\n    const unsigned h=static_cast<unsigned>(std::countr_zero(n));\r\
    \n    return(h&1u)?3u:4u;\r\n}\r\ntemplate<bool convert_input=false>\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ void forward_cache_node(mint* EEZ_NTT998_RESTRICT base,usize block_size,unsigned\
    \ layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)\
    \ noexcept{\r\n    const usize stride=block_size>>2;\r\n    if constexpr(convert_input)forward_radix4_block_lazy<true,true>(base,stride,montgomery_one);\r\
    \n    else if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);\r\
    \n    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);\r\n\
    \    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));\r\
    \n}\r\ntemplate<bool convert_input=false>\r\ninline void forward_cache_block(mint*\
    \ EEZ_NTT998_RESTRICT base,usize block_size,unsigned layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>&\
    \ rotation) noexcept{\r\n    forward_cache_node<convert_input>(base,block_size,layer,block,blocks_at_layer,rotation);\r\
    \n    const usize child_size=block_size>>2;\r\n    for(usize child=0;child<4;++child){\r\
    \n        mint* const p=base+child*child_size;\r\n        const usize cb=block*4+child;\r\
    \n        forward_cache_node(p,child_size,layer+1,cb,blocks_at_layer*4,rotation);\r\
    \n        const usize gsize=child_size>>2;\r\n        for(usize g=0;g<4;++g)forward_cache_node(p+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,rotation);\r\
    \n    }\r\n}\r\ntemplate<bool convert_input=false>\r\ninline void forward_cache_dfs(mint*\
    \ EEZ_NTT998_RESTRICT base,usize block_size,usize leaf_size,unsigned layer,usize\
    \ block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation) noexcept{\r\
    \n    if(block_size==leaf_size*64){\r\n        forward_cache_block<convert_input>(base,block_size,layer,block,blocks_at_layer,rotation);\r\
    \n        return;\r\n    }\r\n    const usize stride=block_size>>2;\r\n    if\
    \ constexpr(convert_input)forward_radix4_block_lazy<true,true>(base,stride,montgomery_one);\r\
    \n    else if(block==0)forward_radix4_block_lazy<true>(base,stride,montgomery_one);\r\
    \n    else forward_radix4_block_lazy<false>(base,stride,rotation[layer]);\r\n\
    \    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));\r\
    \n    const usize child_size=block_size>>2;\r\n    if(child_size==leaf_size)return;\r\
    \n    for(usize child=0;child<4;++child)forward_cache_dfs<false>(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);\r\
    \n}\r\ntemplate<bool convert_input=false>\r\nEEZ_NTT998_ALWAYS_INLINE void forward_cache_pair_node(mint*\
    \ EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize block_size,unsigned\
    \ layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)\
    \ noexcept{\r\n    const usize stride=block_size>>2;\r\n    if constexpr(convert_input)forward_radix4_block_pair_lazy<true,true>(a,b,stride,montgomery_one);\r\
    \n    else if(block==0)forward_radix4_block_pair_lazy<true>(a,b,stride,montgomery_one);\r\
    \n    else forward_radix4_block_pair_lazy<false>(a,b,stride,rotation[layer]);\r\
    \n    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],forward_rate3(twiddle_index(static_cast<u32>(block)))));\r\
    \n}\r\ntemplate<bool convert_input=false>\r\ninline void forward_cache_pair_block(mint*\
    \ EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize block_size,unsigned\
    \ layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)\
    \ noexcept{\r\n    forward_cache_pair_node<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);\r\
    \n    const usize child_size=block_size>>2;\r\n    for(usize child=0;child<4;++child){\r\
    \n        mint* const pa=a+child*child_size;\r\n        mint* const pb=b+child*child_size;\r\
    \n        const usize cb=block*4+child;\r\n        forward_cache_pair_node(pa,pb,child_size,layer+1,cb,blocks_at_layer*4,rotation);\r\
    \n        const usize gsize=child_size>>2;\r\n        for(usize g=0;g<4;++g)forward_cache_pair_node(pa+g*gsize,pb+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,rotation);\r\
    \n    }\r\n}\r\ntemplate<bool convert_input=false>\r\ninline void forward_cache_pair_dfs(mint*\
    \ EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize block_size,usize leaf_size,unsigned\
    \ layer,usize block,usize blocks_at_layer,std::array<word,max_log/2+1>& rotation)\
    \ noexcept{\r\n    if(block_size==leaf_size*64){\r\n        forward_cache_pair_block<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);\r\
    \n        return;\r\n    }\r\n    forward_cache_pair_node<convert_input>(a,b,block_size,layer,block,blocks_at_layer,rotation);\r\
    \n    const usize child_size=block_size>>2;\r\n    if(child_size==leaf_size)return;\r\
    \n    for(usize child=0;child<4;++child)forward_cache_pair_dfs<false>(a+child*child_size,b+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,rotation);\r\
    \n}\r\ntemplate<bool apply_scale,bool convert_output=false,bool direct_output=false>\r\
    \nEEZ_NTT998_ALWAYS_INLINE void inverse_cache_node(mint* EEZ_NTT998_RESTRICT base,usize\
    \ block_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>&\
    \ rotation) noexcept{\r\n    const usize stride=block_size>>2;\r\n    if(block==0)inverse_radix4_block_lazy<true,apply_scale,convert_output,direct_output>(base,stride,montgomery_one,scale);\r\
    \n    else inverse_radix4_block_lazy<false,apply_scale,convert_output,direct_output>(base,stride,rotation[layer],scale);\r\
    \n    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));\r\
    \n}\r\ntemplate<bool scale_leaf,bool convert_output=false,bool direct_output=false>\r\
    \ninline void inverse_cache_block(mint* EEZ_NTT998_RESTRICT base,usize block_size,unsigned\
    \ layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>&\
    \ rotation) noexcept{\r\n    const usize child_size=block_size>>2;\r\n    const\
    \ usize gsize=child_size>>2;\r\n    for(usize child=0;child<4;++child){\r\n  \
    \      mint* const p=base+child*child_size;\r\n        const usize cb=block*4+child;\r\
    \n        for(usize g=0;g<4;++g)inverse_cache_node<scale_leaf>(p+g*gsize,gsize,layer+2,cb*4+g,blocks_at_layer*16,scale,rotation);\r\
    \n        inverse_cache_node<false>(p,child_size,layer+1,cb,blocks_at_layer*4,scale,rotation);\r\
    \n    }\r\n    inverse_cache_node<false,convert_output,direct_output>(base,block_size,layer,block,blocks_at_layer,scale,rotation);\r\
    \n}\r\ntemplate<bool scale_leaf,bool convert_output=false,bool direct_output=false>\r\
    \ninline void inverse_cache_dfs(mint* EEZ_NTT998_RESTRICT base,usize block_size,usize\
    \ leaf_size,unsigned layer,usize block,usize blocks_at_layer,word scale,std::array<word,max_log/2+1>&\
    \ rotation) noexcept{\r\n    if(block_size==leaf_size*64){\r\n        inverse_cache_block<scale_leaf,convert_output,direct_output>(base,block_size,layer,block,blocks_at_layer,scale,rotation);\r\
    \n        return;\r\n    }\r\n    const usize child_size=block_size>>2;\r\n  \
    \  if(child_size!=leaf_size){\r\n        for(usize child=0;child<4;++child)inverse_cache_dfs<scale_leaf,false>(base+child*child_size,child_size,leaf_size,layer+1,block*4+child,blocks_at_layer*4,scale,rotation);\r\
    \n    }\r\n    const usize stride=block_size>>2;\r\n    if constexpr(scale_leaf){\r\
    \n        if(child_size==leaf_size){\r\n            if(block==0)inverse_radix4_block_lazy<true,true,convert_output,direct_output>(base,stride,montgomery_one,scale);\r\
    \n            else inverse_radix4_block_lazy<false,true,convert_output,direct_output>(base,stride,rotation[layer],scale);\r\
    \n        }else if(block==0)inverse_radix4_block_lazy<true,false,convert_output,direct_output>(base,stride,montgomery_one,scale);\r\
    \n        else inverse_radix4_block_lazy<false,false,convert_output,direct_output>(base,stride,rotation[layer],scale);\r\
    \n    }else if(block==0)inverse_radix4_block_lazy<true,false,convert_output,direct_output>(base,stride,montgomery_one,scale);\r\
    \n    else inverse_radix4_block_lazy<false,false,convert_output,direct_output>(base,stride,rotation[layer],scale);\r\
    \n    if(block+1<blocks_at_layer)rotation[layer]=canonicalize(mul(rotation[layer],inverse_rate3(twiddle_index(static_cast<u32>(block)))));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE void convert_to_montgomery(mint* a,usize n) noexcept{\r\
    \n    constexpr word r2=932051910u;\r\n    const vec vr2=broadcast(r2);\r\n  \
    \  const vec vn=broadcast(r2*montgomery_ninv);\r\n    for(usize i=0;i<n;i+=8)store8_aligned(a+i,mul8_fixed(load8_aligned(a+i),vr2,vn));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE void convert_from_montgomery(mint* a,usize n)\
    \ noexcept{\r\n    const vec one=broadcast(1);\r\n    const vec ninv=broadcast(montgomery_ninv);\r\
    \n    for(usize i=0;i<n;i+=8)store8_aligned(a+i,canonicalize8(mul8_fixed(load8_aligned(a+i),one,ninv)));\r\
    \n}\r\ntemplate<bool convert_input=false>\r\ninline void forward_adaptive(mint*\
    \ a,usize n,unsigned leaf_log) noexcept{\r\n    const usize leaf_size=usize(1)<<leaf_log;\r\
    \n    if(n==leaf_size){\r\n        if constexpr(convert_input)convert_to_montgomery(a,n);\r\
    \n        return;\r\n    }\r\n    std::array<word,max_log/2+1> rotation{};\r\n\
    \    rotation.fill(canonicalize(montgomery_one));\r\n    if((std::countr_zero(n)-leaf_log)&1u){\r\
    \n        forward_radix2_first(a,n);\r\n        if constexpr(convert_input)convert_to_montgomery(a,n);\r\
    \n        forward_cache_dfs(a,n>>1,leaf_size,0,0,2,rotation);\r\n        forward_cache_dfs(a+(n>>1),n>>1,leaf_size,0,1,2,rotation);\r\
    \n        return;\r\n    }\r\n    forward_cache_dfs<convert_input>(a,n,leaf_size,0,0,1,rotation);\r\
    \n}\r\ntemplate<bool convert_input=false>\r\ninline void forward_adaptive_pair(mint*\
    \ EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize n,unsigned leaf_log)\
    \ noexcept{\r\n    const usize leaf_size=usize(1)<<leaf_log;\r\n    if(n==leaf_size){\r\
    \n        if constexpr(convert_input){\r\n            convert_to_montgomery(a,n);\r\
    \n            convert_to_montgomery(b,n);\r\n        }\r\n        return;\r\n\
    \    }\r\n    std::array<word,max_log/2+1> rotation{};\r\n    rotation.fill(canonicalize(montgomery_one));\r\
    \n    if((std::countr_zero(n)-leaf_log)&1u){\r\n        forward_radix2_first(a,n);\r\
    \n        forward_radix2_first(b,n);\r\n        if constexpr(convert_input){\r\
    \n            convert_to_montgomery(a,n);\r\n            convert_to_montgomery(b,n);\r\
    \n        }\r\n        forward_cache_pair_dfs(a,b,n>>1,leaf_size,0,0,2,rotation);\r\
    \n        forward_cache_pair_dfs(a+(n>>1),b+(n>>1),n>>1,leaf_size,0,1,2,rotation);\r\
    \n        return;\r\n    }\r\n    forward_cache_pair_dfs<convert_input>(a,b,n,leaf_size,0,0,1,rotation);\r\
    \n}\r\ntemplate<bool convert_output=false,bool direct_output=false>\r\ninline\
    \ void inverse_adaptive(mint* a,usize n,unsigned leaf_log) noexcept{\r\n    const\
    \ usize leaf_size=usize(1)<<leaf_log;\r\n    if(n==leaf_size){\r\n        if constexpr(convert_output)convert_from_montgomery(a,n);\r\
    \n        return;\r\n    }\r\n    const word scale=mint::raw(static_cast<word>(n>>leaf_log)).inv().a;\r\
    \n    std::array<word,max_log/2+1> rotation{};\r\n    rotation.fill(canonicalize(montgomery_one));\r\
    \n    if((std::countr_zero(n)-leaf_log)&1u){\r\n        inverse_cache_dfs<true>(a,n>>1,leaf_size,0,0,2,scale,rotation);\r\
    \n        inverse_cache_dfs<true>(a+(n>>1),n>>1,leaf_size,0,1,2,scale,rotation);\r\
    \n        const usize half=n>>1;\r\n        for(usize i=0;i<half;i+=8){\r\n  \
    \          const vec x=shrink4_to_2(load8_aligned(a+i));\r\n            const\
    \ vec y=shrink4_to_2(load8_aligned(a+half+i));\r\n            vec z0=lazy_add8(x,y);\r\
    \n            vec z1=lazy_sub8(x,y);\r\n            if constexpr(convert_output){\r\
    \n                const vec one=broadcast(1);\r\n                const vec ninv=broadcast(montgomery_ninv);\r\
    \n                z0=canonicalize8(mul8_fixed(z0,one,ninv));\r\n             \
    \   z1=canonicalize8(mul8_fixed(z1,one,ninv));\r\n            }else if constexpr(direct_output){\r\
    \n                z0=canonicalize8(shrink4_to_2(z0));\r\n                z1=canonicalize8(shrink4_to_2(z1));\r\
    \n            }\r\n            store8_aligned(a+i,z0);\r\n            store8_aligned(a+half+i,z1);\r\
    \n        }\r\n        return;\r\n    }\r\n    inverse_cache_dfs<true,convert_output,direct_output>(a,n,leaf_size,0,0,1,scale,rotation);\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE __m128i reduce_four_accumulators(vec x) noexcept{\r\
    \n    const vec ninv=broadcast(montgomery_ninv);\r\n    const vec prime=broadcast(mod);\r\
    \n    const vec q=_mm256_mul_epu32(x,ninv);\r\n    const vec sum=_mm256_add_epi64(x,_mm256_mul_epu32(q,prime));\r\
    \n    const vec high=_mm256_bsrli_epi128(sum,4);\r\n    const vec packed=_mm256_permutevar8x32_epi32(high,_mm256_setr_epi32(0,2,4,6,0,0,0,0));\r\
    \n    return _mm256_castsi256_si128(packed);\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ vec reduce_eight_accumulators(vec even,vec odd) noexcept{\r\n    const vec ninv=broadcast(montgomery_ninv);\r\
    \n    const vec prime=broadcast(mod);\r\n    const vec qe=_mm256_mul_epu32(even,ninv);\r\
    \n    const vec qo=_mm256_mul_epu32(odd,ninv);\r\n    const vec re=_mm256_add_epi64(even,_mm256_mul_epu32(qe,prime));\r\
    \n    const vec ro=_mm256_add_epi64(odd,_mm256_mul_epu32(qo,prime));\r\n    return\
    \ shrink4_to_2(_mm256_or_si256(_mm256_bsrli_epi128(re,4),ro));\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE\
    \ void leaf_product8x4(mint* EEZ_NTT998_RESTRICT a,mint* EEZ_NTT998_RESTRICT b,usize\
    \ first_block,const std::array<word,4>& modulus) noexcept{\r\n    alignas(64)\
    \ word lhs[4][16];\r\n    alignas(64) vec even[4]{};\r\n    alignas(64) vec odd[4]{};\r\
    \n    for(unsigned k=0;k<4;++k){\r\n        const usize off=(first_block+k)*8;\r\
    \n        const vec x=canonicalize8(shrink4_to_2(load8_aligned(a+off)));\r\n \
    \       const vec y=canonicalize8(shrink4_to_2(load8_aligned(b+off)));\r\n   \
    \     const word w=canonicalize(modulus[k]);\r\n        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]),mul8_fixed(x,broadcast(w),broadcast(w*montgomery_ninv)));\r\
    \n        _mm256_store_si256(reinterpret_cast<vec*>(lhs[k]+8),x);\r\n        store8_aligned(b+off,y);\r\
    \n    }\r\n    for(unsigned i=0;i<8;++i){\r\n        for(unsigned k=0;k<4;++k){\r\
    \n            const usize off=(first_block+k)*8;\r\n            const vec y=broadcast(raw(b[off+i]));\r\
    \n            const vec x=_mm256_loadu_si256(reinterpret_cast<const vec*>(lhs[k]+8-i));\r\
    \n            even[k]=_mm256_add_epi64(even[k],_mm256_mul_epu32(y,x));\r\n   \
    \         odd[k]=_mm256_add_epi64(odd[k],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));\r\
    \n        }\r\n    }\r\n    for(unsigned k=0;k<4;++k)store8_aligned(a+(first_block+k)*8,reduce_eight_accumulators(even[k],odd[k]));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE void leaf_product16x2_karatsuba(mint* EEZ_NTT998_RESTRICT\
    \ a,const mint* EEZ_NTT998_RESTRICT b,usize first_block,const std::array<word,2>&\
    \ modulus) noexcept{\r\n    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);\r\
    \n    alignas(64) word lhs[6][16];\r\n    alignas(64) word rhs[6][8];\r\n    alignas(64)\
    \ vec even[6]{};\r\n    alignas(64) vec odd[6]{};\r\n    word w[2];\r\n    for(unsigned\
    \ k=0;k<2;++k){\r\n        const usize off=(first_block+k)*16;\r\n        const\
    \ vec ax0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);\r\
    \n        const vec ax1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);\r\
    \n        const vec bx0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+off))),split);\r\
    \n        const vec bx1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(b+off+8))),split);\r\
    \n        const vec ae=_mm256_permute2x128_si256(ax0,ax1,0x20);\r\n        const\
    \ vec ao=_mm256_permute2x128_si256(ax0,ax1,0x31);\r\n        const vec be=_mm256_permute2x128_si256(bx0,bx1,0x20);\r\
    \n        const vec bo=_mm256_permute2x128_si256(bx0,bx1,0x31);\r\n        const\
    \ vec lx[3]{ae,ao,canonicalize8(add8(ae,ao))};\r\n        const vec ry[3]{be,bo,canonicalize8(add8(be,bo))};\r\
    \n        w[k]=canonicalize(modulus[k]);\r\n        const vec vw=broadcast(w[k]);\r\
    \n        const vec vn=broadcast(w[k]*montgomery_ninv);\r\n        for(unsigned\
    \ j=0;j<3;++j){\r\n            const unsigned p=k*3+j;\r\n            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]),mul8_fixed(lx[j],vw,vn));\r\
    \n            _mm256_store_si256(reinterpret_cast<vec*>(lhs[p]+8),lx[j]);\r\n\
    \            _mm256_store_si256(reinterpret_cast<vec*>(rhs[p]),ry[j]);\r\n   \
    \     }\r\n    }\r\n    for(unsigned i=0;i<8;++i){\r\n        for(unsigned p=0;p<6;++p){\r\
    \n            const vec y=broadcast(rhs[p][i]);\r\n            const vec x=_mm256_loadu_si256(reinterpret_cast<const\
    \ vec*>(lhs[p]+8-i));\r\n            even[p]=_mm256_add_epi64(even[p],_mm256_mul_epu32(y,x));\r\
    \n            odd[p]=_mm256_add_epi64(odd[p],_mm256_mul_epu32(y,_mm256_bsrli_epi128(x,4)));\r\
    \n        }\r\n    }\r\n    for(unsigned k=0;k<2;++k){\r\n        const vec p0=reduce_eight_accumulators(even[k*3],odd[k*3]);\r\
    \n        const vec p1=reduce_eight_accumulators(even[k*3+1],odd[k*3+1]);\r\n\
    \        const vec p2=reduce_eight_accumulators(even[k*3+2],odd[k*3+2]);\r\n \
    \       vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));\r\
    \n        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w[k])),0);\r\
    \n        const vec ce=add8(p0,yp1);\r\n        const vec co=sub8(sub8(p2,p0),p1);\r\
    \n        const vec lo=_mm256_unpacklo_epi32(ce,co);\r\n        const vec hi=_mm256_unpackhi_epi32(ce,co);\r\
    \n        const usize off=(first_block+k)*16;\r\n        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));\r\
    \n        store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));\r\n \
    \   }\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE word twice(word x) noexcept{return x+x;}\r\
    \nEEZ_NTT998_ALWAYS_INLINE vec pack4_u32(word x0,word x1,word x2,word x3) noexcept{\r\
    \n    return _mm256_cvtepu32_epi64(_mm_setr_epi32(static_cast<int>(x0),static_cast<int>(x1),static_cast<int>(x2),static_cast<int>(x3)));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE vec mul4_u32(word a0,word b0,word a1,word b1,word\
    \ a2,word b2,word a3,word b3) noexcept{\r\n    return _mm256_mul_epu32(pack4_u32(a0,a1,a2,a3),pack4_u32(b0,b1,b2,b3));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE u64 hsum4_u64(vec x) noexcept{\r\n    __m128i\
    \ s=_mm_add_epi64(_mm256_castsi256_si128(x),_mm256_extracti128_si256(x,1));\r\n\
    \    s=_mm_add_epi64(s,_mm_srli_si128(s,8));\r\n    return static_cast<u64>(_mm_cvtsi128_si64(s));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE vec square8_packed(vec vx,word w) noexcept{\r\n\
    \    alignas(32) word x[8],xw[8];\r\n    vx=canonicalize8(shrink4_to_2(vx));\r\
    \n    w=canonicalize(w);\r\n    _mm256_store_si256(reinterpret_cast<vec*>(x),vx);\r\
    \n    _mm256_store_si256(reinterpret_cast<vec*>(xw),mul8_fixed(vx,broadcast(w),broadcast(w*montgomery_ninv)));\r\
    \n    u64 a0=hsum4_u64(mul4_u32(x[0],x[0],twice(xw[1]),x[7],twice(xw[2]),x[6],twice(xw[3]),x[5]));\r\
    \n    const u64 a1=hsum4_u64(mul4_u32(twice(x[0]),x[1],twice(xw[2]),x[7],twice(xw[3]),x[6],twice(xw[4]),x[5]));\r\
    \n    u64 a2=hsum4_u64(mul4_u32(twice(x[0]),x[2],x[1],x[1],twice(xw[3]),x[7],twice(xw[4]),x[6]));\r\
    \n    const u64 a3=hsum4_u64(mul4_u32(twice(x[0]),x[3],twice(x[1]),x[2],twice(xw[4]),x[7],twice(xw[5]),x[6]));\r\
    \n    u64 a4=hsum4_u64(mul4_u32(twice(x[0]),x[4],twice(x[1]),x[3],x[2],x[2],twice(xw[5]),x[7]));\r\
    \n    const u64 a5=hsum4_u64(mul4_u32(twice(x[0]),x[5],twice(x[1]),x[4],twice(x[2]),x[3],twice(xw[6]),x[7]));\r\
    \n    u64 a6=hsum4_u64(mul4_u32(twice(x[0]),x[6],twice(x[1]),x[5],twice(x[2]),x[4],x[3],x[3]));\r\
    \n    const u64 a7=hsum4_u64(mul4_u32(twice(x[0]),x[7],twice(x[1]),x[6],twice(x[2]),x[5],twice(x[3]),x[4]));\r\
    \n    const vec extra=mul4_u32(xw[4],x[4],xw[5],x[5],xw[6],x[6],xw[7],x[7]);\r\
    \n    alignas(32) u64 e[4];\r\n    _mm256_store_si256(reinterpret_cast<vec*>(e),extra);\r\
    \n    a0+=e[0];\r\n    a2+=e[1];\r\n    a4+=e[2];\r\n    a6+=e[3];\r\n    const\
    \ vec lo=_mm256_setr_epi64x(static_cast<long long>(a0),static_cast<long long>(a1),static_cast<long\
    \ long>(a2),static_cast<long long>(a3));\r\n    const vec hi=_mm256_setr_epi64x(static_cast<long\
    \ long>(a4),static_cast<long long>(a5),static_cast<long long>(a6),static_cast<long\
    \ long>(a7));\r\n    return shrink4_to_2(_mm256_set_m128i(reduce_four_accumulators(hi),reduce_four_accumulators(lo)));\r\
    \n}\r\nEEZ_NTT998_ALWAYS_INLINE void leaf_square8x4(mint* EEZ_NTT998_RESTRICT\
    \ a,usize first_block,const std::array<word,4>& modulus) noexcept{\r\n    for(unsigned\
    \ k=0;k<4;++k){\r\n        const usize off=(first_block+k)*8;\r\n        store8_aligned(a+off,square8_packed(load8_aligned(a+off),modulus[k]));\r\
    \n    }\r\n}\r\nEEZ_NTT998_ALWAYS_INLINE void leaf_square16x2_karatsuba(mint*\
    \ EEZ_NTT998_RESTRICT a,usize first_block,const std::array<word,2>& modulus) noexcept{\r\
    \n    const vec split=_mm256_setr_epi32(0,2,4,6,1,3,5,7);\r\n    for(unsigned\
    \ k=0;k<2;++k){\r\n        const usize off=(first_block+k)*16;\r\n        const\
    \ vec x0=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off))),split);\r\
    \n        const vec x1=_mm256_permutevar8x32_epi32(canonicalize8(shrink4_to_2(load8_aligned(a+off+8))),split);\r\
    \n        const vec xe=_mm256_permute2x128_si256(x0,x1,0x20);\r\n        const\
    \ vec xo=_mm256_permute2x128_si256(x0,x1,0x31);\r\n        const vec xs=canonicalize8(add8(xe,xo));\r\
    \n        const word w=canonicalize(modulus[k]);\r\n        const vec p0=square8_packed(xe,w);\r\
    \n        const vec p1=square8_packed(xo,w);\r\n        const vec p2=square8_packed(xs,w);\r\
    \n        vec yp1=_mm256_permutevar8x32_epi32(p1,_mm256_setr_epi32(7,0,1,2,3,4,5,6));\r\
    \n        yp1=_mm256_insert_epi32(yp1,canonicalize(mul(static_cast<word>(_mm256_extract_epi32(p1,7)),w)),0);\r\
    \n        const vec ce=add8(p0,yp1);\r\n        const vec co=sub8(sub8(p2,p0),p1);\r\
    \n        const vec lo=_mm256_unpacklo_epi32(ce,co);\r\n        const vec hi=_mm256_unpackhi_epi32(ce,co);\r\
    \n        store8_aligned(a+off,_mm256_permute2x128_si256(lo,hi,0x20));\r\n   \
    \     store8_aligned(a+off+8,_mm256_permute2x128_si256(lo,hi,0x31));\r\n    }\r\
    \n}\r\ntemplate<unsigned leaf_size,unsigned parallel_blocks>\r\ninline void leaf_products(mint*\
    \ a,mint* b,usize n) noexcept{\r\n    const usize blocks=n/leaf_size;\r\n    word\
    \ w=montgomery_one;\r\n    for(usize s=0;s<blocks;s+=parallel_blocks){\r\n   \
    \     std::array<word,parallel_blocks> modulus{};\r\n        for(unsigned k=0;k<parallel_blocks;++k){\r\
    \n            modulus[k]=w;\r\n            const usize block=s+k;\r\n        \
    \    if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));\r\
    \n        }\r\n        if constexpr(leaf_size==8)leaf_product8x4(a,b,s,modulus);\r\
    \n        else leaf_product16x2_karatsuba(a,b,s,modulus);\r\n    }\r\n}\r\ntemplate<unsigned\
    \ leaf_size,unsigned parallel_blocks>\r\ninline void leaf_squares(mint* a,usize\
    \ n) noexcept{\r\n    const usize blocks=n/leaf_size;\r\n    word w=montgomery_one;\r\
    \n    for(usize s=0;s<blocks;s+=parallel_blocks){\r\n        std::array<word,parallel_blocks>\
    \ modulus{};\r\n        for(unsigned k=0;k<parallel_blocks;++k){\r\n         \
    \   modulus[k]=w;\r\n            const usize block=s+k;\r\n            if(block+1<blocks)w=mul(w,forward_rate1(twiddle_index(static_cast<u32>(block))));\r\
    \n        }\r\n        if constexpr(leaf_size==8)leaf_square8x4(a,s,modulus);\r\
    \n        else leaf_square16x2_karatsuba(a,s,modulus);\r\n    }\r\n}\r\ninline\
    \ void convolution_adaptive_inplace(mint* a,mint* b,usize n) noexcept{\r\n   \
    \ const unsigned leaf_log=adaptive_leaf_log(n);\r\n    forward_adaptive_pair(a,b,n,leaf_log);\r\
    \n    if(leaf_log==3)leaf_products<8,4>(a,b,n);\r\n    else leaf_products<16,2>(a,b,n);\r\
    \n    inverse_adaptive(a,n,leaf_log);\r\n}\r\ninline void square_adaptive_inplace(mint*\
    \ a,usize n) noexcept{\r\n    const unsigned leaf_log=adaptive_leaf_log(n);\r\n\
    \    forward_adaptive(a,n,leaf_log);\r\n    if(leaf_log==3)leaf_squares<8,4>(a,n);\r\
    \n    else leaf_squares<16,2>(a,n);\r\n    inverse_adaptive(a,n,leaf_log);\r\n\
    }\r\ninline void convolution_adaptive_normal_inplace(mint* a,mint* b,usize n)\
    \ noexcept{\r\n    const unsigned leaf_log=adaptive_leaf_log(n);\r\n    forward_adaptive_pair<true>(a,b,n,leaf_log);\r\
    \n    if(leaf_log==3)leaf_products<8,4>(a,b,n);\r\n    else leaf_products<16,2>(a,b,n);\r\
    \n    inverse_adaptive<true>(a,n,leaf_log);\r\n}\r\ninline void convolution_adaptive_mixed_normal_inplace(mint*\
    \ a,mint* b,usize n) noexcept{\r\n    const unsigned leaf_log=adaptive_leaf_log(n);\r\
    \n    forward_adaptive_pair(a,b,n,leaf_log);\r\n    if(leaf_log==3)leaf_products<8,4>(a,b,n);\r\
    \n    else leaf_products<16,2>(a,b,n);\r\n    inverse_adaptive<false,true>(a,n,leaf_log);\r\
    \n}\r\n#endif\r\ninline void pointwise_multiply(mint* EEZ_NTT998_RESTRICT a,const\
    \ mint* EEZ_NTT998_RESTRICT b,usize n) noexcept{\r\n    usize i=0;\r\n#if defined(__AVX2__)\
    \ || defined(_M_AVX2)\r\n    for(;i+8<=n;i+=8)store8(a+i,mul8(load8(a+i),load8(b+i)));\r\
    \n#endif\r\n    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(b[i])));\r\n}\r\n\
    inline void pointwise_square(mint* EEZ_NTT998_RESTRICT a,usize n) noexcept{\r\n\
    \    usize i=0;\r\n#if defined(__AVX2__) || defined(_M_AVX2)\r\n    for(;i+8<=n;i+=8){\r\
    \n        const vec x=load8(a+i);\r\n        store8(a+i,mul8(x,x));\r\n    }\r\
    \n#endif\r\n    for(;i<n;++i)a[i]=from_raw(mul(raw(a[i]),raw(a[i])));\r\n}\r\n\
    }\r\ninline void forward(std::span<mint> a) noexcept{\r\n    if(a.size()<=1)return;\r\
    \n    assert(valid_ntt_size(a.size()));\r\n    detail::forward_dif(a.data(),a.size());\r\
    \n}\r\ninline void inverse(std::span<mint> a) noexcept{\r\n    if(a.size()<=1)return;\r\
    \n    assert(valid_ntt_size(a.size()));\r\n    detail::inverse_dit(a.data(),a.size());\r\
    \n}\r\nnamespace detail{\r\ninline std::vector<mint> convolution_naive(std::span<const\
    \ mint> a,std::span<const mint> b){\r\n    std::vector<mint> result(convolution_size(a.size(),b.size()));\r\
    \n    for(usize i=0;i<a.size();++i)\r\n        for(usize j=0;j<b.size();++j)\r\
    \n            result[i+j]+=a[i]*b[j];\r\n    return result;\r\n}\r\ninline std::vector<mint>\
    \ square_naive(std::span<const mint> a){\r\n    std::vector<mint> result(convolution_size(a.size(),a.size()));\r\
    \n    for(usize i=0;i<a.size();++i){\r\n        result[2*i]+=a[i]*a[i];\r\n  \
    \      for(usize j=i+1;j<a.size();++j){\r\n            const mint p=a[i]*a[j];\r\
    \n            result[i+j]+=p+p;\r\n        }\r\n    }\r\n    return result;\r\n\
    }\r\ninline usize checked_transform_size(usize n,usize m){\r\n    const usize\
    \ result=convolution_transform_size(n,m);\r\n    if(n&&m&&!result)throw std::length_error(\"\
    eez::ntt998: convolution exceeds the 2^25 transform limit\");\r\n    return result;\r\
    \n}\r\ninline void require_ntt_size(usize n){\r\n    if(!valid_ntt_size(n))throw\
    \ std::invalid_argument(\"eez::ntt998: transform length must be a power of two\
    \ in [1, 2^23]\");\r\n}\r\n}\r\n#if defined(__AVX2__) || defined(_M_AVX2)\r\n\
    using convolution_buffer=detail::aligned_vector;\r\ninline void convolution_inplace(convolution_buffer&\
    \ a,convolution_buffer& b){\r\n    if(a.size()!=b.size()||!valid_convolution_transform_size(a.size()))\r\
    \n        throw std::invalid_argument(\"eez::ntt998::convolution_inplace: buffer\
    \ sizes must match and be a power of two in [32, 2^25]\");\r\n    detail::convolution_adaptive_inplace(a.data(),b.data(),a.size());\r\
    \n}\r\n#endif\r\ninline std::vector<mint> convolution(std::span<const mint> a,std::span<const\
    \ mint> b){\r\n    if(a.empty()||b.empty())return {};\r\n    if(a.data()==b.data()&&a.size()==b.size())return\
    \ square(a);\r\n    if(std::min(a.size(),b.size())<=naive_cutoff)return detail::convolution_naive(a,b);\r\
    \n    const usize result_size=convolution_size(a.size(),b.size());\r\n    const\
    \ usize n=detail::checked_transform_size(a.size(),b.size());\r\n    detail::aligned_vector\
    \ fa(n),fb(n);\r\n    std::copy(a.begin(),a.end(),fa.begin());\r\n    std::copy(b.begin(),b.end(),fb.begin());\r\
    \n    detail::convolution_adaptive_inplace(fa.data(),fb.data(),n);\r\n    std::vector<mint>\
    \ result(result_size);\r\n    std::copy_n(fa.data(),result_size,result.data());\r\
    \n    return result;\r\n}\r\ninline std::vector<mint> square(std::span<const mint>\
    \ a){\r\n    if(a.empty())return {};\r\n    if(a.size()<=naive_cutoff)return detail::square_naive(a);\r\
    \n    const usize result_size=convolution_size(a.size(),a.size());\r\n    const\
    \ usize n=detail::checked_transform_size(a.size(),a.size());\r\n    detail::aligned_vector\
    \ fa(n);\r\n    std::copy(a.begin(),a.end(),fa.begin());\r\n    detail::square_adaptive_inplace(fa.data(),n);\r\
    \n    std::vector<mint> result(result_size);\r\n    std::copy_n(fa.data(),result_size,result.data());\r\
    \n    return result;\r\n}\r\ninline void convolution_to(std::span<const mint>\
    \ a,std::span<const mint> b,std::span<mint> out,workspace& ws){\r\n    if(a.empty()||b.empty())return;\r\
    \n    const usize result_size=convolution_size(a.size(),b.size());\r\n    if(out.size()<result_size)throw\
    \ std::invalid_argument(\"eez::ntt998::convolution_to: output span is too small\"\
    );\r\n    if(a.data()==b.data()&&a.size()==b.size()){\r\n        square_to(a,out,ws);\r\
    \n        return;\r\n    }\r\n    if(std::min(a.size(),b.size())<=naive_cutoff){\r\
    \n        std::fill_n(out.begin(),result_size,mint{});\r\n        for(usize i=0;i<a.size();++i)\r\
    \n            for(usize j=0;j<b.size();++j)\r\n                out[i+j]+=a[i]*b[j];\r\
    \n        return;\r\n    }\r\n    const usize n=detail::checked_transform_size(a.size(),b.size());\r\
    \n    ws.reserve(n);\r\n    std::fill_n(ws.a_.begin(),n,mint{});\r\n    std::fill_n(ws.b_.begin(),n,mint{});\r\
    \n    std::copy(a.begin(),a.end(),ws.a_.begin());\r\n    std::copy(b.begin(),b.end(),ws.b_.begin());\r\
    \n    detail::convolution_adaptive_inplace(ws.a_.data(),ws.b_.data(),n);\r\n \
    \   std::copy_n(ws.a_.begin(),result_size,out.begin());\r\n}\r\ninline void square_to(std::span<const\
    \ mint> a,std::span<mint> out,workspace& ws){\r\n    if(a.empty())return;\r\n\
    \    const usize result_size=convolution_size(a.size(),a.size());\r\n    if(out.size()<result_size)throw\
    \ std::invalid_argument(\"eez::ntt998::square_to: output span is too small\");\r\
    \n    if(a.size()<=naive_cutoff){\r\n        std::fill_n(out.begin(),result_size,mint{});\r\
    \n        for(usize i=0;i<a.size();++i){\r\n            out[2*i]+=a[i]*a[i];\r\
    \n            for(usize j=i+1;j<a.size();++j){\r\n                const mint p=a[i]*a[j];\r\
    \n                out[i+j]+=p+p;\r\n            }\r\n        }\r\n        return;\r\
    \n    }\r\n    const usize n=detail::checked_transform_size(a.size(),a.size());\r\
    \n    ws.reserve(n);\r\n    std::fill_n(ws.a_.begin(),n,mint{});\r\n    std::copy(a.begin(),a.end(),ws.a_.begin());\r\
    \n    detail::square_adaptive_inplace(ws.a_.data(),n);\r\n    std::copy_n(ws.a_.begin(),result_size,out.begin());\r\
    \n}\r\ninline void forward_to(std::span<const mint> src,frequency_buffer& dst,usize\
    \ n){\r\n    detail::require_ntt_size(n);\r\n    if(src.size()>n)throw std::invalid_argument(\"\
    eez::ntt998::forward_to: source is longer than transform\");\r\n    dst.data_.assign(n,mint{});\r\
    \n    std::copy(src.begin(),src.end(),dst.data_.begin());\r\n    detail::forward_dif(dst.data_.data(),n);\r\
    \n}\r\ninline void pointwise_multiply(frequency_buffer& lhs,const frequency_buffer&\
    \ rhs){\r\n    if(lhs.size()!=rhs.size())throw std::invalid_argument(\"eez::ntt998::pointwise_multiply:\
    \ transform sizes differ\");\r\n    detail::pointwise_multiply(lhs.data_.data(),rhs.data_.data(),lhs.data_.size());\r\
    \n}\r\ninline void pointwise_square(frequency_buffer& a){\r\n    detail::pointwise_square(a.data_.data(),a.data_.size());\r\
    \n}\r\ninline void inverse_to(frequency_buffer& src,std::span<mint> out){\r\n\
    \    if(src.data_.empty()){\r\n        if(!out.empty())throw std::invalid_argument(\"\
    eez::ntt998::inverse_to: empty transform\");\r\n        return;\r\n    }\r\n \
    \   if(out.size()>src.data_.size())throw std::invalid_argument(\"eez::ntt998::inverse_to:\
    \ output is longer than transform\");\r\n    detail::inverse_dit(src.data_.data(),src.data_.size());\r\
    \n    std::copy_n(src.data_.begin(),out.size(),out.begin());\r\n}\r\n}\r\n#undef\
    \ EEZ_NTT998_ALWAYS_INLINE\r\n#undef EEZ_NTT998_RESTRICT\r\n#line 12 \"verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp\"\
    \n\n#if defined(__GNUC__) && !defined(__clang__) && \\\n    (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC\
    \ target(\"avx2,bmi,bmi2,lzcnt,popcnt\")\n#elif defined(__clang__) && \\\n   \
    \ (defined(__x86_64__) || defined(__i386__))\n#pragma clang attribute push( \\\
    \n    __attribute__((target(\"avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))), \\\n    apply_to\
    \ = function)\n#endif\n\n#line 2 \"IO/fastio_unsafe.hpp\"\n\n#line 11 \"IO/fastio_unsafe.hpp\"\
    \n\n#ifdef __linux__\n#include <sys/mman.h>\n#include <sys/stat.h>\n#include <unistd.h>\n\
    #endif\n\n#ifndef FASTIO_UNSAFE_BLOCK_LOG\n#define FASTIO_UNSAFE_BLOCK_LOG 14\n\
    #endif\n\nnamespace fastio_unsafe_impl {\n\nusing i32 = std::int32_t;\nusing u32\
    \ = std::uint32_t;\nusing i64 = std::int64_t;\nusing u64 = std::uint64_t;\nusing\
    \ i128 = __int128_t;\nusing u128 = __uint128_t;\n\nconstexpr auto make_right_align_masks()\
    \ {\n    std::array<std::array<char, 16>, 16> masks{};\n    for (int digits =\
    \ 0; digits < 16; ++digits) {\n        for (int i = 0; i < 16; ++i) {\n      \
    \      masks[digits][i] = i < 16 - digits\n                ? static_cast<char>(0x80)\n\
    \                : static_cast<char>(i - (16 - digits));\n        }\n    }\n \
    \   return masks;\n}\n\nconstexpr auto make_powers_10() {\n    std::array<u64,\
    \ 17> powers{};\n    powers[0] = 1;\n    for (std::size_t i = 1; i < powers.size();\
    \ ++i) {\n        powers[i] = powers[i - 1] * 10;\n    }\n    return powers;\n\
    }\n\nconstexpr auto make_pair_digits() {\n    std::array<unsigned char, 1 << 14>\
    \ table{};\n    table.fill(255);\n    for (unsigned a = 0; a < 10; ++a) {\n  \
    \      for (unsigned b = 0; b < 10; ++b) {\n            table[('0' + a) | (('0'\
    \ + b) << 8)] =\n                static_cast<unsigned char>(a * 10 + b);\n   \
    \     }\n    }\n    return table;\n}\n\nalignas(16) inline constexpr auto right_align_masks\
    \ = make_right_align_masks();\ninline constexpr auto powers_10 = make_powers_10();\n\
    inline constexpr auto pair_digits = make_pair_digits();\n\nstruct input {\n  \
    \  input() {\n#ifdef __linux__\n        struct stat info {};\n        if (::fstat(0,\
    \ &info) == 0 && S_ISREG(info.st_mode) && info.st_size > 0) {\n            const\
    \ off_t current = ::lseek(0, 0, SEEK_CUR);\n            const std::size_t file_size\
    \ = static_cast<std::size_t>(info.st_size);\n            const std::size_t page_size\
    \ = static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));\n            const std::size_t\
    \ rounded_size =\n                (file_size + page_size - 1) / page_size * page_size;\n\
    \            const std::size_t reserved_size = rounded_size + page_size;\n\n \
    \           char* region = static_cast<char*>(::mmap(\n                nullptr,\
    \ reserved_size, PROT_NONE,\n                MAP_PRIVATE | MAP_ANONYMOUS, -1,\
    \ 0));\n            if (region != MAP_FAILED) {\n                void* file_mapping\
    \ = ::mmap(\n                    region, rounded_size, PROT_READ,\n          \
    \          MAP_PRIVATE | MAP_FIXED, 0, 0);\n                void* zero_page =\
    \ file_mapping == MAP_FAILED ? MAP_FAILED : ::mmap(\n                    region\
    \ + rounded_size, page_size, PROT_READ,\n                    MAP_PRIVATE | MAP_ANONYMOUS\
    \ | MAP_FIXED, -1, 0);\n                if (file_mapping != MAP_FAILED && zero_page\
    \ != MAP_FAILED) {\n                    const std::size_t offset = current > 0\n\
    \                        ? std::min(static_cast<std::size_t>(current), file_size)\n\
    \                        : 0;\n                    cursor_ = region + offset;\n\
    \                    return;\n                }\n                ::munmap(region,\
    \ reserved_size);\n            }\n        }\n#endif\n        read_all_fallback();\n\
    \    }\n\n    input(const input&) = delete;\n    input& operator=(const input&)\
    \ = delete;\n\n    char* cursor() const noexcept { return cursor_; }\n\nprivate:\n\
    \    void read_all_fallback() {\n        std::size_t capacity = 1u << 20;\n  \
    \      std::size_t size = 0;\n        char* buffer = static_cast<char*>(std::malloc(capacity\
    \ + 64));\n        if (buffer == nullptr) std::abort();\n\n        for (;;) {\n\
    \            if (size == capacity) {\n                capacity *= 2;\n       \
    \         char* grown = static_cast<char*>(std::realloc(buffer, capacity + 64));\n\
    \                if (grown == nullptr) std::abort();\n                buffer =\
    \ grown;\n            }\n            const std::size_t count = std::fread(\n \
    \               buffer + size, 1, capacity - size, stdin);\n            size +=\
    \ count;\n            if (count == 0) break;\n        }\n        std::memset(buffer\
    \ + size, 0, 64);\n        cursor_ = buffer;\n    }\n\n    char* cursor_ = nullptr;\n\
    };\n\n__attribute__((always_inline)) inline u64 parse_16_digits(__m128i digits)\
    \ noexcept {\n    const __m128i pair_weights = _mm_set1_epi16(0x010A);\n    const\
    \ __m128i quad_weights = _mm_set1_epi32(0x00010064);\n    const __m128i oct_weights\
    \ = _mm_set_epi32(1, 10000, 1, 10000);\n    const __m128i pairs = _mm_maddubs_epi16(digits,\
    \ pair_weights);\n    const __m128i quads = _mm_madd_epi16(pairs, quad_weights);\n\
    \    const __m128i products = _mm_mul_epu32(quads, oct_weights);\n    const __m128i\
    \ odd = _mm_srli_epi64(quads, 32);\n    const __m128i octets = _mm_add_epi64(products,\
    \ odd);\n    const u64 high = static_cast<u64>(_mm_cvtsi128_si64(octets));\n \
    \   const u64 low = static_cast<u64>(_mm_extract_epi64(octets, 1));\n    return\
    \ high * 100000000ULL + low;\n}\n\n__attribute__((always_inline)) inline __m128i\
    \ load_digits(const char* cursor) noexcept {\n    return _mm_sub_epi8(\n     \
    \   _mm_loadu_si128(reinterpret_cast<const __m128i*>(cursor)),\n        _mm_set1_epi8('0'));\n\
    }\n\n__attribute__((always_inline)) inline u64 parse_short_digits(\n    __m128i\
    \ digits, u32 mask, int& length) noexcept {\n    length = __builtin_ctz(mask);\n\
    \    digits = _mm_shuffle_epi8(\n        digits,\n        _mm_load_si128(reinterpret_cast<const\
    \ __m128i*>(\n            right_align_masks[static_cast<std::size_t>(length)].data())));\n\
    \    return parse_16_digits(digits);\n}\n\n__attribute__((always_inline)) inline\
    \ u32 read_u32(char*& cursor) noexcept {\n    const __m128i digits = load_digits(cursor);\n\
    \    const u32 mask = static_cast<u32>(_mm_movemask_epi8(digits));\n    int length;\n\
    \    const u32 value = static_cast<u32>(parse_short_digits(digits, mask, length));\n\
    \    cursor += length + 1;\n    return value;\n}\n\n__attribute__((always_inline))\
    \ inline u32 read_u32_lt1e9(char*& cursor) noexcept {\n    const auto q0 =\n \
    \       pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor + 1)];\n  \
    \  const auto q1 =\n        pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor\
    \ + 3)];\n    const auto q2 =\n        pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor\
    \ + 5)];\n    const auto q3 =\n        pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor\
    \ + 7)];\n\n    if (__builtin_expect((q0 | q1 | q2 | q3) < 128, 1)) {\n      \
    \  u32 value = static_cast<unsigned char>(cursor[0]) - '0';\n        value = value\
    \ * 100 + q0;\n        value = value * 100 + q1;\n        value = value * 100\
    \ + q2;\n        value = value * 100 + q3;\n        cursor += 10;\n        return\
    \ value;\n    }\n\n    u32 value = static_cast<unsigned char>(*cursor++) - '0';\n\
    \n    for (unsigned i = 0; i < 4; ++i) {\n        const auto pair =\n        \
    \    pair_digits[*reinterpret_cast<const std::uint16_t*>(cursor)];\n        if\
    \ (pair > 99) break;\n        value = value * 100 + pair;\n        cursor += 2;\n\
    \    }\n\n    if (*cursor > ' ') {\n        value = value * 10 + static_cast<unsigned>(*cursor++\
    \ & 15);\n    }\n\n    ++cursor;\n    return value;\n}\n\n__attribute__((always_inline))\
    \ inline i32 read_i32(char*& cursor) noexcept {\n    const bool negative = *cursor\
    \ == '-';\n    cursor += static_cast<unsigned>(negative);\n    const u32 magnitude\
    \ = read_u32(cursor);\n    const u32 bits = negative ? u32{0} - magnitude : magnitude;\n\
    \    return static_cast<i32>(bits);\n}\n\n__attribute__((always_inline)) inline\
    \ u64 read_u64(char*& cursor) noexcept {\n    __m128i digits = load_digits(cursor);\n\
    \    const u32 mask = static_cast<u32>(_mm_movemask_epi8(digits));\n\n    if (__builtin_expect(mask\
    \ != 0, 1)) {\n        int length;\n        const u64 value = parse_short_digits(digits,\
    \ mask, length);\n        cursor += length + 1;\n        return value;\n    }\n\
    \n    u64 value = parse_16_digits(digits);\n    cursor += 16;\n    while (*cursor\
    \ >= '0') {\n        value = value * 10 + static_cast<unsigned>(*cursor & 15);\n\
    \        ++cursor;\n    }\n    ++cursor;\n    return value;\n}\n\n__attribute__((always_inline))\
    \ inline i64 read_i64(char*& cursor) noexcept {\n    const bool negative = *cursor\
    \ == '-';\n    cursor += static_cast<unsigned>(negative);\n    const u64 magnitude\
    \ = read_u64(cursor);\n    const u64 bits = negative ? u64{0} - magnitude : magnitude;\n\
    \    return static_cast<i64>(bits);\n}\n\n__attribute__((always_inline)) inline\
    \ u128 read_u128(char*& cursor) noexcept {\n    __m128i digits = load_digits(cursor);\n\
    \    u32 mask = static_cast<u32>(_mm_movemask_epi8(digits));\n\n    if (__builtin_expect(mask\
    \ != 0, 0)) {\n        int length;\n        const u128 value = parse_short_digits(digits,\
    \ mask, length);\n        cursor += length + 1;\n        return value;\n    }\n\
    \n    u128 value = parse_16_digits(digits);\n    cursor += 16;\n\n    digits =\
    \ load_digits(cursor);\n    mask = static_cast<u32>(_mm_movemask_epi8(digits));\n\
    \    if (mask != 0) {\n        int length;\n        const u64 tail = parse_short_digits(digits,\
    \ mask, length);\n        cursor += length + 1;\n        return value * powers_10[static_cast<std::size_t>(length)]\
    \ + tail;\n    }\n\n    value = value * static_cast<u128>(10000000000000000ULL)\n\
    \          + parse_16_digits(digits);\n    cursor += 16;\n\n    digits = load_digits(cursor);\n\
    \    mask = static_cast<u32>(_mm_movemask_epi8(digits));\n    int length;\n  \
    \  const u64 tail = parse_short_digits(digits, mask, length);\n    cursor += length\
    \ + 1;\n    return value * powers_10[static_cast<std::size_t>(length)] + tail;\n\
    }\n\n__attribute__((always_inline)) inline i128 read_i128(char*& cursor) noexcept\
    \ {\n    const bool negative = *cursor == '-';\n    cursor += static_cast<unsigned>(negative);\n\
    \    const u128 magnitude = read_u128(cursor);\n    const u128 bits = negative\
    \ ? u128{0} - magnitude : magnitude;\n    return static_cast<i128>(bits);\n}\n\
    \nconstexpr u32 pack4(char a, char b, char c, char d) noexcept {\n    return static_cast<u32>(static_cast<unsigned\
    \ char>(a)) |\n           (static_cast<u32>(static_cast<unsigned char>(b)) <<\
    \ 8) |\n           (static_cast<u32>(static_cast<unsigned char>(c)) << 16) |\n\
    \           (static_cast<u32>(static_cast<unsigned char>(d)) << 24);\n}\n\nconstexpr\
    \ auto make_padded_groups() {\n    std::array<u32, 10000> table{};\n    for (int\
    \ value = 0; value < 10000; ++value) {\n        table[static_cast<std::size_t>(value)]\
    \ = pack4(\n            static_cast<char>('0' + value / 1000),\n            static_cast<char>('0'\
    \ + value / 100 % 10),\n            static_cast<char>('0' + value / 10 % 10),\n\
    \            static_cast<char>('0' + value % 10));\n    }\n    return table;\n\
    }\n\ninline constexpr auto padded_groups = make_padded_groups();\n\nstruct output\
    \ {\n    output() = default;\n    output(const output&) = delete;\n    output&\
    \ operator=(const output&) = delete;\n\n    char* begin() noexcept { return buffer_.data();\
    \ }\n    char* end() noexcept { return buffer_.data() + buffer_.size(); }\n\n\
    \    __attribute__((noinline)) char* flush(char* cursor) noexcept {\n        std::size_t\
    \ remaining = static_cast<std::size_t>(cursor - buffer_.data());\n        const\
    \ char* data = buffer_.data();\n\n        if (first_flush_ && remaining != 0)\
    \ {\n            ++data;\n            --remaining;\n            first_flush_ =\
    \ false;\n        }\n\n#ifdef __linux__\n        while (remaining != 0) {\n  \
    \          const ssize_t count = ::write(1, data, remaining);\n            if\
    \ (count > 0) {\n                data += count;\n                remaining -=\
    \ static_cast<std::size_t>(count);\n            } else if (count < 0 && errno\
    \ == EINTR) {\n                continue;\n            } else {\n             \
    \   std::abort();\n            }\n        }\n#else\n        while (remaining !=\
    \ 0) {\n            const std::size_t count = std::fwrite(data, 1, remaining,\
    \ stdout);\n            if (count == 0) std::abort();\n            data += count;\n\
    \            remaining -= count;\n        }\n#endif\n\n        return buffer_.data();\n\
    \    }\n\n    void finish(char* cursor) noexcept {\n        if (cursor != buffer_.data()\
    \ || !first_flush_) *cursor++ = '\\n';\n        (void)flush(cursor);\n    }\n\n\
    \    alignas(64) std::array<char, 1u << 19> buffer_;\n    bool first_flush_ =\
    \ true;\n};\n\n__attribute__((always_inline)) inline void store_group(\n    char*&\
    \ cursor, u32 group) noexcept {\n    std::memcpy(cursor, &group, sizeof(group));\n\
    \    cursor += sizeof(group);\n}\n\n__attribute__((always_inline)) inline void\
    \ emit_leading(\n    char*& cursor, u64 value) noexcept {\n    const unsigned\
    \ skip =\n        3u\n        - static_cast<unsigned>(value >= 10)\n        -\
    \ static_cast<unsigned>(value >= 100)\n        - static_cast<unsigned>(value >=\
    \ 1000);\n    const u32 group =\n        padded_groups[static_cast<std::size_t>(value)]\
    \ >> (skip * 8);\n    std::memcpy(cursor, &group, sizeof(group));\n    cursor\
    \ += 4 - skip;\n}\n\n__attribute__((always_inline)) inline void emit_padded(\n\
    \    char*& cursor, u64 value) noexcept {\n    store_group(cursor, padded_groups[static_cast<std::size_t>(value)]);\n\
    }\n\n__attribute__((always_inline)) inline void emit_padded_16(\n    char*& cursor,\
    \ u64 value) noexcept {\n    emit_padded(cursor, value / 1000000000000ULL);\n\
    \    emit_padded(cursor, value / 100000000ULL % 10000);\n    emit_padded(cursor,\
    \ value / 10000ULL % 10000);\n    emit_padded(cursor, value % 10000);\n}\n\n__attribute__((always_inline))\
    \ inline void emit_u32_unchecked(\n    char*& cursor, u32 value) noexcept {\n\
    \    if (value >= 100000000U) {\n        emit_leading(cursor, value / 100000000U);\n\
    \        emit_padded(cursor, value / 10000U % 10000);\n        emit_padded(cursor,\
    \ value % 10000);\n    } else if (value >= 10000U) {\n        emit_leading(cursor,\
    \ value / 10000U);\n        emit_padded(cursor, value % 10000);\n    } else {\n\
    \        emit_leading(cursor, value);\n    }\n}\n\n__attribute__((always_inline))\
    \ inline void emit_u64_unchecked(\n    char*& cursor, u64 value) noexcept {\n\
    \    if (value >= 10000000000000000ULL) {\n        emit_leading(cursor, value\
    \ / 10000000000000000ULL);\n        emit_padded_16(cursor, value % 10000000000000000ULL);\n\
    \    } else if (value >= 1000000000000ULL) {\n        emit_leading(cursor, value\
    \ / 1000000000000ULL);\n        emit_padded(cursor, value / 100000000ULL % 10000);\n\
    \        emit_padded(cursor, value / 10000ULL % 10000);\n        emit_padded(cursor,\
    \ value % 10000);\n    } else if (value >= 100000000ULL) {\n        emit_leading(cursor,\
    \ value / 100000000ULL);\n        emit_padded(cursor, value / 10000ULL % 10000);\n\
    \        emit_padded(cursor, value % 10000);\n    } else if (value >= 10000ULL)\
    \ {\n        emit_leading(cursor, value / 10000);\n        emit_padded(cursor,\
    \ value % 10000);\n    } else {\n        emit_leading(cursor, value);\n    }\n\
    }\n\n__attribute__((always_inline)) inline void emit_u128_unchecked(\n    char*&\
    \ cursor, u128 value) noexcept {\n    constexpr u128 base = static_cast<u128>(10000000000000000ULL);\n\
    \    constexpr u128 u64_max = static_cast<u128>(~u64{0});\n\n    if (value <=\
    \ u64_max) {\n        emit_u64_unchecked(cursor, static_cast<u64>(value));\n \
    \       return;\n    }\n\n    const u64 low = static_cast<u64>(value % base);\n\
    \    const u128 upper = value / base;\n\n    if (upper <= u64_max) {\n       \
    \ emit_u64_unchecked(cursor, static_cast<u64>(upper));\n        emit_padded_16(cursor,\
    \ low);\n        return;\n    }\n\n    const u64 middle = static_cast<u64>(upper\
    \ % base);\n    const u32 high = static_cast<u32>(upper / base);\n    emit_u32_unchecked(cursor,\
    \ high);\n    emit_padded_16(cursor, middle);\n    emit_padded_16(cursor, low);\n\
    }\n\n__attribute__((always_inline)) inline void write_u32_lt1e9(\n    output&\
    \ sink, char*& cursor, char* end, u32 value) noexcept {\n    if (__builtin_expect(end\
    \ - cursor < 16, 0)) cursor = sink.flush(cursor);\n\n    *cursor++ = ' ';\n\n\
    \    if (value >= 100000000U) {\n        const u32 high = value / 100000000U;\n\
    \        *cursor++ = static_cast<char>('0' + high);\n        value -= high * 100000000U;\n\
    \        emit_padded(cursor, value / 10000U);\n        emit_padded(cursor, value\
    \ % 10000U);\n    } else {\n        emit_u32_unchecked(cursor, value);\n    }\n\
    }\n\n__attribute__((always_inline)) inline void write_u32(\n    output& sink,\
    \ char*& cursor, char* end, u32 value) noexcept {\n    if (__builtin_expect(end\
    \ - cursor < 16, 0)) cursor = sink.flush(cursor);\n    *cursor++ = ' ';\n    emit_u32_unchecked(cursor,\
    \ value);\n}\n\n__attribute__((always_inline)) inline void write_i32(\n    output&\
    \ sink, char*& cursor, char* end, i32 value) noexcept {\n    if (__builtin_expect(end\
    \ - cursor < 16, 0)) cursor = sink.flush(cursor);\n    const bool negative = value\
    \ < 0;\n    const u32 bits = static_cast<u32>(value);\n    const u32 magnitude\
    \ = negative ? u32{0} - bits : bits;\n    *cursor++ = ' ';\n    if (negative)\
    \ *cursor++ = '-';\n    emit_u32_unchecked(cursor, magnitude);\n}\n\n__attribute__((always_inline))\
    \ inline void write_u64(\n    output& sink, char*& cursor, char* end, u64 value)\
    \ noexcept {\n    if (__builtin_expect(end - cursor < 24, 0)) cursor = sink.flush(cursor);\n\
    \    *cursor++ = ' ';\n    emit_u64_unchecked(cursor, value);\n}\n\n__attribute__((always_inline))\
    \ inline void write_i64(\n    output& sink, char*& cursor, char* end, i64 value)\
    \ noexcept {\n    if (__builtin_expect(end - cursor < 24, 0)) cursor = sink.flush(cursor);\n\
    \    const bool negative = value < 0;\n    const u64 bits = static_cast<u64>(value);\n\
    \    const u64 magnitude = negative ? u64{0} - bits : bits;\n    *cursor++ = '\
    \ ';\n    if (negative) *cursor++ = '-';\n    emit_u64_unchecked(cursor, magnitude);\n\
    }\n\n__attribute__((always_inline)) inline void write_u128(\n    output& sink,\
    \ char*& cursor, char* end, u128 value) noexcept {\n    if (__builtin_expect(end\
    \ - cursor < 48, 0)) cursor = sink.flush(cursor);\n    *cursor++ = ' ';\n    emit_u128_unchecked(cursor,\
    \ value);\n}\n\n__attribute__((always_inline)) inline void write_i128(\n    output&\
    \ sink, char*& cursor, char* end, i128 value) noexcept {\n    if (__builtin_expect(end\
    \ - cursor < 48, 0)) cursor = sink.flush(cursor);\n    const bool negative = value\
    \ < 0;\n    const u128 bits = static_cast<u128>(value);\n    const u128 magnitude\
    \ = negative ? u128{0} - bits : bits;\n    *cursor++ = ' ';\n    if (negative)\
    \ *cursor++ = '-';\n    emit_u128_unchecked(cursor, magnitude);\n}\n\n}\n\nstruct\
    \ fastio_unsafe {\n    using i32 = fastio_unsafe_impl::i32;\n    using u32 = fastio_unsafe_impl::u32;\n\
    \    using i64 = fastio_unsafe_impl::i64;\n    using u64 = fastio_unsafe_impl::u64;\n\
    \    using i128 = fastio_unsafe_impl::i128;\n    using u128 = fastio_unsafe_impl::u128;\n\
    \n    fastio_unsafe() = default;\n    fastio_unsafe(const fastio_unsafe&) = delete;\n\
    \    fastio_unsafe& operator=(const fastio_unsafe&) = delete;\n\n    char* input_cursor()\
    \ const noexcept { return in.cursor(); }\n    char* output_cursor() noexcept {\
    \ return out.begin(); }\n    char* output_end() noexcept { return out.end(); }\n\
    \    void finish(char* cursor) noexcept { out.finish(cursor); }\n\n    __attribute__((always_inline))\
    \ u32 read_u32(char*& cursor) noexcept {\n        return fastio_unsafe_impl::read_u32(cursor);\n\
    \    }\n\n    __attribute__((always_inline)) u32 read_u32_lt1e9(char*& cursor)\
    \ noexcept {\n        return fastio_unsafe_impl::read_u32_lt1e9(cursor);\n   \
    \ }\n\n    __attribute__((always_inline)) i32 read_i32(char*& cursor) noexcept\
    \ {\n        return fastio_unsafe_impl::read_i32(cursor);\n    }\n\n    __attribute__((always_inline))\
    \ u64 read_u64(char*& cursor) noexcept {\n        return fastio_unsafe_impl::read_u64(cursor);\n\
    \    }\n\n    __attribute__((always_inline)) i64 read_i64(char*& cursor) noexcept\
    \ {\n        return fastio_unsafe_impl::read_i64(cursor);\n    }\n\n    __attribute__((always_inline))\
    \ u128 read_u128(char*& cursor) noexcept {\n        return fastio_unsafe_impl::read_u128(cursor);\n\
    \    }\n\n    __attribute__((always_inline)) i128 read_i128(char*& cursor) noexcept\
    \ {\n        return fastio_unsafe_impl::read_i128(cursor);\n    }\n\n    __attribute__((always_inline))\
    \ void write_u32(\n        char*& cursor, char* end, u32 value) noexcept {\n \
    \       fastio_unsafe_impl::write_u32(out, cursor, end, value);\n    }\n\n   \
    \ __attribute__((always_inline)) void write_u32_lt1e9(\n        char*& cursor,\
    \ char* end, u32 value) noexcept {\n        fastio_unsafe_impl::write_u32_lt1e9(out,\
    \ cursor, end, value);\n    }\n\n    __attribute__((always_inline)) void write_i32(\n\
    \        char*& cursor, char* end, i32 value) noexcept {\n        fastio_unsafe_impl::write_i32(out,\
    \ cursor, end, value);\n    }\n\n    __attribute__((always_inline)) void write_u64(\n\
    \        char*& cursor, char* end, u64 value) noexcept {\n        fastio_unsafe_impl::write_u64(out,\
    \ cursor, end, value);\n    }\n\n    __attribute__((always_inline)) void write_i64(\n\
    \        char*& cursor, char* end, i64 value) noexcept {\n        fastio_unsafe_impl::write_i64(out,\
    \ cursor, end, value);\n    }\n\n    __attribute__((always_inline)) void write_u128(\n\
    \        char*& cursor, char* end, u128 value) noexcept {\n        fastio_unsafe_impl::write_u128(out,\
    \ cursor, end, value);\n    }\n\n    __attribute__((always_inline)) void write_i128(\n\
    \        char*& cursor, char* end, i128 value) noexcept {\n        fastio_unsafe_impl::write_i128(out,\
    \ cursor, end, value);\n    }\n\n    fastio_unsafe_impl::input in;\n    fastio_unsafe_impl::output\
    \ out;\n};\n#line 25 \"verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp\"\
    \n\nint main() {\n    fastio_unsafe io;\n    char* input_cursor = io.input_cursor();\n\
    \    char* output_cursor = io.output_cursor();\n    char* const output_end = io.output_end();\n\
    \n    const std::size_t n = io.read_u32(input_cursor);\n    const std::size_t\
    \ m = io.read_u32(input_cursor);\n    const std::size_t result_size = n + m -\
    \ 1;\n    const std::size_t transform_size = std::bit_ceil(result_size);\n   \
    \ using mint = eez::ntt998::mint;\n\n    if (transform_size < 32) {\n        std::vector<mint>\
    \ a(n), b(m);\n        for (auto& value : a) value = mint::raw(io.read_u32(input_cursor));\n\
    \        for (auto& value : b) value = mint::raw(io.read_u32(input_cursor));\n\
    \n        const auto result = eez::ntt998::convolution(a, b);\n        for (const\
    \ mint value : result) {\n            io.write_u32(output_cursor, output_end,\
    \ value.val());\n        }\n    } else {\n        eez::ntt998::convolution_buffer\
    \ a(transform_size), b(transform_size);\n        for (std::size_t i = 0; i < n;\
    \ ++i) {\n            a[i] = mint::raw(io.read_u32(input_cursor));\n        }\n\
    \        for (std::size_t i = 0; i < m; ++i) {\n            b[i] = mint::raw(io.read_u32(input_cursor));\n\
    \        }\n\n        eez::ntt998::convolution_inplace(a, b);\n        for (std::size_t\
    \ i = 0; i < result_size; ++i) {\n            io.write_u32(output_cursor, output_end,\
    \ a[i].val());\n        }\n    }\n\n    io.finish(output_cursor);\n}\n\n#if defined(__clang__)\
    \ && \\\n    (defined(__x86_64__) || defined(__i386__))\n#pragma clang attribute\
    \ pop\n#endif\n\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/convolution_mod_large\"\
    \n\n#ifndef FASTIO_UNSAFE_BLOCK_LOG\n#define FASTIO_UNSAFE_BLOCK_LOG 12\n#endif\n\
    \n#include <bit>\n#include <cstddef>\n#include <vector>\n\n#include \"../../convolution/ntt998.hpp\"\
    \n\n#if defined(__GNUC__) && !defined(__clang__) && \\\n    (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC\
    \ target(\"avx2,bmi,bmi2,lzcnt,popcnt\")\n#elif defined(__clang__) && \\\n   \
    \ (defined(__x86_64__) || defined(__i386__))\n#pragma clang attribute push( \\\
    \n    __attribute__((target(\"avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))), \\\n    apply_to\
    \ = function)\n#endif\n\n#include \"../../IO/fastio_unsafe.hpp\"\n\nint main()\
    \ {\n    fastio_unsafe io;\n    char* input_cursor = io.input_cursor();\n    char*\
    \ output_cursor = io.output_cursor();\n    char* const output_end = io.output_end();\n\
    \n    const std::size_t n = io.read_u32(input_cursor);\n    const std::size_t\
    \ m = io.read_u32(input_cursor);\n    const std::size_t result_size = n + m -\
    \ 1;\n    const std::size_t transform_size = std::bit_ceil(result_size);\n   \
    \ using mint = eez::ntt998::mint;\n\n    if (transform_size < 32) {\n        std::vector<mint>\
    \ a(n), b(m);\n        for (auto& value : a) value = mint::raw(io.read_u32(input_cursor));\n\
    \        for (auto& value : b) value = mint::raw(io.read_u32(input_cursor));\n\
    \n        const auto result = eez::ntt998::convolution(a, b);\n        for (const\
    \ mint value : result) {\n            io.write_u32(output_cursor, output_end,\
    \ value.val());\n        }\n    } else {\n        eez::ntt998::convolution_buffer\
    \ a(transform_size), b(transform_size);\n        for (std::size_t i = 0; i < n;\
    \ ++i) {\n            a[i] = mint::raw(io.read_u32(input_cursor));\n        }\n\
    \        for (std::size_t i = 0; i < m; ++i) {\n            b[i] = mint::raw(io.read_u32(input_cursor));\n\
    \        }\n\n        eez::ntt998::convolution_inplace(a, b);\n        for (std::size_t\
    \ i = 0; i < result_size; ++i) {\n            io.write_u32(output_cursor, output_end,\
    \ a[i].val());\n        }\n    }\n\n    io.finish(output_cursor);\n}\n\n#if defined(__clang__)\
    \ && \\\n    (defined(__x86_64__) || defined(__i386__))\n#pragma clang attribute\
    \ pop\n#endif\n\n"
  dependsOn: []
  isVerificationFile: true
  path: verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp
  requiredBy: []
  timestamp: '1970-01-01 00:00:00+00:00'
  verificationStatus: TEST_WRONG_ANSWER
  verifiedWith: []
documentation_of: verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp
- /verify/verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp.html
title: verify/verify-yosupo-convolution/yosupo-convolution-mod-large-ntt998.test.cpp
---

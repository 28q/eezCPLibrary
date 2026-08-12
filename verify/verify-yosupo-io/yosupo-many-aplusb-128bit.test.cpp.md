---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: IO/fastio_unsafe.hpp
    title: fastio_unsafe
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/many_aplusb_128bit
    links:
    - https://judge.yosupo.jp/problem/many_aplusb_128bit
  bundledCode: "#line 1 \"verify/verify-yosupo-io/yosupo-many-aplusb-128bit.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/many_aplusb_128bit\"\n\n#if\
    \ defined(__GNUC__) && !defined(__clang__) && \\\n    (defined(__x86_64__) ||\
    \ defined(__i386__))\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt\")\n#elif defined(__clang__) && \\\n    (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute push( \\\n    __attribute__((target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))), \\\n    apply_to = function)\n#endif\n\n\
    #ifndef FASTIO_UNSAFE_BLOCK_LOG\n#define FASTIO_UNSAFE_BLOCK_LOG 12\n#endif\n\n\
    #line 2 \"IO/fastio_unsafe.hpp\"\n\n#include <algorithm>\n#include <array>\n#include\
    \ <cerrno>\n#include <cstdint>\n#include <cstdio>\n#include <cstdlib>\n#include\
    \ <cstring>\n#include <immintrin.h>\n\n#ifdef __linux__\n#include <sys/mman.h>\n\
    #include <sys/stat.h>\n#include <unistd.h>\n#endif\n\n#ifndef FASTIO_UNSAFE_BLOCK_LOG\n\
    #define FASTIO_UNSAFE_BLOCK_LOG 14\n#endif\n\nnamespace fastio_unsafe_impl {\n\
    \nusing i32 = std::int32_t;\nusing u32 = std::uint32_t;\nusing i64 = std::int64_t;\n\
    using u64 = std::uint64_t;\nusing i128 = __int128_t;\nusing u128 = __uint128_t;\n\
    \nconstexpr auto make_right_align_masks() {\n    std::array<std::array<char, 16>,\
    \ 16> masks{};\n    for (int digits = 0; digits < 16; ++digits) {\n        for\
    \ (int i = 0; i < 16; ++i) {\n            masks[digits][i] = i < 16 - digits\n\
    \                ? static_cast<char>(0x80)\n                : static_cast<char>(i\
    \ - (16 - digits));\n        }\n    }\n    return masks;\n}\n\nconstexpr auto\
    \ make_powers_10() {\n    std::array<u64, 17> powers{};\n    powers[0] = 1;\n\
    \    for (std::size_t i = 1; i < powers.size(); ++i) {\n        powers[i] = powers[i\
    \ - 1] * 10;\n    }\n    return powers;\n}\n\nalignas(16) inline constexpr auto\
    \ right_align_masks = make_right_align_masks();\ninline constexpr auto powers_10\
    \ = make_powers_10();\n\nstruct input {\n    input() {\n#ifdef __linux__\n   \
    \     struct stat info {};\n        if (::fstat(0, &info) == 0 && S_ISREG(info.st_mode)\
    \ && info.st_size > 0) {\n            const off_t current = ::lseek(0, 0, SEEK_CUR);\n\
    \            const std::size_t file_size = static_cast<std::size_t>(info.st_size);\n\
    \            const std::size_t page_size = static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));\n\
    \            const std::size_t rounded_size =\n                (file_size + page_size\
    \ - 1) / page_size * page_size;\n            const std::size_t reserved_size =\
    \ rounded_size + page_size;\n\n            char* region = static_cast<char*>(::mmap(\n\
    \                nullptr, reserved_size, PROT_NONE,\n                MAP_PRIVATE\
    \ | MAP_ANONYMOUS, -1, 0));\n            if (region != MAP_FAILED) {\n       \
    \         void* file_mapping = ::mmap(\n                    region, rounded_size,\
    \ PROT_READ,\n                    MAP_PRIVATE | MAP_FIXED, 0, 0);\n          \
    \      void* zero_page = file_mapping == MAP_FAILED ? MAP_FAILED : ::mmap(\n \
    \                   region + rounded_size, page_size, PROT_READ,\n           \
    \         MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);\n                if\
    \ (file_mapping != MAP_FAILED && zero_page != MAP_FAILED) {\n                \
    \    const std::size_t offset = current > 0\n                        ? std::min(static_cast<std::size_t>(current),\
    \ file_size)\n                        : 0;\n                    cursor_ = region\
    \ + offset;\n                    return;\n                }\n                ::munmap(region,\
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
    \ auto make_leading_groups() {\n    std::array<u32, 10000> table{};\n    for (int\
    \ value = 0; value < 10000; ++value) {\n        char a = static_cast<char>('0'\
    \ + value / 1000);\n        char b = static_cast<char>('0' + value / 100 % 10);\n\
    \        char c = static_cast<char>('0' + value / 10 % 10);\n        char d =\
    \ static_cast<char>('0' + value % 10);\n        if (value < 1000) a = ' ';\n \
    \       if (value < 100) b = ' ';\n        if (value < 10) c = ' ';\n        if\
    \ (value == 0) d = ' ';\n        table[static_cast<std::size_t>(value)] = pack4(a,\
    \ b, c, d);\n    }\n    return table;\n}\n\nconstexpr auto make_padded_groups()\
    \ {\n    std::array<u32, 10000> table{};\n    for (int value = 0; value < 10000;\
    \ ++value) {\n        table[static_cast<std::size_t>(value)] = pack4(\n      \
    \      static_cast<char>('0' + value / 1000),\n            static_cast<char>('0'\
    \ + value / 100 % 10),\n            static_cast<char>('0' + value / 10 % 10),\n\
    \            static_cast<char>('0' + value % 10));\n    }\n    return table;\n\
    }\n\ninline constexpr auto leading_groups = make_leading_groups();\ninline constexpr\
    \ auto padded_groups = make_padded_groups();\n\nstruct output {\n    output()\
    \ = default;\n    output(const output&) = delete;\n    output& operator=(const\
    \ output&) = delete;\n\n    char* begin() noexcept { return buffer_.data(); }\n\
    \    char* end() noexcept { return buffer_.data() + buffer_.size(); }\n\n    __attribute__((noinline))\
    \ char* flush(char* cursor) noexcept {\n        std::size_t remaining = static_cast<std::size_t>(cursor\
    \ - buffer_.data());\n        const char* data = buffer_.data();\n#ifdef __linux__\n\
    \        while (remaining != 0) {\n            const ssize_t count = ::write(1,\
    \ data, remaining);\n            if (count > 0) {\n                data += count;\n\
    \                remaining -= static_cast<std::size_t>(count);\n            }\
    \ else if (count < 0 && errno == EINTR) {\n                continue;\n       \
    \     } else {\n                std::abort();\n            }\n        }\n#else\n\
    \        while (remaining != 0) {\n            const std::size_t count = std::fwrite(data,\
    \ 1, remaining, stdout);\n            if (count == 0) std::abort();\n        \
    \    data += count;\n            remaining -= count;\n        }\n#endif\n    \
    \    return buffer_.data();\n    }\n\n    void finish(char* cursor) noexcept {\n\
    \        if (cursor != buffer_.data()) *cursor++ = '\\n';\n        (void)flush(cursor);\n\
    \    }\n\n    alignas(64) std::array<char, 1u << 19> buffer_;\n};\n\n__attribute__((always_inline))\
    \ inline void store_group(\n    char*& cursor, u32 group) noexcept {\n    std::memcpy(cursor,\
    \ &group, sizeof(group));\n    cursor += sizeof(group);\n}\n\n__attribute__((always_inline))\
    \ inline void emit_leading(\n    char*& cursor, u64 value) noexcept {\n    store_group(cursor,\
    \ leading_groups[static_cast<std::size_t>(value)]);\n}\n\n__attribute__((always_inline))\
    \ inline void emit_padded(\n    char*& cursor, u64 value) noexcept {\n    store_group(cursor,\
    \ padded_groups[static_cast<std::size_t>(value)]);\n}\n\n__attribute__((always_inline))\
    \ inline void emit_padded_16(\n    char*& cursor, u64 value) noexcept {\n    emit_padded(cursor,\
    \ value / 1000000000000ULL);\n    emit_padded(cursor, value / 100000000ULL % 10000);\n\
    \    emit_padded(cursor, value / 10000ULL % 10000);\n    emit_padded(cursor, value\
    \ % 10000);\n}\n\n__attribute__((always_inline)) inline void emit_u32_unchecked(\n\
    \    char*& cursor, u32 value) noexcept {\n    if (value >= 10000000U) {\n   \
    \     emit_leading(cursor, value / 100000000U);\n        emit_padded(cursor, value\
    \ / 10000U % 10000);\n        emit_padded(cursor, value % 10000);\n    } else\
    \ if (value >= 1000U) {\n        emit_leading(cursor, value / 10000U);\n     \
    \   emit_padded(cursor, value % 10000);\n    } else if (value != 0) {\n      \
    \  emit_leading(cursor, value);\n    } else {\n        store_group(cursor, pack4('\
    \ ', ' ', ' ', '0'));\n    }\n}\n\n__attribute__((always_inline)) inline void\
    \ emit_u64_unchecked(\n    char*& cursor, u64 value) noexcept {\n    if (value\
    \ >= 10000000000000000000ULL) *cursor++ = ' ';\n\n    if (value >= 1000000000000000ULL)\
    \ {\n        const u64 low8 = value % 100000000ULL;\n        const u64 high =\
    \ value / 100000000ULL;\n        const u64 high_low4 = high % 10000;\n       \
    \ const u64 high_high = high / 10000;\n        emit_leading(cursor, high_high\
    \ / 10000);\n        emit_padded(cursor, high_high % 10000);\n        emit_padded(cursor,\
    \ high_low4);\n        emit_padded(cursor, low8 / 10000);\n        emit_padded(cursor,\
    \ low8 % 10000);\n    } else if (value >= 100000000000ULL) {\n        const u64\
    \ low8 = value % 100000000ULL;\n        const u64 high = value / 100000000ULL;\n\
    \        emit_leading(cursor, high / 10000);\n        emit_padded(cursor, high\
    \ % 10000);\n        emit_padded(cursor, low8 / 10000);\n        emit_padded(cursor,\
    \ low8 % 10000);\n    } else if (value >= 10000000ULL) {\n        const u64 low8\
    \ = value % 100000000ULL;\n        emit_leading(cursor, value / 100000000ULL);\n\
    \        emit_padded(cursor, low8 / 10000);\n        emit_padded(cursor, low8\
    \ % 10000);\n    } else if (value >= 1000ULL) {\n        emit_leading(cursor,\
    \ value / 10000);\n        emit_padded(cursor, value % 10000);\n    } else if\
    \ (value != 0) {\n        emit_leading(cursor, value);\n    } else {\n       \
    \ store_group(cursor, pack4(' ', ' ', ' ', '0'));\n    }\n}\n\n__attribute__((always_inline))\
    \ inline void emit_u128_unchecked(\n    char*& cursor, u128 value) noexcept {\n\
    \    constexpr u128 base = static_cast<u128>(10000000000000000ULL);\n    constexpr\
    \ u128 u64_max = static_cast<u128>(~u64{0});\n\n    if (value <= u64_max) {\n\
    \        emit_u64_unchecked(cursor, static_cast<u64>(value));\n        return;\n\
    \    }\n\n    const u64 low = static_cast<u64>(value % base);\n    const u128\
    \ upper = value / base;\n    if (upper <= u64_max) {\n        emit_u64_unchecked(cursor,\
    \ static_cast<u64>(upper));\n        emit_padded_16(cursor, low);\n        return;\n\
    \    }\n\n    const u64 middle = static_cast<u64>(upper % base);\n    const u32\
    \ high = static_cast<u32>(upper / base);\n    emit_u32_unchecked(cursor, high);\n\
    \    emit_padded_16(cursor, middle);\n    emit_padded_16(cursor, low);\n}\n\n\
    __attribute__((always_inline)) inline void insert_minus(\n    char* start, char*&\
    \ cursor) noexcept {\n    char* digit = start;\n    while (digit != cursor &&\
    \ *digit == ' ') ++digit;\n    const std::size_t leading = static_cast<std::size_t>(digit\
    \ - start);\n\n    if (leading >= 2) {\n        digit[-1] = '-';\n    } else if\
    \ (leading == 1) {\n        std::memmove(digit + 1, digit, static_cast<std::size_t>(cursor\
    \ - digit));\n        *digit = '-';\n        ++cursor;\n    } else {\n       \
    \ std::memmove(start + 2, start, static_cast<std::size_t>(cursor - start));\n\
    \        start[0] = ' ';\n        start[1] = '-';\n        cursor += 2;\n    }\n\
    }\n\n__attribute__((always_inline)) inline void write_u32(\n    output& sink,\
    \ char*& cursor, char* end, u32 value) noexcept {\n    if (__builtin_expect(end\
    \ - cursor < 16, 0)) cursor = sink.flush(cursor);\n    emit_u32_unchecked(cursor,\
    \ value);\n}\n\n__attribute__((always_inline)) inline void write_i32(\n    output&\
    \ sink, char*& cursor, char* end, i32 value) noexcept {\n    if (__builtin_expect(end\
    \ - cursor < 16, 0)) cursor = sink.flush(cursor);\n    const bool negative = value\
    \ < 0;\n    const u32 bits = static_cast<u32>(value);\n    const u32 magnitude\
    \ = negative ? u32{0} - bits : bits;\n    char* const start = cursor;\n    emit_u32_unchecked(cursor,\
    \ magnitude);\n    if (negative) insert_minus(start, cursor);\n}\n\n__attribute__((always_inline))\
    \ inline void write_u64(\n    output& sink, char*& cursor, char* end, u64 value)\
    \ noexcept {\n    if (__builtin_expect(end - cursor < 24, 0)) cursor = sink.flush(cursor);\n\
    \    emit_u64_unchecked(cursor, value);\n}\n\n__attribute__((always_inline)) inline\
    \ void write_i64(\n    output& sink, char*& cursor, char* end, i64 value) noexcept\
    \ {\n    if (__builtin_expect(end - cursor < 24, 0)) cursor = sink.flush(cursor);\n\
    \    const bool negative = value < 0;\n    const u64 bits = static_cast<u64>(value);\n\
    \    const u64 magnitude = negative ? u64{0} - bits : bits;\n    char* const start\
    \ = cursor;\n    emit_u64_unchecked(cursor, magnitude);\n    if (negative) insert_minus(start,\
    \ cursor);\n}\n\n__attribute__((always_inline)) inline void write_u128(\n    output&\
    \ sink, char*& cursor, char* end, u128 value) noexcept {\n    if (__builtin_expect(end\
    \ - cursor < 48, 0)) cursor = sink.flush(cursor);\n    emit_u128_unchecked(cursor,\
    \ value);\n}\n\n__attribute__((always_inline)) inline void write_i128(\n    output&\
    \ sink, char*& cursor, char* end, i128 value) noexcept {\n    if (__builtin_expect(end\
    \ - cursor < 48, 0)) cursor = sink.flush(cursor);\n    const bool negative = value\
    \ < 0;\n    const u128 bits = static_cast<u128>(value);\n    const u128 magnitude\
    \ = negative ? u128{0} - bits : bits;\n    char* const start = cursor;\n    emit_u128_unchecked(cursor,\
    \ magnitude);\n    if (negative) insert_minus(start, cursor);\n}\n\n}\n\nstruct\
    \ fastio_unsafe {\n    using i32 = fastio_unsafe_impl::i32;\n    using u32 = fastio_unsafe_impl::u32;\n\
    \    using i64 = fastio_unsafe_impl::i64;\n    using u64 = fastio_unsafe_impl::u64;\n\
    \    using i128 = fastio_unsafe_impl::i128;\n    using u128 = fastio_unsafe_impl::u128;\n\
    \n    fastio_unsafe() = default;\n    fastio_unsafe(const fastio_unsafe&) = delete;\n\
    \    fastio_unsafe& operator=(const fastio_unsafe&) = delete;\n\n    char* input_cursor()\
    \ const noexcept { return in.cursor(); }\n    char* output_cursor() noexcept {\
    \ return out.begin(); }\n    char* output_end() noexcept { return out.end(); }\n\
    \    void finish(char* cursor) noexcept { out.finish(cursor); }\n\n    __attribute__((always_inline))\
    \ u32 read_u32(char*& cursor) noexcept {\n        return fastio_unsafe_impl::read_u32(cursor);\n\
    \    }\n    __attribute__((always_inline)) i32 read_i32(char*& cursor) noexcept\
    \ {\n        return fastio_unsafe_impl::read_i32(cursor);\n    }\n    __attribute__((always_inline))\
    \ u64 read_u64(char*& cursor) noexcept {\n        return fastio_unsafe_impl::read_u64(cursor);\n\
    \    }\n    __attribute__((always_inline)) i64 read_i64(char*& cursor) noexcept\
    \ {\n        return fastio_unsafe_impl::read_i64(cursor);\n    }\n    __attribute__((always_inline))\
    \ u128 read_u128(char*& cursor) noexcept {\n        return fastio_unsafe_impl::read_u128(cursor);\n\
    \    }\n    __attribute__((always_inline)) i128 read_i128(char*& cursor) noexcept\
    \ {\n        return fastio_unsafe_impl::read_i128(cursor);\n    }\n\n    __attribute__((always_inline))\
    \ void write_u32(\n        char*& cursor, char* end, u32 value) noexcept {\n \
    \       fastio_unsafe_impl::write_u32(out, cursor, end, value);\n    }\n    __attribute__((always_inline))\
    \ void write_i32(\n        char*& cursor, char* end, i32 value) noexcept {\n \
    \       fastio_unsafe_impl::write_i32(out, cursor, end, value);\n    }\n    __attribute__((always_inline))\
    \ void write_u64(\n        char*& cursor, char* end, u64 value) noexcept {\n \
    \       fastio_unsafe_impl::write_u64(out, cursor, end, value);\n    }\n    __attribute__((always_inline))\
    \ void write_i64(\n        char*& cursor, char* end, i64 value) noexcept {\n \
    \       fastio_unsafe_impl::write_i64(out, cursor, end, value);\n    }\n    __attribute__((always_inline))\
    \ void write_u128(\n        char*& cursor, char* end, u128 value) noexcept {\n\
    \        fastio_unsafe_impl::write_u128(out, cursor, end, value);\n    }\n   \
    \ __attribute__((always_inline)) void write_i128(\n        char*& cursor, char*\
    \ end, i128 value) noexcept {\n        fastio_unsafe_impl::write_i128(out, cursor,\
    \ end, value);\n    }\n\n    fastio_unsafe_impl::input in;\n    fastio_unsafe_impl::output\
    \ out;\n};\n#line 19 \"verify/verify-yosupo-io/yosupo-many-aplusb-128bit.test.cpp\"\
    \n\nint main() {\n    fastio_unsafe io;\n    char* input_cursor = io.in.cursor();\n\
    \    char* output_cursor = io.out.begin();\n    char* const output_end = io.out.end();\n\
    \n    const fastio_unsafe_impl::u64 test_count =\n        fastio_unsafe_impl::read_u64(input_cursor);\n\
    \n    constexpr fastio_unsafe_impl::u64 block_size =\n        1u << FASTIO_UNSAFE_BLOCK_LOG;\n\
    \n    alignas(64) __int128_t sums[block_size];\n\n    for (fastio_unsafe_impl::u64\
    \ base = 0;\n         base < test_count; base += block_size) {\n        const\
    \ fastio_unsafe_impl::u64 count =\n            std::min(block_size, test_count\
    \ - base);\n\n        for (fastio_unsafe_impl::u64 i = 0; i < count; ++i) {\n\
    \            const __int128_t a =\n                fastio_unsafe_impl::read_i128(input_cursor);\n\
    \            const __int128_t b =\n                fastio_unsafe_impl::read_i128(input_cursor);\n\
    \            sums[i] = a + b;\n        }\n\n        for (fastio_unsafe_impl::u64\
    \ i = 0; i < count; ++i) {\n            fastio_unsafe_impl::write_i128(\n    \
    \            io.out, output_cursor, output_end, sums[i]);\n        }\n    }\n\
    \    io.out.finish(output_cursor);\n}\n\n#if defined(__clang__) && \\\n    (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute pop\n#endif\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/many_aplusb_128bit\"\n\n\
    #if defined(__GNUC__) && !defined(__clang__) && \\\n    (defined(__x86_64__) ||\
    \ defined(__i386__))\n#pragma GCC optimize(\"O3,unroll-loops\")\n#pragma GCC target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt\")\n#elif defined(__clang__) && \\\n    (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute push( \\\n    __attribute__((target(\"\
    avx2,bmi,bmi2,lzcnt,popcnt,ssse3\"))), \\\n    apply_to = function)\n#endif\n\n\
    #ifndef FASTIO_UNSAFE_BLOCK_LOG\n#define FASTIO_UNSAFE_BLOCK_LOG 12\n#endif\n\n\
    #include \"../../IO/fastio_unsafe.hpp\"\n\nint main() {\n    fastio_unsafe io;\n\
    \    char* input_cursor = io.in.cursor();\n    char* output_cursor = io.out.begin();\n\
    \    char* const output_end = io.out.end();\n\n    const fastio_unsafe_impl::u64\
    \ test_count =\n        fastio_unsafe_impl::read_u64(input_cursor);\n\n    constexpr\
    \ fastio_unsafe_impl::u64 block_size =\n        1u << FASTIO_UNSAFE_BLOCK_LOG;\n\
    \n    alignas(64) __int128_t sums[block_size];\n\n    for (fastio_unsafe_impl::u64\
    \ base = 0;\n         base < test_count; base += block_size) {\n        const\
    \ fastio_unsafe_impl::u64 count =\n            std::min(block_size, test_count\
    \ - base);\n\n        for (fastio_unsafe_impl::u64 i = 0; i < count; ++i) {\n\
    \            const __int128_t a =\n                fastio_unsafe_impl::read_i128(input_cursor);\n\
    \            const __int128_t b =\n                fastio_unsafe_impl::read_i128(input_cursor);\n\
    \            sums[i] = a + b;\n        }\n\n        for (fastio_unsafe_impl::u64\
    \ i = 0; i < count; ++i) {\n            fastio_unsafe_impl::write_i128(\n    \
    \            io.out, output_cursor, output_end, sums[i]);\n        }\n    }\n\
    \    io.out.finish(output_cursor);\n}\n\n#if defined(__clang__) && \\\n    (defined(__x86_64__)\
    \ || defined(__i386__))\n#pragma clang attribute pop\n#endif\n"
  dependsOn:
  - IO/fastio_unsafe.hpp
  isVerificationFile: true
  path: verify/verify-yosupo-io/yosupo-many-aplusb-128bit.test.cpp
  requiredBy: []
  timestamp: '2026-08-12 12:17:36+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-io/yosupo-many-aplusb-128bit.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-io/yosupo-many-aplusb-128bit.test.cpp
- /verify/verify/verify-yosupo-io/yosupo-many-aplusb-128bit.test.cpp.html
title: verify/verify-yosupo-io/yosupo-many-aplusb-128bit.test.cpp
---

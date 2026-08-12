---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: IO/fastio_safe.hpp
    title: fastio_safe
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
  bundledCode: "#line 1 \"verify/verify-yosupo-io/yosupo-many-aplusb-128bit-fastio-safe.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/many_aplusb_128bit\"\n\n#ifdef\
    \ __linux__\n#define FASTIO_SAFE_USE_POSIX 1\n#endif\n#line 1 \"IO/fastio_safe.hpp\"\
    \n\n\n\n#include <algorithm>\n#include <array>\n#include <charconv>\n#include\
    \ <cerrno>\n#include <cstddef>\n#include <cstdint>\n#include <cstdio>\n#include\
    \ <cstdlib>\n#include <cstring>\n#include <istream>\n#include <limits>\n#include\
    \ <ostream>\n#include <string>\n#include <string_view>\n#include <system_error>\n\
    #include <type_traits>\n#include <utility>\n#include <vector>\n\n#ifdef FASTIO_SAFE_USE_POSIX\n\
    #include <unistd.h>\n#endif\n\n#if (defined(__GNUC__) || defined(__clang__)) &&\
    \ \\\n    (defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86))\n\
    #include <immintrin.h>\n#endif\n\nstatic_assert(__cplusplus >= 201703L, \"fastio_safe.hpp\
    \ requires C++17 or newer\");\n\n// Stream helpers kept for compatibility with\
    \ the original snippet.\ntemplate<class T, class U>\nstd::ostream& operator<<(std::ostream&\
    \ os, const std::pair<T, U>& value);\n\ntemplate<class T>\nstd::ostream& operator<<(std::ostream&\
    \ os, const std::vector<T>& value);\n\ntemplate<class T, class U>\nstd::istream&\
    \ operator>>(std::istream& is, std::pair<T, U>& value);\n\ntemplate<class T>\n\
    std::istream& operator>>(std::istream& is, std::vector<T>& value);\n\ntemplate<class\
    \ T, class U>\nstd::ostream& operator<<(std::ostream& os, const std::pair<T, U>&\
    \ value) {\n    return os << value.first << ' ' << value.second;\n}\n\ntemplate<class\
    \ T>\nstd::ostream& operator<<(std::ostream& os, const std::vector<T>& value)\
    \ {\n    for (std::size_t i = 0; i < value.size(); ++i) {\n        if (i != 0)\
    \ os.put(' ');\n        os << value[i];\n    }\n    return os;\n}\n\ntemplate<class\
    \ T, class U>\nstd::istream& operator>>(std::istream& is, std::pair<T, U>& value)\
    \ {\n    return is >> value.first >> value.second;\n}\n\ntemplate<class T>\nstd::istream&\
    \ operator>>(std::istream& is, std::vector<T>& value) {\n    if constexpr (std::is_same_v<T,\
    \ bool>) {\n        for (std::size_t i = 0; i < value.size(); ++i) {\n       \
    \     bool element = false;\n            if (!(is >> element)) return is;\n  \
    \          value[i] = element;\n        }\n    } else {\n        for (auto& element\
    \ : value) is >> element;\n    }\n    return is;\n}\n\nnamespace fastio_safe {\n\
    \nenum class scan_error {\n    none,\n    end_of_file,\n    invalid_input,\n \
    \   out_of_range,\n    io_error\n};\n\nnamespace detail {\n\ninline constexpr\
    \ std::size_t input_buffer_size = 1u << 20;   // 1 MiB\ninline constexpr std::size_t\
    \ output_buffer_size = 1u << 18;  // 256 KiB\n\n#if (defined(__GNUC__) || defined(__clang__))\
    \ && \\\n    (defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86))\n\
    __attribute__((target(\"avx2\")))\ninline std::size_t find_non_space_avx2(const\
    \ unsigned char* data, std::size_t size) noexcept {\n    const __m256i sign_bias\
    \ = _mm256_set1_epi8(static_cast<char>(0x80));\n    const __m256i biased_space\
    \ = _mm256_set1_epi8(static_cast<char>(0x20 ^ 0x80));\n    std::size_t i = 0;\n\
    \    for (; size - i >= 32; i += 32) {\n        const __m256i bytes = _mm256_loadu_si256(\n\
    \            reinterpret_cast<const __m256i*>(data + i));\n        const __m256i\
    \ biased = _mm256_xor_si256(bytes, sign_bias);\n        const __m256i greater\
    \ = _mm256_cmpgt_epi8(biased, biased_space);\n        const std::uint32_t mask\
    \ = static_cast<std::uint32_t>(\n            _mm256_movemask_epi8(greater));\n\
    \        if (mask != 0) return i + static_cast<std::size_t>(__builtin_ctz(mask));\n\
    \    }\n    for (; i < size; ++i) {\n        if (data[i] > 0x20u) return i;\n\
    \    }\n    return size;\n}\n\ninline bool runtime_has_avx2() noexcept {\n   \
    \ static const bool available = []() noexcept {\n#if defined(__GNUC__) && !defined(__clang__)\n\
    \        __builtin_cpu_init();\n#endif\n        return __builtin_cpu_supports(\"\
    avx2\");\n    }();\n    return available;\n}\n#endif\n\nclass input_buffer {\n\
    public:\n    input_buffer() = default;\n    input_buffer(const input_buffer&)\
    \ = delete;\n    input_buffer& operator=(const input_buffer&) = delete;\n\n  \
    \  void begin_value() noexcept { error_ = scan_error::none; }\n    scan_error\
    \ error() const noexcept { return error_; }\n    void set_error(scan_error error)\
    \ noexcept { error_ = error; }\n\n    bool skip_whitespace() noexcept {\n    \
    \    begin_value();\n        for (;;) {\n            if (!ensure_data()) return\
    \ false;\n\n            std::size_t available = size_ - position_;\n         \
    \   std::size_t prefix = 0;\n            const std::size_t scalar_prefix = std::min<std::size_t>(available,\
    \ 16);\n            while (prefix < scalar_prefix &&\n                   static_cast<unsigned\
    \ char>(buffer_[position_ + prefix]) <= 0x20u) {\n                ++prefix;\n\
    \            }\n            position_ += prefix;\n            if (prefix < scalar_prefix)\
    \ return true;\n            if (position_ == size_) continue;\n\n            available\
    \ = size_ - position_;\n#if (defined(__GNUC__) || defined(__clang__)) && \\\n\
    \    (defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86))\n\
    \            if (available >= 32 && runtime_has_avx2()) {\n                const\
    \ std::size_t offset = find_non_space_avx2(\n                    reinterpret_cast<const\
    \ unsigned char*>(buffer_.data() + position_),\n                    available);\n\
    \                position_ += offset;\n                if (offset < available)\
    \ return true;\n                continue;\n            }\n#endif\n           \
    \ while (position_ < size_ &&\n                   static_cast<unsigned char>(buffer_[position_])\
    \ <= 0x20u) {\n                ++position_;\n            }\n            if (position_\
    \ < size_) return true;\n        }\n    }\n\n    bool peek(char& value) noexcept\
    \ {\n        if (!ensure_data()) return false;\n        value = buffer_[position_];\n\
    \        return true;\n    }\n\n    bool get(char& value) noexcept {\n       \
    \ if (!peek(value)) return false;\n        ++position_;\n        return true;\n\
    \    }\n\n    void consume() noexcept { ++position_; }\n\n    void discard_token()\
    \ noexcept {\n        char value = 0;\n        while (peek(value) && static_cast<unsigned\
    \ char>(value) > 0x20u) consume();\n    }\n\n    template<class Sink>\n    bool\
    \ read_token_chunks(Sink&& sink) {\n        if (!skip_whitespace()) return false;\n\
    \        bool found = false;\n        for (;;) {\n            if (!ensure_data())\
    \ {\n                if (found) {\n                    error_ = scan_error::none;\n\
    \                    return true;\n                }\n                return false;\n\
    \            }\n\n            const std::size_t start = position_;\n         \
    \   while (position_ < size_ &&\n                   static_cast<unsigned char>(buffer_[position_])\
    \ > 0x20u) {\n                ++position_;\n            }\n            const std::size_t\
    \ length = position_ - start;\n            if (length != 0) {\n              \
    \  sink(buffer_.data() + start, length);\n                found = true;\n    \
    \        }\n            if (position_ < size_) {\n                error_ = scan_error::none;\n\
    \                return found;\n            }\n        }\n    }\n\nprivate:\n\
    \    bool ensure_data() noexcept {\n        if (position_ < size_) return true;\n\
    \        position_ = 0;\n        size_ = 0;\n\n#ifdef FASTIO_SAFE_USE_POSIX\n\
    \        for (;;) {\n            const ssize_t count = ::read(0, buffer_.data(),\
    \ input_buffer_size);\n            if (count > 0) {\n                size_ = static_cast<std::size_t>(count);\n\
    \                break;\n            }\n            if (count == 0) {\n      \
    \          error_ = scan_error::end_of_file;\n                return false;\n\
    \            }\n            if (errno == EINTR) continue;\n            error_\
    \ = scan_error::io_error;\n            return false;\n        }\n#else\n     \
    \   errno = 0;\n        size_ = std::fread(buffer_.data(), 1, input_buffer_size,\
    \ stdin);\n        if (size_ == 0) {\n            error_ = std::feof(stdin) ?\
    \ scan_error::end_of_file : scan_error::io_error;\n            return false;\n\
    \        }\n#endif\n        return true;\n    }\n\n    alignas(64) std::array<char,\
    \ input_buffer_size> buffer_{};\n    std::size_t position_ = 0;\n    std::size_t\
    \ size_ = 0;\n    scan_error error_ = scan_error::none;\n};\n\ninline input_buffer&\
    \ input() {\n    static input_buffer instance;\n    return instance;\n}\n\nclass\
    \ output_buffer {\npublic:\n    output_buffer(std::FILE* stream, int descriptor)\
    \ noexcept\n        : stream_(stream), descriptor_(descriptor) {}\n\n    output_buffer(const\
    \ output_buffer&) = delete;\n    output_buffer& operator=(const output_buffer&)\
    \ = delete;\n\n    ~output_buffer() { (void)flush(); }\n\n    bool put(char value)\
    \ noexcept {\n        if (size_ == buffer_.size() && !flush()) return false;\n\
    \        buffer_[size_++] = value;\n        return true;\n    }\n\n    bool write(const\
    \ char* data, std::size_t length) noexcept {\n        while (length != 0) {\n\
    \            if (size_ == buffer_.size() && !flush()) return false;\n        \
    \    const std::size_t count = std::min(length, buffer_.size() - size_);\n   \
    \         std::memcpy(buffer_.data() + size_, data, count);\n            size_\
    \ += count;\n            data += count;\n            length -= count;\n      \
    \  }\n        return true;\n    }\n\n    bool flush() noexcept {\n        std::size_t\
    \ written = 0;\n        while (written < size_) {\n#ifdef FASTIO_SAFE_USE_POSIX\n\
    \            const ssize_t count = ::write(\n                descriptor_, buffer_.data()\
    \ + written, size_ - written);\n            if (count > 0) {\n               \
    \ written += static_cast<std::size_t>(count);\n                continue;\n   \
    \         }\n            if (count < 0 && errno == EINTR) continue;\n#else\n \
    \           errno = 0;\n            const std::size_t count = std::fwrite(\n \
    \               buffer_.data() + written, 1, size_ - written, stream_);\n    \
    \        if (count > 0) {\n                written += count;\n               \
    \ continue;\n            }\n            if (errno == EINTR) {\n              \
    \  std::clearerr(stream_);\n                continue;\n            }\n#endif\n\
    \            preserve_unwritten(written);\n            failed_ = true;\n     \
    \       return false;\n        }\n        size_ = 0;\n        return true;\n \
    \   }\n\n    bool failed() const noexcept { return failed_; }\n\nprivate:\n  \
    \  void preserve_unwritten(std::size_t written) noexcept {\n        if (written\
    \ != 0 && written < size_) {\n            std::memmove(buffer_.data(), buffer_.data()\
    \ + written, size_ - written);\n        }\n        size_ -= written;\n    }\n\n\
    \    alignas(64) std::array<char, output_buffer_size> buffer_{};\n    std::size_t\
    \ size_ = 0;\n    std::FILE* stream_ = nullptr;\n    int descriptor_ = -1;\n \
    \   bool failed_ = false;\n};\n\ninline output_buffer& output() {\n    static\
    \ output_buffer instance(stdout, 1);\n    return instance;\n}\n\ninline output_buffer&\
    \ error_output() {\n    static output_buffer instance(stderr, 2);\n    return\
    \ instance;\n}\n\ntemplate<class T>\nstruct is_extended_integer : std::is_integral<T>\
    \ {};\n\n#ifdef __SIZEOF_INT128__\ntemplate<> struct is_extended_integer<__int128_t>\
    \ : std::true_type {};\ntemplate<> struct is_extended_integer<__uint128_t> : std::true_type\
    \ {};\n#endif\n\ntemplate<class T>\ninline constexpr bool is_extended_integer_v\
    \ = is_extended_integer<T>::value;\n\ntemplate<class T>\nstruct make_unsigned_extended\
    \ {\n    using type = std::make_unsigned_t<T>;\n};\n\n#ifdef __SIZEOF_INT128__\n\
    template<> struct make_unsigned_extended<__int128_t> { using type = __uint128_t;\
    \ };\ntemplate<> struct make_unsigned_extended<__uint128_t> { using type = __uint128_t;\
    \ };\n#endif\n\ntemplate<class T>\nusing make_unsigned_extended_t = typename make_unsigned_extended<T>::type;\n\
    \ntemplate<class T>\nbool read_integer(T& result) noexcept {\n    static_assert(is_extended_integer_v<T>);\n\
    \    using unsigned_type = make_unsigned_extended_t<T>;\n\n    input_buffer& source\
    \ = input();\n    if (!source.skip_whitespace()) return false;\n\n    char current\
    \ = 0;\n    if (!source.peek(current)) return false;\n    bool negative = false;\n\
    \    if (current == '+' || current == '-') {\n        negative = current == '-';\n\
    \        source.consume();\n    }\n\n    if constexpr (std::is_unsigned_v<T>\n\
    #ifdef __SIZEOF_INT128__\n                  || std::is_same_v<T, __uint128_t>\n\
    #endif\n    ) {\n        if (negative) {\n            source.discard_token();\n\
    \            source.set_error(scan_error::invalid_input);\n            return\
    \ false;\n        }\n    }\n\n    const unsigned_type positive_limit =\n     \
    \   static_cast<unsigned_type>(std::numeric_limits<T>::max());\n    const unsigned_type\
    \ limit = negative ? positive_limit + 1 : positive_limit;\n    unsigned_type value\
    \ = 0;\n    bool has_digit = false;\n    bool overflow = false;\n\n    while (source.peek(current)\
    \ && current >= '0' && current <= '9') {\n        has_digit = true;\n        source.consume();\n\
    \        const unsigned_type digit = static_cast<unsigned_type>(current - '0');\n\
    \        if (!overflow) {\n            if (value > (limit - digit) / 10) overflow\
    \ = true;\n            else value = value * 10 + digit;\n        }\n    }\n\n\
    \    if (!has_digit) {\n        source.discard_token();\n        source.set_error(scan_error::invalid_input);\n\
    \        return false;\n    }\n    if (overflow) {\n        source.set_error(scan_error::out_of_range);\n\
    \        return false;\n    }\n\n    if (negative) {\n        if (value == positive_limit\
    \ + 1) {\n            result = std::numeric_limits<T>::min();\n        } else\
    \ {\n            result = static_cast<T>(-static_cast<T>(value));\n        }\n\
    \    } else {\n        result = static_cast<T>(value);\n    }\n    source.set_error(scan_error::none);\n\
    \    return true;\n}\n\nclass small_token {\npublic:\n    void append(const char*\
    \ data, std::size_t length) {\n        if (large_.empty() && size_ + length <=\
    \ small_.size()) {\n            std::memcpy(small_.data() + size_, data, length);\n\
    \            size_ += length;\n            return;\n        }\n        if (large_.empty())\
    \ large_.assign(small_.data(), size_);\n        large_.append(data, length);\n\
    \        size_ += length;\n    }\n\n    const char* begin() const noexcept {\n\
    \        return large_.empty() ? small_.data() : large_.data();\n    }\n    const\
    \ char* end() const noexcept { return begin() + size_; }\n\nprivate:\n    std::array<char,\
    \ 128> small_{};\n    std::string large_;\n    std::size_t size_ = 0;\n};\n\n\
    template<class T>\nbool read_floating(T& result) {\n    small_token token;\n \
    \   input_buffer& source = input();\n    if (!source.read_token_chunks(\n    \
    \        [&](const char* data, std::size_t length) { token.append(data, length);\
    \ })) {\n        return false;\n    }\n\n    const char* first = token.begin();\n\
    \    const char* last = token.end();\n    if (first != last && *first == '+')\
    \ ++first;\n    T value{};\n    const auto parsed = std::from_chars(first, last,\
    \ value, std::chars_format::general);\n    if (parsed.ec == std::errc::result_out_of_range)\
    \ {\n        source.set_error(scan_error::out_of_range);\n        return false;\n\
    \    }\n    if (parsed.ec != std::errc{} || parsed.ptr != last) {\n        source.set_error(scan_error::invalid_input);\n\
    \        return false;\n    }\n    result = value;\n    source.set_error(scan_error::none);\n\
    \    return true;\n}\n\nconstexpr std::array<char, 200> make_digit_pairs() noexcept\
    \ {\n    std::array<char, 200> result{};\n    for (std::size_t i = 0; i < 100;\
    \ ++i) {\n        result[i * 2] = static_cast<char>('0' + i / 10);\n        result[i\
    \ * 2 + 1] = static_cast<char>('0' + i % 10);\n    }\n    return result;\n}\n\n\
    inline constexpr auto digit_pairs = make_digit_pairs();\n\ninline char* format_u64_backward(char*\
    \ end, std::uint64_t value) noexcept {\n    while (value >= 100) {\n        const\
    \ std::uint64_t quotient = value / 100;\n        const std::size_t remainder =\
    \ static_cast<std::size_t>(value - quotient * 100);\n        end -= 2;\n     \
    \   end[0] = digit_pairs[remainder * 2];\n        end[1] = digit_pairs[remainder\
    \ * 2 + 1];\n        value = quotient;\n    }\n    if (value < 10) {\n       \
    \ *--end = static_cast<char>('0' + value);\n    } else {\n        end -= 2;\n\
    \        const std::size_t index = static_cast<std::size_t>(value) * 2;\n    \
    \    end[0] = digit_pairs[index];\n        end[1] = digit_pairs[index + 1];\n\
    \    }\n    return end;\n}\n\ninline bool write_u64(output_buffer& destination,\
    \ std::uint64_t value) noexcept {\n    char storage[32];\n    char* const end\
    \ = storage + sizeof(storage);\n    char* const begin = format_u64_backward(end,\
    \ value);\n    return destination.write(begin, static_cast<std::size_t>(end -\
    \ begin));\n}\n\ninline bool write_i64(output_buffer& destination, std::int64_t\
    \ value) noexcept {\n    std::uint64_t magnitude = static_cast<std::uint64_t>(value);\n\
    \    if (value < 0) {\n        if (!destination.put('-')) return false;\n    \
    \    magnitude = std::uint64_t{0} - magnitude;\n    }\n    return write_u64(destination,\
    \ magnitude);\n}\n\n#ifdef __SIZEOF_INT128__\ninline bool write_padded_19(output_buffer&\
    \ destination, std::uint64_t value) noexcept {\n    char storage[19];\n    char*\
    \ cursor = storage + sizeof(storage);\n    for (int i = 0; i < 9; ++i) {\n   \
    \     const std::uint64_t quotient = value / 100;\n        const std::size_t remainder\
    \ = static_cast<std::size_t>(value - quotient * 100);\n        cursor -= 2;\n\
    \        cursor[0] = digit_pairs[remainder * 2];\n        cursor[1] = digit_pairs[remainder\
    \ * 2 + 1];\n        value = quotient;\n    }\n    storage[0] = static_cast<char>('0'\
    \ + value);\n    return destination.write(storage, sizeof(storage));\n}\n\ninline\
    \ bool write_u128(output_buffer& destination, __uint128_t value) noexcept {\n\
    \    constexpr __uint128_t base = static_cast<__uint128_t>(10000000000000000000ULL);\n\
    \    std::uint64_t chunks[3]{};\n    int count = 0;\n    do {\n        chunks[count++]\
    \ = static_cast<std::uint64_t>(value % base);\n        value /= base;\n    } while\
    \ (value != 0);\n\n    if (!write_u64(destination, chunks[count - 1])) return\
    \ false;\n    while (--count != 0) {\n        if (!write_padded_19(destination,\
    \ chunks[count - 1])) return false;\n    }\n    return true;\n}\n\ninline bool\
    \ write_i128(output_buffer& destination, __int128_t value) noexcept {\n    __uint128_t\
    \ magnitude = static_cast<__uint128_t>(value);\n    if (value < 0) {\n       \
    \ if (!destination.put('-')) return false;\n        magnitude = __uint128_t{0}\
    \ - magnitude;\n    }\n    return write_u128(destination, magnitude);\n}\n#endif\n\
    \ntemplate<class T>\nbool write_floating(output_buffer& destination, T value)\
    \ noexcept {\n    char storage[128];\n    const auto converted = std::to_chars(\n\
    \        storage, storage + sizeof(storage), value, std::chars_format::general,\n\
    \        std::numeric_limits<T>::max_digits10);\n    if (converted.ec != std::errc{})\
    \ return false;\n    return destination.write(storage,\n                     \
    \        static_cast<std::size_t>(converted.ptr - storage));\n}\n\ntemplate<class\
    \ T, class U>\nbool write_value(output_buffer& destination, const std::pair<T,\
    \ U>& value);\n\ntemplate<class T>\nbool write_value(output_buffer& destination,\
    \ const std::vector<T>& value);\n\ninline bool write_value(output_buffer& destination,\
    \ bool value) noexcept {\n    return destination.put(value ? '1' : '0');\n}\n\
    inline bool write_value(output_buffer& destination, char value) noexcept {\n \
    \   return destination.put(value);\n}\ninline bool write_value(output_buffer&\
    \ destination, signed char value) noexcept {\n    return destination.put(static_cast<char>(value));\n\
    }\ninline bool write_value(output_buffer& destination, unsigned char value) noexcept\
    \ {\n    return destination.put(static_cast<char>(value));\n}\n\ntemplate<class\
    \ T, std::enable_if_t<\n    is_extended_integer_v<T> && sizeof(T) <= sizeof(std::uint64_t)\
    \ &&\n    !std::is_same_v<T, bool> && !std::is_same_v<T, char> &&\n    !std::is_same_v<T,\
    \ signed char> && !std::is_same_v<T, unsigned char>, int> = 0>\nbool write_value(output_buffer&\
    \ destination, T value) noexcept {\n    if constexpr (std::is_signed_v<T>) {\n\
    \        return write_i64(destination, static_cast<std::int64_t>(value));\n  \
    \  } else {\n        return write_u64(destination, static_cast<std::uint64_t>(value));\n\
    \    }\n}\n\n#ifdef __SIZEOF_INT128__\ninline bool write_value(output_buffer&\
    \ destination, __int128_t value) noexcept {\n    return write_i128(destination,\
    \ value);\n}\ninline bool write_value(output_buffer& destination, __uint128_t\
    \ value) noexcept {\n    return write_u128(destination, value);\n}\n#endif\n\n\
    inline bool write_value(output_buffer& destination, float value) noexcept {\n\
    \    return write_floating(destination, value);\n}\ninline bool write_value(output_buffer&\
    \ destination, double value) noexcept {\n    return write_floating(destination,\
    \ value);\n}\ninline bool write_value(output_buffer& destination, long double\
    \ value) noexcept {\n    return write_floating(destination, value);\n}\ninline\
    \ bool write_value(output_buffer& destination, const std::string& value) noexcept\
    \ {\n    return destination.write(value.data(), value.size());\n}\ninline bool\
    \ write_value(output_buffer& destination, std::string_view value) noexcept {\n\
    \    return destination.write(value.data(), value.size());\n}\ninline bool write_value(output_buffer&\
    \ destination, const char* value) noexcept {\n    if (value == nullptr) return\
    \ destination.write(\"null\", 4);\n    return destination.write(value, std::strlen(value));\n\
    }\ninline bool write_value(output_buffer& destination, char* value) noexcept {\n\
    \    return write_value(destination, const_cast<const char*>(value));\n}\ninline\
    \ bool write_value(output_buffer& destination, std::nullptr_t) noexcept {\n  \
    \  return destination.write(\"null\", 4);\n}\n\ntemplate<class T, class U>\nbool\
    \ write_value(output_buffer& destination, const std::pair<T, U>& value) {\n  \
    \  return write_value(destination, value.first) && destination.put(' ') &&\n \
    \          write_value(destination, value.second);\n}\n\ntemplate<class T>\nbool\
    \ write_value(output_buffer& destination, const std::vector<T>& value) {\n   \
    \ for (std::size_t i = 0; i < value.size(); ++i) {\n        if (i != 0 && !destination.put('\
    \ ')) return false;\n        if (!write_value(destination, value[i])) return false;\n\
    \    }\n    return true;\n}\n\n} // namespace detail\n\ninline scan_error last_scan_error()\
    \ noexcept { return detail::input().error(); }\n\ninline bool read_one(bool& value)\
    \ noexcept {\n    unsigned long long integer = 0;\n    if (!detail::read_integer(integer))\
    \ return false;\n    value = integer != 0;\n    return true;\n}\n\ninline bool\
    \ read_one(char& value) noexcept {\n    detail::input_buffer& source = detail::input();\n\
    \    if (!source.skip_whitespace()) return false;\n    return source.get(value);\n\
    }\n\ninline bool read_one(signed char& value) noexcept {\n    char character =\
    \ 0;\n    if (!read_one(character)) return false;\n    value = static_cast<signed\
    \ char>(character);\n    return true;\n}\n\ninline bool read_one(unsigned char&\
    \ value) noexcept {\n    char character = 0;\n    if (!read_one(character)) return\
    \ false;\n    value = static_cast<unsigned char>(character);\n    return true;\n\
    }\n\ntemplate<class T, std::enable_if_t<\n    detail::is_extended_integer_v<T>\
    \ && !std::is_same_v<T, bool> &&\n    !std::is_same_v<T, char> && !std::is_same_v<T,\
    \ signed char> &&\n    !std::is_same_v<T, unsigned char>, int> = 0>\nbool read_one(T&\
    \ value) noexcept {\n    return detail::read_integer(value);\n}\n\ninline bool\
    \ read_one(float& value) { return detail::read_floating(value); }\ninline bool\
    \ read_one(double& value) { return detail::read_floating(value); }\ninline bool\
    \ read_one(long double& value) { return detail::read_floating(value); }\n\ninline\
    \ bool read_one(std::string& value) {\n    value.clear();\n    return detail::input().read_token_chunks(\n\
    \        [&](const char* data, std::size_t length) { value.append(data, length);\
    \ });\n}\n\ntemplate<class T, class U>\nbool read_one(std::pair<T, U>& value);\n\
    \ntemplate<class T>\nbool read_one(std::vector<T>& value);\n\ninline bool read_one(std::vector<bool>&\
    \ value);\n\ntemplate<class T, class U>\nbool read_one(std::pair<T, U>& value)\
    \ {\n    return read_one(value.first) && read_one(value.second);\n}\n\ntemplate<class\
    \ T>\nbool read_one(std::vector<T>& value) {\n    for (auto& element : value)\
    \ {\n        if (!read_one(element)) return false;\n    }\n    return true;\n\
    }\n\ninline bool read_one(std::vector<bool>& value) {\n    for (std::size_t i\
    \ = 0; i < value.size(); ++i) {\n        bool element = false;\n        if (!read_one(element))\
    \ return false;\n        value[i] = element;\n    }\n    return true;\n}\n\ntemplate<class\
    \ T>\nbool write_one(const T& value) {\n    return detail::write_value(detail::output(),\
    \ value);\n}\n\ntemplate<class T>\nbool write_one_err(const T& value) {\n    return\
    \ detail::write_value(detail::error_output(), value);\n}\n\ninline bool write_char(char\
    \ value) noexcept { return detail::output().put(value); }\ninline bool write_char_err(char\
    \ value) noexcept { return detail::error_output().put(value); }\ninline bool flush_out()\
    \ noexcept { return detail::output().flush(); }\ninline bool flush_err() noexcept\
    \ { return detail::error_output().flush(); }\n\ntemplate<class... T>\nbool scan(T&...\
    \ values) {\n    return (read_one(values) && ...);\n}\n\ntemplate<class... T>\n\
    bool print(const T&... values) {\n    detail::output_buffer& destination = detail::output();\n\
    \    bool ok = true;\n    bool first = true;\n    auto append = [&](const auto&\
    \ value) {\n        if (!ok) return;\n        if (!first) ok = destination.put('\
    \ ');\n        first = false;\n        if (ok) ok = detail::write_value(destination,\
    \ value);\n    };\n    (append(values), ...);\n    return ok && destination.put('\\\
    n');\n}\n\ntemplate<class... T>\nbool printflush(const T&... values) {\n    return\
    \ print(values...) && flush_out();\n}\n\ntemplate<class... T>\nbool printout(const\
    \ T&... values) {\n    detail::output_buffer& destination = detail::output();\n\
    \    bool ok = true;\n    auto append = [&](const auto& value) {\n        if (ok)\
    \ ok = detail::write_value(destination, value);\n    };\n    (append(values),\
    \ ...);\n    return ok;\n}\n\ntemplate<class... T>\nbool debug(const T&... values)\
    \ {\n    detail::output_buffer& destination = detail::error_output();\n    bool\
    \ ok = true;\n    bool first = true;\n    auto append = [&](const auto& value)\
    \ {\n        if (!ok) return;\n        if (!first) ok = destination.put(' ');\n\
    \        first = false;\n        if (ok) ok = detail::write_value(destination,\
    \ value);\n    };\n    (append(values), ...);\n    return ok && destination.put('\\\
    n') && destination.flush();\n}\n\ntemplate<class... T>\nbool debugout(const T&...\
    \ values) {\n    detail::output_buffer& destination = detail::error_output();\n\
    \    bool ok = true;\n    auto append = [&](const auto& value) {\n        if (ok)\
    \ ok = detail::write_value(destination, value);\n    };\n    (append(values),\
    \ ...);\n    return ok;\n}\n\ntemplate<class... T>\n[[noreturn]] void fin(const\
    \ T&... values) {\n    (void)print(values...);\n    (void)flush_out();\n    (void)flush_err();\n\
    \    std::exit(0);\n}\n\n} // namespace fastio_safe\n\n// Selected global imports\
    \ preserve the original call style without importing the entire namespace.\nusing\
    \ fastio_safe::debug;\nusing fastio_safe::debugout;\nusing fastio_safe::fin;\n\
    using fastio_safe::flush_err;\nusing fastio_safe::flush_out;\nusing fastio_safe::last_scan_error;\n\
    using fastio_safe::print;\nusing fastio_safe::printflush;\nusing fastio_safe::printout;\n\
    using fastio_safe::scan;\nusing fastio_safe::scan_error;\n\n#define INT(...) int\
    \ __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)\n#define LL(...) long long __VA_ARGS__;\
    \ ::fastio_safe::scan(__VA_ARGS__)\n#define STR(...) std::string __VA_ARGS__;\
    \ ::fastio_safe::scan(__VA_ARGS__)\n#define CHR(...) char __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)\n\
    #define DBL(...) double __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)\n#define\
    \ LD(...) long double __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)\n\n\n#line\
    \ 7 \"verify/verify-yosupo-io/yosupo-many-aplusb-128bit-fastio-safe.test.cpp\"\
    \n\nint main() {\n    std::uint32_t test_count;\n    if (!fastio_safe::scan(test_count))\
    \ return 1;\n\n    while (test_count-- != 0) {\n        __int128_t a, b;\n   \
    \     if (!fastio_safe::scan(a, b)) return 1;\n        if (!fastio_safe::print(a\
    \ + b)) return 1;\n    }\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/many_aplusb_128bit\"\n\n\
    #ifdef __linux__\n#define FASTIO_SAFE_USE_POSIX 1\n#endif\n#include \"../../IO/fastio_safe.hpp\"\
    \n\nint main() {\n    std::uint32_t test_count;\n    if (!fastio_safe::scan(test_count))\
    \ return 1;\n\n    while (test_count-- != 0) {\n        __int128_t a, b;\n   \
    \     if (!fastio_safe::scan(a, b)) return 1;\n        if (!fastio_safe::print(a\
    \ + b)) return 1;\n    }\n}\n"
  dependsOn:
  - IO/fastio_safe.hpp
  isVerificationFile: true
  path: verify/verify-yosupo-io/yosupo-many-aplusb-128bit-fastio-safe.test.cpp
  requiredBy: []
  timestamp: '2026-08-12 21:41:52+09:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/verify-yosupo-io/yosupo-many-aplusb-128bit-fastio-safe.test.cpp
layout: document
redirect_from:
- /verify/verify/verify-yosupo-io/yosupo-many-aplusb-128bit-fastio-safe.test.cpp
- /verify/verify/verify-yosupo-io/yosupo-many-aplusb-128bit-fastio-safe.test.cpp.html
title: verify/verify-yosupo-io/yosupo-many-aplusb-128bit-fastio-safe.test.cpp
---

#ifndef EEZCP_IO_FASTIO_SAFE_HPP
#define EEZCP_IO_FASTIO_SAFE_HPP

#include <algorithm>
#include <array>
#include <charconv>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>

#ifdef FASTIO_SAFE_USE_POSIX
#include <unistd.h>
#endif

#if (defined(__GNUC__) || defined(__clang__)) && \
    (defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86))
#include <immintrin.h>
#endif

static_assert(__cplusplus >= 201703L, "fastio_safe.hpp requires C++17 or newer");

// Stream helpers kept for compatibility with the original snippet.
template<class T, class U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& value);

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& value);

template<class T, class U>
std::istream& operator>>(std::istream& is, std::pair<T, U>& value);

template<class T>
std::istream& operator>>(std::istream& is, std::vector<T>& value);

template<class T, class U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& value) {
    return os << value.first << ' ' << value.second;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& value) {
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (i != 0) os.put(' ');
        os << value[i];
    }
    return os;
}

template<class T, class U>
std::istream& operator>>(std::istream& is, std::pair<T, U>& value) {
    return is >> value.first >> value.second;
}

template<class T>
std::istream& operator>>(std::istream& is, std::vector<T>& value) {
    if constexpr (std::is_same_v<T, bool>) {
        for (std::size_t i = 0; i < value.size(); ++i) {
            bool element = false;
            if (!(is >> element)) return is;
            value[i] = element;
        }
    } else {
        for (auto& element : value) is >> element;
    }
    return is;
}

namespace fastio_safe {

enum class scan_error {
    none,
    end_of_file,
    invalid_input,
    out_of_range,
    io_error
};

namespace detail {

inline constexpr std::size_t input_buffer_size = 1u << 20;   // 1 MiB
inline constexpr std::size_t output_buffer_size = 1u << 18;  // 256 KiB

#if (defined(__GNUC__) || defined(__clang__)) && \
    (defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86))
__attribute__((target("avx2")))
inline std::size_t find_non_space_avx2(const unsigned char* data, std::size_t size) noexcept {
    const __m256i sign_bias = _mm256_set1_epi8(static_cast<char>(0x80));
    const __m256i biased_space = _mm256_set1_epi8(static_cast<char>(0x20 ^ 0x80));
    std::size_t i = 0;
    for (; size - i >= 32; i += 32) {
        const __m256i bytes = _mm256_loadu_si256(
            reinterpret_cast<const __m256i*>(data + i));
        const __m256i biased = _mm256_xor_si256(bytes, sign_bias);
        const __m256i greater = _mm256_cmpgt_epi8(biased, biased_space);
        const std::uint32_t mask = static_cast<std::uint32_t>(
            _mm256_movemask_epi8(greater));
        if (mask != 0) return i + static_cast<std::size_t>(__builtin_ctz(mask));
    }
    for (; i < size; ++i) {
        if (data[i] > 0x20u) return i;
    }
    return size;
}

inline bool runtime_has_avx2() noexcept {
    static const bool available = []() noexcept {
#if defined(__GNUC__) && !defined(__clang__)
        __builtin_cpu_init();
#endif
        return __builtin_cpu_supports("avx2");
    }();
    return available;
}
#endif

class input_buffer {
public:
    input_buffer() = default;
    input_buffer(const input_buffer&) = delete;
    input_buffer& operator=(const input_buffer&) = delete;

    void begin_value() noexcept { error_ = scan_error::none; }
    scan_error error() const noexcept { return error_; }
    void set_error(scan_error error) noexcept { error_ = error; }

    bool skip_whitespace() noexcept {
        begin_value();
        for (;;) {
            if (!ensure_data()) return false;

            std::size_t available = size_ - position_;
            std::size_t prefix = 0;
            const std::size_t scalar_prefix = std::min<std::size_t>(available, 16);
            while (prefix < scalar_prefix &&
                   static_cast<unsigned char>(buffer_[position_ + prefix]) <= 0x20u) {
                ++prefix;
            }
            position_ += prefix;
            if (prefix < scalar_prefix) return true;
            if (position_ == size_) continue;

            available = size_ - position_;
#if (defined(__GNUC__) || defined(__clang__)) && \
    (defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86))
            if (available >= 32 && runtime_has_avx2()) {
                const std::size_t offset = find_non_space_avx2(
                    reinterpret_cast<const unsigned char*>(buffer_.data() + position_),
                    available);
                position_ += offset;
                if (offset < available) return true;
                continue;
            }
#endif
            while (position_ < size_ &&
                   static_cast<unsigned char>(buffer_[position_]) <= 0x20u) {
                ++position_;
            }
            if (position_ < size_) return true;
        }
    }

    bool peek(char& value) noexcept {
        if (!ensure_data()) return false;
        value = buffer_[position_];
        return true;
    }

    bool get(char& value) noexcept {
        if (!peek(value)) return false;
        ++position_;
        return true;
    }

    void consume() noexcept { ++position_; }

    void discard_token() noexcept {
        char value = 0;
        while (peek(value) && static_cast<unsigned char>(value) > 0x20u) consume();
    }

    template<class Sink>
    bool read_token_chunks(Sink&& sink) {
        if (!skip_whitespace()) return false;
        bool found = false;
        for (;;) {
            if (!ensure_data()) {
                if (found) {
                    error_ = scan_error::none;
                    return true;
                }
                return false;
            }

            const std::size_t start = position_;
            while (position_ < size_ &&
                   static_cast<unsigned char>(buffer_[position_]) > 0x20u) {
                ++position_;
            }
            const std::size_t length = position_ - start;
            if (length != 0) {
                sink(buffer_.data() + start, length);
                found = true;
            }
            if (position_ < size_) {
                error_ = scan_error::none;
                return found;
            }
        }
    }

private:
    bool ensure_data() noexcept {
        if (position_ < size_) return true;
        position_ = 0;
        size_ = 0;

#ifdef FASTIO_SAFE_USE_POSIX
        for (;;) {
            const ssize_t count = ::read(0, buffer_.data(), input_buffer_size);
            if (count > 0) {
                size_ = static_cast<std::size_t>(count);
                break;
            }
            if (count == 0) {
                error_ = scan_error::end_of_file;
                return false;
            }
            if (errno == EINTR) continue;
            error_ = scan_error::io_error;
            return false;
        }
#else
        errno = 0;
        size_ = std::fread(buffer_.data(), 1, input_buffer_size, stdin);
        if (size_ == 0) {
            error_ = std::feof(stdin) ? scan_error::end_of_file : scan_error::io_error;
            return false;
        }
#endif
        return true;
    }

    alignas(64) std::array<char, input_buffer_size> buffer_{};
    std::size_t position_ = 0;
    std::size_t size_ = 0;
    scan_error error_ = scan_error::none;
};

inline input_buffer& input() {
    static input_buffer instance;
    return instance;
}

class output_buffer {
public:
    output_buffer(std::FILE* stream, int descriptor) noexcept
        : stream_(stream), descriptor_(descriptor) {}

    output_buffer(const output_buffer&) = delete;
    output_buffer& operator=(const output_buffer&) = delete;

    ~output_buffer() { (void)flush(); }

    bool put(char value) noexcept {
        if (size_ == buffer_.size() && !flush()) return false;
        buffer_[size_++] = value;
        return true;
    }

    bool write(const char* data, std::size_t length) noexcept {
        while (length != 0) {
            if (size_ == buffer_.size() && !flush()) return false;
            const std::size_t count = std::min(length, buffer_.size() - size_);
            std::memcpy(buffer_.data() + size_, data, count);
            size_ += count;
            data += count;
            length -= count;
        }
        return true;
    }

    bool flush() noexcept {
        std::size_t written = 0;
        while (written < size_) {
#ifdef FASTIO_SAFE_USE_POSIX
            const ssize_t count = ::write(
                descriptor_, buffer_.data() + written, size_ - written);
            if (count > 0) {
                written += static_cast<std::size_t>(count);
                continue;
            }
            if (count < 0 && errno == EINTR) continue;
#else
            errno = 0;
            const std::size_t count = std::fwrite(
                buffer_.data() + written, 1, size_ - written, stream_);
            if (count > 0) {
                written += count;
                continue;
            }
            if (errno == EINTR) {
                std::clearerr(stream_);
                continue;
            }
#endif
            preserve_unwritten(written);
            failed_ = true;
            return false;
        }
        size_ = 0;
        return true;
    }

    bool failed() const noexcept { return failed_; }

private:
    void preserve_unwritten(std::size_t written) noexcept {
        if (written != 0 && written < size_) {
            std::memmove(buffer_.data(), buffer_.data() + written, size_ - written);
        }
        size_ -= written;
    }

    alignas(64) std::array<char, output_buffer_size> buffer_{};
    std::size_t size_ = 0;
    std::FILE* stream_ = nullptr;
    int descriptor_ = -1;
    bool failed_ = false;
};

inline output_buffer& output() {
    static output_buffer instance(stdout, 1);
    return instance;
}

inline output_buffer& error_output() {
    static output_buffer instance(stderr, 2);
    return instance;
}

template<class T>
struct is_extended_integer : std::is_integral<T> {};

#ifdef __SIZEOF_INT128__
template<> struct is_extended_integer<__int128_t> : std::true_type {};
template<> struct is_extended_integer<__uint128_t> : std::true_type {};
#endif

template<class T>
inline constexpr bool is_extended_integer_v = is_extended_integer<T>::value;

template<class T>
struct make_unsigned_extended {
    using type = std::make_unsigned_t<T>;
};

#ifdef __SIZEOF_INT128__
template<> struct make_unsigned_extended<__int128_t> { using type = __uint128_t; };
template<> struct make_unsigned_extended<__uint128_t> { using type = __uint128_t; };
#endif

template<class T>
using make_unsigned_extended_t = typename make_unsigned_extended<T>::type;

template<class T>
bool read_integer(T& result) noexcept {
    static_assert(is_extended_integer_v<T>);
    using unsigned_type = make_unsigned_extended_t<T>;

    input_buffer& source = input();
    if (!source.skip_whitespace()) return false;

    char current = 0;
    if (!source.peek(current)) return false;
    bool negative = false;
    if (current == '+' || current == '-') {
        negative = current == '-';
        source.consume();
    }

    if constexpr (std::is_unsigned_v<T>
#ifdef __SIZEOF_INT128__
                  || std::is_same_v<T, __uint128_t>
#endif
    ) {
        if (negative) {
            source.discard_token();
            source.set_error(scan_error::invalid_input);
            return false;
        }
    }

    const unsigned_type positive_limit =
        static_cast<unsigned_type>(std::numeric_limits<T>::max());
    const unsigned_type limit = negative ? positive_limit + 1 : positive_limit;
    unsigned_type value = 0;
    bool has_digit = false;
    bool overflow = false;

    while (source.peek(current) && current >= '0' && current <= '9') {
        has_digit = true;
        source.consume();
        const unsigned_type digit = static_cast<unsigned_type>(current - '0');
        if (!overflow) {
            if (value > (limit - digit) / 10) overflow = true;
            else value = value * 10 + digit;
        }
    }

    if (!has_digit) {
        source.discard_token();
        source.set_error(scan_error::invalid_input);
        return false;
    }
    if (overflow) {
        source.set_error(scan_error::out_of_range);
        return false;
    }

    if (negative) {
        if (value == positive_limit + 1) {
            result = std::numeric_limits<T>::min();
        } else {
            result = static_cast<T>(-static_cast<T>(value));
        }
    } else {
        result = static_cast<T>(value);
    }
    source.set_error(scan_error::none);
    return true;
}

class small_token {
public:
    void append(const char* data, std::size_t length) {
        if (large_.empty() && size_ + length <= small_.size()) {
            std::memcpy(small_.data() + size_, data, length);
            size_ += length;
            return;
        }
        if (large_.empty()) large_.assign(small_.data(), size_);
        large_.append(data, length);
        size_ += length;
    }

    const char* begin() const noexcept {
        return large_.empty() ? small_.data() : large_.data();
    }
    const char* end() const noexcept { return begin() + size_; }

private:
    std::array<char, 128> small_{};
    std::string large_;
    std::size_t size_ = 0;
};

template<class T>
bool read_floating(T& result) {
    small_token token;
    input_buffer& source = input();
    if (!source.read_token_chunks(
            [&](const char* data, std::size_t length) { token.append(data, length); })) {
        return false;
    }

    const char* first = token.begin();
    const char* last = token.end();
    if (first != last && *first == '+') ++first;
    T value{};
    const auto parsed = std::from_chars(first, last, value, std::chars_format::general);
    if (parsed.ec == std::errc::result_out_of_range) {
        source.set_error(scan_error::out_of_range);
        return false;
    }
    if (parsed.ec != std::errc{} || parsed.ptr != last) {
        source.set_error(scan_error::invalid_input);
        return false;
    }
    result = value;
    source.set_error(scan_error::none);
    return true;
}

constexpr std::array<char, 200> make_digit_pairs() noexcept {
    std::array<char, 200> result{};
    for (std::size_t i = 0; i < 100; ++i) {
        result[i * 2] = static_cast<char>('0' + i / 10);
        result[i * 2 + 1] = static_cast<char>('0' + i % 10);
    }
    return result;
}

inline constexpr auto digit_pairs = make_digit_pairs();

inline char* format_u64_backward(char* end, std::uint64_t value) noexcept {
    while (value >= 100) {
        const std::uint64_t quotient = value / 100;
        const std::size_t remainder = static_cast<std::size_t>(value - quotient * 100);
        end -= 2;
        end[0] = digit_pairs[remainder * 2];
        end[1] = digit_pairs[remainder * 2 + 1];
        value = quotient;
    }
    if (value < 10) {
        *--end = static_cast<char>('0' + value);
    } else {
        end -= 2;
        const std::size_t index = static_cast<std::size_t>(value) * 2;
        end[0] = digit_pairs[index];
        end[1] = digit_pairs[index + 1];
    }
    return end;
}

inline bool write_u64(output_buffer& destination, std::uint64_t value) noexcept {
    char storage[32];
    char* const end = storage + sizeof(storage);
    char* const begin = format_u64_backward(end, value);
    return destination.write(begin, static_cast<std::size_t>(end - begin));
}

inline bool write_i64(output_buffer& destination, std::int64_t value) noexcept {
    std::uint64_t magnitude = static_cast<std::uint64_t>(value);
    if (value < 0) {
        if (!destination.put('-')) return false;
        magnitude = std::uint64_t{0} - magnitude;
    }
    return write_u64(destination, magnitude);
}

#ifdef __SIZEOF_INT128__
inline bool write_padded_19(output_buffer& destination, std::uint64_t value) noexcept {
    char storage[19];
    char* cursor = storage + sizeof(storage);
    for (int i = 0; i < 9; ++i) {
        const std::uint64_t quotient = value / 100;
        const std::size_t remainder = static_cast<std::size_t>(value - quotient * 100);
        cursor -= 2;
        cursor[0] = digit_pairs[remainder * 2];
        cursor[1] = digit_pairs[remainder * 2 + 1];
        value = quotient;
    }
    storage[0] = static_cast<char>('0' + value);
    return destination.write(storage, sizeof(storage));
}

inline bool write_u128(output_buffer& destination, __uint128_t value) noexcept {
    constexpr __uint128_t base = static_cast<__uint128_t>(10000000000000000000ULL);
    std::uint64_t chunks[3]{};
    int count = 0;
    do {
        chunks[count++] = static_cast<std::uint64_t>(value % base);
        value /= base;
    } while (value != 0);

    if (!write_u64(destination, chunks[count - 1])) return false;
    while (--count != 0) {
        if (!write_padded_19(destination, chunks[count - 1])) return false;
    }
    return true;
}

inline bool write_i128(output_buffer& destination, __int128_t value) noexcept {
    __uint128_t magnitude = static_cast<__uint128_t>(value);
    if (value < 0) {
        if (!destination.put('-')) return false;
        magnitude = __uint128_t{0} - magnitude;
    }
    return write_u128(destination, magnitude);
}
#endif

template<class T>
bool write_floating(output_buffer& destination, T value) noexcept {
    char storage[128];
    const auto converted = std::to_chars(
        storage, storage + sizeof(storage), value, std::chars_format::general,
        std::numeric_limits<T>::max_digits10);
    if (converted.ec != std::errc{}) return false;
    return destination.write(storage,
                             static_cast<std::size_t>(converted.ptr - storage));
}

template<class T, class U>
bool write_value(output_buffer& destination, const std::pair<T, U>& value);

template<class T>
bool write_value(output_buffer& destination, const std::vector<T>& value);

inline bool write_value(output_buffer& destination, bool value) noexcept {
    return destination.put(value ? '1' : '0');
}
inline bool write_value(output_buffer& destination, char value) noexcept {
    return destination.put(value);
}
inline bool write_value(output_buffer& destination, signed char value) noexcept {
    return destination.put(static_cast<char>(value));
}
inline bool write_value(output_buffer& destination, unsigned char value) noexcept {
    return destination.put(static_cast<char>(value));
}

template<class T, std::enable_if_t<
    is_extended_integer_v<T> && sizeof(T) <= sizeof(std::uint64_t) &&
    !std::is_same_v<T, bool> && !std::is_same_v<T, char> &&
    !std::is_same_v<T, signed char> && !std::is_same_v<T, unsigned char>, int> = 0>
bool write_value(output_buffer& destination, T value) noexcept {
    if constexpr (std::is_signed_v<T>) {
        return write_i64(destination, static_cast<std::int64_t>(value));
    } else {
        return write_u64(destination, static_cast<std::uint64_t>(value));
    }
}

#ifdef __SIZEOF_INT128__
inline bool write_value(output_buffer& destination, __int128_t value) noexcept {
    return write_i128(destination, value);
}
inline bool write_value(output_buffer& destination, __uint128_t value) noexcept {
    return write_u128(destination, value);
}
#endif

inline bool write_value(output_buffer& destination, float value) noexcept {
    return write_floating(destination, value);
}
inline bool write_value(output_buffer& destination, double value) noexcept {
    return write_floating(destination, value);
}
inline bool write_value(output_buffer& destination, long double value) noexcept {
    return write_floating(destination, value);
}
inline bool write_value(output_buffer& destination, const std::string& value) noexcept {
    return destination.write(value.data(), value.size());
}
inline bool write_value(output_buffer& destination, std::string_view value) noexcept {
    return destination.write(value.data(), value.size());
}
inline bool write_value(output_buffer& destination, const char* value) noexcept {
    if (value == nullptr) return destination.write("null", 4);
    return destination.write(value, std::strlen(value));
}
inline bool write_value(output_buffer& destination, char* value) noexcept {
    return write_value(destination, const_cast<const char*>(value));
}
inline bool write_value(output_buffer& destination, std::nullptr_t) noexcept {
    return destination.write("null", 4);
}

template<class T, class U>
bool write_value(output_buffer& destination, const std::pair<T, U>& value) {
    return write_value(destination, value.first) && destination.put(' ') &&
           write_value(destination, value.second);
}

template<class T>
bool write_value(output_buffer& destination, const std::vector<T>& value) {
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (i != 0 && !destination.put(' ')) return false;
        if (!write_value(destination, value[i])) return false;
    }
    return true;
}

} // namespace detail

inline scan_error last_scan_error() noexcept { return detail::input().error(); }

inline bool read_one(bool& value) noexcept {
    unsigned long long integer = 0;
    if (!detail::read_integer(integer)) return false;
    value = integer != 0;
    return true;
}

inline bool read_one(char& value) noexcept {
    detail::input_buffer& source = detail::input();
    if (!source.skip_whitespace()) return false;
    return source.get(value);
}

inline bool read_one(signed char& value) noexcept {
    char character = 0;
    if (!read_one(character)) return false;
    value = static_cast<signed char>(character);
    return true;
}

inline bool read_one(unsigned char& value) noexcept {
    char character = 0;
    if (!read_one(character)) return false;
    value = static_cast<unsigned char>(character);
    return true;
}

template<class T, std::enable_if_t<
    detail::is_extended_integer_v<T> && !std::is_same_v<T, bool> &&
    !std::is_same_v<T, char> && !std::is_same_v<T, signed char> &&
    !std::is_same_v<T, unsigned char>, int> = 0>
bool read_one(T& value) noexcept {
    return detail::read_integer(value);
}

inline bool read_one(float& value) { return detail::read_floating(value); }
inline bool read_one(double& value) { return detail::read_floating(value); }
inline bool read_one(long double& value) { return detail::read_floating(value); }

inline bool read_one(std::string& value) {
    value.clear();
    return detail::input().read_token_chunks(
        [&](const char* data, std::size_t length) { value.append(data, length); });
}

template<class T, class U>
bool read_one(std::pair<T, U>& value);

template<class T>
bool read_one(std::vector<T>& value);

inline bool read_one(std::vector<bool>& value);

template<class T, class U>
bool read_one(std::pair<T, U>& value) {
    return read_one(value.first) && read_one(value.second);
}

template<class T>
bool read_one(std::vector<T>& value) {
    for (auto& element : value) {
        if (!read_one(element)) return false;
    }
    return true;
}

inline bool read_one(std::vector<bool>& value) {
    for (std::size_t i = 0; i < value.size(); ++i) {
        bool element = false;
        if (!read_one(element)) return false;
        value[i] = element;
    }
    return true;
}

template<class T>
bool write_one(const T& value) {
    return detail::write_value(detail::output(), value);
}

template<class T>
bool write_one_err(const T& value) {
    return detail::write_value(detail::error_output(), value);
}

inline bool write_char(char value) noexcept { return detail::output().put(value); }
inline bool write_char_err(char value) noexcept { return detail::error_output().put(value); }
inline bool flush_out() noexcept { return detail::output().flush(); }
inline bool flush_err() noexcept { return detail::error_output().flush(); }

template<class... T>
bool scan(T&... values) {
    return (read_one(values) && ...);
}

template<class... T>
bool print(const T&... values) {
    detail::output_buffer& destination = detail::output();
    bool ok = true;
    bool first = true;
    auto append = [&](const auto& value) {
        if (!ok) return;
        if (!first) ok = destination.put(' ');
        first = false;
        if (ok) ok = detail::write_value(destination, value);
    };
    (append(values), ...);
    return ok && destination.put('\n');
}

template<class... T>
bool printflush(const T&... values) {
    return print(values...) && flush_out();
}

template<class... T>
bool printout(const T&... values) {
    detail::output_buffer& destination = detail::output();
    bool ok = true;
    auto append = [&](const auto& value) {
        if (ok) ok = detail::write_value(destination, value);
    };
    (append(values), ...);
    return ok;
}

template<class... T>
bool debug(const T&... values) {
    detail::output_buffer& destination = detail::error_output();
    bool ok = true;
    bool first = true;
    auto append = [&](const auto& value) {
        if (!ok) return;
        if (!first) ok = destination.put(' ');
        first = false;
        if (ok) ok = detail::write_value(destination, value);
    };
    (append(values), ...);
    return ok && destination.put('\n') && destination.flush();
}

template<class... T>
bool debugout(const T&... values) {
    detail::output_buffer& destination = detail::error_output();
    bool ok = true;
    auto append = [&](const auto& value) {
        if (ok) ok = detail::write_value(destination, value);
    };
    (append(values), ...);
    return ok;
}

template<class... T>
[[noreturn]] void fin(const T&... values) {
    (void)print(values...);
    (void)flush_out();
    (void)flush_err();
    std::exit(0);
}

} // namespace fastio_safe

// Selected global imports preserve the original call style without importing the entire namespace.
using fastio_safe::debug;
using fastio_safe::debugout;
using fastio_safe::fin;
using fastio_safe::flush_err;
using fastio_safe::flush_out;
using fastio_safe::last_scan_error;
using fastio_safe::print;
using fastio_safe::printflush;
using fastio_safe::printout;
using fastio_safe::scan;
using fastio_safe::scan_error;

#define INT(...) int __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)
#define LL(...) long long __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)
#define STR(...) std::string __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)
#define CHR(...) char __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)
#define DBL(...) double __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)
#define LD(...) long double __VA_ARGS__; ::fastio_safe::scan(__VA_ARGS__)

#endif // EEZCP_IO_FASTIO_SAFE_HPP

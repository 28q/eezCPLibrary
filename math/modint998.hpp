#ifndef EEZCP_MATH_MODINT998_HPP
#define EEZCP_MATH_MODINT998_HPP

#include <cassert>
#include <cstdint>
#include <iostream>
#include <type_traits>

struct modint998 {
    using u32 = std::uint32_t;
    using i32 = std::int32_t;
    using u64 = std::uint64_t;

    static constexpr u32 MOD = 998244353u;
    static constexpr u32 MOD2 = MOD * 2;
    static constexpr u32 primitive_root = 3;
    static constexpr int max_power_of_two = 23;

private:
    static constexpr u32 R = 3296722945u;
    static constexpr u32 N2 = 932051910u;

    struct montgomery_tag {};

    constexpr modint998(u32 x, montgomery_tag) : a(x) {}

    static constexpr u32 reduce(u64 x) {
        return static_cast<u32>(
            (x + u64(static_cast<u32>(x) * u32(-R)) * MOD) >> 32
        );
    }

public:
    u32 a;

    static_assert(MOD < (u32(1) << 30));
    static_assert((MOD & 1) != 0);
    static_assert(R * MOD == 1);

    constexpr modint998() : a(0) {}

    template <class T, std::enable_if_t<std::is_integral_v<T> &&
                                        std::is_signed_v<T>, int> = 0>
    constexpr modint998(T x) : a(0) {
        const std::int64_t y =
            static_cast<std::int64_t>(x) % std::int64_t(MOD) + MOD;
        a = reduce(u64(y) * N2);
    }

    template <class T, std::enable_if_t<std::is_integral_v<T> &&
                                        std::is_unsigned_v<T>, int> = 0>
    constexpr modint998(T x)
        : a(reduce(((u64(x) % MOD) + MOD) * N2)) {}

    static constexpr modint998 raw(u32 x) {
        return modint998(reduce(u64(x) * N2), montgomery_tag{});
    }

    static constexpr modint998 montgomery_raw(u32 x) {
        return modint998(x, montgomery_tag{});
    }

    static constexpr u32 mod() { return MOD; }
    static constexpr u32 get_mod() { return MOD; }

    constexpr u32 val() const {
        const u32 x = reduce(a);
        return x >= MOD ? x - MOD : x;
    }

    constexpr u32 get() const { return val(); }

    constexpr modint998& operator+=(const modint998& rhs) {
        a += rhs.a - MOD2;
        if (i32(a) < 0) a += MOD2;
        return *this;
    }

    constexpr modint998& operator-=(const modint998& rhs) {
        a -= rhs.a;
        if (i32(a) < 0) a += MOD2;
        return *this;
    }

    constexpr modint998& operator*=(const modint998& rhs) {
        a = reduce(u64(a) * rhs.a);
        return *this;
    }

    constexpr modint998& operator/=(const modint998& rhs) {
        return *this *= rhs.inv();
    }

    constexpr modint998 operator+() const { return *this; }
    constexpr modint998 operator-() const { return modint998() - *this; }

    friend constexpr modint998 operator+(modint998 lhs,
                                          const modint998& rhs) {
        return lhs += rhs;
    }

    friend constexpr modint998 operator-(modint998 lhs,
                                          const modint998& rhs) {
        return lhs -= rhs;
    }

    friend constexpr modint998 operator*(modint998 lhs,
                                          const modint998& rhs) {
        return lhs *= rhs;
    }

    friend constexpr modint998 operator/(modint998 lhs,
                                          const modint998& rhs) {
        return lhs /= rhs;
    }

    friend constexpr bool operator==(const modint998& lhs,
                                     const modint998& rhs) {
        const u32 x = lhs.a >= MOD ? lhs.a - MOD : lhs.a;
        const u32 y = rhs.a >= MOD ? rhs.a - MOD : rhs.a;
        return x == y;
    }

    friend constexpr bool operator!=(const modint998& lhs,
                                     const modint998& rhs) {
        return !(lhs == rhs);
    }

    constexpr modint998& operator++() {
        return *this += raw(1);
    }

    constexpr modint998 operator++(int) {
        modint998 old = *this;
        ++*this;
        return old;
    }

    constexpr modint998& operator--() {
        return *this -= raw(1);
    }

    constexpr modint998 operator--(int) {
        modint998 old = *this;
        --*this;
        return old;
    }

    constexpr modint998 pow(u64 exponent) const {
        modint998 result = raw(1);
        modint998 base = *this;
        while (exponent != 0) {
            if (exponent & 1) result *= base;
            base *= base;
            exponent >>= 1;
        }
        return result;
    }

    constexpr modint998 inv() const {
        assert(val() != 0);

        const modint998 x = *this;
        modint998 a2 = x * x;
        modint998 a4 = a2 * a2;
        modint998 a8 = a4 * a4;
        modint998 a16 = a8 * a8;
        modint998 a32 = a16 * a16;
        modint998 a33 = a32 * x;
        modint998 a49 = a33 * a16;
        modint998 a82 = a49 * a33;
        modint998 a164 = a82 * a82;
        modint998 a328 = a164 * a164;
        modint998 r = a328 * a49;

        a2 = r * r;
        a4 = a2 * a2;
        a8 = a4 * a4;
        modint998 a9 = a8 * r;
        modint998 a18 = a9 * a9;
        r = a18 * a9;

        a2 = r * r;
        a4 = a2 * a2;
        a8 = a4 * a4;
        a16 = a8 * a8;
        modint998 a17 = a16 * r;
        a33 = a17 * a16;
        modint998 a66 = a33 * a33;
        modint998 a132 = a66 * a66;
        modint998 a264 = a132 * a132;
        r = a264 * a17;

        a2 = r * r;
        a4 = a2 * a2;
        a8 = a4 * a4;
        a16 = a8 * a8;
        a32 = a16 * a16;
        modint998 a64 = a32 * a32;
        modint998 a65 = a64 * r;
        modint998 a73 = a65 * a8;
        modint998 a138 = a73 * a65;
        modint998 a276 = a138 * a138;
        return a276 * a73;
    }

    constexpr modint998 inverse() const { return inv(); }

    friend std::ostream& operator<<(std::ostream& os, const modint998& x) {
        return os << x.val();
    }

    friend std::istream& operator>>(std::istream& is, modint998& x) {
        std::int64_t value;
        is >> value;
        x = modint998(value);
        return is;
    }
};

static_assert(sizeof(modint998) == 4);
static_assert(std::is_trivially_copyable_v<modint998>);

using mint998 = modint998;

#endif

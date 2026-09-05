#pragma once

#include <bit>
#include <cstddef>
#include <span>
#include <stdexcept>
#include <vector>

#include "detail/fps998_inv.hpp"

namespace eez::fps998{

using mint=ntt998::mint;
using usize=std::size_t;
using poly=std::vector<mint>;

class workspace{
public:
    workspace()=default;

    explicit workspace(usize n){
        reserve(n);
    }

    void reserve(usize n){
        if(n==0) return;
        if(n>ntt998::max_ntt_size)
            throw std::length_error(
                "eez::fps998::workspace: size exceeds 2^23"
            );
        inv_.reserve(std::bit_ceil(n));
    }

    [[nodiscard]] usize capacity() const noexcept{
        return inv_.capacity();
    }

private:
    friend void inv_to(
        std::span<const mint>,
        std::span<mint>,
        workspace&
    );

    detail::inv_workspace inv_;
};

inline void inv_to(
    std::span<const mint> f,
    std::span<mint> out,
    workspace& ws
){
    if(out.empty()) return;

    if(f.empty())
        throw std::invalid_argument(
            "eez::fps998::inv_to: input is empty"
        );

    if(f[0]==mint{})
        throw std::invalid_argument(
            "eez::fps998::inv_to: constant term must be nonzero"
        );

    if(out.size()>ntt998::max_ntt_size)
        throw std::length_error(
            "eez::fps998::inv_to: result exceeds the 2^23 NTT limit"
        );

    detail::inv_to_impl(f,out,ws.inv_);
}

inline void inv_to(
    std::span<const mint> f,
    std::span<mint> out
){
    workspace ws;
    inv_to(f,out,ws);
}

inline poly inv(
    std::span<const mint> f,
    usize n,
    workspace& ws
){
    poly result(n);
    inv_to(f,result,ws);
    return result;
}

inline poly inv(
    std::span<const mint> f,
    usize n
){
    workspace ws;
    return inv(f,n,ws);
}

inline poly inv(
    std::span<const mint> f,
    workspace& ws
){
    return inv(f,f.size(),ws);
}

inline poly inv(
    std::span<const mint> f
){
    workspace ws;
    return inv(f,f.size(),ws);
}

}

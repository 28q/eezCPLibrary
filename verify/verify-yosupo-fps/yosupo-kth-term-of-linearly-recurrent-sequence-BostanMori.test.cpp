#define PROBLEM "https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include "../../fps/BostanMori.hpp"

int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);

    std::size_t d;
    std::uint64_t k;
    std::cin>>d>>k;

    using mint=eez::fps998::mint;
    std::vector<mint> a(d),q(d+1);
    for(auto& x:a)std::cin>>x;

    q[0]=mint::raw(1);
    for(std::size_t i=0;i<d;i++){
        mint c;
        std::cin>>c;
        q[i+1]=-c;
    }

    std::cout<<eez::fps998::BostanMori(a,q,k)<<'\n';
}

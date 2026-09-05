#define PROBLEM "https://judge.yosupo.jp/problem/inv_of_formal_power_series"

#include <iostream>
#include <vector>

#include "../../fps/fps998.hpp"

int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);

    std::size_t n;
    std::cin>>n;

    using mint=eez::fps998::mint;

    std::vector<mint> f(n);
    for(auto& x:f) std::cin>>x;

    const auto g=eez::fps998::inv(f);

    for(std::size_t i=0;i<n;++i){
        if(i) std::cout<<' ';
        std::cout<<g[i];
    }
    std::cout<<'\n';
}

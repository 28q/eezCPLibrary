#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include <iostream>
#include <vector>

#include "../../convolution/ntt998.hpp"

int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);

    std::size_t n, m;
    std::cin >> n >> m;

    using mint = eez::ntt998::mint;
    std::vector<mint> a(n), b(m);

    for(auto& x : a) std::cin >> x;
    for(auto& x : b) std::cin >> x;

    const auto c = eez::ntt998::convolution(a, b);

    for(std::size_t i = 0; i < c.size(); ++i){
        if(i) std::cout << ' ';
        std::cout << c[i];
    }
    std::cout << '\n';
}

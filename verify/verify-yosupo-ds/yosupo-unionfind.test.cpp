#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"
#include<bits/stdc++.h>
using namespace std;

#include"../../data-structure/UnionFind.hpp"

int main(){
	cin.tie(0)->ios::sync_with_stdio(0);
	int N,Q;
	cin >> N >> Q;
	UnionFind uf(N);
	for(;Q--;){
		int t,u,v;
		cin >> t >> u >> v;
		if(t==0) uf.merge(u,v);
		else cout << uf.same(u,v) << '\n';
	}
}

#pragma once

struct UnionFind{
	private:
	int N;
	vector<int> par;
	int root(int x){
		int x0=x;
		while(par[x]>=0){
			x=par[x];
		}
		while(par[x0]>=0){
			int x1=par[x0];
			par[x0]=x;
			swap(x0,x1);
		}
		return x;
	}
	public:
	UnionFind(int siz) : N(siz),par(siz,-1){}
	void merge(int a,int b){
		a=root(a),b=root(b);
		if(a==b) return;
		if(-par[a]<-par[b]) swap(a,b);
		par[a]+=par[b];
		par[b]=a;
	}
	bool same(int a,int b){
		return root(a)==root(b);
	}
};

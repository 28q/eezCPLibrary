#pragma once

struct UnionFind {
	private:
	vector<int32_t> p;
	public:
	explicit UnionFind(int n) : p(n,-1){}
	[[gnu::always_inline]]
	inline bool same(int32_t x,int32_t y) noexcept{
	int32_t* const __restrict__ p=this->p.data();
		for(;;){
			const int32_t px=p[x];
			if(px<0) break;
			const int32_t ppx=p[px];
			if(ppx<0){
				x=px;
				break;
			}
			p[x]=ppx;
			x=ppx;
		}
		for(;;){
			const int32_t py=p[y];
			if(py<0) break;
			const int32_t ppy=p[py];
			if(ppy<0){
				y=py;
				break;
			}
			p[y]=ppy;
			y=ppy;
		}
		return x==y;
	}
	[[gnu::always_inline]]
	inline bool merge(int32_t x,int32_t y)noexcept {
		int32_t* const __restrict__ p=this->p.data();
		for(;;){
			const int32_t px=p[x];
			if(px<0) break;
			const int32_t ppx=p[px];
			if(ppx<0){
				x=px;
				break;
			}
			p[x]=ppx;
			x=ppx;
		}
		for(;;){
			const int32_t py=p[y];
			if(py<0) break;
			const int32_t ppy=p[py];
			if(ppy<0){
				y=py;
				break;
			}
			p[y]=ppy;
			y=ppy;
		}
		if(x==y) return false;
		if(p[x]>p[y]){
			const int32_t t=x;
			x=y;
			y=t;
		}
		p[x]+=p[y];
		p[y]=x;
		return true;
	}
};

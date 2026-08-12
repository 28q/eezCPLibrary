#pragma once

struct UnionFind{
	private:
	vector<int32_t> p;
	public:
	explicit UnionFind(int siz) : p(siz){
		int32_t i=0;
		for (;i+1<siz;i+=2){
			const uint64_t v= uint64_t(uint32_t(i))|(uint64_t(uint32_t(i+1))<<32);
			*reinterpret_cast<uint64_t*>(p.data()+i)=v;
		}
		if(i<siz) p[i]=i;
	}
	[[gnu::always_inline]]
	inline void merge(int32_t x,int32_t y) noexcept{
		int32_t* const __restrict__ pp=p.data();
		int32_t px=pp[x];
		int32_t py=pp[y];
		while(px!=py)[[likely]]{
			if(px<py)[[likely]]{
				if(x==px){
					pp[x]=py;
					return;
				}
				pp[x]=py;
				x=px;
				px=pp[x];
			}
			else{
				if (y == py){
					pp[y]=px;
					return;
				}
				pp[y] = px;
				y=py;
				py=pp[y];
			}
		}
		return;
	}
	[[gnu::always_inline]]
	inline bool same(int32_t x,int32_t y) noexcept{
		int32_t* const __restrict__ pp=p.data();
		while(pp[x]!=x){
			pp[x]=pp[pp[x]];
			x=pp[x];
		}
		while(pp[y]!=y){
			pp[y]=pp[pp[y]];
			y=pp[y];
		}
		return x==y;
	}
};

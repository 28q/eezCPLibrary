#pragma once

using namespace std;
#include<vector>
#include<cstdint>
#include<cstddef>
#include<stdexcept>

#if defined(__linux__)
#include<sys/mman.h>
#define UF_USE_HUGEPAGE 1
#endif

#if UF_USE_HUGEPAGE
namespace uf_detail{
	constexpr size_t kAlign=size_t(1)<<21;
	inline void* huge_alloc(size_t bytes) {
		size_t len=(bytes+kAlign-1)&~(kAlign-1);
		if(len==0)len=kAlign;
		void* ptr=mmap(nullptr,len,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
		if(ptr==MAP_FAILED) throw bad_alloc();
		madvise(ptr,len,MADV_HUGEPAGE);
		return ptr;
	}
	inline void huge_free(void* ptr,size_t bytes) noexcept{
		size_t len=(bytes+kAlign-1)&~(kAlign-1);
		if(len==0) len=kAlign;
		munmap(ptr,len);
	}
}

template<class T>
struct HugePageAllocator{
	using value_type=T;
	HugePageAllocator()noexcept=default;
	template<class U> HugePageAllocator(const HugePageAllocator<U>&) noexcept{}
	T* allocate(size_t n){
		return static_cast<T*>(uf_detail::huge_alloc(n*sizeof(T)));
	}
	void deallocate(T* p,size_t n) noexcept{
		uf_detail::huge_free(p,n*sizeof(T));
	}
	template<class U> bool operator==(const HugePageAllocator<U>&) const noexcept{return true;}
	template<class U> bool operator!=(const HugePageAllocator<U>&) const noexcept{return false;}
};

template<class T>
using UFAllocator = HugePageAllocator<T>;
#else
template<class T>
using UFAllocator = std::allocator<T>;
#endif

struct UnionFind{
	private:
	std::vector<int32_t,UFAllocator<int32_t>> p;
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

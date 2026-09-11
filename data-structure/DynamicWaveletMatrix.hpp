#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cstdint>
#include <cstring>
#include <new>
#include <limits>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>
#if (defined(__AVX2__) || defined(__BMI2__)) && !defined(EEZ_NO_SIMD)
#include <immintrin.h>
#endif

#ifndef EEZ_WM_QUAD_INDEX
#define EEZ_WM_QUAD_INDEX 1
#endif
#ifndef EEZ_BV_RANGE_UPDATE
#define EEZ_BV_RANGE_UPDATE 0
#endif
#ifndef EEZ_BV_MOVE_PAIR
#define EEZ_BV_MOVE_PAIR 0
#endif
#ifndef EEZ_BV_LEAF_SIMD
#define EEZ_BV_LEAF_SIMD 0
#endif
#ifndef EEZ_QUAD_RETAIN
#define EEZ_QUAD_RETAIN 1
#endif
namespace eez {

#ifndef EEZ_BV_WORDS
#define EEZ_BV_WORDS 64
#endif
#ifndef EEZ_BV_FANOUT
#define EEZ_BV_FANOUT 16
#endif
#ifndef EEZ_BV_GROUP
#define EEZ_BV_GROUP 4
#endif
#ifndef EEZ_BV_SIMD_SHIFT
#define EEZ_BV_SIMD_SHIFT 1
#endif
#ifndef EEZ_BV_PREFETCH
#define EEZ_BV_PREFETCH 0
#endif
#ifndef EEZ_BV_SEARCH
#define EEZ_BV_SEARCH 0
#endif
#ifndef EEZ_BV_STATIC_INDEX
#define EEZ_BV_STATIC_INDEX 1
#endif
#ifndef EEZ_BV_BUFFER
#define EEZ_BV_BUFFER 0
#endif
#ifndef EEZ_BV_FILL
#define EEZ_BV_FILL 80
#endif
namespace detail {
// A contiguous arena for trivial nodes. Unused capacity is never zero-filled.
// Allocation creates all object lifetimes with new T[], retaining C++17 support.
template<class T> class NodeArena {
    static_assert(std::is_trivial_v<T> && std::is_trivially_copyable_v<T>);
    std::unique_ptr<T[]> data_;
    std::uint32_t used_ = 0, capacity_ = 0;
public:
    NodeArena() = default;
    NodeArena(const NodeArena& other) {
        reserve(other.used_); used_ = other.used_;
        if (used_) std::memcpy(data_.get(),other.data_.get(),std::size_t(used_)*sizeof(T));
    }
    NodeArena& operator=(const NodeArena& other) { if (this != &other) { NodeArena tmp(other); swap(tmp); } return *this; }
    NodeArena(NodeArena&& other) noexcept { swap(other); }
    NodeArena& operator=(NodeArena&& other) noexcept { if (this != &other) { NodeArena tmp(std::move(other)); swap(tmp); } return *this; }
    void swap(NodeArena& other) noexcept {
        data_.swap(other.data_); std::swap(used_,other.used_); std::swap(capacity_,other.capacity_);
    }
    T& operator[](std::size_t i) noexcept { return data_[i]; }
    const T& operator[](std::size_t i) const noexcept { return data_[i]; }
    std::size_t size() const noexcept { return used_; }
    std::size_t capacity() const noexcept { return capacity_; }
    void clear() noexcept { used_ = 0; }
    void reserve(std::size_t n) {
        if (n <= capacity_) return;
        if (n > std::size_t(INT_MAX)) throw std::bad_alloc();
        std::unique_ptr<T[]> next(new T[n]);
        if (used_) std::memcpy(next.get(),data_.get(),std::size_t(used_)*sizeof(T));
        data_.swap(next); capacity_ = std::uint32_t(n);
    }
    void emplace_back() {
        if (used_ == capacity_) reserve(capacity_ ? std::min(std::size_t(INT_MAX),std::size_t(capacity_)*2) : 16);
        if (used_ == std::uint32_t(INT_MAX)) throw std::bad_alloc();
        ++used_; // The caller initializes the live fields, not padding/capacity.
    }
};
}

// B+ tree with independently tuned leaf blocks and SIMD-searchable inner nodes.
// A negative handle encodes a leaf index (~index); other handles are inner nodes.
class DynamicBitVector {
    using u64 = std::uint64_t;
    static constexpr int WORDS = EEZ_BV_WORDS, FANOUT = EEZ_BV_FANOUT, GROUP = EEZ_BV_GROUP;
    static constexpr int CAP = WORDS*64, HALF = WORDS/2, GROUPS = (WORDS+GROUP-1)/GROUP;
    static constexpr int BUILD_WORDS = WORDS*EEZ_BV_FILL/100 > 0 ? WORDS*EEZ_BV_FILL/100 : 1;
    static constexpr int BUILD_BITS = BUILD_WORDS*64;
    static_assert(WORDS >= 2 && WORDS <= 128);
    static_assert(FANOUT >= 8 && FANOUT <= 64 && (FANOUT&(FANOUT-1)) == 0);
    static_assert(GROUP == 1 || GROUP == 2 || GROUP == 4 || GROUP == 8);
    static_assert(EEZ_BV_FILL >= 50 && EEZ_BV_FILL <= 100);
    struct alignas(64) Leaf {
        u64 words[WORDS];
        std::uint16_t ones[GROUPS];
        int count;
#if EEZ_BV_BUFFER > 0
        std::uint32_t edits[EEZ_BV_BUFFER];
        int edit_count;
        static_assert(EEZ_BV_BUFFER <= 16);
#endif
    };
    struct alignas(64) Node {
        int sizes[FANOUT], ones[FANOUT], child[FANOUT];
        int count;
    };
#ifdef EEZ_BV_STD_VECTOR
    std::vector<Leaf> leaves;
    std::vector<Node> pool;
#else
    detail::NodeArena<Leaf> leaves;
    detail::NodeArena<Node> pool;
#endif
    int root = -1, free_leaf = -1, free_node = -1;
    int length = 0, total_ones = 0;
#if EEZ_BV_STATIC_INDEX
    struct StaticWord { u64 bits; int prefix; };
    std::unique_ptr<StaticWord[]> read_index;
    std::pair<bool,int> indexed_access_rank(int k) const noexcept {
        const auto& w = read_index[k >> 6];
        return {bool((w.bits >> (k & 63)) & 1),w.prefix+pc(w.bits & mask(k & 63))};
    }
    int indexed_rank(int k) const noexcept {
        if (k == length) return total_ones;
        const auto& w = read_index[k >> 6]; return w.prefix+pc(w.bits & mask(k & 63));
    }
    void collect_words(int p,StaticWord* output,int& offset) {
        if (p >= 0) { const auto& a = pool[p]; for (int i = 0; i < a.count; ++i) collect_words(a.child[i],output,offset); return; }
        auto& a = leaves[~p];
#if EEZ_BV_BUFFER > 0
        flush_leaf(a);
#endif
        for (int j = 0; j*64 < a.count; ++j) {
            int count = std::min(64,a.count-j*64), shift = offset & 63;
            u64 w = a.words[j] & mask(count); int dst = offset >> 6;
            output[dst].bits |= w << shift;
            if (shift && count > 64-shift) output[dst+1].bits |= w >> (64-shift);
            offset += count;
        }
    }
    void index_packed(const u64* words) {
        if (!total_ones || total_ones == length) return;
        std::size_t count = (std::size_t(length)+63)/64;
        std::unique_ptr<StaticWord[]> next(new StaticWord[count]); int rank = 0;
        for (std::size_t i = 0; i < count; ++i) {
            u64 w = words[i]; if (i+1 == count && (length & 63)) w &= mask(length & 63);
            next[i].bits = w; next[i].prefix = rank; rank += pc(w);
        }
        read_index = std::move(next);
    }
#endif
    void collect_packed(int p,u64* out,int& offset){
        if(p>=0){const auto& a=pool[p];for(int i=0;i<a.count;++i)collect_packed(a.child[i],out,offset);return;}
        auto& a=leaves[~p];
#if EEZ_BV_BUFFER > 0
        flush_leaf(a);
#endif
        for(int j=0;j*64<a.count;++j){
            int count=std::min(64,a.count-j*64),shift=offset&63;u64 word=a.words[j]&mask(count);
            out[offset>>6]|=word<<shift;
            if(shift&&count>64-shift)out[(offset>>6)+1]|=word>>(64-shift);
            offset+=count;
        }
    }
    static u64 mask(int k) { return k == 64 ? ~u64(0) : (u64(1) << k) - 1; }
    static int pc(u64 x) {
#if defined(__GNUC__) || defined(__clang__)
        return __builtin_popcountll(x);
#else
        x -= (x >> 1) & 0x5555555555555555ull;
        x = (x & 0x3333333333333333ull) + ((x >> 2) & 0x3333333333333333ull);
        x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0full;
        return int((x * 0x0101010101010101ull) >> 56);
#endif
    }
    void prefetch(int p) const noexcept {
#if EEZ_BV_PREFETCH && (defined(__GNUC__) || defined(__clang__))
        if (p < 0) { __builtin_prefetch(&leaves[~p],0,3); __builtin_prefetch(leaves[~p].ones,0,3); }
        else __builtin_prefetch(pool[p].sizes,0,3);
#else
        (void)p;
#endif
    }
    int size_of(int p) const { return p < 0 ? leaves[~p].count : pool[p].sizes[pool[p].count-1]; }
    int ones_of(int p) const { return p < 0 ? leaf_total(leaves[~p]) : pool[p].ones[pool[p].count-1]; }
    int alloc_leaf() {
        int i;
        if (free_leaf >= 0) { i = free_leaf; free_leaf = leaves[i].count; leaves[i] = Leaf{}; }
        else { i = int(leaves.size()); leaves.emplace_back(); leaves[i] = Leaf{}; }
        return ~i;
    }
    int alloc_node() {
        int p;
        if (free_node >= 0) { p = free_node; free_node = pool[p].child[0]; }
        else { p = int(pool.size()); pool.emplace_back(); }
        return p;
    }
    void release(int p) {
        if (p < 0) { leaves[~p].count = free_leaf; free_leaf = ~p; }
        else { pool[p].child[0] = free_node; free_node = p; }
    }
    static void leaf_counts(Leaf& a) {
        int sum = 0;
        for (int j = 0; j < WORDS; ++j) { sum += pc(a.words[j]); if ((j+1)%GROUP == 0 || j+1 == WORDS) a.ones[j/GROUP] = std::uint16_t(sum); }
    }
    void inner_counts(int p) {
        auto& a = pool[p]; int s = 0, o = 0;
        for (int i = 0; i < a.count; ++i) {
            s += size_of(a.child[i]); o += ones_of(a.child[i]);
            a.sizes[i] = s; a.ones[i] = o;
        }
        for (int i = a.count; i < FANOUT; ++i) { a.sizes[i] = INT_MAX; a.ones[i] = o; }
    }
    static int locate(const Node& a, int k) {
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD) && EEZ_BV_SEARCH == 0
        const auto key = _mm256_set1_epi32(k);
        u64 bits = 0;
        for (int j = 0; j < FANOUT; j += 8) {
            auto v = _mm256_cmpgt_epi32(_mm256_load_si256(reinterpret_cast<const __m256i*>(a.sizes+j)),key);
            bits |= u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(v)))) << j;
        }
        int i;
#if defined(__GNUC__) || defined(__clang__)
        i = bits ? __builtin_ctzll(bits) : FANOUT;
#else
        i = 0; while (i < FANOUT && !(bits & (u64(1) << i))) ++i;
#endif
        return i < a.count ? i : a.count-1;
#else
        int i = 0;
        for (int step = FANOUT/2; step; step >>= 1) i += step*int(a.sizes[i+step-1] <= k);
        i += int(a.sizes[i] <= k); return i < a.count ? i : a.count-1;
#endif
    }
    static std::pair<int,int> locate_two(const Node& a,int l,int r){
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)
        auto kl=_mm256_set1_epi32(l),kr=_mm256_set1_epi32(r);u64 bl=0,br=0;
        for(int j=0;j<FANOUT;j+=8){auto v=_mm256_load_si256(reinterpret_cast<const __m256i*>(a.sizes+j));bl|=u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(v,kl)))))<<j;br|=u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(v,kr)))))<<j;}
#if defined(__GNUC__) || defined(__clang__)
        int i=bl?__builtin_ctzll(bl):FANOUT,j=br?__builtin_ctzll(br):FANOUT;
#else
        int i=bl?pc((bl&-bl)-1):FANOUT,j=br?pc((br&-br)-1):FANOUT;
#endif
        return {std::min(i,a.count-1),std::min(j,a.count-1)};
#else
        return {locate(a,l),locate(a,r)};
#endif
    }
    static void add_leaf_ones(Leaf& a,int first,int delta){
        int i=first;
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD) && EEZ_BV_LEAF_SIMD
        i=first&~15;
        for(;i+16<=GROUPS;i+=16){
            auto ix=_mm256_setr_epi16(i,i+1,i+2,i+3,i+4,i+5,i+6,i+7,i+8,i+9,i+10,i+11,i+12,i+13,i+14,i+15);
            auto m=_mm256_cmpgt_epi16(ix,_mm256_set1_epi16(first-1));
            auto q=reinterpret_cast<__m256i*>(a.ones+i);_mm256_storeu_si256(q,_mm256_add_epi16(_mm256_loadu_si256(q),_mm256_and_si256(m,_mm256_set1_epi16(delta))));
        }
        i=std::max(i,first);
#endif
        for(;i<GROUPS;++i)a.ones[i]=std::uint16_t(a.ones[i]+delta);
    }
    static int leaf_rank_flat(const Leaf& a, int k) {
        int j = k >> 6, bits = k & 63, group = j/GROUP;
        int sum = group ? a.ones[group-1] : 0;
        for (int w = group*GROUP; w < j; ++w) sum += pc(a.words[w]);
        return sum + (bits ? pc(a.words[j] & mask(bits)) : 0);
    }
    static bool leaf_access(const Leaf& a,int k) {
#if EEZ_BV_BUFFER > 0
        for (int i = a.edit_count-1; i >= 0; --i) {
            unsigned e = a.edits[i]; int pos = e & 65535, op = (e >> 18) & 3;
            if (op == 0) { if (k == pos) return (e >> 16) & 1; k -= k > pos; }
            else if (op == 1) k += k >= pos;
            else if (k == pos) return (e >> 16) & 1;
        }
#endif
        return (a.words[k >> 6] >> (k & 63)) & 1;
    }
    static int leaf_rank(const Leaf& a,int k) {
        int delta = 0;
#if EEZ_BV_BUFFER > 0
        for (int i = a.edit_count-1; i >= 0; --i) {
            unsigned e = a.edits[i]; int pos = e & 65535, op = (e >> 18) & 3;
            if (pos < k) {
                if (op == 0) { --k; delta += (e >> 16) & 1; }
                else if (op == 1) { ++k; delta -= (e >> 17) & 1; }
                else delta += int((e >> 16) & 1)-int((e >> 17) & 1);
            }
        }
#endif
        return delta+leaf_rank_flat(a,k);
    }
    static int leaf_total(const Leaf& a) {
        int total = a.ones[GROUPS-1];
#if EEZ_BV_BUFFER > 0
        for (int i = 0; i < a.edit_count; ++i) {
            unsigned e = a.edits[i]; total += int((e >> 16) & 1)-int((e >> 17) & 1);
        }
#endif
        return total;
    }
    static std::pair<int,int> leaf_pair(const Leaf& a,int l,int r) {
        int dl = 0, dr = 0;
#if EEZ_BV_BUFFER > 0
        for (int i = a.edit_count-1; i >= 0; --i) {
            unsigned e = a.edits[i]; int p = e & 65535, op = (e >> 18) & 3;
            int left = p < l, right = p < r;
            int old = (e >> 17) & 1, bit = (e >> 16) & 1;
            if (op == 0) { dl += left*bit; dr += right*bit; l -= left; r -= right; }
            else if (op == 1) { dl -= left*old; dr -= right*old; l += left; r += right; }
            else { dl += left*(bit-old); dr += right*(bit-old); }
        }
#endif
        int first = leaf_rank_flat(a,l), last;
        if (l == r) last = first;
        else if ((l >> 6) == ((r-1) >> 6)) last = first+pc((a.words[l >> 6] >> (l & 63)) & mask(r-l));
        else last = leaf_rank_flat(a,r);
        return {first+dl,last+dr};
    }
    static std::pair<bool,int> leaf_access_rank(const Leaf& a,int k) {
#if EEZ_BV_BUFFER > 0
        auto q = leaf_pair(a,k,k+1); return {q.first != q.second,q.first};
#else
        return {leaf_access(a,k),leaf_rank_flat(a,k)};
#endif
    }
#if EEZ_BV_BUFFER > 0
    // Materialize the journal by copying unchanged bit ranges once, rather
    // than shifting the whole leaf separately for every buffered operation.
    static void flush_leaf(Leaf& a) {
        if (!a.edit_count) return;
        struct Run { int start, length; } runs[2*EEZ_BV_BUFFER+3];
        int base_count = a.count;
        for (int i = 0; i < a.edit_count; ++i) { int op = (a.edits[i] >> 18) & 3; base_count += int(op == 1)-int(op == 0); }
        int nr = 1; runs[0] = {0,base_count};
        auto erase_at = [&](int position) {
            int r = 0, offset = position;
            while (r < nr && offset >= runs[r].length) { offset -= runs[r].length; ++r; }
            assert(r < nr);
            Run old = runs[r];
            if (old.length == 1) { std::memmove(runs+r,runs+r+1,std::size_t(nr-r-1)*sizeof(Run)); --nr; }
            else if (!offset) { if (old.start >= 0) ++runs[r].start; --runs[r].length; }
            else if (offset+1 == old.length) --runs[r].length;
            else {
                std::memmove(runs+r+2,runs+r+1,std::size_t(nr-r-1)*sizeof(Run)); ++nr;
                runs[r] = {old.start,offset};
                runs[r+1] = {old.start+offset+1,old.length-offset-1};
            }
        };
        auto insert_at = [&](int position,bool bit) {
            int r = 0, offset = position;
            while (r < nr && offset >= runs[r].length) { offset -= runs[r].length; ++r; }
            if (r == nr || !offset) {
                std::memmove(runs+r+1,runs+r,std::size_t(nr-r)*sizeof(Run));
                runs[r] = {bit ? -2 : -1,1}; ++nr;
            } else {
                Run old = runs[r];
                std::memmove(runs+r+3,runs+r+1,std::size_t(nr-r-1)*sizeof(Run)); nr += 2;
                runs[r] = {old.start,offset}; runs[r+1] = {bit ? -2 : -1,1};
                runs[r+2] = {old.start+offset,old.length-offset};
            }
        };
        for (int i = 0; i < a.edit_count; ++i) {
            unsigned e = a.edits[i]; int pos = e & 65535, op = (e >> 18) & 3;
            if (op != 0) erase_at(pos);
            if (op != 1) insert_at(pos,(e >> 16) & 1);
        }
        u64 out[WORDS]{}; int dst = 0;
        for (int i = 0; i < nr; ++i) {
            auto run = runs[i];
            if (run.start < 0) { if (run.start == -2) out[dst >> 6] |= u64(1) << (dst & 63); dst += run.length; continue; }
            int src = run.start, remaining = run.length;
            while (remaining) {
                int count = std::min(64-(dst & 63),remaining), word = src >> 6, shift = src & 63;
                u64 bits = a.words[word] >> shift;
                if (shift && word+1 < WORDS) bits |= a.words[word+1] << (64-shift);
                out[dst >> 6] |= (bits & mask(count)) << (dst & 63);
                dst += count; src += count; remaining -= count;
            }
        }
        assert(dst == a.count);
        std::memcpy(a.words,out,sizeof(out)); a.edit_count = 0; leaf_counts(a);
    }
    static void record_edit(Leaf& a,int pos,int op,bool old,bool bit) {
        a.edits[a.edit_count++] = unsigned(pos) | (unsigned(bit) << 16) | (unsigned(old) << 17) | (unsigned(op) << 18);
        if (a.edit_count == EEZ_BV_BUFFER) flush_leaf(a);
    }
#endif
#if EEZ_BV_RANGE_UPDATE
    static void adjust_range(Node& a,int first,int last,int ds,int d1) {
        if(first==last || (!ds&&!d1))return;
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)
        alignas(32) static constexpr auto masks=[](){
            std::array<std::array<int,8>,9> out{};
            for(int i=0;i<=8;++i)for(int j=i;j<8;++j)out[i][j]=-1;
            return out;
        }();
        for(int j=first&~7;j<last;j+=8){
            int lo=std::max(0,first-j),hi=std::min(8,last-j);
            auto m=_mm256_xor_si256(_mm256_load_si256(reinterpret_cast<const __m256i*>(masks[lo].data())),_mm256_load_si256(reinterpret_cast<const __m256i*>(masks[hi].data())));
            if(ds){auto q=reinterpret_cast<__m256i*>(a.sizes+j);_mm256_store_si256(q,_mm256_add_epi32(_mm256_load_si256(q),_mm256_and_si256(m,_mm256_set1_epi32(ds))));}
            if(d1){auto q=reinterpret_cast<__m256i*>(a.ones+j);_mm256_store_si256(q,_mm256_add_epi32(_mm256_load_si256(q),_mm256_and_si256(m,_mm256_set1_epi32(d1))));}
        }
#else
        for(int j=first;j<last;++j){a.sizes[j]+=ds;a.ones[j]+=d1;}
#endif
    }
    static void adjust(Node& a,int first,int ds,int d1){adjust_range(a,first,a.count,ds,d1);}
#else
    static void adjust(Node& a, int first, int ds, int d1) {
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)
        const auto begin = _mm256_set1_epi32(first-1), end = _mm256_set1_epi32(a.count);
        for (int j = first & ~7; j < a.count; j += 8) {
            auto ix = _mm256_setr_epi32(j,j+1,j+2,j+3,j+4,j+5,j+6,j+7);
            auto m = _mm256_and_si256(_mm256_cmpgt_epi32(ix,begin), _mm256_cmpgt_epi32(end,ix));
            auto s = reinterpret_cast<__m256i*>(a.sizes+j);
            auto o = reinterpret_cast<__m256i*>(a.ones+j);
            if (ds) _mm256_store_si256(s,_mm256_add_epi32(_mm256_load_si256(s),_mm256_and_si256(m,_mm256_set1_epi32(ds))));
            if (d1) _mm256_store_si256(o,_mm256_add_epi32(_mm256_load_si256(o),_mm256_and_si256(m,_mm256_set1_epi32(d1))));
        }
#else
        for (int i = first; i < a.count; ++i) { a.sizes[i] += ds; a.ones[i] += d1; }
#endif
    }
#endif
    static void insert_plain(Leaf& a,int k,bool bit) {
        int j = k >> 6, b = k & 63, limit = (a.count+64)/64;
        int total = a.ones[GROUPS-1]+int(bit);
        u64 w = a.words[j];
        a.words[j] = (w & mask(b)) | ((w & ~mask(b)) << 1) | (u64(bit) << b);
        u64 carry = w >> 63;
        if ((j+1)%GROUP == 0) a.ones[j/GROUP] = std::uint16_t(a.ones[j/GROUP]+int(bit)-int(carry));
        int i = j+1;
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD) && EEZ_BV_SIMD_SHIFT
        if constexpr(GROUP == 4) {
            for (; i < limit && (i & 3); ++i) {
                u64 old = a.words[i]; a.words[i] = (old << 1) | carry; carry = old >> 63;
                if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(bit)-int(carry));
            }
            for (; i+4 <= limit; i += 4) {
                auto p = reinterpret_cast<__m256i*>(a.words+i);
                auto old = _mm256_load_si256(p);
                auto incoming = _mm256_srli_epi64(_mm256_permute4x64_epi64(old,0x90),63);
                incoming = _mm256_blend_epi32(incoming,_mm256_set_epi64x(0,0,0,carry),0x03);
                u64 next = a.words[i+3] >> 63;
                _mm256_store_si256(p,_mm256_or_si256(_mm256_slli_epi64(old,1),incoming));
                carry = next; a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(bit)-int(carry));
            }
        }
#endif
        for (; i < limit; ++i) {
            u64 old = a.words[i]; a.words[i] = (old << 1) | carry; carry = old >> 63;
            if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(bit)-int(carry));
        }
        for (int g = limit/GROUP; g < GROUPS; ++g) a.ones[g] = std::uint16_t(total);
        ++a.count;
    }
    static void erase_plain(Leaf& a,int k,bool bit) {
        int j = k >> 6, b = k & 63, limit = (a.count+63)/64;
        int total = a.ones[GROUPS-1]-int(bit);
        u64 w = a.words[j], incoming = j+1 < limit ? a.words[j+1]&1 : 0;
        a.words[j] = (w & mask(b)) | (b == 63 ? 0 : (w >> (b+1)) << b) | (incoming << 63);
        if ((j+1)%GROUP == 0) a.ones[j/GROUP] = std::uint16_t(a.ones[j/GROUP]+int(incoming)-int(bit));
        int i = j+1;
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD) && EEZ_BV_SIMD_SHIFT
        if constexpr(GROUP == 4) {
            for (; i < limit && (i & 3); ++i) {
                u64 next = i+1 < limit ? a.words[i+1]&1 : 0;
                a.words[i] = (a.words[i] >> 1) | (next << 63);
                if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(next)-int(bit));
            }
            for (; i+4 <= limit; i += 4) {
                auto p = reinterpret_cast<__m256i*>(a.words+i);
                auto old = _mm256_load_si256(p); u64 next = i+4 < limit ? a.words[i+4]&1 : 0;
                auto carries = _mm256_slli_epi64(_mm256_permute4x64_epi64(old,0xf9),63);
                carries = _mm256_blend_epi32(carries,_mm256_set_epi64x(next << 63,0,0,0),0xc0);
                _mm256_store_si256(p,_mm256_or_si256(_mm256_srli_epi64(old,1),carries));
                a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(next)-int(bit));
            }
        }
#endif
        for (; i < limit; ++i) {
            u64 next = i+1 < limit ? a.words[i+1]&1 : 0;
            a.words[i] = (a.words[i] >> 1) | (next << 63);
            if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(next)-int(bit));
        }
        for (int g = limit/GROUP; g < GROUPS; ++g) a.ones[g] = std::uint16_t(total);
        --a.count;
    }
    std::pair<bool,int> replace(int p,int k,bool bit) {
        if (p < 0) {
            auto& a = leaves[~p];
#if EEZ_BV_BUFFER > 0
            auto q = leaf_access_rank(a,k); bool old = q.first; int rank = q.second;
            if (old != bit) record_edit(a,k,2,old,bit);
            return {old,rank};
#else
            int j = k >> 6;
            bool old = (a.words[j] >> (k&63)) & 1; int rank = leaf_rank(a,k);
            if (old != bit) {
                a.words[j] ^= u64(1) << (k&63);
                add_leaf_ones(a,j/GROUP,int(bit)-int(old));
            }
            return {old,rank};
#endif
        }
        auto& a = pool[p]; int i = locate(a,k), o = i ? a.ones[i-1] : 0;
        auto result = replace(a.child[i],k-(i ? a.sizes[i-1] : 0),bit);
        if (result.first != bit) adjust(a,i,0,int(bit)-int(result.first));
        result.second += o; return result;
    }
    // Returns whether a split occurred; sibling handles may be negative.
    bool ins(int p,int k,bool bit,int& rank,int& sibling) {
        if (p < 0) {
            auto& a = leaves[~p];
#if EEZ_BV_BUFFER > 0
            if (a.count == CAP) flush_leaf(a);
            else if (a.edit_count || !((!bit && !a.ones[GROUPS-1]) || (bit && a.ones[GROUPS-1] == a.count))) {
                rank += leaf_rank(a,k); ++a.count; record_edit(a,k,0,false,bit); return false;
            }
#endif
            if (a.count < CAP) {
                if (!bit && !a.ones[GROUPS-1]) { ++a.count; return false; }
                if (bit && a.ones[GROUPS-1] == a.count) {
                    rank += k; int j = a.count >> 6;
                    a.words[j] |= u64(1) << (a.count & 63);
                    add_leaf_ones(a,j/GROUP,1);
                    ++a.count; return false;
                }
            }
            rank += leaf_rank(a,k);
            int len = a.count, j = k >> 6, b = k & 63; u64 carry = u64(bit);
            if (len < CAP) { insert_plain(a,k,bit); return false; }
            u64 w[WORDS+1]{};
            for (int i = 0; i < WORDS; ++i) w[i] = a.words[i];
            for (int i = j; i < WORDS+1; ++i) {
                u64 old = w[i];
                if (i == j) w[i] = (old & mask(b)) | ((old & ~mask(b)) << 1) | (carry << b);
                else w[i] = (old << 1) | carry;
                carry = old >> 63;
            }
            int q = alloc_leaf(); // May move the leaf arena: reacquire both leaves.
            auto& left = leaves[~p]; auto& right = leaves[~q];
            for (int i = 0; i < WORDS; ++i) { left.words[i] = i < HALF ? w[i] : 0; right.words[i] = i < WORDS-HALF+1 ? w[i+HALF] : 0; }
            left.count = HALF*64; right.count = CAP+1-HALF*64;
#if EEZ_BV_BUFFER > 0
            left.edit_count = right.edit_count = 0;
#endif
            leaf_counts(left); leaf_counts(right);
            sibling = q; return true;
        }
        int i = locate(pool[p],k), offset = i ? pool[p].sizes[i-1] : 0;
        rank += i ? pool[p].ones[i-1] : 0;
        int q;
        if (!ins(pool[p].child[i],k-offset,bit,rank,q)) { adjust(pool[p],i,1,int(bit)); return false; }
        int children[FANOUT+1], count = pool[p].count;
        for (int j = 0; j <= i; ++j) children[j] = pool[p].child[j];
        children[i+1] = q;
        for (int j = i+1; j < count; ++j) children[j+1] = pool[p].child[j];
        if (count < FANOUT) {
            pool[p].count = count+1;
            for (int j = 0; j <= count; ++j) pool[p].child[j] = children[j];
            inner_counts(p); return false;
        }
        sibling = alloc_node(); pool[p].count = FANOUT/2; pool[sibling].count = FANOUT/2+1;
        for (int j = 0; j < FANOUT/2; ++j) pool[p].child[j] = children[j];
        for (int j = 0; j < FANOUT/2+1; ++j) pool[sibling].child[j] = children[j+FANOUT/2];
        inner_counts(p); inner_counts(sibling); return true;
    }
    // Empty nodes are removed by their parent, keeping live leaves at equal depth.
    bool del(int p,int k,int& rank,bool& bit) {
        if (p < 0) {
            auto& a = leaves[~p];
#if EEZ_BV_BUFFER > 0
            if (a.edit_count || (a.ones[GROUPS-1] && a.ones[GROUPS-1] != a.count)) {
                auto q = leaf_access_rank(a,k); rank += q.second; bit = q.first; --a.count; record_edit(a,k,1,bit,false); return a.count == 0;
            }
#endif
            if (!a.ones[GROUPS-1]) { bit = false; return --a.count == 0; }
            if (a.ones[GROUPS-1] == a.count) {
                bit = true; rank += k; --a.count; int j = a.count >> 6;
                a.words[j] &= ~(u64(1) << (a.count & 63));
                add_leaf_ones(a,j/GROUP,-1);
                return a.count == 0;
            }
            rank += leaf_rank(a,k); bit = (a.words[k >> 6] >> (k & 63)) & 1;
            erase_plain(a,k,bit); return a.count == 0;
        }
        auto& a = pool[p]; int i = locate(a,k); rank += i ? a.ones[i-1] : 0;
        if (!del(a.child[i],k-(i ? a.sizes[i-1] : 0),rank,bit)) { adjust(a,i,-1,-int(bit)); return false; }
        release(a.child[i]);
        for (int j = i+1; j < a.count; ++j) a.child[j-1] = a.child[j];
        if (--a.count == 0) return true;
        inner_counts(p); return false;
    }
    struct PathStep { int node, child; };
    // Insert an already-split right sibling into an inner node.
    bool attach(int p,int i,int& sibling) {
        int count = pool[p].count;
        if (count < FANOUT) {
            auto& a = pool[p];
            std::memmove(a.child+i+2,a.child+i+1,std::size_t(count-i-1)*sizeof(int));
            a.child[i+1] = sibling; ++a.count; inner_counts(p); return false;
        }
        int children[FANOUT+1];
        std::memcpy(children,pool[p].child,std::size_t(i+1)*sizeof(int));
        children[i+1] = sibling;
        std::memcpy(children+i+2,pool[p].child+i+1,std::size_t(count-i-1)*sizeof(int));
        sibling = alloc_node(); pool[p].count = FANOUT/2; pool[sibling].count = FANOUT/2+1;
        std::memcpy(pool[p].child,children,std::size_t(FANOUT/2)*sizeof(int));
        std::memcpy(pool[sibling].child,children+FANOUT/2,std::size_t(FANOUT/2+1)*sizeof(int));
        inner_counts(p); inner_counts(sibling); return true;
    }
#if EEZ_BV_RANGE_UPDATE
    static void adjust_move(Node& a,int from,int to,bool old,bool bit){
        if(from<to){adjust_range(a,from,to,-1,-int(old));adjust_range(a,to,a.count,0,int(bit)-int(old));}
        else if(to<from){adjust_range(a,to,from,1,int(bit));adjust_range(a,from,a.count,0,int(bit)-int(old));}
        else adjust_range(a,from,a.count,0,int(bit)-int(old));
    }
#else
    static void adjust_move(Node& a,int from,int to,bool old,bool bit) {
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)
        const auto f = _mm256_set1_epi32(from-1), t = _mm256_set1_epi32(to-1);
        const auto end = _mm256_set1_epi32(a.count);
        for (int j = std::min(from,to) & ~7; j < a.count; j += 8) {
            auto ix = _mm256_setr_epi32(j,j+1,j+2,j+3,j+4,j+5,j+6,j+7);
            auto live = _mm256_cmpgt_epi32(end,ix);
            auto fm = _mm256_and_si256(live,_mm256_cmpgt_epi32(ix,f));
            auto tm = _mm256_and_si256(live,_mm256_cmpgt_epi32(ix,t));
            if (from != to) {
                auto p = reinterpret_cast<__m256i*>(a.sizes+j);
                _mm256_store_si256(p,_mm256_add_epi32(_mm256_load_si256(p),_mm256_sub_epi32(fm,tm)));
            }
            if (old || bit) {
                auto p = reinterpret_cast<__m256i*>(a.ones+j);
                auto d = _mm256_sub_epi32(_mm256_and_si256(fm,_mm256_set1_epi32(-int(old))),_mm256_and_si256(tm,_mm256_set1_epi32(-int(bit))));
                _mm256_store_si256(p,_mm256_add_epi32(_mm256_load_si256(p),d));
            }
        }
#else
        for (int j = std::min(from,to); j < a.count; ++j) {
            a.sizes[j] += int(j >= to)-int(j >= from);
            a.ones[j] += int(bit && j >= to)-int(old && j >= from);
        }
#endif
    }
#endif
public:
    DynamicBitVector() = default;
    DynamicBitVector(const DynamicBitVector& other)
        : leaves(other.leaves),pool(other.pool),root(other.root),free_leaf(other.free_leaf),free_node(other.free_node),length(other.length),total_ones(other.total_ones) {
#if EEZ_BV_STATIC_INDEX
        if (other.read_index) {
            std::size_t count = (std::size_t(length)+63)/64;
            read_index.reset(new StaticWord[count]); std::memcpy(read_index.get(),other.read_index.get(),count*sizeof(StaticWord));
        }
#endif
    }
    DynamicBitVector& operator=(const DynamicBitVector& b) { if (this != &b) { DynamicBitVector tmp(b); swap(tmp); } return *this; }
    DynamicBitVector(DynamicBitVector&& b) noexcept { swap(b); }
    DynamicBitVector& operator=(DynamicBitVector&& b) noexcept { if (this != &b) { DynamicBitVector tmp(std::move(b)); swap(tmp); } return *this; }
    void swap(DynamicBitVector& b) noexcept {
#if EEZ_BV_STATIC_INDEX
        read_index.swap(b.read_index);
#endif
        using std::swap; swap(leaves,b.leaves); swap(pool,b.pool); swap(root,b.root);
        swap(free_leaf,b.free_leaf); swap(free_node,b.free_node); swap(length,b.length); swap(total_ones,b.total_ones);
    }
    int size() const { return length; }
    bool empty() const { return length == 0; }
    void clear() {
#if EEZ_BV_STATIC_INDEX
        read_index.reset();
#endif
        leaves.clear(); pool.clear(); root = -1; free_leaf = free_node = -1; length = total_ones = 0; }
    void reserve(int n) { assert(n >= 0); leaves.reserve(std::size_t(n)/(HALF*64)+2); pool.reserve(std::size_t(n)/(HALF*64)/(FANOUT/2)+16); }
    std::vector<u64> export_packed(){
        std::vector<u64> out((std::size_t(length)+63)/64,total_ones==length?~u64(0):0);
        if(total_ones && total_ones!=length){int offset=0;collect_packed(root,out.data(),offset);assert(offset==length);}
        if((length&63)&&!out.empty())out.back()&=mask(length&63);
        return out;
    }
    void discard_query_index(){
#if EEZ_BV_STATIC_INDEX
        read_index.reset();
#endif
    }
    // Packed input is little-endian within each word; padding bits are ignored.
    void build_packed(const std::vector<u64>& words,int n){build_packed(words,n,true);}
    void build_packed(const std::vector<u64>& words,int n,bool use_index) {
        assert(n >= 0 && words.size() >= (std::size_t(n)+63)/64);
        clear(); reserve(n); if (!n) return;
        std::vector<int> layer; layer.reserve((std::size_t(n)+BUILD_BITS-1)/BUILD_BITS);
        for (int pos = 0; pos < n;) {
            int p = alloc_leaf(), len = std::min(BUILD_BITS,n-pos); auto& leaf = leaves[~p];
            leaf.count = len; int count = (len+63)/64;
            for (int j = 0; j < count; ++j) leaf.words[j] = words[std::size_t(pos)/64+j];
            if (len & 63) leaf.words[count-1] &= mask(len & 63);
            leaf_counts(leaf); layer.push_back(p); pos += len;
        }
        while (layer.size() > 1) {
            std::vector<int> next; next.reserve((layer.size()+FANOUT-1)/FANOUT);
            for (std::size_t i = 0; i < layer.size();) {
                int p = alloc_node(), count = int(std::min<std::size_t>(FANOUT,layer.size()-i)); pool[p].count = count;
                for (int j = 0; j < count; ++j) pool[p].child[j] = layer[i+j];
                inner_counts(p); next.push_back(p); i += count;
            }
            layer.swap(next);
        }
        root = layer[0]; length = n; total_ones = ones_of(root);
#if EEZ_BV_STATIC_INDEX
        if(use_index)index_packed(words.data());
#endif
    }
    void build(const std::vector<std::uint8_t>& a){build(a,true);}
    void build(const std::vector<std::uint8_t>& a,bool use_index) {
        assert(a.size() <= std::size_t(INT_MAX));
        clear(); reserve(int(a.size())); if (a.empty()) return;
        std::vector<int> layer; layer.reserve((a.size()+BUILD_BITS-1)/BUILD_BITS);
        for (std::size_t pos = 0; pos < a.size();) {
            int p = alloc_leaf(), len = int(std::min<std::size_t>(BUILD_BITS,a.size()-pos)); auto& leaf = leaves[~p];
            leaf.count = len; int j = 0;
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)
            for (; j+64 <= len; j += 64) {
                const auto zero = _mm256_setzero_si256();
                auto lo = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.data()+pos+j));
                auto hi = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.data()+pos+j+32));
                auto l = std::uint32_t(~_mm256_movemask_epi8(_mm256_cmpeq_epi8(lo,zero)));
                auto h = std::uint32_t(~_mm256_movemask_epi8(_mm256_cmpeq_epi8(hi,zero)));
                leaf.words[j>>6] = u64(l) | (u64(h)<<32);
            }
#endif
            for (; j < len; ++j) leaf.words[j>>6] |= u64(a[pos+j] != 0) << (j&63);
            leaf_counts(leaf); layer.push_back(p); pos += len;
        }
        while (layer.size() > 1) {
            std::vector<int> next; next.reserve((layer.size()+FANOUT-1)/FANOUT);
            for (std::size_t i = 0; i < layer.size();) {
                int p = alloc_node(), count = int(std::min<std::size_t>(FANOUT,layer.size()-i)); pool[p].count = count;
                for (int j = 0; j < count; ++j) pool[p].child[j] = layer[i+j];
                inner_counts(p); next.push_back(p); i += count;
            }
            layer.swap(next);
        }
        root = layer[0]; length = int(a.size()); total_ones = ones_of(root);
#if EEZ_BV_STATIC_INDEX
        if(use_index)prepare_queries();
#endif
    }
#if defined(__GNUC__) || defined(__clang__)
    __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
    __forceinline
#endif
    std::pair<bool,int> access_rank(int k) const {
        assert(0 <= k && k < size());
        if (!total_ones) return {false,0};
        if (total_ones == length) return {true,k};
#if EEZ_BV_STATIC_INDEX
        if (read_index) return indexed_access_rank(k);
#endif
        int p = root, rank = 0;
        while (p >= 0) {
            const auto& a = pool[p]; int i = locate(a,k);
            if (i) { k -= a.sizes[i-1]; rank += a.ones[i-1]; } p = a.child[i]; prefetch(p);
        }
        auto q = leaf_access_rank(leaves[~p],k); q.second += rank; return q;
    }
    bool access(int k) const {
        assert(0 <= k && k < size());
        if (!total_ones) return false;
        if (total_ones == length) return true;
#if EEZ_BV_STATIC_INDEX
        if (read_index) return (read_index[k >> 6].bits >> (k & 63)) & 1;
#endif
        int p = root;
        while (p >= 0) { const auto& a = pool[p]; int i = locate(a,k); if (i) k -= a.sizes[i-1]; p = a.child[i]; prefetch(p); }
        return leaf_access(leaves[~p],k);
    }
    int rank1(int k) const {
        assert(0 <= k && k <= size()); if (!k || !total_ones) return 0;
        if (total_ones == length) return k;
        if (k == length) return total_ones;
#if EEZ_BV_STATIC_INDEX
        if (read_index) return indexed_rank(k);
#endif
        int p = root, rank = 0;
        while (p >= 0) { const auto& a = pool[p]; int i = locate(a,k); if (i) { k -= a.sizes[i-1]; rank += a.ones[i-1]; } p = a.child[i]; prefetch(p); }
        return rank+leaf_rank(leaves[~p],k);
    }
    int rank0(int k) const { return k-rank1(k); }
#if defined(__GNUC__) || defined(__clang__)
    __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
    __forceinline
#endif
    std::pair<int,int> rank1_pair(int l,int r) const {
        assert(0 <= l && l <= r && r <= size());
        if (!total_ones) return {0,0};
        if (total_ones == length) return {l,r};
#if EEZ_BV_STATIC_INDEX
        if (read_index) return {indexed_rank(l),indexed_rank(r)};
#endif
        if (l == r) { int o = rank1(l); return {o,o}; }
        if (r-l == 1) { auto q = access_rank(l); return {q.second,q.second+int(q.first)}; }
        if (!l) return {0,rank1(r)};
        if (r == length) return {rank1(l),total_ones};
        int p = root, base = 0;
        while (p >= 0) {
            const auto& a = pool[p]; int i = locate(a,l), j = r <= a.sizes[i] ? i : locate(a,r);
            if (i != j) {
                int ol = i ? a.ones[i-1] : 0, orr = j ? a.ones[j-1] : 0;
                l -= i ? a.sizes[i-1] : 0; r -= j ? a.sizes[j-1] : 0;
                int left = a.child[i], right = a.child[j];
                while (left >= 0) {
                    const auto& al = pool[left]; const auto& ar = pool[right];
                    int il = locate(al,l), ir = locate(ar,r);
                    if (il) { l -= al.sizes[il-1]; ol += al.ones[il-1]; }
                    if (ir) { r -= ar.sizes[ir-1]; orr += ar.ones[ir-1]; }
                    left = al.child[il]; right = ar.child[ir]; prefetch(left); prefetch(right);
                }
                return {base+ol+leaf_rank(leaves[~left],l),base+orr+leaf_rank(leaves[~right],r)};
            }
            if (i) { l -= a.sizes[i-1]; r -= a.sizes[i-1]; base += a.ones[i-1]; }
            p = a.child[i]; prefetch(p);
        }
        auto q = leaf_pair(leaves[~p],l,r); q.first += base; q.second += base; return q;
    }
    int count1(int l,int r) const { auto q = rank1_pair(l,r); return q.second-q.first; }
    std::pair<bool,int> set_rank(int k,bool bit) {
        assert(0 <= k && k < size());
        if (!total_ones && !bit) return {false,0};
        if (total_ones == length && bit) return {true,k};
#if EEZ_BV_STATIC_INDEX
        if (read_index) { auto q = indexed_access_rank(k); if (q.first == bit) return q; read_index.reset(); }
#endif
        PathStep path[16]; int depth = 0, p = root, local = k, rank = 0;
        while (p >= 0 && depth < 16) {
            const auto& a = pool[p]; int i = locate(a,local);
            path[depth++] = {p,i};
            if (i) { local -= a.sizes[i-1]; rank += a.ones[i-1]; }
            p = a.child[i]; prefetch(p);
        }
        std::pair<bool,int> result;
        if (p >= 0) result = replace(root,k,bit);
        else {
            result = replace(p,local,bit); result.second += rank;
            if (result.first != bit) for (int d = depth-1; d >= 0; --d) adjust(pool[path[d].node],path[d].child,0,int(bit)-int(result.first));
        }
        total_ones += int(bit)-int(result.first); return result;
    }
    bool set(int k,bool bit) { return set_rank(k,bit).first; }
    struct MoveRanks { bool previous; int old_rank, new_rank; };
    // Equivalent to erase_rank(from), then insert_rank(to,bit). 'to' is
    // an insertion position after erasure. Fuse searches when no split/removal
    // is needed; fall back to the general operations for structural changes.
    MoveRanks move_rank(int from,int to,bool bit) {
        assert(0 <= from && from < length && 0 <= to && to < length);
        if (!total_ones && !bit) return {false,0,0};
        if (total_ones == length && bit) return {true,from,to};
        if (from == to) { auto q = set_rank(from,bit); return {q.first,q.second,q.second}; }
#if EEZ_BV_STATIC_INDEX
        read_index.reset();
#endif
        struct Step { int node, child; } left_path[16], right_path[16];
        int left = root, right = root, l = from, r = to, ol = 0, orr = 0, depth = 0;
        while (left >= 0) {
            if (depth == 16) { auto q = erase_rank(from); int o = insert_rank(to,bit); return {q.first,q.second,o}; }
            const auto& a = pool[left]; const auto& b = pool[right];
            int i,j;
#if EEZ_BV_MOVE_PAIR
            if(left==right){auto q=locate_two(a,l,r);i=q.first;j=q.second;}
            else
#endif
            {i=locate(a,l);j=locate(b,r);}
            left_path[depth] = {left,i}; right_path[depth] = {right,j}; ++depth;
            if (i) { l -= a.sizes[i-1]; ol += a.ones[i-1]; }
            if (j) { r -= b.sizes[j-1]; orr += b.ones[j-1]; }
            left = a.child[i]; right = b.child[j];
        }
        if (left != right && (leaves[~left].count == 1 || leaves[~right].count == CAP)) {
            auto q = erase_rank(from); int o = insert_rank(to,bit); return {q.first,q.second,o};
        }
        bool old; del(left,l,ol,old);
        if (left != right && from < to) { ++r; orr -= int(old); }
        int unused; [[maybe_unused]] bool split = ins(right,r,bit,orr,unused); assert(!split);
        for (int d = 0; d < depth; ++d) {
            auto a = left_path[d], b = right_path[d];
            if (a.node == b.node) {
                if (a.child != b.child || old != bit) adjust_move(pool[a.node],a.child,b.child,old,bit);
            } else {
                adjust(pool[a.node],a.child,-1,-int(old));
                adjust(pool[b.node],b.child,1,int(bit));
            }
        }
        total_ones += int(bit)-int(old); return {old,ol,orr};
    }
    template<int WIDTH> void access_rank_batch(const int* indices,bool* bits,int* ranks) const {
        static_assert(WIDTH > 0); int nodes[WIDTH], offsets[WIDTH];
#if EEZ_BV_STATIC_INDEX
        if (read_index) {
            for (int j = 0; j < WIDTH; ++j) { assert(0 <= indices[j] && indices[j] < size()); auto q = indexed_access_rank(indices[j]); bits[j] = q.first; ranks[j] = q.second; }
            return;
        }
#endif
        if (!total_ones || total_ones == length) {
            bool bit = total_ones != 0;
            for (int j = 0; j < WIDTH; ++j) { assert(0 <= indices[j] && indices[j] < size()); bits[j] = bit; ranks[j] = bit ? indices[j] : 0; }
            return;
        }
        for (int j = 0; j < WIDTH; ++j) { assert(0 <= indices[j] && indices[j] < size()); nodes[j] = root; offsets[j] = indices[j]; ranks[j] = 0; }
        while (nodes[0] >= 0) {
            for (int j = 0; j < WIDTH; ++j) {
                const auto& a = pool[nodes[j]]; int i = locate(a,offsets[j]);
                if (i) { offsets[j] -= a.sizes[i-1]; ranks[j] += a.ones[i-1]; } nodes[j] = a.child[i]; prefetch(nodes[j]);
            }
        }
        for (int j = 0; j < WIDTH; ++j) { const auto& a = leaves[~nodes[j]]; int k = offsets[j]; auto q = leaf_access_rank(a,k); bits[j] = q.first; ranks[j] += q.second; }
    }
    // Optional after an update batch. Const queries never allocate or mutate.
    void prepare_queries() {
#if EEZ_BV_STATIC_INDEX
        if (read_index || !total_ones || total_ones == length) return;
        std::size_t count = (std::size_t(length)+63)/64;
        std::unique_ptr<StaticWord[]> next(new StaticWord[count]);
        for (std::size_t i = 0; i < count; ++i) next[i].bits = 0;
        int offset = 0; collect_words(root,next.get(),offset); assert(offset == length);
        int rank = 0;
        for (std::size_t i = 0; i < count; ++i) { next[i].prefix = rank; rank += pc(next[i].bits); }
        read_index = std::move(next);
#endif
    }
    int insert_rank(int k,bool bit) {
        assert(0 <= k && k <= size()); assert(length < INT_MAX);
#if EEZ_BV_STATIC_INDEX
        read_index.reset();
#endif
        if (!length) root = alloc_leaf();
        PathStep path[16]; int depth = 0, p = root, local = k, rank = 0, sibling;
        while (p >= 0 && depth < 16) {
            const auto& a = pool[p]; int i = locate(a,local);
            path[depth++] = {p,i};
            if (i) { local -= a.sizes[i-1]; rank += a.ones[i-1]; }
            p = a.child[i]; prefetch(p);
        }
        bool split;
        if (p >= 0) { rank = 0; split = ins(root,k,bit,rank,sibling); }
        else {
            split = ins(p,local,bit,rank,sibling);
            for (int d = depth-1; d >= 0; --d) {
                if (split) split = attach(path[d].node,path[d].child,sibling);
                else adjust(pool[path[d].node],path[d].child,1,int(bit));
            }
        }
        if (split) {
            p = alloc_node(); pool[p].count = 2;
            pool[p].child[0] = root; pool[p].child[1] = sibling; inner_counts(p); root = p;
        }
        ++length; total_ones += int(bit); return rank;
    }
    void insert(int k,bool bit) { (void)insert_rank(k,bit); }
    std::pair<bool,int> erase_rank(int k) {
        assert(0 <= k && k < size());
#if EEZ_BV_STATIC_INDEX
        read_index.reset();
#endif
        PathStep path[16]; int depth = 0, p = root, local = k, rank = 0; bool bit;
        while (p >= 0 && depth < 16) {
            const auto& a = pool[p]; int i = locate(a,local);
            path[depth++] = {p,i};
            if (i) { local -= a.sizes[i-1]; rank += a.ones[i-1]; }
            p = a.child[i]; prefetch(p);
        }
        bool empty;
        if (p >= 0) { rank = 0; empty = del(root,k,rank,bit); }
        else {
            empty = del(p,local,rank,bit);
            for (int d = depth-1; d >= 0; --d) {
                auto& a = pool[path[d].node]; int i = path[d].child;
                if (!empty) adjust(a,i,-1,-int(bit));
                else {
                    release(a.child[i]);
                    std::memmove(a.child+i,a.child+i+1,std::size_t(a.count-i-1)*sizeof(int));
                    empty = --a.count == 0;
                    if (!empty) inner_counts(path[d].node);
                }
            }
        }
        if (empty) { release(root); root = -1; }
        else while (root >= 0 && pool[root].count == 1) { int old = root; root = pool[old].child[0]; release(old); }
        --length; total_ones -= int(bit); return {bit,rank};
    }
    bool erase(int k) { return erase_rank(k).first; }
};


// Read-only 4-ary wavelet matrix, invalidated before any edit.
template<class T,int LOG> class QuadIndex {
    using U=std::uint64_t;
    static constexpr int LEVELS=(LOG+1)/2;
    struct Block { U low,high; int prefix[4]; };
    std::array<std::vector<Block>,LEVELS> data;
    std::array<std::array<int,4>,LEVELS> start,total;
    int n=0;
    std::array<int,LEVELS> uniform;
    static U mask(int k){return k==64?~U(0):(U(1)<<k)-1;}
    static int pc(U x){
#if defined(__GNUC__) || defined(__clang__)
        return __builtin_popcountll(x);
#else
        int n=0;while(x){x&=x-1;++n;}return n;
#endif
    }
    static U matches(const Block& b,int c){return (c&1?b.low:~b.low)&(c&2?b.high:~b.high);}
    int rank(int level,int c,int k)const{
        if(uniform[level]>=0)return uniform[level]==c?k:0;
        if(k==n)return total[level][c];
        const auto& b=data[level][k>>6];return b.prefix[c]+pc(matches(b,c)&mask(k&63));
    }
    std::array<int,4> counts(int level,int l,int r)const{
        std::array<int,4> out;
        for(int c=0;c<4;++c)out[c]=rank(level,c,r)-rank(level,c,l);
        return out;
    }
public:
    QuadIndex(){uniform.fill(-1);}
    // The binary builder orders each pair's buckets 0,2,1,3.
    void append(const std::vector<T>& cur,int binary_level){
        bool single=(LOG&1)&&binary_level==0;
        if(!single && ((LOG-binary_level)&1))return;
        n=int(cur.size());int level=(binary_level+1)/2;uniform[level]=-1;
        int shift=2*(LEVELS-level-1);
        auto& blocks=data[level];blocks.resize((std::size_t(n)+63)/64);
        std::array<int,4> count{};
        for(int i=0;i<n;i+=64){
            auto& b=blocks[i>>6];b.low=b.high=0;
            for(int c=0;c<4;++c)b.prefix[c]=count[c];
            int end=std::min(n,i+64),j=i;
#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)
            if constexpr(sizeof(T)==4){
                auto sh=_mm256_set1_epi32(shift),one=_mm256_set1_epi32(1);
                for(;j+8<=end;j+=8){
                    auto v=_mm256_loadu_si256(reinterpret_cast<const __m256i*>(cur.data()+j));v=_mm256_srlv_epi32(v,sh);
                    auto lo=_mm256_cmpeq_epi32(_mm256_and_si256(v,one),one);
                    auto hi=_mm256_cmpeq_epi32(_mm256_and_si256(_mm256_srli_epi32(v,1),one),one);
                    b.low|=U(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(lo))))<<(j-i);
                    b.high|=U(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(hi))))<<(j-i);
                }
            }
#endif
            for(;j<end;++j){int c=int((U(cur[j])>>shift)&3);b.low|=U(c&1)<<(j-i);b.high|=U(c>>1)<<(j-i);}
            U valid=mask(end-i);int lo=pc(b.low&valid),hi=pc(b.high&valid),both=pc(b.low&b.high&valid);
            count[0]+=end-i-lo-hi+both;count[1]+=lo-both;count[2]+=hi-both;count[3]+=both;
        }
        total[level]=count;
        for(int c=0;c<4;++c)if(count[c]==n){uniform[level]=c;std::vector<Block>().swap(blocks);break;}
        int sum=0;
        for(int j=0;j<4;++j){int c=single?j:((j&1)<<1)|(j>>1);start[level][c]=sum;sum+=count[c];}
    }
    static U take(const std::vector<U>& words,int pos,int count){
        if(!count)return 0;
        int shift=pos&63;U value=words[std::size_t(pos)/64]>>shift;
        if(shift&&count>64-shift)value|=words[std::size_t(pos)/64+1]<<(64-shift);
        return value&mask(count);
    }
    static U deposit(U value,U positions){
#if defined(__BMI2__) && !defined(EEZ_NO_SIMD)
        return _pdep_u64(value,positions);
#else
        U out=0;while(positions){U bit=positions&-positions;out|=bit*U(value&1);positions&=positions-1;value>>=1;}return out;
#endif
    }
    void append_binary(const std::vector<U>& high,const std::vector<U>& low,int length,int binary_level,int zeros){
        bool single=(LOG&1)&&binary_level==0;
        n=length;int level=(binary_level+1)/2;uniform[level]=-1;
        auto& blocks=data[level];blocks.resize((std::size_t(n)+63)/64);
        std::array<int,4> count{};int zp=0,op=zeros;
        for(int i=0;i<n;i+=64){
            int valid=std::min(64,n-i);auto& b=blocks[i>>6];
            for(int c=0;c<4;++c)b.prefix[c]=count[c];
            if(single){b.low=high[i>>6]&mask(valid);b.high=0;}
            else{
                b.high=high[i>>6]&mask(valid);int ones=pc(b.high),zero=valid-ones;
                b.low=deposit(take(low,zp,zero),~b.high&mask(valid))|deposit(take(low,op,ones),b.high);
                zp+=zero;op+=ones;
            }
            int lo=pc(b.low),hi=pc(b.high),both=pc(b.low&b.high);
            count[0]+=valid-lo-hi+both;count[1]+=lo-both;count[2]+=hi-both;count[3]+=both;
        }
        total[level]=count;
        for(int c=0;c<4;++c)if(count[c]==n){uniform[level]=c;std::vector<Block>().swap(blocks);break;}
        int sum=0;for(int j=0;j<4;++j){int c=single?j:((j&1)<<1)|(j>>1);start[level][c]=sum;sum+=count[c];}
    }
    int less_rank(int level,int c,int k)const{
        if(uniform[level]>=0)return uniform[level]<c?k:0;
        if(!c)return 0;
        if(c==1)return rank(level,0,k);
        if(c==3)return k-rank(level,3,k);
        if(k==n)return total[level][0]+total[level][1];
        const auto& b=data[level][k>>6];return b.prefix[0]+b.prefix[1]+pc(~b.high&mask(k&63));
    }
    T access(int k)const{
        U value=0;
        for(int level=0;level<LEVELS;++level){
            if(uniform[level]>=0){value=(value<<2)|U(uniform[level]);continue;}
            const auto& b=data[level][k>>6];int shift=k&63;
            int c=int((b.low>>shift)&1)|int(((b.high>>shift)&1)<<1);
            value=(value<<2)|U(c);k=start[level][c]+b.prefix[c]+pc(matches(b,c)&mask(shift));
        }
        return T(value);
    }
    void access_many(const int* ix,std::size_t count,T* output)const{
        for(std::size_t base=0;base<count;base+=8){
            int size=int(std::min<std::size_t>(8,count-base));int pos[8];U values[8]{};
            for(int j=0;j<size;++j){assert(ix[base+j]>=0&&ix[base+j]<n);pos[j]=ix[base+j];}
            for(int level=0;level<LEVELS;++level)for(int j=0;j<size;++j){
                if(uniform[level]>=0){values[j]=(values[j]<<2)|U(uniform[level]);continue;}
                int k=pos[j],shift=k&63;const auto& b=data[level][k>>6];
                int c=int((b.low>>shift)&1)|int(((b.high>>shift)&1)<<1);
                values[j]=(values[j]<<2)|U(c);pos[j]=start[level][c]+b.prefix[c]+pc(matches(b,c)&mask(shift));
            }
            for(int j=0;j<size;++j)output[base+j]=T(values[j]);
        }
    }
    int equal(T value,int l,int r)const{
        for(int level=0;level<LEVELS&&l<r;++level){int c=int((U(value)>>(2*(LEVELS-level-1)))&3);l=start[level][c]+rank(level,c,l);r=start[level][c]+rank(level,c,r);}
        return r-l;
    }
    std::pair<int,int> rank_less(int level,int c,int k)const{
        if(uniform[level]>=0)return {uniform[level]==c?k:0,uniform[level]<c?k:0};
        if(k==n){int less=0;for(int j=0;j<c;++j)less+=total[level][j];return {total[level][c],less};}
        const auto& b=data[level][k>>6];U m=mask(k&63);
        int equal=b.prefix[c]+pc(matches(b,c)&m);
        if(c==0)return {equal,0};if(c==3)return {equal,k-equal};
        int less=c==1?b.prefix[0]+pc(~(b.high|b.low)&m):b.prefix[0]+b.prefix[1]+pc(~b.high&m);
        return {equal,less};
    }
    T extreme(int level,int l,int r,U value,bool maximum)const{
        for(;level<LEVELS;++level){
            if(uniform[level]>=0){value=(value<<2)|U(uniform[level]);continue;}
            int c=maximum?3:0,nl=0,nr=0;
            for(;;){nl=rank(level,c,l);nr=rank(level,c,r);if(nl<nr)break;c+=maximum?-1:1;}
            value=(value<<2)|U(c);l=start[level][c]+nl;r=start[level][c]+nr;
        }
        return T(value);
    }
    template<bool PREV,bool NEXT>std::pair<std::optional<T>,std::optional<T>> neighbors(T x,int l,int r)const{
        struct Candidate{int level=-1,l=0,r=0;U prefix=0;};Candidate prev,next;U value=0;
        std::pair<std::optional<T>,std::optional<T>> result;
        for(int level=0;level<LEVELS&&l<r;++level){
            int c=int((U(x)>>(2*(LEVELS-level-1)))&3);
            if constexpr(PREV)for(int d=c-1;d>=0;--d){int a=rank(level,d,l),b=rank(level,d,r);if(a<b){prev={level+1,start[level][d]+a,start[level][d]+b,(value<<2)|U(d)};break;}}
            if constexpr(NEXT)for(int d=c+1;d<4;++d){int a=rank(level,d,l),b=rank(level,d,r);if(a<b){next={level+1,start[level][d]+a,start[level][d]+b,(value<<2)|U(d)};break;}}
            value=(value<<2)|U(c);l=start[level][c]+rank(level,c,l);r=start[level][c]+rank(level,c,r);
        }
        if constexpr(PREV)if(prev.level>=0)result.first=extreme(prev.level,prev.l,prev.r,prev.prefix,true);
        if constexpr(NEXT){if(l<r)result.second=x;else if(next.level>=0)result.second=extreme(next.level,next.l,next.r,next.prefix,false);}
        return result;
    }
    int less(T value,int l,int r)const{
        int answer=0;
        for(int level=0;level<LEVELS&&l<r;++level){
            int c=int((U(value)>>(2*(LEVELS-level-1)))&3);
            auto a=rank_less(level,c,l),b=rank_less(level,c,r);answer+=b.second-a.second;
            l=start[level][c]+a.first;r=start[level][c]+b.first;
        }
        return answer;
    }
    std::pair<int,int> less_equal(T value,int l,int r)const{
        int answer=0;
        for(int level=0;level<LEVELS&&l<r;++level){
            int c=int((U(value)>>(2*(LEVELS-level-1)))&3);
            auto a=rank_less(level,c,l),b=rank_less(level,c,r);answer+=b.second-a.second;
            l=start[level][c]+a.first;r=start[level][c]+b.first;
        }
        return {answer,r-l};
    }
    struct Result {T value;int less,equal;};
    Result kth(int l,int r,int k)const{
        U value=0;int less=0;
        for(int level=0;level<LEVELS;++level){
            if(uniform[level]>=0){value=(value<<2)|U(uniform[level]);continue;}
            int hl=less_rank(level,2,l),hr=less_rank(level,2,r),zeros=hr-hl,c;
            int nl,nr;
            if(k<zeros){
                int zl=rank(level,0,l),zr=rank(level,0,r),z=zr-zl;
                if(k<z){c=0;nl=zl;nr=zr;}else{c=1;k-=z;less+=z;nl=hl-zl;nr=hr-zr;}
            }else{
                k-=zeros;less+=zeros;int zl=rank(level,2,l),zr=rank(level,2,r),z=zr-zl;
                if(k<z){c=2;nl=zl;nr=zr;}else{c=3;k-=z;less+=z;nl=l-hl-zl;nr=r-hr-zr;}
            }
            value=(value<<2)|U(c);l=start[level][c]+nl;r=start[level][c]+nr;
        }
        return {T(value),less,r-l};
    }
};

template<class T=int,int LOG=std::numeric_limits<T>::digits>
class DynamicWaveletMatrix{
    std::optional<QuadIndex<T,LOG>> quad;
    bool quad_valid=false;
    void invalidate_quad(){quad_valid=false;if constexpr(!EEZ_QUAD_RETAIN)quad.reset();}
public:
    struct Counts { int less,equal,greater; };
    struct Quantile { T value; int less,equal; };
    struct Neighbors { std::optional<T> previous,next; };
private:
    static_assert(std::is_integral_v<T>);
    static_assert(!std::is_same_v<T,bool>);
    static_assert(1<=LOG&&LOG<=64);

    using U=std::make_unsigned_t<T>;
    using u64=std::uint64_t;

    std::array<DynamicBitVector,LOG> bv;
    std::array<int,LOG> mid{};
    int n=0;

    static bool negative(T x){
        if constexpr(std::is_signed_v<T>)return x<0;
        else return false;
    }
    static u64 as_u64(T x){
        return (u64)(U)x;
    }
    static bool valid_value(T x){
        if(negative(x))return false;
        if constexpr(LOG==64)return true;
        else return as_u64(x)<(u64(1)<<LOG);
    }
    static bool bit(u64 x,int level){
        return (x>>(LOG-1-level))&1;
    }
    void range_check([[maybe_unused]] int l,[[maybe_unused]] int r)const{
        assert(0<=l&&l<=r&&r<=n);
    }
    int less_suffix(int level,int l,int r,u64 v,int ans)const{
        for(;level<LOG&&l<r;++level){
            auto [ol,orr]=bv[level].rank1_pair(l,r);
            if(bit(v,level)){ans+=(r-l)-(orr-ol);l=mid[level]+ol;r=mid[level]+orr;}
            else{l-=ol;r-=orr;}
        }
        return ans;
    }
    T extreme_suffix(int level,int l,int r,u64 value,bool maximum)const{
        for(;level<LOG;++level){
            auto [ol,orr]=bv[level].rank1_pair(l,r);
            int zeros=(r-l)-(orr-ol);
            bool b=maximum?(orr>ol):(zeros==0);
            if(b){value|=u64(1)<<(LOG-1-level);l=mid[level]+ol;r=mid[level]+orr;}
            else{l-=ol;r-=orr;}
        }
        return T(value);
    }
    template<bool PREV,bool NEXT>
    Neighbors neighbors_impl(int l,int r,T x)const{
        range_check(l,r);Neighbors result{};if(l==r)return result;
        if(quad_valid){
            if(negative(x)){if constexpr(NEXT)result.next=quad->extreme(0,l,r,0,false);return result;}
            if(!valid_value(x)){if constexpr(PREV)result.previous=quad->extreme(0,l,r,0,true);return result;}
            auto q=quad->template neighbors<PREV,NEXT>(x,l,r);return {q.first,q.second};
        }

        if(negative(x)){if constexpr(NEXT)result.next=extreme_suffix(0,l,r,0,false);return result;}
        if(!valid_value(x)){if constexpr(PREV)result.previous=extreme_suffix(0,l,r,0,true);return result;}
        struct Candidate{int level=-1,l=0,r=0;u64 value=0;};
        Candidate prev,next;u64 prefix=0,v=as_u64(x);
        for(int level=0;level<LOG&&l<r;++level){
            auto [ol,orr]=bv[level].rank1_pair(l,r);
            int zl=l-ol,zr=r-orr;u64 m=u64(1)<<(LOG-1-level);
            if(bit(v,level)){
                if constexpr(PREV)if(zl<zr)prev={level+1,zl,zr,prefix};
                prefix|=m;l=mid[level]+ol;r=mid[level]+orr;
            }else{
                if constexpr(NEXT)if(ol<orr)next={level+1,mid[level]+ol,mid[level]+orr,prefix|m};
                l=zl;r=zr;
            }
        }
        if constexpr(PREV)if(prev.level>=0)result.previous=extreme_suffix(prev.level,prev.l,prev.r,prev.value,true);
        if constexpr(NEXT){
            if(l<r)result.next=x;
            else if(next.level>=0)result.next=extreme_suffix(next.level,next.l,next.r,next.value,false);
        }
        return result;
    }

public:
    using value_type=T;

    DynamicWaveletMatrix()=default;
    explicit DynamicWaveletMatrix(const std::vector<T>& a){build(a);}

    int size()const{return n;}
    bool empty()const{return n==0;}

    void clear(){
        quad.reset();quad_valid=false; n=0;
        mid.fill(0);
        for(auto& b:bv)b.clear();
    }
    // Rebuild read-only rank indexes after a batch of edits. Any later edit
    // invalidates its affected layers automatically; all existing APIs remain usable.
    void prepare_queries(){ if(quad_valid)return;for(auto& b:bv)b.prepare_queries(); }
    // Rebuild directly from binary levels; no value-array reconstruction.
    void prepare_quad_queries(){
        if constexpr(!EEZ_WM_QUAD_INDEX)return;
        if(quad_valid)return;
        if(!quad)quad.emplace();auto& index=*quad;
        for(int level=0;level<LOG;){
            auto high=bv[level].export_packed();
            if((LOG&1)&&level==0){index.append_binary(high,{},n,level,mid[level]);++level;}
            else{auto low=bv[level+1].export_packed();index.append_binary(high,low,n,level,mid[level]);level+=2;}
        }
        quad_valid=true;
        for(auto& b:bv)b.discard_query_index();
    }
    void reserve(int m){
        for(auto& b:bv)b.reserve(m);
    }
    void build(const std::vector<T>& a){
        assert(a.size()<=std::size_t(INT_MAX));
        invalidate_quad(); n=(int)a.size();
        if constexpr(EEZ_WM_QUAD_INDEX)if(!quad)quad.emplace();
        for([[maybe_unused]] T x:a)assert(valid_value(x));

#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)
        // Extract 8 x 32-bit or 4 x 64-bit values per vector directly into
        // packed leaves, avoiding the intermediate byte-per-bit array.
        if constexpr(sizeof(T)==4 || sizeof(T)==8) {
            std::vector<T> cur=a,nxt(n);
            std::vector<u64> packed((std::size_t(n)+63)/64);
            for(int level=0;level<LOG;++level){
                if(quad)quad->append(cur,level);
                const auto shift=_mm_cvtsi32_si128(LOG-1-level);
                for(int pos=0;pos<n;) {
                    int len=std::min(64,n-pos),j=0;u64 word=0;
                    if constexpr(sizeof(T)==4) {
                        for(;j+8<=len;j+=8) {
                            auto v=_mm256_loadu_si256(reinterpret_cast<const __m256i*>(cur.data()+pos+j));
                            v=_mm256_slli_epi32(_mm256_srl_epi32(v,shift),31);
                            word|=u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(v))))<<j;
                        }
                    } else {
                        for(;j+4<=len;j+=4) {
                            auto v=_mm256_loadu_si256(reinterpret_cast<const __m256i*>(cur.data()+pos+j));
                            v=_mm256_slli_epi64(_mm256_srl_epi64(v,shift),63);
                            word|=u64(unsigned(_mm256_movemask_pd(_mm256_castsi256_pd(v))))<<j;
                        }
                    }
                    for(;j<len;++j)word|=u64(bit(as_u64(cur[pos+j]),level))<<j;
                    packed[std::size_t(pos)/64]=word;pos+=len;
                }
                bv[level].build_packed(packed,n,!quad);
                int z=n-bv[level].rank1(n);mid[level]=z;
                if(level+1==LOG)break;
                int p0=0,p1=z;
                for(int i=0;i<n;++i) {
                    int b=bit(as_u64(cur[i]),level);nxt[b?p1:p0]=cur[i];p1+=b;p0+=1-b;
                }
                cur.swap(nxt);
            }
            quad_valid=bool(quad);return;
        }
#endif

        std::vector<T> cur=a,nxt(n);
        std::vector<std::uint8_t> bits(n);

        for(int level=0;level<LOG;++level){
                if(quad)quad->append(cur,level);
            int z=0;
            for(int i=0;i<n;++i){
                bool b=bit(as_u64(cur[i]),level);
                bits[i]=(std::uint8_t)b;
                z+=!b;
            }
            mid[level]=z;
            bv[level].build(bits,!quad);

            if(level+1==LOG)break;

            int p0=0,p1=z;
            for(int i=0;i<n;++i){
                int b=bits[i];nxt[b?p1:p0]=cur[i];p1+=b;p0+=1-b;
            }
            cur.swap(nxt);
        }

        quad_valid=bool(quad);
    }

    T access(int k)const{
        assert(0<=k&&k<n);
        if(quad_valid)return quad->access(k);
        u64 x=0;
        int p=k;
        for(int level=0;level<LOG;++level){
            auto [b,o]=bv[level].access_rank(p);
            x=(x<<1)|u64(b);
            p=b?mid[level]+o:p-o;
        }
        return (T)x;
    }
    T get(int k)const{return access(k);}
    T operator[](int k)const{return access(k);}
    // Output must not overlap indices. Repeated and unsorted positions are allowed.
    void access_many(const int* indices,std::size_t count,T* output)const{
        assert(count==0||(indices&&output));
        if(quad_valid){quad->access_many(indices,count,output);return;}
        std::size_t i=0;
        for(;count-i>=8;i+=8){
            int pos[8],ranks[8];bool bits[8];u64 values[8]{};
            for(int j=0;j<8;++j)pos[j]=indices[i+j];
            for(int level=0;level<LOG;++level){
                bv[level].template access_rank_batch<8>(pos,bits,ranks);
                for(int j=0;j<8;++j){
                    values[j]=(values[j]<<1)|u64(bits[j]);
                    pos[j]=bits[j]?mid[level]+ranks[j]:pos[j]-ranks[j];
                }
            }
            for(int j=0;j<8;++j)output[i+j]=T(values[j]);
        }
        for(;i<count;++i)output[i]=access(indices[i]);
    }
    std::vector<T> access_many(const std::vector<int>& indices)const{
        std::vector<T> result(indices.size());access_many(indices.data(),indices.size(),result.data());return result;
    }

    void insert(int k,T x){
        invalidate_quad();
        assert(0<=k&&k<=n);
        assert(valid_value(x));
        u64 v=as_u64(x);
        int p=k;
        for(int level=0;level<LOG;++level){
            bool b=bit(v,level);
            int o=bv[level].insert_rank(p,b);
            int np=b?mid[level]+o:p-o;
            if(!b)++mid[level];
            p=np;
        }
        ++n;
    }
    void push_back(T x){insert(n,x);}

    T erase(int k){
        invalidate_quad();
        assert(0<=k&&k<n);
        u64 x=0;
        int p=k;
        for(int level=0;level<LOG;++level){
            auto [b,o]=bv[level].erase_rank(p);
            int np=b?mid[level]+o:p-o;
            if(b)x|=u64(1)<<(LOG-1-level);

            if(!b)--mid[level];
            p=np;
        }
        --n;
        return (T)x;
    }
    T pop_back(){
        assert(n);
        return erase(n-1);
    }
    T set(int k,T x){
        assert(0<=k&&k<n);assert(valid_value(x));
        if(quad_valid&&quad->access(k)==x)return x;
        invalidate_quad();
        int from=k,to=k;u64 old=0,v=as_u64(x);
        for(int level=0;level<LOG;++level){
            bool b=bit(v,level),ob;int old_rank,new_rank;
            if(from==to){auto q=bv[level].set_rank(from,b);ob=q.first;old_rank=new_rank=q.second;}
            else{
                auto q=bv[level].move_rank(from,to,b);ob=q.previous;old_rank=q.old_rank;new_rank=q.new_rank;
            }
            old=(old<<1)|u64(ob);
            int next_from=ob?mid[level]+old_rank:from-old_rank;
            int after_erase=mid[level]-int(!ob);
            int next_to=b?after_erase+new_rank:to-new_rank;
            mid[level]=after_erase+int(!b);from=next_from;to=next_to;
        }
        return T(old);
    }

    int rank(T x,int l,int r)const{
        range_check(l,r);
        if(!valid_value(x))return 0;
        u64 v=as_u64(x);

        if(quad_valid)return quad->equal(x,l,r);
        for(int level=0;level<LOG && l<r;++level){
            auto [ol,orr]=bv[level].rank1_pair(l,r);
            if(bit(v,level)){
                l=mid[level]+ol;
                r=mid[level]+orr;
            }else{
                l-=ol;
                r-=orr;
            }
        }
        return r-l;
    }
    int rank(T x,int r)const{return rank(x,0,r);}
    int count(int l,int r,T x)const{return rank(x,l,r);}

    Quantile kth_smallest_with_count(int l,int r,int k)const{
        range_check(l,r);
        assert(0<=k&&k<r-l);

        if(quad_valid){auto q=quad->kth(l,r,k);return {q.value,q.less,q.equal};}
        u64 x=0;int original_k=k;
        for(int level=0;level<LOG && l<r;++level){
            auto [ol,orr]=bv[level].rank1_pair(l,r);
            int zl=l-ol,zr=r-orr;
            int z=zr-zl;

            if(k<z){
                l=zl;
                r=zr;
            }else{
                x|=u64(1)<<(LOG-1-level);
                k-=z;
                l=mid[level]+ol;
                r=mid[level]+orr;
            }
        }
        return {T(x),original_k-k,r-l};
    }
    T kth_smallest(int l,int r,int k)const{return kth_smallest_with_count(l,r,k).value;}
    T kth_largest(int l,int r,int k)const{
        range_check(l,r);
        assert(0<=k&&k<r-l);
        return kth_smallest(l,r,r-l-1-k);
    }

    int range_freq(int l,int r,T upper)const{
        range_check(l,r);
        if(negative(upper))return 0;

        u64 v=as_u64(upper);
        if constexpr(LOG<64){
            if(v>=(u64(1)<<LOG))return r-l;
        }

        if(quad_valid)return quad->less(upper,l,r);
        int ans=0;
        for(int level=0;level<LOG && l<r;++level){
            auto [ol,orr]=bv[level].rank1_pair(l,r);
            int zl=l-ol,zr=r-orr;

            if(bit(v,level)){
                ans+=zr-zl;
                l=mid[level]+ol;
                r=mid[level]+orr;
            }else{
                l=zl;
                r=zr;
            }
        }
        return ans;
    }
    int range_freq(int l,int r,T lower,T upper)const{
        range_check(l,r);
        if(!(lower<upper))return 0;
        auto result=count_less_pair(l,r,lower,upper);return result.second-result.first;
    }

    Counts rank_all(int l,int r,T x)const{
        range_check(l,r);int total=r-l;
        if(negative(x))return {0,0,total};
        if(!valid_value(x))return {total,0,0};
        if(quad_valid){auto q=quad->less_equal(x,l,r);return {q.first,q.second,total-q.first-q.second};}
        int less=0;u64 v=as_u64(x);
        for(int level=0;level<LOG&&l<r;++level){
            auto [ol,orr]=bv[level].rank1_pair(l,r);
            if(bit(v,level)){less+=(r-l)-(orr-ol);l=mid[level]+ol;r=mid[level]+orr;}
            else{l-=ol;r-=orr;}
        }
        return {less,r-l,total-less-(r-l)};
    }
    // Counts values <lower and <upper, sharing their common bit prefix.
    std::pair<int,int> count_less_pair(int l,int r,T lower,T upper)const{
        range_check(l,r);assert(lower<=upper);
        if(lower==upper){int c=range_freq(l,r,lower);return {c,c};}
        if(!valid_value(lower)||!valid_value(upper))return {range_freq(l,r,lower),range_freq(l,r,upper)};
        if(quad_valid)return {quad->less(lower,l,r),quad->less(upper,l,r)};
        u64 lo=as_u64(lower),hi=as_u64(upper);int ans=0;
        for(int level=0;level<LOG&&l<r;++level){
            auto [ol,orr]=bv[level].rank1_pair(l,r);
            int zl=l-ol,zr=r-orr;bool a=bit(lo,level),b=bit(hi,level);
            if(a!=b)return {less_suffix(level+1,zl,zr,lo,ans),less_suffix(level+1,mid[level]+ol,mid[level]+orr,hi,ans+zr-zl)};
            if(a){ans+=zr-zl;l=mid[level]+ol;r=mid[level]+orr;}else{l=zl;r=zr;}
        }
        return {ans,ans};
    }
    Neighbors neighbors(int l,int r,T x)const{return neighbors_impl<true,true>(l,r,x);}

    std::optional<T> prev_value(int l,int r,T upper)const{
        return neighbors_impl<true,false>(l,r,upper).previous;
    }
    std::optional<T> next_value(int l,int r,T lower)const{
        return neighbors_impl<false,true>(l,r,lower).next;
    }
};

}


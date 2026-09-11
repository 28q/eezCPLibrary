---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"data-structure/DynamicWaveletMatrix.hpp\"\n#include <algorithm>\n\
    #include <array>\n#include <cassert>\n#include <climits>\n#include <cstdint>\n\
    #include <cstring>\n#include <new>\n#include <limits>\n#include <memory>\n#include\
    \ <optional>\n#include <type_traits>\n#include <utility>\n#include <vector>\n\
    #if (defined(__AVX2__) || defined(__BMI2__)) && !defined(EEZ_NO_SIMD)\n#include\
    \ <immintrin.h>\n#endif\n\n#ifndef EEZ_WM_QUAD_INDEX\n#define EEZ_WM_QUAD_INDEX\
    \ 1\n#endif\n#ifndef EEZ_BV_RANGE_UPDATE\n#define EEZ_BV_RANGE_UPDATE 0\n#endif\n\
    #ifndef EEZ_BV_MOVE_PAIR\n#define EEZ_BV_MOVE_PAIR 0\n#endif\n#ifndef EEZ_BV_LEAF_SIMD\n\
    #define EEZ_BV_LEAF_SIMD 0\n#endif\n#ifndef EEZ_QUAD_RETAIN\n#define EEZ_QUAD_RETAIN\
    \ 1\n#endif\nnamespace eez {\n\n#ifndef EEZ_BV_WORDS\n#define EEZ_BV_WORDS 64\n\
    #endif\n#ifndef EEZ_BV_FANOUT\n#define EEZ_BV_FANOUT 16\n#endif\n#ifndef EEZ_BV_GROUP\n\
    #define EEZ_BV_GROUP 4\n#endif\n#ifndef EEZ_BV_SIMD_SHIFT\n#define EEZ_BV_SIMD_SHIFT\
    \ 1\n#endif\n#ifndef EEZ_BV_PREFETCH\n#define EEZ_BV_PREFETCH 0\n#endif\n#ifndef\
    \ EEZ_BV_SEARCH\n#define EEZ_BV_SEARCH 0\n#endif\n#ifndef EEZ_BV_STATIC_INDEX\n\
    #define EEZ_BV_STATIC_INDEX 1\n#endif\n#ifndef EEZ_BV_BUFFER\n#define EEZ_BV_BUFFER\
    \ 0\n#endif\n#ifndef EEZ_BV_FILL\n#define EEZ_BV_FILL 80\n#endif\nnamespace detail\
    \ {\n// A contiguous arena for trivial nodes. Unused capacity is never zero-filled.\n\
    // Allocation creates all object lifetimes with new T[], retaining C++17 support.\n\
    template<class T> class NodeArena {\n    static_assert(std::is_trivial_v<T> &&\
    \ std::is_trivially_copyable_v<T>);\n    std::unique_ptr<T[]> data_;\n    std::uint32_t\
    \ used_ = 0, capacity_ = 0;\npublic:\n    NodeArena() = default;\n    NodeArena(const\
    \ NodeArena& other) {\n        reserve(other.used_); used_ = other.used_;\n  \
    \      if (used_) std::memcpy(data_.get(),other.data_.get(),std::size_t(used_)*sizeof(T));\n\
    \    }\n    NodeArena& operator=(const NodeArena& other) { if (this != &other)\
    \ { NodeArena tmp(other); swap(tmp); } return *this; }\n    NodeArena(NodeArena&&\
    \ other) noexcept { swap(other); }\n    NodeArena& operator=(NodeArena&& other)\
    \ noexcept { if (this != &other) { NodeArena tmp(std::move(other)); swap(tmp);\
    \ } return *this; }\n    void swap(NodeArena& other) noexcept {\n        data_.swap(other.data_);\
    \ std::swap(used_,other.used_); std::swap(capacity_,other.capacity_);\n    }\n\
    \    T& operator[](std::size_t i) noexcept { return data_[i]; }\n    const T&\
    \ operator[](std::size_t i) const noexcept { return data_[i]; }\n    std::size_t\
    \ size() const noexcept { return used_; }\n    std::size_t capacity() const noexcept\
    \ { return capacity_; }\n    void clear() noexcept { used_ = 0; }\n    void reserve(std::size_t\
    \ n) {\n        if (n <= capacity_) return;\n        if (n > std::size_t(INT_MAX))\
    \ throw std::bad_alloc();\n        std::unique_ptr<T[]> next(new T[n]);\n    \
    \    if (used_) std::memcpy(next.get(),data_.get(),std::size_t(used_)*sizeof(T));\n\
    \        data_.swap(next); capacity_ = std::uint32_t(n);\n    }\n    void emplace_back()\
    \ {\n        if (used_ == capacity_) reserve(capacity_ ? std::min(std::size_t(INT_MAX),std::size_t(capacity_)*2)\
    \ : 16);\n        if (used_ == std::uint32_t(INT_MAX)) throw std::bad_alloc();\n\
    \        ++used_; // The caller initializes the live fields, not padding/capacity.\n\
    \    }\n};\n}\n\n// B+ tree with independently tuned leaf blocks and SIMD-searchable\
    \ inner nodes.\n// A negative handle encodes a leaf index (~index); other handles\
    \ are inner nodes.\nclass DynamicBitVector {\n    using u64 = std::uint64_t;\n\
    \    static constexpr int WORDS = EEZ_BV_WORDS, FANOUT = EEZ_BV_FANOUT, GROUP\
    \ = EEZ_BV_GROUP;\n    static constexpr int CAP = WORDS*64, HALF = WORDS/2, GROUPS\
    \ = (WORDS+GROUP-1)/GROUP;\n    static constexpr int BUILD_WORDS = WORDS*EEZ_BV_FILL/100\
    \ > 0 ? WORDS*EEZ_BV_FILL/100 : 1;\n    static constexpr int BUILD_BITS = BUILD_WORDS*64;\n\
    \    static_assert(WORDS >= 2 && WORDS <= 128);\n    static_assert(FANOUT >= 8\
    \ && FANOUT <= 64 && (FANOUT&(FANOUT-1)) == 0);\n    static_assert(GROUP == 1\
    \ || GROUP == 2 || GROUP == 4 || GROUP == 8);\n    static_assert(EEZ_BV_FILL >=\
    \ 50 && EEZ_BV_FILL <= 100);\n    struct alignas(64) Leaf {\n        u64 words[WORDS];\n\
    \        std::uint16_t ones[GROUPS];\n        int count;\n#if EEZ_BV_BUFFER >\
    \ 0\n        std::uint32_t edits[EEZ_BV_BUFFER];\n        int edit_count;\n  \
    \      static_assert(EEZ_BV_BUFFER <= 16);\n#endif\n    };\n    struct alignas(64)\
    \ Node {\n        int sizes[FANOUT], ones[FANOUT], child[FANOUT];\n        int\
    \ count;\n    };\n#ifdef EEZ_BV_STD_VECTOR\n    std::vector<Leaf> leaves;\n  \
    \  std::vector<Node> pool;\n#else\n    detail::NodeArena<Leaf> leaves;\n    detail::NodeArena<Node>\
    \ pool;\n#endif\n    int root = -1, free_leaf = -1, free_node = -1;\n    int length\
    \ = 0, total_ones = 0;\n#if EEZ_BV_STATIC_INDEX\n    struct StaticWord { u64 bits;\
    \ int prefix; };\n    std::unique_ptr<StaticWord[]> read_index;\n    std::pair<bool,int>\
    \ indexed_access_rank(int k) const noexcept {\n        const auto& w = read_index[k\
    \ >> 6];\n        return {bool((w.bits >> (k & 63)) & 1),w.prefix+pc(w.bits &\
    \ mask(k & 63))};\n    }\n    int indexed_rank(int k) const noexcept {\n     \
    \   if (k == length) return total_ones;\n        const auto& w = read_index[k\
    \ >> 6]; return w.prefix+pc(w.bits & mask(k & 63));\n    }\n    void collect_words(int\
    \ p,StaticWord* output,int& offset) {\n        if (p >= 0) { const auto& a = pool[p];\
    \ for (int i = 0; i < a.count; ++i) collect_words(a.child[i],output,offset); return;\
    \ }\n        auto& a = leaves[~p];\n#if EEZ_BV_BUFFER > 0\n        flush_leaf(a);\n\
    #endif\n        for (int j = 0; j*64 < a.count; ++j) {\n            int count\
    \ = std::min(64,a.count-j*64), shift = offset & 63;\n            u64 w = a.words[j]\
    \ & mask(count); int dst = offset >> 6;\n            output[dst].bits |= w <<\
    \ shift;\n            if (shift && count > 64-shift) output[dst+1].bits |= w >>\
    \ (64-shift);\n            offset += count;\n        }\n    }\n    void index_packed(const\
    \ u64* words) {\n        if (!total_ones || total_ones == length) return;\n  \
    \      std::size_t count = (std::size_t(length)+63)/64;\n        std::unique_ptr<StaticWord[]>\
    \ next(new StaticWord[count]); int rank = 0;\n        for (std::size_t i = 0;\
    \ i < count; ++i) {\n            u64 w = words[i]; if (i+1 == count && (length\
    \ & 63)) w &= mask(length & 63);\n            next[i].bits = w; next[i].prefix\
    \ = rank; rank += pc(w);\n        }\n        read_index = std::move(next);\n \
    \   }\n#endif\n    void collect_packed(int p,u64* out,int& offset){\n        if(p>=0){const\
    \ auto& a=pool[p];for(int i=0;i<a.count;++i)collect_packed(a.child[i],out,offset);return;}\n\
    \        auto& a=leaves[~p];\n#if EEZ_BV_BUFFER > 0\n        flush_leaf(a);\n\
    #endif\n        for(int j=0;j*64<a.count;++j){\n            int count=std::min(64,a.count-j*64),shift=offset&63;u64\
    \ word=a.words[j]&mask(count);\n            out[offset>>6]|=word<<shift;\n   \
    \         if(shift&&count>64-shift)out[(offset>>6)+1]|=word>>(64-shift);\n   \
    \         offset+=count;\n        }\n    }\n    static u64 mask(int k) { return\
    \ k == 64 ? ~u64(0) : (u64(1) << k) - 1; }\n    static int pc(u64 x) {\n#if defined(__GNUC__)\
    \ || defined(__clang__)\n        return __builtin_popcountll(x);\n#else\n    \
    \    x -= (x >> 1) & 0x5555555555555555ull;\n        x = (x & 0x3333333333333333ull)\
    \ + ((x >> 2) & 0x3333333333333333ull);\n        x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0full;\n\
    \        return int((x * 0x0101010101010101ull) >> 56);\n#endif\n    }\n    void\
    \ prefetch(int p) const noexcept {\n#if EEZ_BV_PREFETCH && (defined(__GNUC__)\
    \ || defined(__clang__))\n        if (p < 0) { __builtin_prefetch(&leaves[~p],0,3);\
    \ __builtin_prefetch(leaves[~p].ones,0,3); }\n        else __builtin_prefetch(pool[p].sizes,0,3);\n\
    #else\n        (void)p;\n#endif\n    }\n    int size_of(int p) const { return\
    \ p < 0 ? leaves[~p].count : pool[p].sizes[pool[p].count-1]; }\n    int ones_of(int\
    \ p) const { return p < 0 ? leaf_total(leaves[~p]) : pool[p].ones[pool[p].count-1];\
    \ }\n    int alloc_leaf() {\n        int i;\n        if (free_leaf >= 0) { i =\
    \ free_leaf; free_leaf = leaves[i].count; leaves[i] = Leaf{}; }\n        else\
    \ { i = int(leaves.size()); leaves.emplace_back(); leaves[i] = Leaf{}; }\n   \
    \     return ~i;\n    }\n    int alloc_node() {\n        int p;\n        if (free_node\
    \ >= 0) { p = free_node; free_node = pool[p].child[0]; }\n        else { p = int(pool.size());\
    \ pool.emplace_back(); }\n        return p;\n    }\n    void release(int p) {\n\
    \        if (p < 0) { leaves[~p].count = free_leaf; free_leaf = ~p; }\n      \
    \  else { pool[p].child[0] = free_node; free_node = p; }\n    }\n    static void\
    \ leaf_counts(Leaf& a) {\n        int sum = 0;\n        for (int j = 0; j < WORDS;\
    \ ++j) { sum += pc(a.words[j]); if ((j+1)%GROUP == 0 || j+1 == WORDS) a.ones[j/GROUP]\
    \ = std::uint16_t(sum); }\n    }\n    void inner_counts(int p) {\n        auto&\
    \ a = pool[p]; int s = 0, o = 0;\n        for (int i = 0; i < a.count; ++i) {\n\
    \            s += size_of(a.child[i]); o += ones_of(a.child[i]);\n           \
    \ a.sizes[i] = s; a.ones[i] = o;\n        }\n        for (int i = a.count; i <\
    \ FANOUT; ++i) { a.sizes[i] = INT_MAX; a.ones[i] = o; }\n    }\n    static int\
    \ locate(const Node& a, int k) {\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\
    \ && EEZ_BV_SEARCH == 0\n        const auto key = _mm256_set1_epi32(k);\n    \
    \    u64 bits = 0;\n        for (int j = 0; j < FANOUT; j += 8) {\n          \
    \  auto v = _mm256_cmpgt_epi32(_mm256_load_si256(reinterpret_cast<const __m256i*>(a.sizes+j)),key);\n\
    \            bits |= u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(v))))\
    \ << j;\n        }\n        int i;\n#if defined(__GNUC__) || defined(__clang__)\n\
    \        i = bits ? __builtin_ctzll(bits) : FANOUT;\n#else\n        i = 0; while\
    \ (i < FANOUT && !(bits & (u64(1) << i))) ++i;\n#endif\n        return i < a.count\
    \ ? i : a.count-1;\n#else\n        int i = 0;\n        for (int step = FANOUT/2;\
    \ step; step >>= 1) i += step*int(a.sizes[i+step-1] <= k);\n        i += int(a.sizes[i]\
    \ <= k); return i < a.count ? i : a.count-1;\n#endif\n    }\n    static std::pair<int,int>\
    \ locate_two(const Node& a,int l,int r){\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n\
    \        auto kl=_mm256_set1_epi32(l),kr=_mm256_set1_epi32(r);u64 bl=0,br=0;\n\
    \        for(int j=0;j<FANOUT;j+=8){auto v=_mm256_load_si256(reinterpret_cast<const\
    \ __m256i*>(a.sizes+j));bl|=u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(v,kl)))))<<j;br|=u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(v,kr)))))<<j;}\n\
    #if defined(__GNUC__) || defined(__clang__)\n        int i=bl?__builtin_ctzll(bl):FANOUT,j=br?__builtin_ctzll(br):FANOUT;\n\
    #else\n        int i=bl?pc((bl&-bl)-1):FANOUT,j=br?pc((br&-br)-1):FANOUT;\n#endif\n\
    \        return {std::min(i,a.count-1),std::min(j,a.count-1)};\n#else\n      \
    \  return {locate(a,l),locate(a,r)};\n#endif\n    }\n    static void add_leaf_ones(Leaf&\
    \ a,int first,int delta){\n        int i=first;\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\
    \ && EEZ_BV_LEAF_SIMD\n        i=first&~15;\n        for(;i+16<=GROUPS;i+=16){\n\
    \            auto ix=_mm256_setr_epi16(i,i+1,i+2,i+3,i+4,i+5,i+6,i+7,i+8,i+9,i+10,i+11,i+12,i+13,i+14,i+15);\n\
    \            auto m=_mm256_cmpgt_epi16(ix,_mm256_set1_epi16(first-1));\n     \
    \       auto q=reinterpret_cast<__m256i*>(a.ones+i);_mm256_storeu_si256(q,_mm256_add_epi16(_mm256_loadu_si256(q),_mm256_and_si256(m,_mm256_set1_epi16(delta))));\n\
    \        }\n        i=std::max(i,first);\n#endif\n        for(;i<GROUPS;++i)a.ones[i]=std::uint16_t(a.ones[i]+delta);\n\
    \    }\n    static int leaf_rank_flat(const Leaf& a, int k) {\n        int j =\
    \ k >> 6, bits = k & 63, group = j/GROUP;\n        int sum = group ? a.ones[group-1]\
    \ : 0;\n        for (int w = group*GROUP; w < j; ++w) sum += pc(a.words[w]);\n\
    \        return sum + (bits ? pc(a.words[j] & mask(bits)) : 0);\n    }\n    static\
    \ bool leaf_access(const Leaf& a,int k) {\n#if EEZ_BV_BUFFER > 0\n        for\
    \ (int i = a.edit_count-1; i >= 0; --i) {\n            unsigned e = a.edits[i];\
    \ int pos = e & 65535, op = (e >> 18) & 3;\n            if (op == 0) { if (k ==\
    \ pos) return (e >> 16) & 1; k -= k > pos; }\n            else if (op == 1) k\
    \ += k >= pos;\n            else if (k == pos) return (e >> 16) & 1;\n       \
    \ }\n#endif\n        return (a.words[k >> 6] >> (k & 63)) & 1;\n    }\n    static\
    \ int leaf_rank(const Leaf& a,int k) {\n        int delta = 0;\n#if EEZ_BV_BUFFER\
    \ > 0\n        for (int i = a.edit_count-1; i >= 0; --i) {\n            unsigned\
    \ e = a.edits[i]; int pos = e & 65535, op = (e >> 18) & 3;\n            if (pos\
    \ < k) {\n                if (op == 0) { --k; delta += (e >> 16) & 1; }\n    \
    \            else if (op == 1) { ++k; delta -= (e >> 17) & 1; }\n            \
    \    else delta += int((e >> 16) & 1)-int((e >> 17) & 1);\n            }\n   \
    \     }\n#endif\n        return delta+leaf_rank_flat(a,k);\n    }\n    static\
    \ int leaf_total(const Leaf& a) {\n        int total = a.ones[GROUPS-1];\n#if\
    \ EEZ_BV_BUFFER > 0\n        for (int i = 0; i < a.edit_count; ++i) {\n      \
    \      unsigned e = a.edits[i]; total += int((e >> 16) & 1)-int((e >> 17) & 1);\n\
    \        }\n#endif\n        return total;\n    }\n    static std::pair<int,int>\
    \ leaf_pair(const Leaf& a,int l,int r) {\n        int dl = 0, dr = 0;\n#if EEZ_BV_BUFFER\
    \ > 0\n        for (int i = a.edit_count-1; i >= 0; --i) {\n            unsigned\
    \ e = a.edits[i]; int p = e & 65535, op = (e >> 18) & 3;\n            int left\
    \ = p < l, right = p < r;\n            int old = (e >> 17) & 1, bit = (e >> 16)\
    \ & 1;\n            if (op == 0) { dl += left*bit; dr += right*bit; l -= left;\
    \ r -= right; }\n            else if (op == 1) { dl -= left*old; dr -= right*old;\
    \ l += left; r += right; }\n            else { dl += left*(bit-old); dr += right*(bit-old);\
    \ }\n        }\n#endif\n        int first = leaf_rank_flat(a,l), last;\n     \
    \   if (l == r) last = first;\n        else if ((l >> 6) == ((r-1) >> 6)) last\
    \ = first+pc((a.words[l >> 6] >> (l & 63)) & mask(r-l));\n        else last =\
    \ leaf_rank_flat(a,r);\n        return {first+dl,last+dr};\n    }\n    static\
    \ std::pair<bool,int> leaf_access_rank(const Leaf& a,int k) {\n#if EEZ_BV_BUFFER\
    \ > 0\n        auto q = leaf_pair(a,k,k+1); return {q.first != q.second,q.first};\n\
    #else\n        return {leaf_access(a,k),leaf_rank_flat(a,k)};\n#endif\n    }\n\
    #if EEZ_BV_BUFFER > 0\n    // Materialize the journal by copying unchanged bit\
    \ ranges once, rather\n    // than shifting the whole leaf separately for every\
    \ buffered operation.\n    static void flush_leaf(Leaf& a) {\n        if (!a.edit_count)\
    \ return;\n        struct Run { int start, length; } runs[2*EEZ_BV_BUFFER+3];\n\
    \        int base_count = a.count;\n        for (int i = 0; i < a.edit_count;\
    \ ++i) { int op = (a.edits[i] >> 18) & 3; base_count += int(op == 1)-int(op ==\
    \ 0); }\n        int nr = 1; runs[0] = {0,base_count};\n        auto erase_at\
    \ = [&](int position) {\n            int r = 0, offset = position;\n         \
    \   while (r < nr && offset >= runs[r].length) { offset -= runs[r].length; ++r;\
    \ }\n            assert(r < nr);\n            Run old = runs[r];\n           \
    \ if (old.length == 1) { std::memmove(runs+r,runs+r+1,std::size_t(nr-r-1)*sizeof(Run));\
    \ --nr; }\n            else if (!offset) { if (old.start >= 0) ++runs[r].start;\
    \ --runs[r].length; }\n            else if (offset+1 == old.length) --runs[r].length;\n\
    \            else {\n                std::memmove(runs+r+2,runs+r+1,std::size_t(nr-r-1)*sizeof(Run));\
    \ ++nr;\n                runs[r] = {old.start,offset};\n                runs[r+1]\
    \ = {old.start+offset+1,old.length-offset-1};\n            }\n        };\n   \
    \     auto insert_at = [&](int position,bool bit) {\n            int r = 0, offset\
    \ = position;\n            while (r < nr && offset >= runs[r].length) { offset\
    \ -= runs[r].length; ++r; }\n            if (r == nr || !offset) {\n         \
    \       std::memmove(runs+r+1,runs+r,std::size_t(nr-r)*sizeof(Run));\n       \
    \         runs[r] = {bit ? -2 : -1,1}; ++nr;\n            } else {\n         \
    \       Run old = runs[r];\n                std::memmove(runs+r+3,runs+r+1,std::size_t(nr-r-1)*sizeof(Run));\
    \ nr += 2;\n                runs[r] = {old.start,offset}; runs[r+1] = {bit ? -2\
    \ : -1,1};\n                runs[r+2] = {old.start+offset,old.length-offset};\n\
    \            }\n        };\n        for (int i = 0; i < a.edit_count; ++i) {\n\
    \            unsigned e = a.edits[i]; int pos = e & 65535, op = (e >> 18) & 3;\n\
    \            if (op != 0) erase_at(pos);\n            if (op != 1) insert_at(pos,(e\
    \ >> 16) & 1);\n        }\n        u64 out[WORDS]{}; int dst = 0;\n        for\
    \ (int i = 0; i < nr; ++i) {\n            auto run = runs[i];\n            if\
    \ (run.start < 0) { if (run.start == -2) out[dst >> 6] |= u64(1) << (dst & 63);\
    \ dst += run.length; continue; }\n            int src = run.start, remaining =\
    \ run.length;\n            while (remaining) {\n                int count = std::min(64-(dst\
    \ & 63),remaining), word = src >> 6, shift = src & 63;\n                u64 bits\
    \ = a.words[word] >> shift;\n                if (shift && word+1 < WORDS) bits\
    \ |= a.words[word+1] << (64-shift);\n                out[dst >> 6] |= (bits &\
    \ mask(count)) << (dst & 63);\n                dst += count; src += count; remaining\
    \ -= count;\n            }\n        }\n        assert(dst == a.count);\n     \
    \   std::memcpy(a.words,out,sizeof(out)); a.edit_count = 0; leaf_counts(a);\n\
    \    }\n    static void record_edit(Leaf& a,int pos,int op,bool old,bool bit)\
    \ {\n        a.edits[a.edit_count++] = unsigned(pos) | (unsigned(bit) << 16) |\
    \ (unsigned(old) << 17) | (unsigned(op) << 18);\n        if (a.edit_count == EEZ_BV_BUFFER)\
    \ flush_leaf(a);\n    }\n#endif\n#if EEZ_BV_RANGE_UPDATE\n    static void adjust_range(Node&\
    \ a,int first,int last,int ds,int d1) {\n        if(first==last || (!ds&&!d1))return;\n\
    #if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n        alignas(32) static constexpr\
    \ auto masks=[](){\n            std::array<std::array<int,8>,9> out{};\n     \
    \       for(int i=0;i<=8;++i)for(int j=i;j<8;++j)out[i][j]=-1;\n            return\
    \ out;\n        }();\n        for(int j=first&~7;j<last;j+=8){\n            int\
    \ lo=std::max(0,first-j),hi=std::min(8,last-j);\n            auto m=_mm256_xor_si256(_mm256_load_si256(reinterpret_cast<const\
    \ __m256i*>(masks[lo].data())),_mm256_load_si256(reinterpret_cast<const __m256i*>(masks[hi].data())));\n\
    \            if(ds){auto q=reinterpret_cast<__m256i*>(a.sizes+j);_mm256_store_si256(q,_mm256_add_epi32(_mm256_load_si256(q),_mm256_and_si256(m,_mm256_set1_epi32(ds))));}\n\
    \            if(d1){auto q=reinterpret_cast<__m256i*>(a.ones+j);_mm256_store_si256(q,_mm256_add_epi32(_mm256_load_si256(q),_mm256_and_si256(m,_mm256_set1_epi32(d1))));}\n\
    \        }\n#else\n        for(int j=first;j<last;++j){a.sizes[j]+=ds;a.ones[j]+=d1;}\n\
    #endif\n    }\n    static void adjust(Node& a,int first,int ds,int d1){adjust_range(a,first,a.count,ds,d1);}\n\
    #else\n    static void adjust(Node& a, int first, int ds, int d1) {\n#if defined(__AVX2__)\
    \ && !defined(EEZ_NO_SIMD)\n        const auto begin = _mm256_set1_epi32(first-1),\
    \ end = _mm256_set1_epi32(a.count);\n        for (int j = first & ~7; j < a.count;\
    \ j += 8) {\n            auto ix = _mm256_setr_epi32(j,j+1,j+2,j+3,j+4,j+5,j+6,j+7);\n\
    \            auto m = _mm256_and_si256(_mm256_cmpgt_epi32(ix,begin), _mm256_cmpgt_epi32(end,ix));\n\
    \            auto s = reinterpret_cast<__m256i*>(a.sizes+j);\n            auto\
    \ o = reinterpret_cast<__m256i*>(a.ones+j);\n            if (ds) _mm256_store_si256(s,_mm256_add_epi32(_mm256_load_si256(s),_mm256_and_si256(m,_mm256_set1_epi32(ds))));\n\
    \            if (d1) _mm256_store_si256(o,_mm256_add_epi32(_mm256_load_si256(o),_mm256_and_si256(m,_mm256_set1_epi32(d1))));\n\
    \        }\n#else\n        for (int i = first; i < a.count; ++i) { a.sizes[i]\
    \ += ds; a.ones[i] += d1; }\n#endif\n    }\n#endif\n    static void insert_plain(Leaf&\
    \ a,int k,bool bit) {\n        int j = k >> 6, b = k & 63, limit = (a.count+64)/64;\n\
    \        int total = a.ones[GROUPS-1]+int(bit);\n        u64 w = a.words[j];\n\
    \        a.words[j] = (w & mask(b)) | ((w & ~mask(b)) << 1) | (u64(bit) << b);\n\
    \        u64 carry = w >> 63;\n        if ((j+1)%GROUP == 0) a.ones[j/GROUP] =\
    \ std::uint16_t(a.ones[j/GROUP]+int(bit)-int(carry));\n        int i = j+1;\n\
    #if defined(__AVX2__) && !defined(EEZ_NO_SIMD) && EEZ_BV_SIMD_SHIFT\n        if\
    \ constexpr(GROUP == 4) {\n            for (; i < limit && (i & 3); ++i) {\n \
    \               u64 old = a.words[i]; a.words[i] = (old << 1) | carry; carry =\
    \ old >> 63;\n                if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(bit)-int(carry));\n\
    \            }\n            for (; i+4 <= limit; i += 4) {\n                auto\
    \ p = reinterpret_cast<__m256i*>(a.words+i);\n                auto old = _mm256_load_si256(p);\n\
    \                auto incoming = _mm256_srli_epi64(_mm256_permute4x64_epi64(old,0x90),63);\n\
    \                incoming = _mm256_blend_epi32(incoming,_mm256_set_epi64x(0,0,0,carry),0x03);\n\
    \                u64 next = a.words[i+3] >> 63;\n                _mm256_store_si256(p,_mm256_or_si256(_mm256_slli_epi64(old,1),incoming));\n\
    \                carry = next; a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(bit)-int(carry));\n\
    \            }\n        }\n#endif\n        for (; i < limit; ++i) {\n        \
    \    u64 old = a.words[i]; a.words[i] = (old << 1) | carry; carry = old >> 63;\n\
    \            if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(bit)-int(carry));\n\
    \        }\n        for (int g = limit/GROUP; g < GROUPS; ++g) a.ones[g] = std::uint16_t(total);\n\
    \        ++a.count;\n    }\n    static void erase_plain(Leaf& a,int k,bool bit)\
    \ {\n        int j = k >> 6, b = k & 63, limit = (a.count+63)/64;\n        int\
    \ total = a.ones[GROUPS-1]-int(bit);\n        u64 w = a.words[j], incoming = j+1\
    \ < limit ? a.words[j+1]&1 : 0;\n        a.words[j] = (w & mask(b)) | (b == 63\
    \ ? 0 : (w >> (b+1)) << b) | (incoming << 63);\n        if ((j+1)%GROUP == 0)\
    \ a.ones[j/GROUP] = std::uint16_t(a.ones[j/GROUP]+int(incoming)-int(bit));\n \
    \       int i = j+1;\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD) && EEZ_BV_SIMD_SHIFT\n\
    \        if constexpr(GROUP == 4) {\n            for (; i < limit && (i & 3);\
    \ ++i) {\n                u64 next = i+1 < limit ? a.words[i+1]&1 : 0;\n     \
    \           a.words[i] = (a.words[i] >> 1) | (next << 63);\n                if\
    \ ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(next)-int(bit));\n\
    \            }\n            for (; i+4 <= limit; i += 4) {\n                auto\
    \ p = reinterpret_cast<__m256i*>(a.words+i);\n                auto old = _mm256_load_si256(p);\
    \ u64 next = i+4 < limit ? a.words[i+4]&1 : 0;\n                auto carries =\
    \ _mm256_slli_epi64(_mm256_permute4x64_epi64(old,0xf9),63);\n                carries\
    \ = _mm256_blend_epi32(carries,_mm256_set_epi64x(next << 63,0,0,0),0xc0);\n  \
    \              _mm256_store_si256(p,_mm256_or_si256(_mm256_srli_epi64(old,1),carries));\n\
    \                a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(next)-int(bit));\n\
    \            }\n        }\n#endif\n        for (; i < limit; ++i) {\n        \
    \    u64 next = i+1 < limit ? a.words[i+1]&1 : 0;\n            a.words[i] = (a.words[i]\
    \ >> 1) | (next << 63);\n            if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(next)-int(bit));\n\
    \        }\n        for (int g = limit/GROUP; g < GROUPS; ++g) a.ones[g] = std::uint16_t(total);\n\
    \        --a.count;\n    }\n    std::pair<bool,int> replace(int p,int k,bool bit)\
    \ {\n        if (p < 0) {\n            auto& a = leaves[~p];\n#if EEZ_BV_BUFFER\
    \ > 0\n            auto q = leaf_access_rank(a,k); bool old = q.first; int rank\
    \ = q.second;\n            if (old != bit) record_edit(a,k,2,old,bit);\n     \
    \       return {old,rank};\n#else\n            int j = k >> 6;\n            bool\
    \ old = (a.words[j] >> (k&63)) & 1; int rank = leaf_rank(a,k);\n            if\
    \ (old != bit) {\n                a.words[j] ^= u64(1) << (k&63);\n          \
    \      add_leaf_ones(a,j/GROUP,int(bit)-int(old));\n            }\n          \
    \  return {old,rank};\n#endif\n        }\n        auto& a = pool[p]; int i = locate(a,k),\
    \ o = i ? a.ones[i-1] : 0;\n        auto result = replace(a.child[i],k-(i ? a.sizes[i-1]\
    \ : 0),bit);\n        if (result.first != bit) adjust(a,i,0,int(bit)-int(result.first));\n\
    \        result.second += o; return result;\n    }\n    // Returns whether a split\
    \ occurred; sibling handles may be negative.\n    bool ins(int p,int k,bool bit,int&\
    \ rank,int& sibling) {\n        if (p < 0) {\n            auto& a = leaves[~p];\n\
    #if EEZ_BV_BUFFER > 0\n            if (a.count == CAP) flush_leaf(a);\n      \
    \      else if (a.edit_count || !((!bit && !a.ones[GROUPS-1]) || (bit && a.ones[GROUPS-1]\
    \ == a.count))) {\n                rank += leaf_rank(a,k); ++a.count; record_edit(a,k,0,false,bit);\
    \ return false;\n            }\n#endif\n            if (a.count < CAP) {\n   \
    \             if (!bit && !a.ones[GROUPS-1]) { ++a.count; return false; }\n  \
    \              if (bit && a.ones[GROUPS-1] == a.count) {\n                   \
    \ rank += k; int j = a.count >> 6;\n                    a.words[j] |= u64(1) <<\
    \ (a.count & 63);\n                    add_leaf_ones(a,j/GROUP,1);\n         \
    \           ++a.count; return false;\n                }\n            }\n     \
    \       rank += leaf_rank(a,k);\n            int len = a.count, j = k >> 6, b\
    \ = k & 63; u64 carry = u64(bit);\n            if (len < CAP) { insert_plain(a,k,bit);\
    \ return false; }\n            u64 w[WORDS+1]{};\n            for (int i = 0;\
    \ i < WORDS; ++i) w[i] = a.words[i];\n            for (int i = j; i < WORDS+1;\
    \ ++i) {\n                u64 old = w[i];\n                if (i == j) w[i] =\
    \ (old & mask(b)) | ((old & ~mask(b)) << 1) | (carry << b);\n                else\
    \ w[i] = (old << 1) | carry;\n                carry = old >> 63;\n           \
    \ }\n            int q = alloc_leaf(); // May move the leaf arena: reacquire both\
    \ leaves.\n            auto& left = leaves[~p]; auto& right = leaves[~q];\n  \
    \          for (int i = 0; i < WORDS; ++i) { left.words[i] = i < HALF ? w[i] :\
    \ 0; right.words[i] = i < WORDS-HALF+1 ? w[i+HALF] : 0; }\n            left.count\
    \ = HALF*64; right.count = CAP+1-HALF*64;\n#if EEZ_BV_BUFFER > 0\n           \
    \ left.edit_count = right.edit_count = 0;\n#endif\n            leaf_counts(left);\
    \ leaf_counts(right);\n            sibling = q; return true;\n        }\n    \
    \    int i = locate(pool[p],k), offset = i ? pool[p].sizes[i-1] : 0;\n       \
    \ rank += i ? pool[p].ones[i-1] : 0;\n        int q;\n        if (!ins(pool[p].child[i],k-offset,bit,rank,q))\
    \ { adjust(pool[p],i,1,int(bit)); return false; }\n        int children[FANOUT+1],\
    \ count = pool[p].count;\n        for (int j = 0; j <= i; ++j) children[j] = pool[p].child[j];\n\
    \        children[i+1] = q;\n        for (int j = i+1; j < count; ++j) children[j+1]\
    \ = pool[p].child[j];\n        if (count < FANOUT) {\n            pool[p].count\
    \ = count+1;\n            for (int j = 0; j <= count; ++j) pool[p].child[j] =\
    \ children[j];\n            inner_counts(p); return false;\n        }\n      \
    \  sibling = alloc_node(); pool[p].count = FANOUT/2; pool[sibling].count = FANOUT/2+1;\n\
    \        for (int j = 0; j < FANOUT/2; ++j) pool[p].child[j] = children[j];\n\
    \        for (int j = 0; j < FANOUT/2+1; ++j) pool[sibling].child[j] = children[j+FANOUT/2];\n\
    \        inner_counts(p); inner_counts(sibling); return true;\n    }\n    // Empty\
    \ nodes are removed by their parent, keeping live leaves at equal depth.\n   \
    \ bool del(int p,int k,int& rank,bool& bit) {\n        if (p < 0) {\n        \
    \    auto& a = leaves[~p];\n#if EEZ_BV_BUFFER > 0\n            if (a.edit_count\
    \ || (a.ones[GROUPS-1] && a.ones[GROUPS-1] != a.count)) {\n                auto\
    \ q = leaf_access_rank(a,k); rank += q.second; bit = q.first; --a.count; record_edit(a,k,1,bit,false);\
    \ return a.count == 0;\n            }\n#endif\n            if (!a.ones[GROUPS-1])\
    \ { bit = false; return --a.count == 0; }\n            if (a.ones[GROUPS-1] ==\
    \ a.count) {\n                bit = true; rank += k; --a.count; int j = a.count\
    \ >> 6;\n                a.words[j] &= ~(u64(1) << (a.count & 63));\n        \
    \        add_leaf_ones(a,j/GROUP,-1);\n                return a.count == 0;\n\
    \            }\n            rank += leaf_rank(a,k); bit = (a.words[k >> 6] >>\
    \ (k & 63)) & 1;\n            erase_plain(a,k,bit); return a.count == 0;\n   \
    \     }\n        auto& a = pool[p]; int i = locate(a,k); rank += i ? a.ones[i-1]\
    \ : 0;\n        if (!del(a.child[i],k-(i ? a.sizes[i-1] : 0),rank,bit)) { adjust(a,i,-1,-int(bit));\
    \ return false; }\n        release(a.child[i]);\n        for (int j = i+1; j <\
    \ a.count; ++j) a.child[j-1] = a.child[j];\n        if (--a.count == 0) return\
    \ true;\n        inner_counts(p); return false;\n    }\n    struct PathStep {\
    \ int node, child; };\n    // Insert an already-split right sibling into an inner\
    \ node.\n    bool attach(int p,int i,int& sibling) {\n        int count = pool[p].count;\n\
    \        if (count < FANOUT) {\n            auto& a = pool[p];\n            std::memmove(a.child+i+2,a.child+i+1,std::size_t(count-i-1)*sizeof(int));\n\
    \            a.child[i+1] = sibling; ++a.count; inner_counts(p); return false;\n\
    \        }\n        int children[FANOUT+1];\n        std::memcpy(children,pool[p].child,std::size_t(i+1)*sizeof(int));\n\
    \        children[i+1] = sibling;\n        std::memcpy(children+i+2,pool[p].child+i+1,std::size_t(count-i-1)*sizeof(int));\n\
    \        sibling = alloc_node(); pool[p].count = FANOUT/2; pool[sibling].count\
    \ = FANOUT/2+1;\n        std::memcpy(pool[p].child,children,std::size_t(FANOUT/2)*sizeof(int));\n\
    \        std::memcpy(pool[sibling].child,children+FANOUT/2,std::size_t(FANOUT/2+1)*sizeof(int));\n\
    \        inner_counts(p); inner_counts(sibling); return true;\n    }\n#if EEZ_BV_RANGE_UPDATE\n\
    \    static void adjust_move(Node& a,int from,int to,bool old,bool bit){\n   \
    \     if(from<to){adjust_range(a,from,to,-1,-int(old));adjust_range(a,to,a.count,0,int(bit)-int(old));}\n\
    \        else if(to<from){adjust_range(a,to,from,1,int(bit));adjust_range(a,from,a.count,0,int(bit)-int(old));}\n\
    \        else adjust_range(a,from,a.count,0,int(bit)-int(old));\n    }\n#else\n\
    \    static void adjust_move(Node& a,int from,int to,bool old,bool bit) {\n#if\
    \ defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n        const auto f = _mm256_set1_epi32(from-1),\
    \ t = _mm256_set1_epi32(to-1);\n        const auto end = _mm256_set1_epi32(a.count);\n\
    \        for (int j = std::min(from,to) & ~7; j < a.count; j += 8) {\n       \
    \     auto ix = _mm256_setr_epi32(j,j+1,j+2,j+3,j+4,j+5,j+6,j+7);\n          \
    \  auto live = _mm256_cmpgt_epi32(end,ix);\n            auto fm = _mm256_and_si256(live,_mm256_cmpgt_epi32(ix,f));\n\
    \            auto tm = _mm256_and_si256(live,_mm256_cmpgt_epi32(ix,t));\n    \
    \        if (from != to) {\n                auto p = reinterpret_cast<__m256i*>(a.sizes+j);\n\
    \                _mm256_store_si256(p,_mm256_add_epi32(_mm256_load_si256(p),_mm256_sub_epi32(fm,tm)));\n\
    \            }\n            if (old || bit) {\n                auto p = reinterpret_cast<__m256i*>(a.ones+j);\n\
    \                auto d = _mm256_sub_epi32(_mm256_and_si256(fm,_mm256_set1_epi32(-int(old))),_mm256_and_si256(tm,_mm256_set1_epi32(-int(bit))));\n\
    \                _mm256_store_si256(p,_mm256_add_epi32(_mm256_load_si256(p),d));\n\
    \            }\n        }\n#else\n        for (int j = std::min(from,to); j <\
    \ a.count; ++j) {\n            a.sizes[j] += int(j >= to)-int(j >= from);\n  \
    \          a.ones[j] += int(bit && j >= to)-int(old && j >= from);\n        }\n\
    #endif\n    }\n#endif\npublic:\n    DynamicBitVector() = default;\n    DynamicBitVector(const\
    \ DynamicBitVector& other)\n        : leaves(other.leaves),pool(other.pool),root(other.root),free_leaf(other.free_leaf),free_node(other.free_node),length(other.length),total_ones(other.total_ones)\
    \ {\n#if EEZ_BV_STATIC_INDEX\n        if (other.read_index) {\n            std::size_t\
    \ count = (std::size_t(length)+63)/64;\n            read_index.reset(new StaticWord[count]);\
    \ std::memcpy(read_index.get(),other.read_index.get(),count*sizeof(StaticWord));\n\
    \        }\n#endif\n    }\n    DynamicBitVector& operator=(const DynamicBitVector&\
    \ b) { if (this != &b) { DynamicBitVector tmp(b); swap(tmp); } return *this; }\n\
    \    DynamicBitVector(DynamicBitVector&& b) noexcept { swap(b); }\n    DynamicBitVector&\
    \ operator=(DynamicBitVector&& b) noexcept { if (this != &b) { DynamicBitVector\
    \ tmp(std::move(b)); swap(tmp); } return *this; }\n    void swap(DynamicBitVector&\
    \ b) noexcept {\n#if EEZ_BV_STATIC_INDEX\n        read_index.swap(b.read_index);\n\
    #endif\n        using std::swap; swap(leaves,b.leaves); swap(pool,b.pool); swap(root,b.root);\n\
    \        swap(free_leaf,b.free_leaf); swap(free_node,b.free_node); swap(length,b.length);\
    \ swap(total_ones,b.total_ones);\n    }\n    int size() const { return length;\
    \ }\n    bool empty() const { return length == 0; }\n    void clear() {\n#if EEZ_BV_STATIC_INDEX\n\
    \        read_index.reset();\n#endif\n        leaves.clear(); pool.clear(); root\
    \ = -1; free_leaf = free_node = -1; length = total_ones = 0; }\n    void reserve(int\
    \ n) { assert(n >= 0); leaves.reserve(std::size_t(n)/(HALF*64)+2); pool.reserve(std::size_t(n)/(HALF*64)/(FANOUT/2)+16);\
    \ }\n    std::vector<u64> export_packed(){\n        std::vector<u64> out((std::size_t(length)+63)/64,total_ones==length?~u64(0):0);\n\
    \        if(total_ones && total_ones!=length){int offset=0;collect_packed(root,out.data(),offset);assert(offset==length);}\n\
    \        if((length&63)&&!out.empty())out.back()&=mask(length&63);\n        return\
    \ out;\n    }\n    void discard_query_index(){\n#if EEZ_BV_STATIC_INDEX\n    \
    \    read_index.reset();\n#endif\n    }\n    // Packed input is little-endian\
    \ within each word; padding bits are ignored.\n    void build_packed(const std::vector<u64>&\
    \ words,int n){build_packed(words,n,true);}\n    void build_packed(const std::vector<u64>&\
    \ words,int n,bool use_index) {\n        assert(n >= 0 && words.size() >= (std::size_t(n)+63)/64);\n\
    \        clear(); reserve(n); if (!n) return;\n        std::vector<int> layer;\
    \ layer.reserve((std::size_t(n)+BUILD_BITS-1)/BUILD_BITS);\n        for (int pos\
    \ = 0; pos < n;) {\n            int p = alloc_leaf(), len = std::min(BUILD_BITS,n-pos);\
    \ auto& leaf = leaves[~p];\n            leaf.count = len; int count = (len+63)/64;\n\
    \            for (int j = 0; j < count; ++j) leaf.words[j] = words[std::size_t(pos)/64+j];\n\
    \            if (len & 63) leaf.words[count-1] &= mask(len & 63);\n          \
    \  leaf_counts(leaf); layer.push_back(p); pos += len;\n        }\n        while\
    \ (layer.size() > 1) {\n            std::vector<int> next; next.reserve((layer.size()+FANOUT-1)/FANOUT);\n\
    \            for (std::size_t i = 0; i < layer.size();) {\n                int\
    \ p = alloc_node(), count = int(std::min<std::size_t>(FANOUT,layer.size()-i));\
    \ pool[p].count = count;\n                for (int j = 0; j < count; ++j) pool[p].child[j]\
    \ = layer[i+j];\n                inner_counts(p); next.push_back(p); i += count;\n\
    \            }\n            layer.swap(next);\n        }\n        root = layer[0];\
    \ length = n; total_ones = ones_of(root);\n#if EEZ_BV_STATIC_INDEX\n        if(use_index)index_packed(words.data());\n\
    #endif\n    }\n    void build(const std::vector<std::uint8_t>& a){build(a,true);}\n\
    \    void build(const std::vector<std::uint8_t>& a,bool use_index) {\n       \
    \ assert(a.size() <= std::size_t(INT_MAX));\n        clear(); reserve(int(a.size()));\
    \ if (a.empty()) return;\n        std::vector<int> layer; layer.reserve((a.size()+BUILD_BITS-1)/BUILD_BITS);\n\
    \        for (std::size_t pos = 0; pos < a.size();) {\n            int p = alloc_leaf(),\
    \ len = int(std::min<std::size_t>(BUILD_BITS,a.size()-pos)); auto& leaf = leaves[~p];\n\
    \            leaf.count = len; int j = 0;\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n\
    \            for (; j+64 <= len; j += 64) {\n                const auto zero =\
    \ _mm256_setzero_si256();\n                auto lo = _mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(a.data()+pos+j));\n                auto hi = _mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(a.data()+pos+j+32));\n                auto l = std::uint32_t(~_mm256_movemask_epi8(_mm256_cmpeq_epi8(lo,zero)));\n\
    \                auto h = std::uint32_t(~_mm256_movemask_epi8(_mm256_cmpeq_epi8(hi,zero)));\n\
    \                leaf.words[j>>6] = u64(l) | (u64(h)<<32);\n            }\n#endif\n\
    \            for (; j < len; ++j) leaf.words[j>>6] |= u64(a[pos+j] != 0) << (j&63);\n\
    \            leaf_counts(leaf); layer.push_back(p); pos += len;\n        }\n \
    \       while (layer.size() > 1) {\n            std::vector<int> next; next.reserve((layer.size()+FANOUT-1)/FANOUT);\n\
    \            for (std::size_t i = 0; i < layer.size();) {\n                int\
    \ p = alloc_node(), count = int(std::min<std::size_t>(FANOUT,layer.size()-i));\
    \ pool[p].count = count;\n                for (int j = 0; j < count; ++j) pool[p].child[j]\
    \ = layer[i+j];\n                inner_counts(p); next.push_back(p); i += count;\n\
    \            }\n            layer.swap(next);\n        }\n        root = layer[0];\
    \ length = int(a.size()); total_ones = ones_of(root);\n#if EEZ_BV_STATIC_INDEX\n\
    \        if(use_index)prepare_queries();\n#endif\n    }\n#if defined(__GNUC__)\
    \ || defined(__clang__)\n    __attribute__((always_inline)) inline\n#elif defined(_MSC_VER)\n\
    \    __forceinline\n#endif\n    std::pair<bool,int> access_rank(int k) const {\n\
    \        assert(0 <= k && k < size());\n        if (!total_ones) return {false,0};\n\
    \        if (total_ones == length) return {true,k};\n#if EEZ_BV_STATIC_INDEX\n\
    \        if (read_index) return indexed_access_rank(k);\n#endif\n        int p\
    \ = root, rank = 0;\n        while (p >= 0) {\n            const auto& a = pool[p];\
    \ int i = locate(a,k);\n            if (i) { k -= a.sizes[i-1]; rank += a.ones[i-1];\
    \ } p = a.child[i]; prefetch(p);\n        }\n        auto q = leaf_access_rank(leaves[~p],k);\
    \ q.second += rank; return q;\n    }\n    bool access(int k) const {\n       \
    \ assert(0 <= k && k < size());\n        if (!total_ones) return false;\n    \
    \    if (total_ones == length) return true;\n#if EEZ_BV_STATIC_INDEX\n       \
    \ if (read_index) return (read_index[k >> 6].bits >> (k & 63)) & 1;\n#endif\n\
    \        int p = root;\n        while (p >= 0) { const auto& a = pool[p]; int\
    \ i = locate(a,k); if (i) k -= a.sizes[i-1]; p = a.child[i]; prefetch(p); }\n\
    \        return leaf_access(leaves[~p],k);\n    }\n    int rank1(int k) const\
    \ {\n        assert(0 <= k && k <= size()); if (!k || !total_ones) return 0;\n\
    \        if (total_ones == length) return k;\n        if (k == length) return\
    \ total_ones;\n#if EEZ_BV_STATIC_INDEX\n        if (read_index) return indexed_rank(k);\n\
    #endif\n        int p = root, rank = 0;\n        while (p >= 0) { const auto&\
    \ a = pool[p]; int i = locate(a,k); if (i) { k -= a.sizes[i-1]; rank += a.ones[i-1];\
    \ } p = a.child[i]; prefetch(p); }\n        return rank+leaf_rank(leaves[~p],k);\n\
    \    }\n    int rank0(int k) const { return k-rank1(k); }\n#if defined(__GNUC__)\
    \ || defined(__clang__)\n    __attribute__((always_inline)) inline\n#elif defined(_MSC_VER)\n\
    \    __forceinline\n#endif\n    std::pair<int,int> rank1_pair(int l,int r) const\
    \ {\n        assert(0 <= l && l <= r && r <= size());\n        if (!total_ones)\
    \ return {0,0};\n        if (total_ones == length) return {l,r};\n#if EEZ_BV_STATIC_INDEX\n\
    \        if (read_index) return {indexed_rank(l),indexed_rank(r)};\n#endif\n \
    \       if (l == r) { int o = rank1(l); return {o,o}; }\n        if (r-l == 1)\
    \ { auto q = access_rank(l); return {q.second,q.second+int(q.first)}; }\n    \
    \    if (!l) return {0,rank1(r)};\n        if (r == length) return {rank1(l),total_ones};\n\
    \        int p = root, base = 0;\n        while (p >= 0) {\n            const\
    \ auto& a = pool[p]; int i = locate(a,l), j = r <= a.sizes[i] ? i : locate(a,r);\n\
    \            if (i != j) {\n                int ol = i ? a.ones[i-1] : 0, orr\
    \ = j ? a.ones[j-1] : 0;\n                l -= i ? a.sizes[i-1] : 0; r -= j ?\
    \ a.sizes[j-1] : 0;\n                int left = a.child[i], right = a.child[j];\n\
    \                while (left >= 0) {\n                    const auto& al = pool[left];\
    \ const auto& ar = pool[right];\n                    int il = locate(al,l), ir\
    \ = locate(ar,r);\n                    if (il) { l -= al.sizes[il-1]; ol += al.ones[il-1];\
    \ }\n                    if (ir) { r -= ar.sizes[ir-1]; orr += ar.ones[ir-1];\
    \ }\n                    left = al.child[il]; right = ar.child[ir]; prefetch(left);\
    \ prefetch(right);\n                }\n                return {base+ol+leaf_rank(leaves[~left],l),base+orr+leaf_rank(leaves[~right],r)};\n\
    \            }\n            if (i) { l -= a.sizes[i-1]; r -= a.sizes[i-1]; base\
    \ += a.ones[i-1]; }\n            p = a.child[i]; prefetch(p);\n        }\n   \
    \     auto q = leaf_pair(leaves[~p],l,r); q.first += base; q.second += base; return\
    \ q;\n    }\n    int count1(int l,int r) const { auto q = rank1_pair(l,r); return\
    \ q.second-q.first; }\n    std::pair<bool,int> set_rank(int k,bool bit) {\n  \
    \      assert(0 <= k && k < size());\n        if (!total_ones && !bit) return\
    \ {false,0};\n        if (total_ones == length && bit) return {true,k};\n#if EEZ_BV_STATIC_INDEX\n\
    \        if (read_index) { auto q = indexed_access_rank(k); if (q.first == bit)\
    \ return q; read_index.reset(); }\n#endif\n        PathStep path[16]; int depth\
    \ = 0, p = root, local = k, rank = 0;\n        while (p >= 0 && depth < 16) {\n\
    \            const auto& a = pool[p]; int i = locate(a,local);\n            path[depth++]\
    \ = {p,i};\n            if (i) { local -= a.sizes[i-1]; rank += a.ones[i-1]; }\n\
    \            p = a.child[i]; prefetch(p);\n        }\n        std::pair<bool,int>\
    \ result;\n        if (p >= 0) result = replace(root,k,bit);\n        else {\n\
    \            result = replace(p,local,bit); result.second += rank;\n         \
    \   if (result.first != bit) for (int d = depth-1; d >= 0; --d) adjust(pool[path[d].node],path[d].child,0,int(bit)-int(result.first));\n\
    \        }\n        total_ones += int(bit)-int(result.first); return result;\n\
    \    }\n    bool set(int k,bool bit) { return set_rank(k,bit).first; }\n    struct\
    \ MoveRanks { bool previous; int old_rank, new_rank; };\n    // Equivalent to\
    \ erase_rank(from), then insert_rank(to,bit). 'to' is\n    // an insertion position\
    \ after erasure. Fuse searches when no split/removal\n    // is needed; fall back\
    \ to the general operations for structural changes.\n    MoveRanks move_rank(int\
    \ from,int to,bool bit) {\n        assert(0 <= from && from < length && 0 <= to\
    \ && to < length);\n        if (!total_ones && !bit) return {false,0,0};\n   \
    \     if (total_ones == length && bit) return {true,from,to};\n        if (from\
    \ == to) { auto q = set_rank(from,bit); return {q.first,q.second,q.second}; }\n\
    #if EEZ_BV_STATIC_INDEX\n        read_index.reset();\n#endif\n        struct Step\
    \ { int node, child; } left_path[16], right_path[16];\n        int left = root,\
    \ right = root, l = from, r = to, ol = 0, orr = 0, depth = 0;\n        while (left\
    \ >= 0) {\n            if (depth == 16) { auto q = erase_rank(from); int o = insert_rank(to,bit);\
    \ return {q.first,q.second,o}; }\n            const auto& a = pool[left]; const\
    \ auto& b = pool[right];\n            int i,j;\n#if EEZ_BV_MOVE_PAIR\n       \
    \     if(left==right){auto q=locate_two(a,l,r);i=q.first;j=q.second;}\n      \
    \      else\n#endif\n            {i=locate(a,l);j=locate(b,r);}\n            left_path[depth]\
    \ = {left,i}; right_path[depth] = {right,j}; ++depth;\n            if (i) { l\
    \ -= a.sizes[i-1]; ol += a.ones[i-1]; }\n            if (j) { r -= b.sizes[j-1];\
    \ orr += b.ones[j-1]; }\n            left = a.child[i]; right = b.child[j];\n\
    \        }\n        if (left != right && (leaves[~left].count == 1 || leaves[~right].count\
    \ == CAP)) {\n            auto q = erase_rank(from); int o = insert_rank(to,bit);\
    \ return {q.first,q.second,o};\n        }\n        bool old; del(left,l,ol,old);\n\
    \        if (left != right && from < to) { ++r; orr -= int(old); }\n        int\
    \ unused; [[maybe_unused]] bool split = ins(right,r,bit,orr,unused); assert(!split);\n\
    \        for (int d = 0; d < depth; ++d) {\n            auto a = left_path[d],\
    \ b = right_path[d];\n            if (a.node == b.node) {\n                if\
    \ (a.child != b.child || old != bit) adjust_move(pool[a.node],a.child,b.child,old,bit);\n\
    \            } else {\n                adjust(pool[a.node],a.child,-1,-int(old));\n\
    \                adjust(pool[b.node],b.child,1,int(bit));\n            }\n   \
    \     }\n        total_ones += int(bit)-int(old); return {old,ol,orr};\n    }\n\
    \    template<int WIDTH> void access_rank_batch(const int* indices,bool* bits,int*\
    \ ranks) const {\n        static_assert(WIDTH > 0); int nodes[WIDTH], offsets[WIDTH];\n\
    #if EEZ_BV_STATIC_INDEX\n        if (read_index) {\n            for (int j = 0;\
    \ j < WIDTH; ++j) { assert(0 <= indices[j] && indices[j] < size()); auto q = indexed_access_rank(indices[j]);\
    \ bits[j] = q.first; ranks[j] = q.second; }\n            return;\n        }\n\
    #endif\n        if (!total_ones || total_ones == length) {\n            bool bit\
    \ = total_ones != 0;\n            for (int j = 0; j < WIDTH; ++j) { assert(0 <=\
    \ indices[j] && indices[j] < size()); bits[j] = bit; ranks[j] = bit ? indices[j]\
    \ : 0; }\n            return;\n        }\n        for (int j = 0; j < WIDTH; ++j)\
    \ { assert(0 <= indices[j] && indices[j] < size()); nodes[j] = root; offsets[j]\
    \ = indices[j]; ranks[j] = 0; }\n        while (nodes[0] >= 0) {\n           \
    \ for (int j = 0; j < WIDTH; ++j) {\n                const auto& a = pool[nodes[j]];\
    \ int i = locate(a,offsets[j]);\n                if (i) { offsets[j] -= a.sizes[i-1];\
    \ ranks[j] += a.ones[i-1]; } nodes[j] = a.child[i]; prefetch(nodes[j]);\n    \
    \        }\n        }\n        for (int j = 0; j < WIDTH; ++j) { const auto& a\
    \ = leaves[~nodes[j]]; int k = offsets[j]; auto q = leaf_access_rank(a,k); bits[j]\
    \ = q.first; ranks[j] += q.second; }\n    }\n    // Optional after an update batch.\
    \ Const queries never allocate or mutate.\n    void prepare_queries() {\n#if EEZ_BV_STATIC_INDEX\n\
    \        if (read_index || !total_ones || total_ones == length) return;\n    \
    \    std::size_t count = (std::size_t(length)+63)/64;\n        std::unique_ptr<StaticWord[]>\
    \ next(new StaticWord[count]);\n        for (std::size_t i = 0; i < count; ++i)\
    \ next[i].bits = 0;\n        int offset = 0; collect_words(root,next.get(),offset);\
    \ assert(offset == length);\n        int rank = 0;\n        for (std::size_t i\
    \ = 0; i < count; ++i) { next[i].prefix = rank; rank += pc(next[i].bits); }\n\
    \        read_index = std::move(next);\n#endif\n    }\n    int insert_rank(int\
    \ k,bool bit) {\n        assert(0 <= k && k <= size()); assert(length < INT_MAX);\n\
    #if EEZ_BV_STATIC_INDEX\n        read_index.reset();\n#endif\n        if (!length)\
    \ root = alloc_leaf();\n        PathStep path[16]; int depth = 0, p = root, local\
    \ = k, rank = 0, sibling;\n        while (p >= 0 && depth < 16) {\n          \
    \  const auto& a = pool[p]; int i = locate(a,local);\n            path[depth++]\
    \ = {p,i};\n            if (i) { local -= a.sizes[i-1]; rank += a.ones[i-1]; }\n\
    \            p = a.child[i]; prefetch(p);\n        }\n        bool split;\n  \
    \      if (p >= 0) { rank = 0; split = ins(root,k,bit,rank,sibling); }\n     \
    \   else {\n            split = ins(p,local,bit,rank,sibling);\n            for\
    \ (int d = depth-1; d >= 0; --d) {\n                if (split) split = attach(path[d].node,path[d].child,sibling);\n\
    \                else adjust(pool[path[d].node],path[d].child,1,int(bit));\n \
    \           }\n        }\n        if (split) {\n            p = alloc_node();\
    \ pool[p].count = 2;\n            pool[p].child[0] = root; pool[p].child[1] =\
    \ sibling; inner_counts(p); root = p;\n        }\n        ++length; total_ones\
    \ += int(bit); return rank;\n    }\n    void insert(int k,bool bit) { (void)insert_rank(k,bit);\
    \ }\n    std::pair<bool,int> erase_rank(int k) {\n        assert(0 <= k && k <\
    \ size());\n#if EEZ_BV_STATIC_INDEX\n        read_index.reset();\n#endif\n   \
    \     PathStep path[16]; int depth = 0, p = root, local = k, rank = 0; bool bit;\n\
    \        while (p >= 0 && depth < 16) {\n            const auto& a = pool[p];\
    \ int i = locate(a,local);\n            path[depth++] = {p,i};\n            if\
    \ (i) { local -= a.sizes[i-1]; rank += a.ones[i-1]; }\n            p = a.child[i];\
    \ prefetch(p);\n        }\n        bool empty;\n        if (p >= 0) { rank = 0;\
    \ empty = del(root,k,rank,bit); }\n        else {\n            empty = del(p,local,rank,bit);\n\
    \            for (int d = depth-1; d >= 0; --d) {\n                auto& a = pool[path[d].node];\
    \ int i = path[d].child;\n                if (!empty) adjust(a,i,-1,-int(bit));\n\
    \                else {\n                    release(a.child[i]);\n          \
    \          std::memmove(a.child+i,a.child+i+1,std::size_t(a.count-i-1)*sizeof(int));\n\
    \                    empty = --a.count == 0;\n                    if (!empty)\
    \ inner_counts(path[d].node);\n                }\n            }\n        }\n \
    \       if (empty) { release(root); root = -1; }\n        else while (root >=\
    \ 0 && pool[root].count == 1) { int old = root; root = pool[old].child[0]; release(old);\
    \ }\n        --length; total_ones -= int(bit); return {bit,rank};\n    }\n   \
    \ bool erase(int k) { return erase_rank(k).first; }\n};\n\n\n// Read-only 4-ary\
    \ wavelet matrix, invalidated before any edit.\ntemplate<class T,int LOG> class\
    \ QuadIndex {\n    using U=std::uint64_t;\n    static constexpr int LEVELS=(LOG+1)/2;\n\
    \    struct Block { U low,high; int prefix[4]; };\n    std::array<std::vector<Block>,LEVELS>\
    \ data;\n    std::array<std::array<int,4>,LEVELS> start,total;\n    int n=0;\n\
    \    std::array<int,LEVELS> uniform;\n    static U mask(int k){return k==64?~U(0):(U(1)<<k)-1;}\n\
    \    static int pc(U x){\n#if defined(__GNUC__) || defined(__clang__)\n      \
    \  return __builtin_popcountll(x);\n#else\n        int n=0;while(x){x&=x-1;++n;}return\
    \ n;\n#endif\n    }\n    static U matches(const Block& b,int c){return (c&1?b.low:~b.low)&(c&2?b.high:~b.high);}\n\
    \    int rank(int level,int c,int k)const{\n        if(uniform[level]>=0)return\
    \ uniform[level]==c?k:0;\n        if(k==n)return total[level][c];\n        const\
    \ auto& b=data[level][k>>6];return b.prefix[c]+pc(matches(b,c)&mask(k&63));\n\
    \    }\n    std::array<int,4> counts(int level,int l,int r)const{\n        std::array<int,4>\
    \ out;\n        for(int c=0;c<4;++c)out[c]=rank(level,c,r)-rank(level,c,l);\n\
    \        return out;\n    }\npublic:\n    QuadIndex(){uniform.fill(-1);}\n   \
    \ // The binary builder orders each pair's buckets 0,2,1,3.\n    void append(const\
    \ std::vector<T>& cur,int binary_level){\n        bool single=(LOG&1)&&binary_level==0;\n\
    \        if(!single && ((LOG-binary_level)&1))return;\n        n=int(cur.size());int\
    \ level=(binary_level+1)/2;uniform[level]=-1;\n        int shift=2*(LEVELS-level-1);\n\
    \        auto& blocks=data[level];blocks.resize((std::size_t(n)+63)/64);\n   \
    \     std::array<int,4> count{};\n        for(int i=0;i<n;i+=64){\n          \
    \  auto& b=blocks[i>>6];b.low=b.high=0;\n            for(int c=0;c<4;++c)b.prefix[c]=count[c];\n\
    \            int end=std::min(n,i+64),j=i;\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n\
    \            if constexpr(sizeof(T)==4){\n                auto sh=_mm256_set1_epi32(shift),one=_mm256_set1_epi32(1);\n\
    \                for(;j+8<=end;j+=8){\n                    auto v=_mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(cur.data()+j));v=_mm256_srlv_epi32(v,sh);\n                    auto\
    \ lo=_mm256_cmpeq_epi32(_mm256_and_si256(v,one),one);\n                    auto\
    \ hi=_mm256_cmpeq_epi32(_mm256_and_si256(_mm256_srli_epi32(v,1),one),one);\n \
    \                   b.low|=U(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(lo))))<<(j-i);\n\
    \                    b.high|=U(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(hi))))<<(j-i);\n\
    \                }\n            }\n#endif\n            for(;j<end;++j){int c=int((U(cur[j])>>shift)&3);b.low|=U(c&1)<<(j-i);b.high|=U(c>>1)<<(j-i);}\n\
    \            U valid=mask(end-i);int lo=pc(b.low&valid),hi=pc(b.high&valid),both=pc(b.low&b.high&valid);\n\
    \            count[0]+=end-i-lo-hi+both;count[1]+=lo-both;count[2]+=hi-both;count[3]+=both;\n\
    \        }\n        total[level]=count;\n        for(int c=0;c<4;++c)if(count[c]==n){uniform[level]=c;std::vector<Block>().swap(blocks);break;}\n\
    \        int sum=0;\n        for(int j=0;j<4;++j){int c=single?j:((j&1)<<1)|(j>>1);start[level][c]=sum;sum+=count[c];}\n\
    \    }\n    static U take(const std::vector<U>& words,int pos,int count){\n  \
    \      if(!count)return 0;\n        int shift=pos&63;U value=words[std::size_t(pos)/64]>>shift;\n\
    \        if(shift&&count>64-shift)value|=words[std::size_t(pos)/64+1]<<(64-shift);\n\
    \        return value&mask(count);\n    }\n    static U deposit(U value,U positions){\n\
    #if defined(__BMI2__) && !defined(EEZ_NO_SIMD)\n        return _pdep_u64(value,positions);\n\
    #else\n        U out=0;while(positions){U bit=positions&-positions;out|=bit*U(value&1);positions&=positions-1;value>>=1;}return\
    \ out;\n#endif\n    }\n    void append_binary(const std::vector<U>& high,const\
    \ std::vector<U>& low,int length,int binary_level,int zeros){\n        bool single=(LOG&1)&&binary_level==0;\n\
    \        n=length;int level=(binary_level+1)/2;uniform[level]=-1;\n        auto&\
    \ blocks=data[level];blocks.resize((std::size_t(n)+63)/64);\n        std::array<int,4>\
    \ count{};int zp=0,op=zeros;\n        for(int i=0;i<n;i+=64){\n            int\
    \ valid=std::min(64,n-i);auto& b=blocks[i>>6];\n            for(int c=0;c<4;++c)b.prefix[c]=count[c];\n\
    \            if(single){b.low=high[i>>6]&mask(valid);b.high=0;}\n            else{\n\
    \                b.high=high[i>>6]&mask(valid);int ones=pc(b.high),zero=valid-ones;\n\
    \                b.low=deposit(take(low,zp,zero),~b.high&mask(valid))|deposit(take(low,op,ones),b.high);\n\
    \                zp+=zero;op+=ones;\n            }\n            int lo=pc(b.low),hi=pc(b.high),both=pc(b.low&b.high);\n\
    \            count[0]+=valid-lo-hi+both;count[1]+=lo-both;count[2]+=hi-both;count[3]+=both;\n\
    \        }\n        total[level]=count;\n        for(int c=0;c<4;++c)if(count[c]==n){uniform[level]=c;std::vector<Block>().swap(blocks);break;}\n\
    \        int sum=0;for(int j=0;j<4;++j){int c=single?j:((j&1)<<1)|(j>>1);start[level][c]=sum;sum+=count[c];}\n\
    \    }\n    int less_rank(int level,int c,int k)const{\n        if(uniform[level]>=0)return\
    \ uniform[level]<c?k:0;\n        if(!c)return 0;\n        if(c==1)return rank(level,0,k);\n\
    \        if(c==3)return k-rank(level,3,k);\n        if(k==n)return total[level][0]+total[level][1];\n\
    \        const auto& b=data[level][k>>6];return b.prefix[0]+b.prefix[1]+pc(~b.high&mask(k&63));\n\
    \    }\n    T access(int k)const{\n        U value=0;\n        for(int level=0;level<LEVELS;++level){\n\
    \            if(uniform[level]>=0){value=(value<<2)|U(uniform[level]);continue;}\n\
    \            const auto& b=data[level][k>>6];int shift=k&63;\n            int\
    \ c=int((b.low>>shift)&1)|int(((b.high>>shift)&1)<<1);\n            value=(value<<2)|U(c);k=start[level][c]+b.prefix[c]+pc(matches(b,c)&mask(shift));\n\
    \        }\n        return T(value);\n    }\n    void access_many(const int* ix,std::size_t\
    \ count,T* output)const{\n        for(std::size_t base=0;base<count;base+=8){\n\
    \            int size=int(std::min<std::size_t>(8,count-base));int pos[8];U values[8]{};\n\
    \            for(int j=0;j<size;++j){assert(ix[base+j]>=0&&ix[base+j]<n);pos[j]=ix[base+j];}\n\
    \            for(int level=0;level<LEVELS;++level)for(int j=0;j<size;++j){\n \
    \               if(uniform[level]>=0){values[j]=(values[j]<<2)|U(uniform[level]);continue;}\n\
    \                int k=pos[j],shift=k&63;const auto& b=data[level][k>>6];\n  \
    \              int c=int((b.low>>shift)&1)|int(((b.high>>shift)&1)<<1);\n    \
    \            values[j]=(values[j]<<2)|U(c);pos[j]=start[level][c]+b.prefix[c]+pc(matches(b,c)&mask(shift));\n\
    \            }\n            for(int j=0;j<size;++j)output[base+j]=T(values[j]);\n\
    \        }\n    }\n    int equal(T value,int l,int r)const{\n        for(int level=0;level<LEVELS&&l<r;++level){int\
    \ c=int((U(value)>>(2*(LEVELS-level-1)))&3);l=start[level][c]+rank(level,c,l);r=start[level][c]+rank(level,c,r);}\n\
    \        return r-l;\n    }\n    std::pair<int,int> rank_less(int level,int c,int\
    \ k)const{\n        if(uniform[level]>=0)return {uniform[level]==c?k:0,uniform[level]<c?k:0};\n\
    \        if(k==n){int less=0;for(int j=0;j<c;++j)less+=total[level][j];return\
    \ {total[level][c],less};}\n        const auto& b=data[level][k>>6];U m=mask(k&63);\n\
    \        int equal=b.prefix[c]+pc(matches(b,c)&m);\n        if(c==0)return {equal,0};if(c==3)return\
    \ {equal,k-equal};\n        int less=c==1?b.prefix[0]+pc(~(b.high|b.low)&m):b.prefix[0]+b.prefix[1]+pc(~b.high&m);\n\
    \        return {equal,less};\n    }\n    T extreme(int level,int l,int r,U value,bool\
    \ maximum)const{\n        for(;level<LEVELS;++level){\n            if(uniform[level]>=0){value=(value<<2)|U(uniform[level]);continue;}\n\
    \            int c=maximum?3:0,nl=0,nr=0;\n            for(;;){nl=rank(level,c,l);nr=rank(level,c,r);if(nl<nr)break;c+=maximum?-1:1;}\n\
    \            value=(value<<2)|U(c);l=start[level][c]+nl;r=start[level][c]+nr;\n\
    \        }\n        return T(value);\n    }\n    template<bool PREV,bool NEXT>std::pair<std::optional<T>,std::optional<T>>\
    \ neighbors(T x,int l,int r)const{\n        struct Candidate{int level=-1,l=0,r=0;U\
    \ prefix=0;};Candidate prev,next;U value=0;\n        std::pair<std::optional<T>,std::optional<T>>\
    \ result;\n        for(int level=0;level<LEVELS&&l<r;++level){\n            int\
    \ c=int((U(x)>>(2*(LEVELS-level-1)))&3);\n            if constexpr(PREV)for(int\
    \ d=c-1;d>=0;--d){int a=rank(level,d,l),b=rank(level,d,r);if(a<b){prev={level+1,start[level][d]+a,start[level][d]+b,(value<<2)|U(d)};break;}}\n\
    \            if constexpr(NEXT)for(int d=c+1;d<4;++d){int a=rank(level,d,l),b=rank(level,d,r);if(a<b){next={level+1,start[level][d]+a,start[level][d]+b,(value<<2)|U(d)};break;}}\n\
    \            value=(value<<2)|U(c);l=start[level][c]+rank(level,c,l);r=start[level][c]+rank(level,c,r);\n\
    \        }\n        if constexpr(PREV)if(prev.level>=0)result.first=extreme(prev.level,prev.l,prev.r,prev.prefix,true);\n\
    \        if constexpr(NEXT){if(l<r)result.second=x;else if(next.level>=0)result.second=extreme(next.level,next.l,next.r,next.prefix,false);}\n\
    \        return result;\n    }\n    int less(T value,int l,int r)const{\n    \
    \    int answer=0;\n        for(int level=0;level<LEVELS&&l<r;++level){\n    \
    \        int c=int((U(value)>>(2*(LEVELS-level-1)))&3);\n            auto a=rank_less(level,c,l),b=rank_less(level,c,r);answer+=b.second-a.second;\n\
    \            l=start[level][c]+a.first;r=start[level][c]+b.first;\n        }\n\
    \        return answer;\n    }\n    std::pair<int,int> less_equal(T value,int\
    \ l,int r)const{\n        int answer=0;\n        for(int level=0;level<LEVELS&&l<r;++level){\n\
    \            int c=int((U(value)>>(2*(LEVELS-level-1)))&3);\n            auto\
    \ a=rank_less(level,c,l),b=rank_less(level,c,r);answer+=b.second-a.second;\n \
    \           l=start[level][c]+a.first;r=start[level][c]+b.first;\n        }\n\
    \        return {answer,r-l};\n    }\n    struct Result {T value;int less,equal;};\n\
    \    Result kth(int l,int r,int k)const{\n        U value=0;int less=0;\n    \
    \    for(int level=0;level<LEVELS;++level){\n            if(uniform[level]>=0){value=(value<<2)|U(uniform[level]);continue;}\n\
    \            int hl=less_rank(level,2,l),hr=less_rank(level,2,r),zeros=hr-hl,c;\n\
    \            int nl,nr;\n            if(k<zeros){\n                int zl=rank(level,0,l),zr=rank(level,0,r),z=zr-zl;\n\
    \                if(k<z){c=0;nl=zl;nr=zr;}else{c=1;k-=z;less+=z;nl=hl-zl;nr=hr-zr;}\n\
    \            }else{\n                k-=zeros;less+=zeros;int zl=rank(level,2,l),zr=rank(level,2,r),z=zr-zl;\n\
    \                if(k<z){c=2;nl=zl;nr=zr;}else{c=3;k-=z;less+=z;nl=l-hl-zl;nr=r-hr-zr;}\n\
    \            }\n            value=(value<<2)|U(c);l=start[level][c]+nl;r=start[level][c]+nr;\n\
    \        }\n        return {T(value),less,r-l};\n    }\n};\n\ntemplate<class T=int,int\
    \ LOG=std::numeric_limits<T>::digits>\nclass DynamicWaveletMatrix{\n    std::optional<QuadIndex<T,LOG>>\
    \ quad;\n    bool quad_valid=false;\n    void invalidate_quad(){quad_valid=false;if\
    \ constexpr(!EEZ_QUAD_RETAIN)quad.reset();}\npublic:\n    struct Counts { int\
    \ less,equal,greater; };\n    struct Quantile { T value; int less,equal; };\n\
    \    struct Neighbors { std::optional<T> previous,next; };\nprivate:\n    static_assert(std::is_integral_v<T>);\n\
    \    static_assert(!std::is_same_v<T,bool>);\n    static_assert(1<=LOG&&LOG<=64);\n\
    \n    using U=std::make_unsigned_t<T>;\n    using u64=std::uint64_t;\n\n    std::array<DynamicBitVector,LOG>\
    \ bv;\n    std::array<int,LOG> mid{};\n    int n=0;\n\n    static bool negative(T\
    \ x){\n        if constexpr(std::is_signed_v<T>)return x<0;\n        else return\
    \ false;\n    }\n    static u64 as_u64(T x){\n        return (u64)(U)x;\n    }\n\
    \    static bool valid_value(T x){\n        if(negative(x))return false;\n   \
    \     if constexpr(LOG==64)return true;\n        else return as_u64(x)<(u64(1)<<LOG);\n\
    \    }\n    static bool bit(u64 x,int level){\n        return (x>>(LOG-1-level))&1;\n\
    \    }\n    void range_check([[maybe_unused]] int l,[[maybe_unused]] int r)const{\n\
    \        assert(0<=l&&l<=r&&r<=n);\n    }\n    int less_suffix(int level,int l,int\
    \ r,u64 v,int ans)const{\n        for(;level<LOG&&l<r;++level){\n            auto\
    \ [ol,orr]=bv[level].rank1_pair(l,r);\n            if(bit(v,level)){ans+=(r-l)-(orr-ol);l=mid[level]+ol;r=mid[level]+orr;}\n\
    \            else{l-=ol;r-=orr;}\n        }\n        return ans;\n    }\n    T\
    \ extreme_suffix(int level,int l,int r,u64 value,bool maximum)const{\n       \
    \ for(;level<LOG;++level){\n            auto [ol,orr]=bv[level].rank1_pair(l,r);\n\
    \            int zeros=(r-l)-(orr-ol);\n            bool b=maximum?(orr>ol):(zeros==0);\n\
    \            if(b){value|=u64(1)<<(LOG-1-level);l=mid[level]+ol;r=mid[level]+orr;}\n\
    \            else{l-=ol;r-=orr;}\n        }\n        return T(value);\n    }\n\
    \    template<bool PREV,bool NEXT>\n    Neighbors neighbors_impl(int l,int r,T\
    \ x)const{\n        range_check(l,r);Neighbors result{};if(l==r)return result;\n\
    \        if(quad_valid){\n            if(negative(x)){if constexpr(NEXT)result.next=quad->extreme(0,l,r,0,false);return\
    \ result;}\n            if(!valid_value(x)){if constexpr(PREV)result.previous=quad->extreme(0,l,r,0,true);return\
    \ result;}\n            auto q=quad->template neighbors<PREV,NEXT>(x,l,r);return\
    \ {q.first,q.second};\n        }\n\n        if(negative(x)){if constexpr(NEXT)result.next=extreme_suffix(0,l,r,0,false);return\
    \ result;}\n        if(!valid_value(x)){if constexpr(PREV)result.previous=extreme_suffix(0,l,r,0,true);return\
    \ result;}\n        struct Candidate{int level=-1,l=0,r=0;u64 value=0;};\n   \
    \     Candidate prev,next;u64 prefix=0,v=as_u64(x);\n        for(int level=0;level<LOG&&l<r;++level){\n\
    \            auto [ol,orr]=bv[level].rank1_pair(l,r);\n            int zl=l-ol,zr=r-orr;u64\
    \ m=u64(1)<<(LOG-1-level);\n            if(bit(v,level)){\n                if\
    \ constexpr(PREV)if(zl<zr)prev={level+1,zl,zr,prefix};\n                prefix|=m;l=mid[level]+ol;r=mid[level]+orr;\n\
    \            }else{\n                if constexpr(NEXT)if(ol<orr)next={level+1,mid[level]+ol,mid[level]+orr,prefix|m};\n\
    \                l=zl;r=zr;\n            }\n        }\n        if constexpr(PREV)if(prev.level>=0)result.previous=extreme_suffix(prev.level,prev.l,prev.r,prev.value,true);\n\
    \        if constexpr(NEXT){\n            if(l<r)result.next=x;\n            else\
    \ if(next.level>=0)result.next=extreme_suffix(next.level,next.l,next.r,next.value,false);\n\
    \        }\n        return result;\n    }\n\npublic:\n    using value_type=T;\n\
    \n    DynamicWaveletMatrix()=default;\n    explicit DynamicWaveletMatrix(const\
    \ std::vector<T>& a){build(a);}\n\n    int size()const{return n;}\n    bool empty()const{return\
    \ n==0;}\n\n    void clear(){\n        quad.reset();quad_valid=false; n=0;\n \
    \       mid.fill(0);\n        for(auto& b:bv)b.clear();\n    }\n    // Rebuild\
    \ read-only rank indexes after a batch of edits. Any later edit\n    // invalidates\
    \ its affected layers automatically; all existing APIs remain usable.\n    void\
    \ prepare_queries(){ if(quad_valid)return;for(auto& b:bv)b.prepare_queries();\
    \ }\n    // Rebuild directly from binary levels; no value-array reconstruction.\n\
    \    void prepare_quad_queries(){\n        if constexpr(!EEZ_WM_QUAD_INDEX)return;\n\
    \        if(quad_valid)return;\n        if(!quad)quad.emplace();auto& index=*quad;\n\
    \        for(int level=0;level<LOG;){\n            auto high=bv[level].export_packed();\n\
    \            if((LOG&1)&&level==0){index.append_binary(high,{},n,level,mid[level]);++level;}\n\
    \            else{auto low=bv[level+1].export_packed();index.append_binary(high,low,n,level,mid[level]);level+=2;}\n\
    \        }\n        quad_valid=true;\n        for(auto& b:bv)b.discard_query_index();\n\
    \    }\n    void reserve(int m){\n        for(auto& b:bv)b.reserve(m);\n    }\n\
    \    void build(const std::vector<T>& a){\n        assert(a.size()<=std::size_t(INT_MAX));\n\
    \        invalidate_quad(); n=(int)a.size();\n        if constexpr(EEZ_WM_QUAD_INDEX)if(!quad)quad.emplace();\n\
    \        for([[maybe_unused]] T x:a)assert(valid_value(x));\n\n#if defined(__AVX2__)\
    \ && !defined(EEZ_NO_SIMD)\n        // Extract 8 x 32-bit or 4 x 64-bit values\
    \ per vector directly into\n        // packed leaves, avoiding the intermediate\
    \ byte-per-bit array.\n        if constexpr(sizeof(T)==4 || sizeof(T)==8) {\n\
    \            std::vector<T> cur=a,nxt(n);\n            std::vector<u64> packed((std::size_t(n)+63)/64);\n\
    \            for(int level=0;level<LOG;++level){\n                if(quad)quad->append(cur,level);\n\
    \                const auto shift=_mm_cvtsi32_si128(LOG-1-level);\n          \
    \      for(int pos=0;pos<n;) {\n                    int len=std::min(64,n-pos),j=0;u64\
    \ word=0;\n                    if constexpr(sizeof(T)==4) {\n                \
    \        for(;j+8<=len;j+=8) {\n                            auto v=_mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(cur.data()+pos+j));\n                            v=_mm256_slli_epi32(_mm256_srl_epi32(v,shift),31);\n\
    \                            word|=u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(v))))<<j;\n\
    \                        }\n                    } else {\n                   \
    \     for(;j+4<=len;j+=4) {\n                            auto v=_mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(cur.data()+pos+j));\n                            v=_mm256_slli_epi64(_mm256_srl_epi64(v,shift),63);\n\
    \                            word|=u64(unsigned(_mm256_movemask_pd(_mm256_castsi256_pd(v))))<<j;\n\
    \                        }\n                    }\n                    for(;j<len;++j)word|=u64(bit(as_u64(cur[pos+j]),level))<<j;\n\
    \                    packed[std::size_t(pos)/64]=word;pos+=len;\n            \
    \    }\n                bv[level].build_packed(packed,n,!quad);\n            \
    \    int z=n-bv[level].rank1(n);mid[level]=z;\n                if(level+1==LOG)break;\n\
    \                int p0=0,p1=z;\n                for(int i=0;i<n;++i) {\n    \
    \                int b=bit(as_u64(cur[i]),level);nxt[b?p1:p0]=cur[i];p1+=b;p0+=1-b;\n\
    \                }\n                cur.swap(nxt);\n            }\n          \
    \  quad_valid=bool(quad);return;\n        }\n#endif\n\n        std::vector<T>\
    \ cur=a,nxt(n);\n        std::vector<std::uint8_t> bits(n);\n\n        for(int\
    \ level=0;level<LOG;++level){\n                if(quad)quad->append(cur,level);\n\
    \            int z=0;\n            for(int i=0;i<n;++i){\n                bool\
    \ b=bit(as_u64(cur[i]),level);\n                bits[i]=(std::uint8_t)b;\n   \
    \             z+=!b;\n            }\n            mid[level]=z;\n            bv[level].build(bits,!quad);\n\
    \n            if(level+1==LOG)break;\n\n            int p0=0,p1=z;\n         \
    \   for(int i=0;i<n;++i){\n                int b=bits[i];nxt[b?p1:p0]=cur[i];p1+=b;p0+=1-b;\n\
    \            }\n            cur.swap(nxt);\n        }\n\n        quad_valid=bool(quad);\n\
    \    }\n\n    T access(int k)const{\n        assert(0<=k&&k<n);\n        if(quad_valid)return\
    \ quad->access(k);\n        u64 x=0;\n        int p=k;\n        for(int level=0;level<LOG;++level){\n\
    \            auto [b,o]=bv[level].access_rank(p);\n            x=(x<<1)|u64(b);\n\
    \            p=b?mid[level]+o:p-o;\n        }\n        return (T)x;\n    }\n \
    \   T get(int k)const{return access(k);}\n    T operator[](int k)const{return\
    \ access(k);}\n    // Output must not overlap indices. Repeated and unsorted positions\
    \ are allowed.\n    void access_many(const int* indices,std::size_t count,T* output)const{\n\
    \        assert(count==0||(indices&&output));\n        if(quad_valid){quad->access_many(indices,count,output);return;}\n\
    \        std::size_t i=0;\n        for(;count-i>=8;i+=8){\n            int pos[8],ranks[8];bool\
    \ bits[8];u64 values[8]{};\n            for(int j=0;j<8;++j)pos[j]=indices[i+j];\n\
    \            for(int level=0;level<LOG;++level){\n                bv[level].template\
    \ access_rank_batch<8>(pos,bits,ranks);\n                for(int j=0;j<8;++j){\n\
    \                    values[j]=(values[j]<<1)|u64(bits[j]);\n                \
    \    pos[j]=bits[j]?mid[level]+ranks[j]:pos[j]-ranks[j];\n                }\n\
    \            }\n            for(int j=0;j<8;++j)output[i+j]=T(values[j]);\n  \
    \      }\n        for(;i<count;++i)output[i]=access(indices[i]);\n    }\n    std::vector<T>\
    \ access_many(const std::vector<int>& indices)const{\n        std::vector<T> result(indices.size());access_many(indices.data(),indices.size(),result.data());return\
    \ result;\n    }\n\n    void insert(int k,T x){\n        invalidate_quad();\n\
    \        assert(0<=k&&k<=n);\n        assert(valid_value(x));\n        u64 v=as_u64(x);\n\
    \        int p=k;\n        for(int level=0;level<LOG;++level){\n            bool\
    \ b=bit(v,level);\n            int o=bv[level].insert_rank(p,b);\n           \
    \ int np=b?mid[level]+o:p-o;\n            if(!b)++mid[level];\n            p=np;\n\
    \        }\n        ++n;\n    }\n    void push_back(T x){insert(n,x);}\n\n   \
    \ T erase(int k){\n        invalidate_quad();\n        assert(0<=k&&k<n);\n  \
    \      u64 x=0;\n        int p=k;\n        for(int level=0;level<LOG;++level){\n\
    \            auto [b,o]=bv[level].erase_rank(p);\n            int np=b?mid[level]+o:p-o;\n\
    \            if(b)x|=u64(1)<<(LOG-1-level);\n\n            if(!b)--mid[level];\n\
    \            p=np;\n        }\n        --n;\n        return (T)x;\n    }\n   \
    \ T pop_back(){\n        assert(n);\n        return erase(n-1);\n    }\n    T\
    \ set(int k,T x){\n        assert(0<=k&&k<n);assert(valid_value(x));\n       \
    \ if(quad_valid&&quad->access(k)==x)return x;\n        invalidate_quad();\n  \
    \      int from=k,to=k;u64 old=0,v=as_u64(x);\n        for(int level=0;level<LOG;++level){\n\
    \            bool b=bit(v,level),ob;int old_rank,new_rank;\n            if(from==to){auto\
    \ q=bv[level].set_rank(from,b);ob=q.first;old_rank=new_rank=q.second;}\n     \
    \       else{\n                auto q=bv[level].move_rank(from,to,b);ob=q.previous;old_rank=q.old_rank;new_rank=q.new_rank;\n\
    \            }\n            old=(old<<1)|u64(ob);\n            int next_from=ob?mid[level]+old_rank:from-old_rank;\n\
    \            int after_erase=mid[level]-int(!ob);\n            int next_to=b?after_erase+new_rank:to-new_rank;\n\
    \            mid[level]=after_erase+int(!b);from=next_from;to=next_to;\n     \
    \   }\n        return T(old);\n    }\n\n    int rank(T x,int l,int r)const{\n\
    \        range_check(l,r);\n        if(!valid_value(x))return 0;\n        u64\
    \ v=as_u64(x);\n\n        if(quad_valid)return quad->equal(x,l,r);\n        for(int\
    \ level=0;level<LOG && l<r;++level){\n            auto [ol,orr]=bv[level].rank1_pair(l,r);\n\
    \            if(bit(v,level)){\n                l=mid[level]+ol;\n           \
    \     r=mid[level]+orr;\n            }else{\n                l-=ol;\n        \
    \        r-=orr;\n            }\n        }\n        return r-l;\n    }\n    int\
    \ rank(T x,int r)const{return rank(x,0,r);}\n    int count(int l,int r,T x)const{return\
    \ rank(x,l,r);}\n\n    Quantile kth_smallest_with_count(int l,int r,int k)const{\n\
    \        range_check(l,r);\n        assert(0<=k&&k<r-l);\n\n        if(quad_valid){auto\
    \ q=quad->kth(l,r,k);return {q.value,q.less,q.equal};}\n        u64 x=0;int original_k=k;\n\
    \        for(int level=0;level<LOG && l<r;++level){\n            auto [ol,orr]=bv[level].rank1_pair(l,r);\n\
    \            int zl=l-ol,zr=r-orr;\n            int z=zr-zl;\n\n            if(k<z){\n\
    \                l=zl;\n                r=zr;\n            }else{\n          \
    \      x|=u64(1)<<(LOG-1-level);\n                k-=z;\n                l=mid[level]+ol;\n\
    \                r=mid[level]+orr;\n            }\n        }\n        return {T(x),original_k-k,r-l};\n\
    \    }\n    T kth_smallest(int l,int r,int k)const{return kth_smallest_with_count(l,r,k).value;}\n\
    \    T kth_largest(int l,int r,int k)const{\n        range_check(l,r);\n     \
    \   assert(0<=k&&k<r-l);\n        return kth_smallest(l,r,r-l-1-k);\n    }\n\n\
    \    int range_freq(int l,int r,T upper)const{\n        range_check(l,r);\n  \
    \      if(negative(upper))return 0;\n\n        u64 v=as_u64(upper);\n        if\
    \ constexpr(LOG<64){\n            if(v>=(u64(1)<<LOG))return r-l;\n        }\n\
    \n        if(quad_valid)return quad->less(upper,l,r);\n        int ans=0;\n  \
    \      for(int level=0;level<LOG && l<r;++level){\n            auto [ol,orr]=bv[level].rank1_pair(l,r);\n\
    \            int zl=l-ol,zr=r-orr;\n\n            if(bit(v,level)){\n        \
    \        ans+=zr-zl;\n                l=mid[level]+ol;\n                r=mid[level]+orr;\n\
    \            }else{\n                l=zl;\n                r=zr;\n          \
    \  }\n        }\n        return ans;\n    }\n    int range_freq(int l,int r,T\
    \ lower,T upper)const{\n        range_check(l,r);\n        if(!(lower<upper))return\
    \ 0;\n        auto result=count_less_pair(l,r,lower,upper);return result.second-result.first;\n\
    \    }\n\n    Counts rank_all(int l,int r,T x)const{\n        range_check(l,r);int\
    \ total=r-l;\n        if(negative(x))return {0,0,total};\n        if(!valid_value(x))return\
    \ {total,0,0};\n        if(quad_valid){auto q=quad->less_equal(x,l,r);return {q.first,q.second,total-q.first-q.second};}\n\
    \        int less=0;u64 v=as_u64(x);\n        for(int level=0;level<LOG&&l<r;++level){\n\
    \            auto [ol,orr]=bv[level].rank1_pair(l,r);\n            if(bit(v,level)){less+=(r-l)-(orr-ol);l=mid[level]+ol;r=mid[level]+orr;}\n\
    \            else{l-=ol;r-=orr;}\n        }\n        return {less,r-l,total-less-(r-l)};\n\
    \    }\n    // Counts values <lower and <upper, sharing their common bit prefix.\n\
    \    std::pair<int,int> count_less_pair(int l,int r,T lower,T upper)const{\n \
    \       range_check(l,r);assert(lower<=upper);\n        if(lower==upper){int c=range_freq(l,r,lower);return\
    \ {c,c};}\n        if(!valid_value(lower)||!valid_value(upper))return {range_freq(l,r,lower),range_freq(l,r,upper)};\n\
    \        if(quad_valid)return {quad->less(lower,l,r),quad->less(upper,l,r)};\n\
    \        u64 lo=as_u64(lower),hi=as_u64(upper);int ans=0;\n        for(int level=0;level<LOG&&l<r;++level){\n\
    \            auto [ol,orr]=bv[level].rank1_pair(l,r);\n            int zl=l-ol,zr=r-orr;bool\
    \ a=bit(lo,level),b=bit(hi,level);\n            if(a!=b)return {less_suffix(level+1,zl,zr,lo,ans),less_suffix(level+1,mid[level]+ol,mid[level]+orr,hi,ans+zr-zl)};\n\
    \            if(a){ans+=zr-zl;l=mid[level]+ol;r=mid[level]+orr;}else{l=zl;r=zr;}\n\
    \        }\n        return {ans,ans};\n    }\n    Neighbors neighbors(int l,int\
    \ r,T x)const{return neighbors_impl<true,true>(l,r,x);}\n\n    std::optional<T>\
    \ prev_value(int l,int r,T upper)const{\n        return neighbors_impl<true,false>(l,r,upper).previous;\n\
    \    }\n    std::optional<T> next_value(int l,int r,T lower)const{\n        return\
    \ neighbors_impl<false,true>(l,r,lower).next;\n    }\n};\n\n}\n\n"
  code: "#pragma once\n#include <algorithm>\n#include <array>\n#include <cassert>\n\
    #include <climits>\n#include <cstdint>\n#include <cstring>\n#include <new>\n#include\
    \ <limits>\n#include <memory>\n#include <optional>\n#include <type_traits>\n#include\
    \ <utility>\n#include <vector>\n#if (defined(__AVX2__) || defined(__BMI2__)) &&\
    \ !defined(EEZ_NO_SIMD)\n#include <immintrin.h>\n#endif\n\n#ifndef EEZ_WM_QUAD_INDEX\n\
    #define EEZ_WM_QUAD_INDEX 1\n#endif\n#ifndef EEZ_BV_RANGE_UPDATE\n#define EEZ_BV_RANGE_UPDATE\
    \ 0\n#endif\n#ifndef EEZ_BV_MOVE_PAIR\n#define EEZ_BV_MOVE_PAIR 0\n#endif\n#ifndef\
    \ EEZ_BV_LEAF_SIMD\n#define EEZ_BV_LEAF_SIMD 0\n#endif\n#ifndef EEZ_QUAD_RETAIN\n\
    #define EEZ_QUAD_RETAIN 1\n#endif\nnamespace eez {\n\n#ifndef EEZ_BV_WORDS\n#define\
    \ EEZ_BV_WORDS 64\n#endif\n#ifndef EEZ_BV_FANOUT\n#define EEZ_BV_FANOUT 16\n#endif\n\
    #ifndef EEZ_BV_GROUP\n#define EEZ_BV_GROUP 4\n#endif\n#ifndef EEZ_BV_SIMD_SHIFT\n\
    #define EEZ_BV_SIMD_SHIFT 1\n#endif\n#ifndef EEZ_BV_PREFETCH\n#define EEZ_BV_PREFETCH\
    \ 0\n#endif\n#ifndef EEZ_BV_SEARCH\n#define EEZ_BV_SEARCH 0\n#endif\n#ifndef EEZ_BV_STATIC_INDEX\n\
    #define EEZ_BV_STATIC_INDEX 1\n#endif\n#ifndef EEZ_BV_BUFFER\n#define EEZ_BV_BUFFER\
    \ 0\n#endif\n#ifndef EEZ_BV_FILL\n#define EEZ_BV_FILL 80\n#endif\nnamespace detail\
    \ {\n// A contiguous arena for trivial nodes. Unused capacity is never zero-filled.\n\
    // Allocation creates all object lifetimes with new T[], retaining C++17 support.\n\
    template<class T> class NodeArena {\n    static_assert(std::is_trivial_v<T> &&\
    \ std::is_trivially_copyable_v<T>);\n    std::unique_ptr<T[]> data_;\n    std::uint32_t\
    \ used_ = 0, capacity_ = 0;\npublic:\n    NodeArena() = default;\n    NodeArena(const\
    \ NodeArena& other) {\n        reserve(other.used_); used_ = other.used_;\n  \
    \      if (used_) std::memcpy(data_.get(),other.data_.get(),std::size_t(used_)*sizeof(T));\n\
    \    }\n    NodeArena& operator=(const NodeArena& other) { if (this != &other)\
    \ { NodeArena tmp(other); swap(tmp); } return *this; }\n    NodeArena(NodeArena&&\
    \ other) noexcept { swap(other); }\n    NodeArena& operator=(NodeArena&& other)\
    \ noexcept { if (this != &other) { NodeArena tmp(std::move(other)); swap(tmp);\
    \ } return *this; }\n    void swap(NodeArena& other) noexcept {\n        data_.swap(other.data_);\
    \ std::swap(used_,other.used_); std::swap(capacity_,other.capacity_);\n    }\n\
    \    T& operator[](std::size_t i) noexcept { return data_[i]; }\n    const T&\
    \ operator[](std::size_t i) const noexcept { return data_[i]; }\n    std::size_t\
    \ size() const noexcept { return used_; }\n    std::size_t capacity() const noexcept\
    \ { return capacity_; }\n    void clear() noexcept { used_ = 0; }\n    void reserve(std::size_t\
    \ n) {\n        if (n <= capacity_) return;\n        if (n > std::size_t(INT_MAX))\
    \ throw std::bad_alloc();\n        std::unique_ptr<T[]> next(new T[n]);\n    \
    \    if (used_) std::memcpy(next.get(),data_.get(),std::size_t(used_)*sizeof(T));\n\
    \        data_.swap(next); capacity_ = std::uint32_t(n);\n    }\n    void emplace_back()\
    \ {\n        if (used_ == capacity_) reserve(capacity_ ? std::min(std::size_t(INT_MAX),std::size_t(capacity_)*2)\
    \ : 16);\n        if (used_ == std::uint32_t(INT_MAX)) throw std::bad_alloc();\n\
    \        ++used_; // The caller initializes the live fields, not padding/capacity.\n\
    \    }\n};\n}\n\n// B+ tree with independently tuned leaf blocks and SIMD-searchable\
    \ inner nodes.\n// A negative handle encodes a leaf index (~index); other handles\
    \ are inner nodes.\nclass DynamicBitVector {\n    using u64 = std::uint64_t;\n\
    \    static constexpr int WORDS = EEZ_BV_WORDS, FANOUT = EEZ_BV_FANOUT, GROUP\
    \ = EEZ_BV_GROUP;\n    static constexpr int CAP = WORDS*64, HALF = WORDS/2, GROUPS\
    \ = (WORDS+GROUP-1)/GROUP;\n    static constexpr int BUILD_WORDS = WORDS*EEZ_BV_FILL/100\
    \ > 0 ? WORDS*EEZ_BV_FILL/100 : 1;\n    static constexpr int BUILD_BITS = BUILD_WORDS*64;\n\
    \    static_assert(WORDS >= 2 && WORDS <= 128);\n    static_assert(FANOUT >= 8\
    \ && FANOUT <= 64 && (FANOUT&(FANOUT-1)) == 0);\n    static_assert(GROUP == 1\
    \ || GROUP == 2 || GROUP == 4 || GROUP == 8);\n    static_assert(EEZ_BV_FILL >=\
    \ 50 && EEZ_BV_FILL <= 100);\n    struct alignas(64) Leaf {\n        u64 words[WORDS];\n\
    \        std::uint16_t ones[GROUPS];\n        int count;\n#if EEZ_BV_BUFFER >\
    \ 0\n        std::uint32_t edits[EEZ_BV_BUFFER];\n        int edit_count;\n  \
    \      static_assert(EEZ_BV_BUFFER <= 16);\n#endif\n    };\n    struct alignas(64)\
    \ Node {\n        int sizes[FANOUT], ones[FANOUT], child[FANOUT];\n        int\
    \ count;\n    };\n#ifdef EEZ_BV_STD_VECTOR\n    std::vector<Leaf> leaves;\n  \
    \  std::vector<Node> pool;\n#else\n    detail::NodeArena<Leaf> leaves;\n    detail::NodeArena<Node>\
    \ pool;\n#endif\n    int root = -1, free_leaf = -1, free_node = -1;\n    int length\
    \ = 0, total_ones = 0;\n#if EEZ_BV_STATIC_INDEX\n    struct StaticWord { u64 bits;\
    \ int prefix; };\n    std::unique_ptr<StaticWord[]> read_index;\n    std::pair<bool,int>\
    \ indexed_access_rank(int k) const noexcept {\n        const auto& w = read_index[k\
    \ >> 6];\n        return {bool((w.bits >> (k & 63)) & 1),w.prefix+pc(w.bits &\
    \ mask(k & 63))};\n    }\n    int indexed_rank(int k) const noexcept {\n     \
    \   if (k == length) return total_ones;\n        const auto& w = read_index[k\
    \ >> 6]; return w.prefix+pc(w.bits & mask(k & 63));\n    }\n    void collect_words(int\
    \ p,StaticWord* output,int& offset) {\n        if (p >= 0) { const auto& a = pool[p];\
    \ for (int i = 0; i < a.count; ++i) collect_words(a.child[i],output,offset); return;\
    \ }\n        auto& a = leaves[~p];\n#if EEZ_BV_BUFFER > 0\n        flush_leaf(a);\n\
    #endif\n        for (int j = 0; j*64 < a.count; ++j) {\n            int count\
    \ = std::min(64,a.count-j*64), shift = offset & 63;\n            u64 w = a.words[j]\
    \ & mask(count); int dst = offset >> 6;\n            output[dst].bits |= w <<\
    \ shift;\n            if (shift && count > 64-shift) output[dst+1].bits |= w >>\
    \ (64-shift);\n            offset += count;\n        }\n    }\n    void index_packed(const\
    \ u64* words) {\n        if (!total_ones || total_ones == length) return;\n  \
    \      std::size_t count = (std::size_t(length)+63)/64;\n        std::unique_ptr<StaticWord[]>\
    \ next(new StaticWord[count]); int rank = 0;\n        for (std::size_t i = 0;\
    \ i < count; ++i) {\n            u64 w = words[i]; if (i+1 == count && (length\
    \ & 63)) w &= mask(length & 63);\n            next[i].bits = w; next[i].prefix\
    \ = rank; rank += pc(w);\n        }\n        read_index = std::move(next);\n \
    \   }\n#endif\n    void collect_packed(int p,u64* out,int& offset){\n        if(p>=0){const\
    \ auto& a=pool[p];for(int i=0;i<a.count;++i)collect_packed(a.child[i],out,offset);return;}\n\
    \        auto& a=leaves[~p];\n#if EEZ_BV_BUFFER > 0\n        flush_leaf(a);\n\
    #endif\n        for(int j=0;j*64<a.count;++j){\n            int count=std::min(64,a.count-j*64),shift=offset&63;u64\
    \ word=a.words[j]&mask(count);\n            out[offset>>6]|=word<<shift;\n   \
    \         if(shift&&count>64-shift)out[(offset>>6)+1]|=word>>(64-shift);\n   \
    \         offset+=count;\n        }\n    }\n    static u64 mask(int k) { return\
    \ k == 64 ? ~u64(0) : (u64(1) << k) - 1; }\n    static int pc(u64 x) {\n#if defined(__GNUC__)\
    \ || defined(__clang__)\n        return __builtin_popcountll(x);\n#else\n    \
    \    x -= (x >> 1) & 0x5555555555555555ull;\n        x = (x & 0x3333333333333333ull)\
    \ + ((x >> 2) & 0x3333333333333333ull);\n        x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0full;\n\
    \        return int((x * 0x0101010101010101ull) >> 56);\n#endif\n    }\n    void\
    \ prefetch(int p) const noexcept {\n#if EEZ_BV_PREFETCH && (defined(__GNUC__)\
    \ || defined(__clang__))\n        if (p < 0) { __builtin_prefetch(&leaves[~p],0,3);\
    \ __builtin_prefetch(leaves[~p].ones,0,3); }\n        else __builtin_prefetch(pool[p].sizes,0,3);\n\
    #else\n        (void)p;\n#endif\n    }\n    int size_of(int p) const { return\
    \ p < 0 ? leaves[~p].count : pool[p].sizes[pool[p].count-1]; }\n    int ones_of(int\
    \ p) const { return p < 0 ? leaf_total(leaves[~p]) : pool[p].ones[pool[p].count-1];\
    \ }\n    int alloc_leaf() {\n        int i;\n        if (free_leaf >= 0) { i =\
    \ free_leaf; free_leaf = leaves[i].count; leaves[i] = Leaf{}; }\n        else\
    \ { i = int(leaves.size()); leaves.emplace_back(); leaves[i] = Leaf{}; }\n   \
    \     return ~i;\n    }\n    int alloc_node() {\n        int p;\n        if (free_node\
    \ >= 0) { p = free_node; free_node = pool[p].child[0]; }\n        else { p = int(pool.size());\
    \ pool.emplace_back(); }\n        return p;\n    }\n    void release(int p) {\n\
    \        if (p < 0) { leaves[~p].count = free_leaf; free_leaf = ~p; }\n      \
    \  else { pool[p].child[0] = free_node; free_node = p; }\n    }\n    static void\
    \ leaf_counts(Leaf& a) {\n        int sum = 0;\n        for (int j = 0; j < WORDS;\
    \ ++j) { sum += pc(a.words[j]); if ((j+1)%GROUP == 0 || j+1 == WORDS) a.ones[j/GROUP]\
    \ = std::uint16_t(sum); }\n    }\n    void inner_counts(int p) {\n        auto&\
    \ a = pool[p]; int s = 0, o = 0;\n        for (int i = 0; i < a.count; ++i) {\n\
    \            s += size_of(a.child[i]); o += ones_of(a.child[i]);\n           \
    \ a.sizes[i] = s; a.ones[i] = o;\n        }\n        for (int i = a.count; i <\
    \ FANOUT; ++i) { a.sizes[i] = INT_MAX; a.ones[i] = o; }\n    }\n    static int\
    \ locate(const Node& a, int k) {\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\
    \ && EEZ_BV_SEARCH == 0\n        const auto key = _mm256_set1_epi32(k);\n    \
    \    u64 bits = 0;\n        for (int j = 0; j < FANOUT; j += 8) {\n          \
    \  auto v = _mm256_cmpgt_epi32(_mm256_load_si256(reinterpret_cast<const __m256i*>(a.sizes+j)),key);\n\
    \            bits |= u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(v))))\
    \ << j;\n        }\n        int i;\n#if defined(__GNUC__) || defined(__clang__)\n\
    \        i = bits ? __builtin_ctzll(bits) : FANOUT;\n#else\n        i = 0; while\
    \ (i < FANOUT && !(bits & (u64(1) << i))) ++i;\n#endif\n        return i < a.count\
    \ ? i : a.count-1;\n#else\n        int i = 0;\n        for (int step = FANOUT/2;\
    \ step; step >>= 1) i += step*int(a.sizes[i+step-1] <= k);\n        i += int(a.sizes[i]\
    \ <= k); return i < a.count ? i : a.count-1;\n#endif\n    }\n    static std::pair<int,int>\
    \ locate_two(const Node& a,int l,int r){\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n\
    \        auto kl=_mm256_set1_epi32(l),kr=_mm256_set1_epi32(r);u64 bl=0,br=0;\n\
    \        for(int j=0;j<FANOUT;j+=8){auto v=_mm256_load_si256(reinterpret_cast<const\
    \ __m256i*>(a.sizes+j));bl|=u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(v,kl)))))<<j;br|=u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(v,kr)))))<<j;}\n\
    #if defined(__GNUC__) || defined(__clang__)\n        int i=bl?__builtin_ctzll(bl):FANOUT,j=br?__builtin_ctzll(br):FANOUT;\n\
    #else\n        int i=bl?pc((bl&-bl)-1):FANOUT,j=br?pc((br&-br)-1):FANOUT;\n#endif\n\
    \        return {std::min(i,a.count-1),std::min(j,a.count-1)};\n#else\n      \
    \  return {locate(a,l),locate(a,r)};\n#endif\n    }\n    static void add_leaf_ones(Leaf&\
    \ a,int first,int delta){\n        int i=first;\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\
    \ && EEZ_BV_LEAF_SIMD\n        i=first&~15;\n        for(;i+16<=GROUPS;i+=16){\n\
    \            auto ix=_mm256_setr_epi16(i,i+1,i+2,i+3,i+4,i+5,i+6,i+7,i+8,i+9,i+10,i+11,i+12,i+13,i+14,i+15);\n\
    \            auto m=_mm256_cmpgt_epi16(ix,_mm256_set1_epi16(first-1));\n     \
    \       auto q=reinterpret_cast<__m256i*>(a.ones+i);_mm256_storeu_si256(q,_mm256_add_epi16(_mm256_loadu_si256(q),_mm256_and_si256(m,_mm256_set1_epi16(delta))));\n\
    \        }\n        i=std::max(i,first);\n#endif\n        for(;i<GROUPS;++i)a.ones[i]=std::uint16_t(a.ones[i]+delta);\n\
    \    }\n    static int leaf_rank_flat(const Leaf& a, int k) {\n        int j =\
    \ k >> 6, bits = k & 63, group = j/GROUP;\n        int sum = group ? a.ones[group-1]\
    \ : 0;\n        for (int w = group*GROUP; w < j; ++w) sum += pc(a.words[w]);\n\
    \        return sum + (bits ? pc(a.words[j] & mask(bits)) : 0);\n    }\n    static\
    \ bool leaf_access(const Leaf& a,int k) {\n#if EEZ_BV_BUFFER > 0\n        for\
    \ (int i = a.edit_count-1; i >= 0; --i) {\n            unsigned e = a.edits[i];\
    \ int pos = e & 65535, op = (e >> 18) & 3;\n            if (op == 0) { if (k ==\
    \ pos) return (e >> 16) & 1; k -= k > pos; }\n            else if (op == 1) k\
    \ += k >= pos;\n            else if (k == pos) return (e >> 16) & 1;\n       \
    \ }\n#endif\n        return (a.words[k >> 6] >> (k & 63)) & 1;\n    }\n    static\
    \ int leaf_rank(const Leaf& a,int k) {\n        int delta = 0;\n#if EEZ_BV_BUFFER\
    \ > 0\n        for (int i = a.edit_count-1; i >= 0; --i) {\n            unsigned\
    \ e = a.edits[i]; int pos = e & 65535, op = (e >> 18) & 3;\n            if (pos\
    \ < k) {\n                if (op == 0) { --k; delta += (e >> 16) & 1; }\n    \
    \            else if (op == 1) { ++k; delta -= (e >> 17) & 1; }\n            \
    \    else delta += int((e >> 16) & 1)-int((e >> 17) & 1);\n            }\n   \
    \     }\n#endif\n        return delta+leaf_rank_flat(a,k);\n    }\n    static\
    \ int leaf_total(const Leaf& a) {\n        int total = a.ones[GROUPS-1];\n#if\
    \ EEZ_BV_BUFFER > 0\n        for (int i = 0; i < a.edit_count; ++i) {\n      \
    \      unsigned e = a.edits[i]; total += int((e >> 16) & 1)-int((e >> 17) & 1);\n\
    \        }\n#endif\n        return total;\n    }\n    static std::pair<int,int>\
    \ leaf_pair(const Leaf& a,int l,int r) {\n        int dl = 0, dr = 0;\n#if EEZ_BV_BUFFER\
    \ > 0\n        for (int i = a.edit_count-1; i >= 0; --i) {\n            unsigned\
    \ e = a.edits[i]; int p = e & 65535, op = (e >> 18) & 3;\n            int left\
    \ = p < l, right = p < r;\n            int old = (e >> 17) & 1, bit = (e >> 16)\
    \ & 1;\n            if (op == 0) { dl += left*bit; dr += right*bit; l -= left;\
    \ r -= right; }\n            else if (op == 1) { dl -= left*old; dr -= right*old;\
    \ l += left; r += right; }\n            else { dl += left*(bit-old); dr += right*(bit-old);\
    \ }\n        }\n#endif\n        int first = leaf_rank_flat(a,l), last;\n     \
    \   if (l == r) last = first;\n        else if ((l >> 6) == ((r-1) >> 6)) last\
    \ = first+pc((a.words[l >> 6] >> (l & 63)) & mask(r-l));\n        else last =\
    \ leaf_rank_flat(a,r);\n        return {first+dl,last+dr};\n    }\n    static\
    \ std::pair<bool,int> leaf_access_rank(const Leaf& a,int k) {\n#if EEZ_BV_BUFFER\
    \ > 0\n        auto q = leaf_pair(a,k,k+1); return {q.first != q.second,q.first};\n\
    #else\n        return {leaf_access(a,k),leaf_rank_flat(a,k)};\n#endif\n    }\n\
    #if EEZ_BV_BUFFER > 0\n    // Materialize the journal by copying unchanged bit\
    \ ranges once, rather\n    // than shifting the whole leaf separately for every\
    \ buffered operation.\n    static void flush_leaf(Leaf& a) {\n        if (!a.edit_count)\
    \ return;\n        struct Run { int start, length; } runs[2*EEZ_BV_BUFFER+3];\n\
    \        int base_count = a.count;\n        for (int i = 0; i < a.edit_count;\
    \ ++i) { int op = (a.edits[i] >> 18) & 3; base_count += int(op == 1)-int(op ==\
    \ 0); }\n        int nr = 1; runs[0] = {0,base_count};\n        auto erase_at\
    \ = [&](int position) {\n            int r = 0, offset = position;\n         \
    \   while (r < nr && offset >= runs[r].length) { offset -= runs[r].length; ++r;\
    \ }\n            assert(r < nr);\n            Run old = runs[r];\n           \
    \ if (old.length == 1) { std::memmove(runs+r,runs+r+1,std::size_t(nr-r-1)*sizeof(Run));\
    \ --nr; }\n            else if (!offset) { if (old.start >= 0) ++runs[r].start;\
    \ --runs[r].length; }\n            else if (offset+1 == old.length) --runs[r].length;\n\
    \            else {\n                std::memmove(runs+r+2,runs+r+1,std::size_t(nr-r-1)*sizeof(Run));\
    \ ++nr;\n                runs[r] = {old.start,offset};\n                runs[r+1]\
    \ = {old.start+offset+1,old.length-offset-1};\n            }\n        };\n   \
    \     auto insert_at = [&](int position,bool bit) {\n            int r = 0, offset\
    \ = position;\n            while (r < nr && offset >= runs[r].length) { offset\
    \ -= runs[r].length; ++r; }\n            if (r == nr || !offset) {\n         \
    \       std::memmove(runs+r+1,runs+r,std::size_t(nr-r)*sizeof(Run));\n       \
    \         runs[r] = {bit ? -2 : -1,1}; ++nr;\n            } else {\n         \
    \       Run old = runs[r];\n                std::memmove(runs+r+3,runs+r+1,std::size_t(nr-r-1)*sizeof(Run));\
    \ nr += 2;\n                runs[r] = {old.start,offset}; runs[r+1] = {bit ? -2\
    \ : -1,1};\n                runs[r+2] = {old.start+offset,old.length-offset};\n\
    \            }\n        };\n        for (int i = 0; i < a.edit_count; ++i) {\n\
    \            unsigned e = a.edits[i]; int pos = e & 65535, op = (e >> 18) & 3;\n\
    \            if (op != 0) erase_at(pos);\n            if (op != 1) insert_at(pos,(e\
    \ >> 16) & 1);\n        }\n        u64 out[WORDS]{}; int dst = 0;\n        for\
    \ (int i = 0; i < nr; ++i) {\n            auto run = runs[i];\n            if\
    \ (run.start < 0) { if (run.start == -2) out[dst >> 6] |= u64(1) << (dst & 63);\
    \ dst += run.length; continue; }\n            int src = run.start, remaining =\
    \ run.length;\n            while (remaining) {\n                int count = std::min(64-(dst\
    \ & 63),remaining), word = src >> 6, shift = src & 63;\n                u64 bits\
    \ = a.words[word] >> shift;\n                if (shift && word+1 < WORDS) bits\
    \ |= a.words[word+1] << (64-shift);\n                out[dst >> 6] |= (bits &\
    \ mask(count)) << (dst & 63);\n                dst += count; src += count; remaining\
    \ -= count;\n            }\n        }\n        assert(dst == a.count);\n     \
    \   std::memcpy(a.words,out,sizeof(out)); a.edit_count = 0; leaf_counts(a);\n\
    \    }\n    static void record_edit(Leaf& a,int pos,int op,bool old,bool bit)\
    \ {\n        a.edits[a.edit_count++] = unsigned(pos) | (unsigned(bit) << 16) |\
    \ (unsigned(old) << 17) | (unsigned(op) << 18);\n        if (a.edit_count == EEZ_BV_BUFFER)\
    \ flush_leaf(a);\n    }\n#endif\n#if EEZ_BV_RANGE_UPDATE\n    static void adjust_range(Node&\
    \ a,int first,int last,int ds,int d1) {\n        if(first==last || (!ds&&!d1))return;\n\
    #if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n        alignas(32) static constexpr\
    \ auto masks=[](){\n            std::array<std::array<int,8>,9> out{};\n     \
    \       for(int i=0;i<=8;++i)for(int j=i;j<8;++j)out[i][j]=-1;\n            return\
    \ out;\n        }();\n        for(int j=first&~7;j<last;j+=8){\n            int\
    \ lo=std::max(0,first-j),hi=std::min(8,last-j);\n            auto m=_mm256_xor_si256(_mm256_load_si256(reinterpret_cast<const\
    \ __m256i*>(masks[lo].data())),_mm256_load_si256(reinterpret_cast<const __m256i*>(masks[hi].data())));\n\
    \            if(ds){auto q=reinterpret_cast<__m256i*>(a.sizes+j);_mm256_store_si256(q,_mm256_add_epi32(_mm256_load_si256(q),_mm256_and_si256(m,_mm256_set1_epi32(ds))));}\n\
    \            if(d1){auto q=reinterpret_cast<__m256i*>(a.ones+j);_mm256_store_si256(q,_mm256_add_epi32(_mm256_load_si256(q),_mm256_and_si256(m,_mm256_set1_epi32(d1))));}\n\
    \        }\n#else\n        for(int j=first;j<last;++j){a.sizes[j]+=ds;a.ones[j]+=d1;}\n\
    #endif\n    }\n    static void adjust(Node& a,int first,int ds,int d1){adjust_range(a,first,a.count,ds,d1);}\n\
    #else\n    static void adjust(Node& a, int first, int ds, int d1) {\n#if defined(__AVX2__)\
    \ && !defined(EEZ_NO_SIMD)\n        const auto begin = _mm256_set1_epi32(first-1),\
    \ end = _mm256_set1_epi32(a.count);\n        for (int j = first & ~7; j < a.count;\
    \ j += 8) {\n            auto ix = _mm256_setr_epi32(j,j+1,j+2,j+3,j+4,j+5,j+6,j+7);\n\
    \            auto m = _mm256_and_si256(_mm256_cmpgt_epi32(ix,begin), _mm256_cmpgt_epi32(end,ix));\n\
    \            auto s = reinterpret_cast<__m256i*>(a.sizes+j);\n            auto\
    \ o = reinterpret_cast<__m256i*>(a.ones+j);\n            if (ds) _mm256_store_si256(s,_mm256_add_epi32(_mm256_load_si256(s),_mm256_and_si256(m,_mm256_set1_epi32(ds))));\n\
    \            if (d1) _mm256_store_si256(o,_mm256_add_epi32(_mm256_load_si256(o),_mm256_and_si256(m,_mm256_set1_epi32(d1))));\n\
    \        }\n#else\n        for (int i = first; i < a.count; ++i) { a.sizes[i]\
    \ += ds; a.ones[i] += d1; }\n#endif\n    }\n#endif\n    static void insert_plain(Leaf&\
    \ a,int k,bool bit) {\n        int j = k >> 6, b = k & 63, limit = (a.count+64)/64;\n\
    \        int total = a.ones[GROUPS-1]+int(bit);\n        u64 w = a.words[j];\n\
    \        a.words[j] = (w & mask(b)) | ((w & ~mask(b)) << 1) | (u64(bit) << b);\n\
    \        u64 carry = w >> 63;\n        if ((j+1)%GROUP == 0) a.ones[j/GROUP] =\
    \ std::uint16_t(a.ones[j/GROUP]+int(bit)-int(carry));\n        int i = j+1;\n\
    #if defined(__AVX2__) && !defined(EEZ_NO_SIMD) && EEZ_BV_SIMD_SHIFT\n        if\
    \ constexpr(GROUP == 4) {\n            for (; i < limit && (i & 3); ++i) {\n \
    \               u64 old = a.words[i]; a.words[i] = (old << 1) | carry; carry =\
    \ old >> 63;\n                if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(bit)-int(carry));\n\
    \            }\n            for (; i+4 <= limit; i += 4) {\n                auto\
    \ p = reinterpret_cast<__m256i*>(a.words+i);\n                auto old = _mm256_load_si256(p);\n\
    \                auto incoming = _mm256_srli_epi64(_mm256_permute4x64_epi64(old,0x90),63);\n\
    \                incoming = _mm256_blend_epi32(incoming,_mm256_set_epi64x(0,0,0,carry),0x03);\n\
    \                u64 next = a.words[i+3] >> 63;\n                _mm256_store_si256(p,_mm256_or_si256(_mm256_slli_epi64(old,1),incoming));\n\
    \                carry = next; a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(bit)-int(carry));\n\
    \            }\n        }\n#endif\n        for (; i < limit; ++i) {\n        \
    \    u64 old = a.words[i]; a.words[i] = (old << 1) | carry; carry = old >> 63;\n\
    \            if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(bit)-int(carry));\n\
    \        }\n        for (int g = limit/GROUP; g < GROUPS; ++g) a.ones[g] = std::uint16_t(total);\n\
    \        ++a.count;\n    }\n    static void erase_plain(Leaf& a,int k,bool bit)\
    \ {\n        int j = k >> 6, b = k & 63, limit = (a.count+63)/64;\n        int\
    \ total = a.ones[GROUPS-1]-int(bit);\n        u64 w = a.words[j], incoming = j+1\
    \ < limit ? a.words[j+1]&1 : 0;\n        a.words[j] = (w & mask(b)) | (b == 63\
    \ ? 0 : (w >> (b+1)) << b) | (incoming << 63);\n        if ((j+1)%GROUP == 0)\
    \ a.ones[j/GROUP] = std::uint16_t(a.ones[j/GROUP]+int(incoming)-int(bit));\n \
    \       int i = j+1;\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD) && EEZ_BV_SIMD_SHIFT\n\
    \        if constexpr(GROUP == 4) {\n            for (; i < limit && (i & 3);\
    \ ++i) {\n                u64 next = i+1 < limit ? a.words[i+1]&1 : 0;\n     \
    \           a.words[i] = (a.words[i] >> 1) | (next << 63);\n                if\
    \ ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(next)-int(bit));\n\
    \            }\n            for (; i+4 <= limit; i += 4) {\n                auto\
    \ p = reinterpret_cast<__m256i*>(a.words+i);\n                auto old = _mm256_load_si256(p);\
    \ u64 next = i+4 < limit ? a.words[i+4]&1 : 0;\n                auto carries =\
    \ _mm256_slli_epi64(_mm256_permute4x64_epi64(old,0xf9),63);\n                carries\
    \ = _mm256_blend_epi32(carries,_mm256_set_epi64x(next << 63,0,0,0),0xc0);\n  \
    \              _mm256_store_si256(p,_mm256_or_si256(_mm256_srli_epi64(old,1),carries));\n\
    \                a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(next)-int(bit));\n\
    \            }\n        }\n#endif\n        for (; i < limit; ++i) {\n        \
    \    u64 next = i+1 < limit ? a.words[i+1]&1 : 0;\n            a.words[i] = (a.words[i]\
    \ >> 1) | (next << 63);\n            if ((i+1)%GROUP == 0) a.ones[i/GROUP] = std::uint16_t(a.ones[i/GROUP]+int(next)-int(bit));\n\
    \        }\n        for (int g = limit/GROUP; g < GROUPS; ++g) a.ones[g] = std::uint16_t(total);\n\
    \        --a.count;\n    }\n    std::pair<bool,int> replace(int p,int k,bool bit)\
    \ {\n        if (p < 0) {\n            auto& a = leaves[~p];\n#if EEZ_BV_BUFFER\
    \ > 0\n            auto q = leaf_access_rank(a,k); bool old = q.first; int rank\
    \ = q.second;\n            if (old != bit) record_edit(a,k,2,old,bit);\n     \
    \       return {old,rank};\n#else\n            int j = k >> 6;\n            bool\
    \ old = (a.words[j] >> (k&63)) & 1; int rank = leaf_rank(a,k);\n            if\
    \ (old != bit) {\n                a.words[j] ^= u64(1) << (k&63);\n          \
    \      add_leaf_ones(a,j/GROUP,int(bit)-int(old));\n            }\n          \
    \  return {old,rank};\n#endif\n        }\n        auto& a = pool[p]; int i = locate(a,k),\
    \ o = i ? a.ones[i-1] : 0;\n        auto result = replace(a.child[i],k-(i ? a.sizes[i-1]\
    \ : 0),bit);\n        if (result.first != bit) adjust(a,i,0,int(bit)-int(result.first));\n\
    \        result.second += o; return result;\n    }\n    // Returns whether a split\
    \ occurred; sibling handles may be negative.\n    bool ins(int p,int k,bool bit,int&\
    \ rank,int& sibling) {\n        if (p < 0) {\n            auto& a = leaves[~p];\n\
    #if EEZ_BV_BUFFER > 0\n            if (a.count == CAP) flush_leaf(a);\n      \
    \      else if (a.edit_count || !((!bit && !a.ones[GROUPS-1]) || (bit && a.ones[GROUPS-1]\
    \ == a.count))) {\n                rank += leaf_rank(a,k); ++a.count; record_edit(a,k,0,false,bit);\
    \ return false;\n            }\n#endif\n            if (a.count < CAP) {\n   \
    \             if (!bit && !a.ones[GROUPS-1]) { ++a.count; return false; }\n  \
    \              if (bit && a.ones[GROUPS-1] == a.count) {\n                   \
    \ rank += k; int j = a.count >> 6;\n                    a.words[j] |= u64(1) <<\
    \ (a.count & 63);\n                    add_leaf_ones(a,j/GROUP,1);\n         \
    \           ++a.count; return false;\n                }\n            }\n     \
    \       rank += leaf_rank(a,k);\n            int len = a.count, j = k >> 6, b\
    \ = k & 63; u64 carry = u64(bit);\n            if (len < CAP) { insert_plain(a,k,bit);\
    \ return false; }\n            u64 w[WORDS+1]{};\n            for (int i = 0;\
    \ i < WORDS; ++i) w[i] = a.words[i];\n            for (int i = j; i < WORDS+1;\
    \ ++i) {\n                u64 old = w[i];\n                if (i == j) w[i] =\
    \ (old & mask(b)) | ((old & ~mask(b)) << 1) | (carry << b);\n                else\
    \ w[i] = (old << 1) | carry;\n                carry = old >> 63;\n           \
    \ }\n            int q = alloc_leaf(); // May move the leaf arena: reacquire both\
    \ leaves.\n            auto& left = leaves[~p]; auto& right = leaves[~q];\n  \
    \          for (int i = 0; i < WORDS; ++i) { left.words[i] = i < HALF ? w[i] :\
    \ 0; right.words[i] = i < WORDS-HALF+1 ? w[i+HALF] : 0; }\n            left.count\
    \ = HALF*64; right.count = CAP+1-HALF*64;\n#if EEZ_BV_BUFFER > 0\n           \
    \ left.edit_count = right.edit_count = 0;\n#endif\n            leaf_counts(left);\
    \ leaf_counts(right);\n            sibling = q; return true;\n        }\n    \
    \    int i = locate(pool[p],k), offset = i ? pool[p].sizes[i-1] : 0;\n       \
    \ rank += i ? pool[p].ones[i-1] : 0;\n        int q;\n        if (!ins(pool[p].child[i],k-offset,bit,rank,q))\
    \ { adjust(pool[p],i,1,int(bit)); return false; }\n        int children[FANOUT+1],\
    \ count = pool[p].count;\n        for (int j = 0; j <= i; ++j) children[j] = pool[p].child[j];\n\
    \        children[i+1] = q;\n        for (int j = i+1; j < count; ++j) children[j+1]\
    \ = pool[p].child[j];\n        if (count < FANOUT) {\n            pool[p].count\
    \ = count+1;\n            for (int j = 0; j <= count; ++j) pool[p].child[j] =\
    \ children[j];\n            inner_counts(p); return false;\n        }\n      \
    \  sibling = alloc_node(); pool[p].count = FANOUT/2; pool[sibling].count = FANOUT/2+1;\n\
    \        for (int j = 0; j < FANOUT/2; ++j) pool[p].child[j] = children[j];\n\
    \        for (int j = 0; j < FANOUT/2+1; ++j) pool[sibling].child[j] = children[j+FANOUT/2];\n\
    \        inner_counts(p); inner_counts(sibling); return true;\n    }\n    // Empty\
    \ nodes are removed by their parent, keeping live leaves at equal depth.\n   \
    \ bool del(int p,int k,int& rank,bool& bit) {\n        if (p < 0) {\n        \
    \    auto& a = leaves[~p];\n#if EEZ_BV_BUFFER > 0\n            if (a.edit_count\
    \ || (a.ones[GROUPS-1] && a.ones[GROUPS-1] != a.count)) {\n                auto\
    \ q = leaf_access_rank(a,k); rank += q.second; bit = q.first; --a.count; record_edit(a,k,1,bit,false);\
    \ return a.count == 0;\n            }\n#endif\n            if (!a.ones[GROUPS-1])\
    \ { bit = false; return --a.count == 0; }\n            if (a.ones[GROUPS-1] ==\
    \ a.count) {\n                bit = true; rank += k; --a.count; int j = a.count\
    \ >> 6;\n                a.words[j] &= ~(u64(1) << (a.count & 63));\n        \
    \        add_leaf_ones(a,j/GROUP,-1);\n                return a.count == 0;\n\
    \            }\n            rank += leaf_rank(a,k); bit = (a.words[k >> 6] >>\
    \ (k & 63)) & 1;\n            erase_plain(a,k,bit); return a.count == 0;\n   \
    \     }\n        auto& a = pool[p]; int i = locate(a,k); rank += i ? a.ones[i-1]\
    \ : 0;\n        if (!del(a.child[i],k-(i ? a.sizes[i-1] : 0),rank,bit)) { adjust(a,i,-1,-int(bit));\
    \ return false; }\n        release(a.child[i]);\n        for (int j = i+1; j <\
    \ a.count; ++j) a.child[j-1] = a.child[j];\n        if (--a.count == 0) return\
    \ true;\n        inner_counts(p); return false;\n    }\n    struct PathStep {\
    \ int node, child; };\n    // Insert an already-split right sibling into an inner\
    \ node.\n    bool attach(int p,int i,int& sibling) {\n        int count = pool[p].count;\n\
    \        if (count < FANOUT) {\n            auto& a = pool[p];\n            std::memmove(a.child+i+2,a.child+i+1,std::size_t(count-i-1)*sizeof(int));\n\
    \            a.child[i+1] = sibling; ++a.count; inner_counts(p); return false;\n\
    \        }\n        int children[FANOUT+1];\n        std::memcpy(children,pool[p].child,std::size_t(i+1)*sizeof(int));\n\
    \        children[i+1] = sibling;\n        std::memcpy(children+i+2,pool[p].child+i+1,std::size_t(count-i-1)*sizeof(int));\n\
    \        sibling = alloc_node(); pool[p].count = FANOUT/2; pool[sibling].count\
    \ = FANOUT/2+1;\n        std::memcpy(pool[p].child,children,std::size_t(FANOUT/2)*sizeof(int));\n\
    \        std::memcpy(pool[sibling].child,children+FANOUT/2,std::size_t(FANOUT/2+1)*sizeof(int));\n\
    \        inner_counts(p); inner_counts(sibling); return true;\n    }\n#if EEZ_BV_RANGE_UPDATE\n\
    \    static void adjust_move(Node& a,int from,int to,bool old,bool bit){\n   \
    \     if(from<to){adjust_range(a,from,to,-1,-int(old));adjust_range(a,to,a.count,0,int(bit)-int(old));}\n\
    \        else if(to<from){adjust_range(a,to,from,1,int(bit));adjust_range(a,from,a.count,0,int(bit)-int(old));}\n\
    \        else adjust_range(a,from,a.count,0,int(bit)-int(old));\n    }\n#else\n\
    \    static void adjust_move(Node& a,int from,int to,bool old,bool bit) {\n#if\
    \ defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n        const auto f = _mm256_set1_epi32(from-1),\
    \ t = _mm256_set1_epi32(to-1);\n        const auto end = _mm256_set1_epi32(a.count);\n\
    \        for (int j = std::min(from,to) & ~7; j < a.count; j += 8) {\n       \
    \     auto ix = _mm256_setr_epi32(j,j+1,j+2,j+3,j+4,j+5,j+6,j+7);\n          \
    \  auto live = _mm256_cmpgt_epi32(end,ix);\n            auto fm = _mm256_and_si256(live,_mm256_cmpgt_epi32(ix,f));\n\
    \            auto tm = _mm256_and_si256(live,_mm256_cmpgt_epi32(ix,t));\n    \
    \        if (from != to) {\n                auto p = reinterpret_cast<__m256i*>(a.sizes+j);\n\
    \                _mm256_store_si256(p,_mm256_add_epi32(_mm256_load_si256(p),_mm256_sub_epi32(fm,tm)));\n\
    \            }\n            if (old || bit) {\n                auto p = reinterpret_cast<__m256i*>(a.ones+j);\n\
    \                auto d = _mm256_sub_epi32(_mm256_and_si256(fm,_mm256_set1_epi32(-int(old))),_mm256_and_si256(tm,_mm256_set1_epi32(-int(bit))));\n\
    \                _mm256_store_si256(p,_mm256_add_epi32(_mm256_load_si256(p),d));\n\
    \            }\n        }\n#else\n        for (int j = std::min(from,to); j <\
    \ a.count; ++j) {\n            a.sizes[j] += int(j >= to)-int(j >= from);\n  \
    \          a.ones[j] += int(bit && j >= to)-int(old && j >= from);\n        }\n\
    #endif\n    }\n#endif\npublic:\n    DynamicBitVector() = default;\n    DynamicBitVector(const\
    \ DynamicBitVector& other)\n        : leaves(other.leaves),pool(other.pool),root(other.root),free_leaf(other.free_leaf),free_node(other.free_node),length(other.length),total_ones(other.total_ones)\
    \ {\n#if EEZ_BV_STATIC_INDEX\n        if (other.read_index) {\n            std::size_t\
    \ count = (std::size_t(length)+63)/64;\n            read_index.reset(new StaticWord[count]);\
    \ std::memcpy(read_index.get(),other.read_index.get(),count*sizeof(StaticWord));\n\
    \        }\n#endif\n    }\n    DynamicBitVector& operator=(const DynamicBitVector&\
    \ b) { if (this != &b) { DynamicBitVector tmp(b); swap(tmp); } return *this; }\n\
    \    DynamicBitVector(DynamicBitVector&& b) noexcept { swap(b); }\n    DynamicBitVector&\
    \ operator=(DynamicBitVector&& b) noexcept { if (this != &b) { DynamicBitVector\
    \ tmp(std::move(b)); swap(tmp); } return *this; }\n    void swap(DynamicBitVector&\
    \ b) noexcept {\n#if EEZ_BV_STATIC_INDEX\n        read_index.swap(b.read_index);\n\
    #endif\n        using std::swap; swap(leaves,b.leaves); swap(pool,b.pool); swap(root,b.root);\n\
    \        swap(free_leaf,b.free_leaf); swap(free_node,b.free_node); swap(length,b.length);\
    \ swap(total_ones,b.total_ones);\n    }\n    int size() const { return length;\
    \ }\n    bool empty() const { return length == 0; }\n    void clear() {\n#if EEZ_BV_STATIC_INDEX\n\
    \        read_index.reset();\n#endif\n        leaves.clear(); pool.clear(); root\
    \ = -1; free_leaf = free_node = -1; length = total_ones = 0; }\n    void reserve(int\
    \ n) { assert(n >= 0); leaves.reserve(std::size_t(n)/(HALF*64)+2); pool.reserve(std::size_t(n)/(HALF*64)/(FANOUT/2)+16);\
    \ }\n    std::vector<u64> export_packed(){\n        std::vector<u64> out((std::size_t(length)+63)/64,total_ones==length?~u64(0):0);\n\
    \        if(total_ones && total_ones!=length){int offset=0;collect_packed(root,out.data(),offset);assert(offset==length);}\n\
    \        if((length&63)&&!out.empty())out.back()&=mask(length&63);\n        return\
    \ out;\n    }\n    void discard_query_index(){\n#if EEZ_BV_STATIC_INDEX\n    \
    \    read_index.reset();\n#endif\n    }\n    // Packed input is little-endian\
    \ within each word; padding bits are ignored.\n    void build_packed(const std::vector<u64>&\
    \ words,int n){build_packed(words,n,true);}\n    void build_packed(const std::vector<u64>&\
    \ words,int n,bool use_index) {\n        assert(n >= 0 && words.size() >= (std::size_t(n)+63)/64);\n\
    \        clear(); reserve(n); if (!n) return;\n        std::vector<int> layer;\
    \ layer.reserve((std::size_t(n)+BUILD_BITS-1)/BUILD_BITS);\n        for (int pos\
    \ = 0; pos < n;) {\n            int p = alloc_leaf(), len = std::min(BUILD_BITS,n-pos);\
    \ auto& leaf = leaves[~p];\n            leaf.count = len; int count = (len+63)/64;\n\
    \            for (int j = 0; j < count; ++j) leaf.words[j] = words[std::size_t(pos)/64+j];\n\
    \            if (len & 63) leaf.words[count-1] &= mask(len & 63);\n          \
    \  leaf_counts(leaf); layer.push_back(p); pos += len;\n        }\n        while\
    \ (layer.size() > 1) {\n            std::vector<int> next; next.reserve((layer.size()+FANOUT-1)/FANOUT);\n\
    \            for (std::size_t i = 0; i < layer.size();) {\n                int\
    \ p = alloc_node(), count = int(std::min<std::size_t>(FANOUT,layer.size()-i));\
    \ pool[p].count = count;\n                for (int j = 0; j < count; ++j) pool[p].child[j]\
    \ = layer[i+j];\n                inner_counts(p); next.push_back(p); i += count;\n\
    \            }\n            layer.swap(next);\n        }\n        root = layer[0];\
    \ length = n; total_ones = ones_of(root);\n#if EEZ_BV_STATIC_INDEX\n        if(use_index)index_packed(words.data());\n\
    #endif\n    }\n    void build(const std::vector<std::uint8_t>& a){build(a,true);}\n\
    \    void build(const std::vector<std::uint8_t>& a,bool use_index) {\n       \
    \ assert(a.size() <= std::size_t(INT_MAX));\n        clear(); reserve(int(a.size()));\
    \ if (a.empty()) return;\n        std::vector<int> layer; layer.reserve((a.size()+BUILD_BITS-1)/BUILD_BITS);\n\
    \        for (std::size_t pos = 0; pos < a.size();) {\n            int p = alloc_leaf(),\
    \ len = int(std::min<std::size_t>(BUILD_BITS,a.size()-pos)); auto& leaf = leaves[~p];\n\
    \            leaf.count = len; int j = 0;\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n\
    \            for (; j+64 <= len; j += 64) {\n                const auto zero =\
    \ _mm256_setzero_si256();\n                auto lo = _mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(a.data()+pos+j));\n                auto hi = _mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(a.data()+pos+j+32));\n                auto l = std::uint32_t(~_mm256_movemask_epi8(_mm256_cmpeq_epi8(lo,zero)));\n\
    \                auto h = std::uint32_t(~_mm256_movemask_epi8(_mm256_cmpeq_epi8(hi,zero)));\n\
    \                leaf.words[j>>6] = u64(l) | (u64(h)<<32);\n            }\n#endif\n\
    \            for (; j < len; ++j) leaf.words[j>>6] |= u64(a[pos+j] != 0) << (j&63);\n\
    \            leaf_counts(leaf); layer.push_back(p); pos += len;\n        }\n \
    \       while (layer.size() > 1) {\n            std::vector<int> next; next.reserve((layer.size()+FANOUT-1)/FANOUT);\n\
    \            for (std::size_t i = 0; i < layer.size();) {\n                int\
    \ p = alloc_node(), count = int(std::min<std::size_t>(FANOUT,layer.size()-i));\
    \ pool[p].count = count;\n                for (int j = 0; j < count; ++j) pool[p].child[j]\
    \ = layer[i+j];\n                inner_counts(p); next.push_back(p); i += count;\n\
    \            }\n            layer.swap(next);\n        }\n        root = layer[0];\
    \ length = int(a.size()); total_ones = ones_of(root);\n#if EEZ_BV_STATIC_INDEX\n\
    \        if(use_index)prepare_queries();\n#endif\n    }\n#if defined(__GNUC__)\
    \ || defined(__clang__)\n    __attribute__((always_inline)) inline\n#elif defined(_MSC_VER)\n\
    \    __forceinline\n#endif\n    std::pair<bool,int> access_rank(int k) const {\n\
    \        assert(0 <= k && k < size());\n        if (!total_ones) return {false,0};\n\
    \        if (total_ones == length) return {true,k};\n#if EEZ_BV_STATIC_INDEX\n\
    \        if (read_index) return indexed_access_rank(k);\n#endif\n        int p\
    \ = root, rank = 0;\n        while (p >= 0) {\n            const auto& a = pool[p];\
    \ int i = locate(a,k);\n            if (i) { k -= a.sizes[i-1]; rank += a.ones[i-1];\
    \ } p = a.child[i]; prefetch(p);\n        }\n        auto q = leaf_access_rank(leaves[~p],k);\
    \ q.second += rank; return q;\n    }\n    bool access(int k) const {\n       \
    \ assert(0 <= k && k < size());\n        if (!total_ones) return false;\n    \
    \    if (total_ones == length) return true;\n#if EEZ_BV_STATIC_INDEX\n       \
    \ if (read_index) return (read_index[k >> 6].bits >> (k & 63)) & 1;\n#endif\n\
    \        int p = root;\n        while (p >= 0) { const auto& a = pool[p]; int\
    \ i = locate(a,k); if (i) k -= a.sizes[i-1]; p = a.child[i]; prefetch(p); }\n\
    \        return leaf_access(leaves[~p],k);\n    }\n    int rank1(int k) const\
    \ {\n        assert(0 <= k && k <= size()); if (!k || !total_ones) return 0;\n\
    \        if (total_ones == length) return k;\n        if (k == length) return\
    \ total_ones;\n#if EEZ_BV_STATIC_INDEX\n        if (read_index) return indexed_rank(k);\n\
    #endif\n        int p = root, rank = 0;\n        while (p >= 0) { const auto&\
    \ a = pool[p]; int i = locate(a,k); if (i) { k -= a.sizes[i-1]; rank += a.ones[i-1];\
    \ } p = a.child[i]; prefetch(p); }\n        return rank+leaf_rank(leaves[~p],k);\n\
    \    }\n    int rank0(int k) const { return k-rank1(k); }\n#if defined(__GNUC__)\
    \ || defined(__clang__)\n    __attribute__((always_inline)) inline\n#elif defined(_MSC_VER)\n\
    \    __forceinline\n#endif\n    std::pair<int,int> rank1_pair(int l,int r) const\
    \ {\n        assert(0 <= l && l <= r && r <= size());\n        if (!total_ones)\
    \ return {0,0};\n        if (total_ones == length) return {l,r};\n#if EEZ_BV_STATIC_INDEX\n\
    \        if (read_index) return {indexed_rank(l),indexed_rank(r)};\n#endif\n \
    \       if (l == r) { int o = rank1(l); return {o,o}; }\n        if (r-l == 1)\
    \ { auto q = access_rank(l); return {q.second,q.second+int(q.first)}; }\n    \
    \    if (!l) return {0,rank1(r)};\n        if (r == length) return {rank1(l),total_ones};\n\
    \        int p = root, base = 0;\n        while (p >= 0) {\n            const\
    \ auto& a = pool[p]; int i = locate(a,l), j = r <= a.sizes[i] ? i : locate(a,r);\n\
    \            if (i != j) {\n                int ol = i ? a.ones[i-1] : 0, orr\
    \ = j ? a.ones[j-1] : 0;\n                l -= i ? a.sizes[i-1] : 0; r -= j ?\
    \ a.sizes[j-1] : 0;\n                int left = a.child[i], right = a.child[j];\n\
    \                while (left >= 0) {\n                    const auto& al = pool[left];\
    \ const auto& ar = pool[right];\n                    int il = locate(al,l), ir\
    \ = locate(ar,r);\n                    if (il) { l -= al.sizes[il-1]; ol += al.ones[il-1];\
    \ }\n                    if (ir) { r -= ar.sizes[ir-1]; orr += ar.ones[ir-1];\
    \ }\n                    left = al.child[il]; right = ar.child[ir]; prefetch(left);\
    \ prefetch(right);\n                }\n                return {base+ol+leaf_rank(leaves[~left],l),base+orr+leaf_rank(leaves[~right],r)};\n\
    \            }\n            if (i) { l -= a.sizes[i-1]; r -= a.sizes[i-1]; base\
    \ += a.ones[i-1]; }\n            p = a.child[i]; prefetch(p);\n        }\n   \
    \     auto q = leaf_pair(leaves[~p],l,r); q.first += base; q.second += base; return\
    \ q;\n    }\n    int count1(int l,int r) const { auto q = rank1_pair(l,r); return\
    \ q.second-q.first; }\n    std::pair<bool,int> set_rank(int k,bool bit) {\n  \
    \      assert(0 <= k && k < size());\n        if (!total_ones && !bit) return\
    \ {false,0};\n        if (total_ones == length && bit) return {true,k};\n#if EEZ_BV_STATIC_INDEX\n\
    \        if (read_index) { auto q = indexed_access_rank(k); if (q.first == bit)\
    \ return q; read_index.reset(); }\n#endif\n        PathStep path[16]; int depth\
    \ = 0, p = root, local = k, rank = 0;\n        while (p >= 0 && depth < 16) {\n\
    \            const auto& a = pool[p]; int i = locate(a,local);\n            path[depth++]\
    \ = {p,i};\n            if (i) { local -= a.sizes[i-1]; rank += a.ones[i-1]; }\n\
    \            p = a.child[i]; prefetch(p);\n        }\n        std::pair<bool,int>\
    \ result;\n        if (p >= 0) result = replace(root,k,bit);\n        else {\n\
    \            result = replace(p,local,bit); result.second += rank;\n         \
    \   if (result.first != bit) for (int d = depth-1; d >= 0; --d) adjust(pool[path[d].node],path[d].child,0,int(bit)-int(result.first));\n\
    \        }\n        total_ones += int(bit)-int(result.first); return result;\n\
    \    }\n    bool set(int k,bool bit) { return set_rank(k,bit).first; }\n    struct\
    \ MoveRanks { bool previous; int old_rank, new_rank; };\n    // Equivalent to\
    \ erase_rank(from), then insert_rank(to,bit). 'to' is\n    // an insertion position\
    \ after erasure. Fuse searches when no split/removal\n    // is needed; fall back\
    \ to the general operations for structural changes.\n    MoveRanks move_rank(int\
    \ from,int to,bool bit) {\n        assert(0 <= from && from < length && 0 <= to\
    \ && to < length);\n        if (!total_ones && !bit) return {false,0,0};\n   \
    \     if (total_ones == length && bit) return {true,from,to};\n        if (from\
    \ == to) { auto q = set_rank(from,bit); return {q.first,q.second,q.second}; }\n\
    #if EEZ_BV_STATIC_INDEX\n        read_index.reset();\n#endif\n        struct Step\
    \ { int node, child; } left_path[16], right_path[16];\n        int left = root,\
    \ right = root, l = from, r = to, ol = 0, orr = 0, depth = 0;\n        while (left\
    \ >= 0) {\n            if (depth == 16) { auto q = erase_rank(from); int o = insert_rank(to,bit);\
    \ return {q.first,q.second,o}; }\n            const auto& a = pool[left]; const\
    \ auto& b = pool[right];\n            int i,j;\n#if EEZ_BV_MOVE_PAIR\n       \
    \     if(left==right){auto q=locate_two(a,l,r);i=q.first;j=q.second;}\n      \
    \      else\n#endif\n            {i=locate(a,l);j=locate(b,r);}\n            left_path[depth]\
    \ = {left,i}; right_path[depth] = {right,j}; ++depth;\n            if (i) { l\
    \ -= a.sizes[i-1]; ol += a.ones[i-1]; }\n            if (j) { r -= b.sizes[j-1];\
    \ orr += b.ones[j-1]; }\n            left = a.child[i]; right = b.child[j];\n\
    \        }\n        if (left != right && (leaves[~left].count == 1 || leaves[~right].count\
    \ == CAP)) {\n            auto q = erase_rank(from); int o = insert_rank(to,bit);\
    \ return {q.first,q.second,o};\n        }\n        bool old; del(left,l,ol,old);\n\
    \        if (left != right && from < to) { ++r; orr -= int(old); }\n        int\
    \ unused; [[maybe_unused]] bool split = ins(right,r,bit,orr,unused); assert(!split);\n\
    \        for (int d = 0; d < depth; ++d) {\n            auto a = left_path[d],\
    \ b = right_path[d];\n            if (a.node == b.node) {\n                if\
    \ (a.child != b.child || old != bit) adjust_move(pool[a.node],a.child,b.child,old,bit);\n\
    \            } else {\n                adjust(pool[a.node],a.child,-1,-int(old));\n\
    \                adjust(pool[b.node],b.child,1,int(bit));\n            }\n   \
    \     }\n        total_ones += int(bit)-int(old); return {old,ol,orr};\n    }\n\
    \    template<int WIDTH> void access_rank_batch(const int* indices,bool* bits,int*\
    \ ranks) const {\n        static_assert(WIDTH > 0); int nodes[WIDTH], offsets[WIDTH];\n\
    #if EEZ_BV_STATIC_INDEX\n        if (read_index) {\n            for (int j = 0;\
    \ j < WIDTH; ++j) { assert(0 <= indices[j] && indices[j] < size()); auto q = indexed_access_rank(indices[j]);\
    \ bits[j] = q.first; ranks[j] = q.second; }\n            return;\n        }\n\
    #endif\n        if (!total_ones || total_ones == length) {\n            bool bit\
    \ = total_ones != 0;\n            for (int j = 0; j < WIDTH; ++j) { assert(0 <=\
    \ indices[j] && indices[j] < size()); bits[j] = bit; ranks[j] = bit ? indices[j]\
    \ : 0; }\n            return;\n        }\n        for (int j = 0; j < WIDTH; ++j)\
    \ { assert(0 <= indices[j] && indices[j] < size()); nodes[j] = root; offsets[j]\
    \ = indices[j]; ranks[j] = 0; }\n        while (nodes[0] >= 0) {\n           \
    \ for (int j = 0; j < WIDTH; ++j) {\n                const auto& a = pool[nodes[j]];\
    \ int i = locate(a,offsets[j]);\n                if (i) { offsets[j] -= a.sizes[i-1];\
    \ ranks[j] += a.ones[i-1]; } nodes[j] = a.child[i]; prefetch(nodes[j]);\n    \
    \        }\n        }\n        for (int j = 0; j < WIDTH; ++j) { const auto& a\
    \ = leaves[~nodes[j]]; int k = offsets[j]; auto q = leaf_access_rank(a,k); bits[j]\
    \ = q.first; ranks[j] += q.second; }\n    }\n    // Optional after an update batch.\
    \ Const queries never allocate or mutate.\n    void prepare_queries() {\n#if EEZ_BV_STATIC_INDEX\n\
    \        if (read_index || !total_ones || total_ones == length) return;\n    \
    \    std::size_t count = (std::size_t(length)+63)/64;\n        std::unique_ptr<StaticWord[]>\
    \ next(new StaticWord[count]);\n        for (std::size_t i = 0; i < count; ++i)\
    \ next[i].bits = 0;\n        int offset = 0; collect_words(root,next.get(),offset);\
    \ assert(offset == length);\n        int rank = 0;\n        for (std::size_t i\
    \ = 0; i < count; ++i) { next[i].prefix = rank; rank += pc(next[i].bits); }\n\
    \        read_index = std::move(next);\n#endif\n    }\n    int insert_rank(int\
    \ k,bool bit) {\n        assert(0 <= k && k <= size()); assert(length < INT_MAX);\n\
    #if EEZ_BV_STATIC_INDEX\n        read_index.reset();\n#endif\n        if (!length)\
    \ root = alloc_leaf();\n        PathStep path[16]; int depth = 0, p = root, local\
    \ = k, rank = 0, sibling;\n        while (p >= 0 && depth < 16) {\n          \
    \  const auto& a = pool[p]; int i = locate(a,local);\n            path[depth++]\
    \ = {p,i};\n            if (i) { local -= a.sizes[i-1]; rank += a.ones[i-1]; }\n\
    \            p = a.child[i]; prefetch(p);\n        }\n        bool split;\n  \
    \      if (p >= 0) { rank = 0; split = ins(root,k,bit,rank,sibling); }\n     \
    \   else {\n            split = ins(p,local,bit,rank,sibling);\n            for\
    \ (int d = depth-1; d >= 0; --d) {\n                if (split) split = attach(path[d].node,path[d].child,sibling);\n\
    \                else adjust(pool[path[d].node],path[d].child,1,int(bit));\n \
    \           }\n        }\n        if (split) {\n            p = alloc_node();\
    \ pool[p].count = 2;\n            pool[p].child[0] = root; pool[p].child[1] =\
    \ sibling; inner_counts(p); root = p;\n        }\n        ++length; total_ones\
    \ += int(bit); return rank;\n    }\n    void insert(int k,bool bit) { (void)insert_rank(k,bit);\
    \ }\n    std::pair<bool,int> erase_rank(int k) {\n        assert(0 <= k && k <\
    \ size());\n#if EEZ_BV_STATIC_INDEX\n        read_index.reset();\n#endif\n   \
    \     PathStep path[16]; int depth = 0, p = root, local = k, rank = 0; bool bit;\n\
    \        while (p >= 0 && depth < 16) {\n            const auto& a = pool[p];\
    \ int i = locate(a,local);\n            path[depth++] = {p,i};\n            if\
    \ (i) { local -= a.sizes[i-1]; rank += a.ones[i-1]; }\n            p = a.child[i];\
    \ prefetch(p);\n        }\n        bool empty;\n        if (p >= 0) { rank = 0;\
    \ empty = del(root,k,rank,bit); }\n        else {\n            empty = del(p,local,rank,bit);\n\
    \            for (int d = depth-1; d >= 0; --d) {\n                auto& a = pool[path[d].node];\
    \ int i = path[d].child;\n                if (!empty) adjust(a,i,-1,-int(bit));\n\
    \                else {\n                    release(a.child[i]);\n          \
    \          std::memmove(a.child+i,a.child+i+1,std::size_t(a.count-i-1)*sizeof(int));\n\
    \                    empty = --a.count == 0;\n                    if (!empty)\
    \ inner_counts(path[d].node);\n                }\n            }\n        }\n \
    \       if (empty) { release(root); root = -1; }\n        else while (root >=\
    \ 0 && pool[root].count == 1) { int old = root; root = pool[old].child[0]; release(old);\
    \ }\n        --length; total_ones -= int(bit); return {bit,rank};\n    }\n   \
    \ bool erase(int k) { return erase_rank(k).first; }\n};\n\n\n// Read-only 4-ary\
    \ wavelet matrix, invalidated before any edit.\ntemplate<class T,int LOG> class\
    \ QuadIndex {\n    using U=std::uint64_t;\n    static constexpr int LEVELS=(LOG+1)/2;\n\
    \    struct Block { U low,high; int prefix[4]; };\n    std::array<std::vector<Block>,LEVELS>\
    \ data;\n    std::array<std::array<int,4>,LEVELS> start,total;\n    int n=0;\n\
    \    std::array<int,LEVELS> uniform;\n    static U mask(int k){return k==64?~U(0):(U(1)<<k)-1;}\n\
    \    static int pc(U x){\n#if defined(__GNUC__) || defined(__clang__)\n      \
    \  return __builtin_popcountll(x);\n#else\n        int n=0;while(x){x&=x-1;++n;}return\
    \ n;\n#endif\n    }\n    static U matches(const Block& b,int c){return (c&1?b.low:~b.low)&(c&2?b.high:~b.high);}\n\
    \    int rank(int level,int c,int k)const{\n        if(uniform[level]>=0)return\
    \ uniform[level]==c?k:0;\n        if(k==n)return total[level][c];\n        const\
    \ auto& b=data[level][k>>6];return b.prefix[c]+pc(matches(b,c)&mask(k&63));\n\
    \    }\n    std::array<int,4> counts(int level,int l,int r)const{\n        std::array<int,4>\
    \ out;\n        for(int c=0;c<4;++c)out[c]=rank(level,c,r)-rank(level,c,l);\n\
    \        return out;\n    }\npublic:\n    QuadIndex(){uniform.fill(-1);}\n   \
    \ // The binary builder orders each pair's buckets 0,2,1,3.\n    void append(const\
    \ std::vector<T>& cur,int binary_level){\n        bool single=(LOG&1)&&binary_level==0;\n\
    \        if(!single && ((LOG-binary_level)&1))return;\n        n=int(cur.size());int\
    \ level=(binary_level+1)/2;uniform[level]=-1;\n        int shift=2*(LEVELS-level-1);\n\
    \        auto& blocks=data[level];blocks.resize((std::size_t(n)+63)/64);\n   \
    \     std::array<int,4> count{};\n        for(int i=0;i<n;i+=64){\n          \
    \  auto& b=blocks[i>>6];b.low=b.high=0;\n            for(int c=0;c<4;++c)b.prefix[c]=count[c];\n\
    \            int end=std::min(n,i+64),j=i;\n#if defined(__AVX2__) && !defined(EEZ_NO_SIMD)\n\
    \            if constexpr(sizeof(T)==4){\n                auto sh=_mm256_set1_epi32(shift),one=_mm256_set1_epi32(1);\n\
    \                for(;j+8<=end;j+=8){\n                    auto v=_mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(cur.data()+j));v=_mm256_srlv_epi32(v,sh);\n                    auto\
    \ lo=_mm256_cmpeq_epi32(_mm256_and_si256(v,one),one);\n                    auto\
    \ hi=_mm256_cmpeq_epi32(_mm256_and_si256(_mm256_srli_epi32(v,1),one),one);\n \
    \                   b.low|=U(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(lo))))<<(j-i);\n\
    \                    b.high|=U(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(hi))))<<(j-i);\n\
    \                }\n            }\n#endif\n            for(;j<end;++j){int c=int((U(cur[j])>>shift)&3);b.low|=U(c&1)<<(j-i);b.high|=U(c>>1)<<(j-i);}\n\
    \            U valid=mask(end-i);int lo=pc(b.low&valid),hi=pc(b.high&valid),both=pc(b.low&b.high&valid);\n\
    \            count[0]+=end-i-lo-hi+both;count[1]+=lo-both;count[2]+=hi-both;count[3]+=both;\n\
    \        }\n        total[level]=count;\n        for(int c=0;c<4;++c)if(count[c]==n){uniform[level]=c;std::vector<Block>().swap(blocks);break;}\n\
    \        int sum=0;\n        for(int j=0;j<4;++j){int c=single?j:((j&1)<<1)|(j>>1);start[level][c]=sum;sum+=count[c];}\n\
    \    }\n    static U take(const std::vector<U>& words,int pos,int count){\n  \
    \      if(!count)return 0;\n        int shift=pos&63;U value=words[std::size_t(pos)/64]>>shift;\n\
    \        if(shift&&count>64-shift)value|=words[std::size_t(pos)/64+1]<<(64-shift);\n\
    \        return value&mask(count);\n    }\n    static U deposit(U value,U positions){\n\
    #if defined(__BMI2__) && !defined(EEZ_NO_SIMD)\n        return _pdep_u64(value,positions);\n\
    #else\n        U out=0;while(positions){U bit=positions&-positions;out|=bit*U(value&1);positions&=positions-1;value>>=1;}return\
    \ out;\n#endif\n    }\n    void append_binary(const std::vector<U>& high,const\
    \ std::vector<U>& low,int length,int binary_level,int zeros){\n        bool single=(LOG&1)&&binary_level==0;\n\
    \        n=length;int level=(binary_level+1)/2;uniform[level]=-1;\n        auto&\
    \ blocks=data[level];blocks.resize((std::size_t(n)+63)/64);\n        std::array<int,4>\
    \ count{};int zp=0,op=zeros;\n        for(int i=0;i<n;i+=64){\n            int\
    \ valid=std::min(64,n-i);auto& b=blocks[i>>6];\n            for(int c=0;c<4;++c)b.prefix[c]=count[c];\n\
    \            if(single){b.low=high[i>>6]&mask(valid);b.high=0;}\n            else{\n\
    \                b.high=high[i>>6]&mask(valid);int ones=pc(b.high),zero=valid-ones;\n\
    \                b.low=deposit(take(low,zp,zero),~b.high&mask(valid))|deposit(take(low,op,ones),b.high);\n\
    \                zp+=zero;op+=ones;\n            }\n            int lo=pc(b.low),hi=pc(b.high),both=pc(b.low&b.high);\n\
    \            count[0]+=valid-lo-hi+both;count[1]+=lo-both;count[2]+=hi-both;count[3]+=both;\n\
    \        }\n        total[level]=count;\n        for(int c=0;c<4;++c)if(count[c]==n){uniform[level]=c;std::vector<Block>().swap(blocks);break;}\n\
    \        int sum=0;for(int j=0;j<4;++j){int c=single?j:((j&1)<<1)|(j>>1);start[level][c]=sum;sum+=count[c];}\n\
    \    }\n    int less_rank(int level,int c,int k)const{\n        if(uniform[level]>=0)return\
    \ uniform[level]<c?k:0;\n        if(!c)return 0;\n        if(c==1)return rank(level,0,k);\n\
    \        if(c==3)return k-rank(level,3,k);\n        if(k==n)return total[level][0]+total[level][1];\n\
    \        const auto& b=data[level][k>>6];return b.prefix[0]+b.prefix[1]+pc(~b.high&mask(k&63));\n\
    \    }\n    T access(int k)const{\n        U value=0;\n        for(int level=0;level<LEVELS;++level){\n\
    \            if(uniform[level]>=0){value=(value<<2)|U(uniform[level]);continue;}\n\
    \            const auto& b=data[level][k>>6];int shift=k&63;\n            int\
    \ c=int((b.low>>shift)&1)|int(((b.high>>shift)&1)<<1);\n            value=(value<<2)|U(c);k=start[level][c]+b.prefix[c]+pc(matches(b,c)&mask(shift));\n\
    \        }\n        return T(value);\n    }\n    void access_many(const int* ix,std::size_t\
    \ count,T* output)const{\n        for(std::size_t base=0;base<count;base+=8){\n\
    \            int size=int(std::min<std::size_t>(8,count-base));int pos[8];U values[8]{};\n\
    \            for(int j=0;j<size;++j){assert(ix[base+j]>=0&&ix[base+j]<n);pos[j]=ix[base+j];}\n\
    \            for(int level=0;level<LEVELS;++level)for(int j=0;j<size;++j){\n \
    \               if(uniform[level]>=0){values[j]=(values[j]<<2)|U(uniform[level]);continue;}\n\
    \                int k=pos[j],shift=k&63;const auto& b=data[level][k>>6];\n  \
    \              int c=int((b.low>>shift)&1)|int(((b.high>>shift)&1)<<1);\n    \
    \            values[j]=(values[j]<<2)|U(c);pos[j]=start[level][c]+b.prefix[c]+pc(matches(b,c)&mask(shift));\n\
    \            }\n            for(int j=0;j<size;++j)output[base+j]=T(values[j]);\n\
    \        }\n    }\n    int equal(T value,int l,int r)const{\n        for(int level=0;level<LEVELS&&l<r;++level){int\
    \ c=int((U(value)>>(2*(LEVELS-level-1)))&3);l=start[level][c]+rank(level,c,l);r=start[level][c]+rank(level,c,r);}\n\
    \        return r-l;\n    }\n    std::pair<int,int> rank_less(int level,int c,int\
    \ k)const{\n        if(uniform[level]>=0)return {uniform[level]==c?k:0,uniform[level]<c?k:0};\n\
    \        if(k==n){int less=0;for(int j=0;j<c;++j)less+=total[level][j];return\
    \ {total[level][c],less};}\n        const auto& b=data[level][k>>6];U m=mask(k&63);\n\
    \        int equal=b.prefix[c]+pc(matches(b,c)&m);\n        if(c==0)return {equal,0};if(c==3)return\
    \ {equal,k-equal};\n        int less=c==1?b.prefix[0]+pc(~(b.high|b.low)&m):b.prefix[0]+b.prefix[1]+pc(~b.high&m);\n\
    \        return {equal,less};\n    }\n    T extreme(int level,int l,int r,U value,bool\
    \ maximum)const{\n        for(;level<LEVELS;++level){\n            if(uniform[level]>=0){value=(value<<2)|U(uniform[level]);continue;}\n\
    \            int c=maximum?3:0,nl=0,nr=0;\n            for(;;){nl=rank(level,c,l);nr=rank(level,c,r);if(nl<nr)break;c+=maximum?-1:1;}\n\
    \            value=(value<<2)|U(c);l=start[level][c]+nl;r=start[level][c]+nr;\n\
    \        }\n        return T(value);\n    }\n    template<bool PREV,bool NEXT>std::pair<std::optional<T>,std::optional<T>>\
    \ neighbors(T x,int l,int r)const{\n        struct Candidate{int level=-1,l=0,r=0;U\
    \ prefix=0;};Candidate prev,next;U value=0;\n        std::pair<std::optional<T>,std::optional<T>>\
    \ result;\n        for(int level=0;level<LEVELS&&l<r;++level){\n            int\
    \ c=int((U(x)>>(2*(LEVELS-level-1)))&3);\n            if constexpr(PREV)for(int\
    \ d=c-1;d>=0;--d){int a=rank(level,d,l),b=rank(level,d,r);if(a<b){prev={level+1,start[level][d]+a,start[level][d]+b,(value<<2)|U(d)};break;}}\n\
    \            if constexpr(NEXT)for(int d=c+1;d<4;++d){int a=rank(level,d,l),b=rank(level,d,r);if(a<b){next={level+1,start[level][d]+a,start[level][d]+b,(value<<2)|U(d)};break;}}\n\
    \            value=(value<<2)|U(c);l=start[level][c]+rank(level,c,l);r=start[level][c]+rank(level,c,r);\n\
    \        }\n        if constexpr(PREV)if(prev.level>=0)result.first=extreme(prev.level,prev.l,prev.r,prev.prefix,true);\n\
    \        if constexpr(NEXT){if(l<r)result.second=x;else if(next.level>=0)result.second=extreme(next.level,next.l,next.r,next.prefix,false);}\n\
    \        return result;\n    }\n    int less(T value,int l,int r)const{\n    \
    \    int answer=0;\n        for(int level=0;level<LEVELS&&l<r;++level){\n    \
    \        int c=int((U(value)>>(2*(LEVELS-level-1)))&3);\n            auto a=rank_less(level,c,l),b=rank_less(level,c,r);answer+=b.second-a.second;\n\
    \            l=start[level][c]+a.first;r=start[level][c]+b.first;\n        }\n\
    \        return answer;\n    }\n    std::pair<int,int> less_equal(T value,int\
    \ l,int r)const{\n        int answer=0;\n        for(int level=0;level<LEVELS&&l<r;++level){\n\
    \            int c=int((U(value)>>(2*(LEVELS-level-1)))&3);\n            auto\
    \ a=rank_less(level,c,l),b=rank_less(level,c,r);answer+=b.second-a.second;\n \
    \           l=start[level][c]+a.first;r=start[level][c]+b.first;\n        }\n\
    \        return {answer,r-l};\n    }\n    struct Result {T value;int less,equal;};\n\
    \    Result kth(int l,int r,int k)const{\n        U value=0;int less=0;\n    \
    \    for(int level=0;level<LEVELS;++level){\n            if(uniform[level]>=0){value=(value<<2)|U(uniform[level]);continue;}\n\
    \            int hl=less_rank(level,2,l),hr=less_rank(level,2,r),zeros=hr-hl,c;\n\
    \            int nl,nr;\n            if(k<zeros){\n                int zl=rank(level,0,l),zr=rank(level,0,r),z=zr-zl;\n\
    \                if(k<z){c=0;nl=zl;nr=zr;}else{c=1;k-=z;less+=z;nl=hl-zl;nr=hr-zr;}\n\
    \            }else{\n                k-=zeros;less+=zeros;int zl=rank(level,2,l),zr=rank(level,2,r),z=zr-zl;\n\
    \                if(k<z){c=2;nl=zl;nr=zr;}else{c=3;k-=z;less+=z;nl=l-hl-zl;nr=r-hr-zr;}\n\
    \            }\n            value=(value<<2)|U(c);l=start[level][c]+nl;r=start[level][c]+nr;\n\
    \        }\n        return {T(value),less,r-l};\n    }\n};\n\ntemplate<class T=int,int\
    \ LOG=std::numeric_limits<T>::digits>\nclass DynamicWaveletMatrix{\n    std::optional<QuadIndex<T,LOG>>\
    \ quad;\n    bool quad_valid=false;\n    void invalidate_quad(){quad_valid=false;if\
    \ constexpr(!EEZ_QUAD_RETAIN)quad.reset();}\npublic:\n    struct Counts { int\
    \ less,equal,greater; };\n    struct Quantile { T value; int less,equal; };\n\
    \    struct Neighbors { std::optional<T> previous,next; };\nprivate:\n    static_assert(std::is_integral_v<T>);\n\
    \    static_assert(!std::is_same_v<T,bool>);\n    static_assert(1<=LOG&&LOG<=64);\n\
    \n    using U=std::make_unsigned_t<T>;\n    using u64=std::uint64_t;\n\n    std::array<DynamicBitVector,LOG>\
    \ bv;\n    std::array<int,LOG> mid{};\n    int n=0;\n\n    static bool negative(T\
    \ x){\n        if constexpr(std::is_signed_v<T>)return x<0;\n        else return\
    \ false;\n    }\n    static u64 as_u64(T x){\n        return (u64)(U)x;\n    }\n\
    \    static bool valid_value(T x){\n        if(negative(x))return false;\n   \
    \     if constexpr(LOG==64)return true;\n        else return as_u64(x)<(u64(1)<<LOG);\n\
    \    }\n    static bool bit(u64 x,int level){\n        return (x>>(LOG-1-level))&1;\n\
    \    }\n    void range_check([[maybe_unused]] int l,[[maybe_unused]] int r)const{\n\
    \        assert(0<=l&&l<=r&&r<=n);\n    }\n    int less_suffix(int level,int l,int\
    \ r,u64 v,int ans)const{\n        for(;level<LOG&&l<r;++level){\n            auto\
    \ [ol,orr]=bv[level].rank1_pair(l,r);\n            if(bit(v,level)){ans+=(r-l)-(orr-ol);l=mid[level]+ol;r=mid[level]+orr;}\n\
    \            else{l-=ol;r-=orr;}\n        }\n        return ans;\n    }\n    T\
    \ extreme_suffix(int level,int l,int r,u64 value,bool maximum)const{\n       \
    \ for(;level<LOG;++level){\n            auto [ol,orr]=bv[level].rank1_pair(l,r);\n\
    \            int zeros=(r-l)-(orr-ol);\n            bool b=maximum?(orr>ol):(zeros==0);\n\
    \            if(b){value|=u64(1)<<(LOG-1-level);l=mid[level]+ol;r=mid[level]+orr;}\n\
    \            else{l-=ol;r-=orr;}\n        }\n        return T(value);\n    }\n\
    \    template<bool PREV,bool NEXT>\n    Neighbors neighbors_impl(int l,int r,T\
    \ x)const{\n        range_check(l,r);Neighbors result{};if(l==r)return result;\n\
    \        if(quad_valid){\n            if(negative(x)){if constexpr(NEXT)result.next=quad->extreme(0,l,r,0,false);return\
    \ result;}\n            if(!valid_value(x)){if constexpr(PREV)result.previous=quad->extreme(0,l,r,0,true);return\
    \ result;}\n            auto q=quad->template neighbors<PREV,NEXT>(x,l,r);return\
    \ {q.first,q.second};\n        }\n\n        if(negative(x)){if constexpr(NEXT)result.next=extreme_suffix(0,l,r,0,false);return\
    \ result;}\n        if(!valid_value(x)){if constexpr(PREV)result.previous=extreme_suffix(0,l,r,0,true);return\
    \ result;}\n        struct Candidate{int level=-1,l=0,r=0;u64 value=0;};\n   \
    \     Candidate prev,next;u64 prefix=0,v=as_u64(x);\n        for(int level=0;level<LOG&&l<r;++level){\n\
    \            auto [ol,orr]=bv[level].rank1_pair(l,r);\n            int zl=l-ol,zr=r-orr;u64\
    \ m=u64(1)<<(LOG-1-level);\n            if(bit(v,level)){\n                if\
    \ constexpr(PREV)if(zl<zr)prev={level+1,zl,zr,prefix};\n                prefix|=m;l=mid[level]+ol;r=mid[level]+orr;\n\
    \            }else{\n                if constexpr(NEXT)if(ol<orr)next={level+1,mid[level]+ol,mid[level]+orr,prefix|m};\n\
    \                l=zl;r=zr;\n            }\n        }\n        if constexpr(PREV)if(prev.level>=0)result.previous=extreme_suffix(prev.level,prev.l,prev.r,prev.value,true);\n\
    \        if constexpr(NEXT){\n            if(l<r)result.next=x;\n            else\
    \ if(next.level>=0)result.next=extreme_suffix(next.level,next.l,next.r,next.value,false);\n\
    \        }\n        return result;\n    }\n\npublic:\n    using value_type=T;\n\
    \n    DynamicWaveletMatrix()=default;\n    explicit DynamicWaveletMatrix(const\
    \ std::vector<T>& a){build(a);}\n\n    int size()const{return n;}\n    bool empty()const{return\
    \ n==0;}\n\n    void clear(){\n        quad.reset();quad_valid=false; n=0;\n \
    \       mid.fill(0);\n        for(auto& b:bv)b.clear();\n    }\n    // Rebuild\
    \ read-only rank indexes after a batch of edits. Any later edit\n    // invalidates\
    \ its affected layers automatically; all existing APIs remain usable.\n    void\
    \ prepare_queries(){ if(quad_valid)return;for(auto& b:bv)b.prepare_queries();\
    \ }\n    // Rebuild directly from binary levels; no value-array reconstruction.\n\
    \    void prepare_quad_queries(){\n        if constexpr(!EEZ_WM_QUAD_INDEX)return;\n\
    \        if(quad_valid)return;\n        if(!quad)quad.emplace();auto& index=*quad;\n\
    \        for(int level=0;level<LOG;){\n            auto high=bv[level].export_packed();\n\
    \            if((LOG&1)&&level==0){index.append_binary(high,{},n,level,mid[level]);++level;}\n\
    \            else{auto low=bv[level+1].export_packed();index.append_binary(high,low,n,level,mid[level]);level+=2;}\n\
    \        }\n        quad_valid=true;\n        for(auto& b:bv)b.discard_query_index();\n\
    \    }\n    void reserve(int m){\n        for(auto& b:bv)b.reserve(m);\n    }\n\
    \    void build(const std::vector<T>& a){\n        assert(a.size()<=std::size_t(INT_MAX));\n\
    \        invalidate_quad(); n=(int)a.size();\n        if constexpr(EEZ_WM_QUAD_INDEX)if(!quad)quad.emplace();\n\
    \        for([[maybe_unused]] T x:a)assert(valid_value(x));\n\n#if defined(__AVX2__)\
    \ && !defined(EEZ_NO_SIMD)\n        // Extract 8 x 32-bit or 4 x 64-bit values\
    \ per vector directly into\n        // packed leaves, avoiding the intermediate\
    \ byte-per-bit array.\n        if constexpr(sizeof(T)==4 || sizeof(T)==8) {\n\
    \            std::vector<T> cur=a,nxt(n);\n            std::vector<u64> packed((std::size_t(n)+63)/64);\n\
    \            for(int level=0;level<LOG;++level){\n                if(quad)quad->append(cur,level);\n\
    \                const auto shift=_mm_cvtsi32_si128(LOG-1-level);\n          \
    \      for(int pos=0;pos<n;) {\n                    int len=std::min(64,n-pos),j=0;u64\
    \ word=0;\n                    if constexpr(sizeof(T)==4) {\n                \
    \        for(;j+8<=len;j+=8) {\n                            auto v=_mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(cur.data()+pos+j));\n                            v=_mm256_slli_epi32(_mm256_srl_epi32(v,shift),31);\n\
    \                            word|=u64(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(v))))<<j;\n\
    \                        }\n                    } else {\n                   \
    \     for(;j+4<=len;j+=4) {\n                            auto v=_mm256_loadu_si256(reinterpret_cast<const\
    \ __m256i*>(cur.data()+pos+j));\n                            v=_mm256_slli_epi64(_mm256_srl_epi64(v,shift),63);\n\
    \                            word|=u64(unsigned(_mm256_movemask_pd(_mm256_castsi256_pd(v))))<<j;\n\
    \                        }\n                    }\n                    for(;j<len;++j)word|=u64(bit(as_u64(cur[pos+j]),level))<<j;\n\
    \                    packed[std::size_t(pos)/64]=word;pos+=len;\n            \
    \    }\n                bv[level].build_packed(packed,n,!quad);\n            \
    \    int z=n-bv[level].rank1(n);mid[level]=z;\n                if(level+1==LOG)break;\n\
    \                int p0=0,p1=z;\n                for(int i=0;i<n;++i) {\n    \
    \                int b=bit(as_u64(cur[i]),level);nxt[b?p1:p0]=cur[i];p1+=b;p0+=1-b;\n\
    \                }\n                cur.swap(nxt);\n            }\n          \
    \  quad_valid=bool(quad);return;\n        }\n#endif\n\n        std::vector<T>\
    \ cur=a,nxt(n);\n        std::vector<std::uint8_t> bits(n);\n\n        for(int\
    \ level=0;level<LOG;++level){\n                if(quad)quad->append(cur,level);\n\
    \            int z=0;\n            for(int i=0;i<n;++i){\n                bool\
    \ b=bit(as_u64(cur[i]),level);\n                bits[i]=(std::uint8_t)b;\n   \
    \             z+=!b;\n            }\n            mid[level]=z;\n            bv[level].build(bits,!quad);\n\
    \n            if(level+1==LOG)break;\n\n            int p0=0,p1=z;\n         \
    \   for(int i=0;i<n;++i){\n                int b=bits[i];nxt[b?p1:p0]=cur[i];p1+=b;p0+=1-b;\n\
    \            }\n            cur.swap(nxt);\n        }\n\n        quad_valid=bool(quad);\n\
    \    }\n\n    T access(int k)const{\n        assert(0<=k&&k<n);\n        if(quad_valid)return\
    \ quad->access(k);\n        u64 x=0;\n        int p=k;\n        for(int level=0;level<LOG;++level){\n\
    \            auto [b,o]=bv[level].access_rank(p);\n            x=(x<<1)|u64(b);\n\
    \            p=b?mid[level]+o:p-o;\n        }\n        return (T)x;\n    }\n \
    \   T get(int k)const{return access(k);}\n    T operator[](int k)const{return\
    \ access(k);}\n    // Output must not overlap indices. Repeated and unsorted positions\
    \ are allowed.\n    void access_many(const int* indices,std::size_t count,T* output)const{\n\
    \        assert(count==0||(indices&&output));\n        if(quad_valid){quad->access_many(indices,count,output);return;}\n\
    \        std::size_t i=0;\n        for(;count-i>=8;i+=8){\n            int pos[8],ranks[8];bool\
    \ bits[8];u64 values[8]{};\n            for(int j=0;j<8;++j)pos[j]=indices[i+j];\n\
    \            for(int level=0;level<LOG;++level){\n                bv[level].template\
    \ access_rank_batch<8>(pos,bits,ranks);\n                for(int j=0;j<8;++j){\n\
    \                    values[j]=(values[j]<<1)|u64(bits[j]);\n                \
    \    pos[j]=bits[j]?mid[level]+ranks[j]:pos[j]-ranks[j];\n                }\n\
    \            }\n            for(int j=0;j<8;++j)output[i+j]=T(values[j]);\n  \
    \      }\n        for(;i<count;++i)output[i]=access(indices[i]);\n    }\n    std::vector<T>\
    \ access_many(const std::vector<int>& indices)const{\n        std::vector<T> result(indices.size());access_many(indices.data(),indices.size(),result.data());return\
    \ result;\n    }\n\n    void insert(int k,T x){\n        invalidate_quad();\n\
    \        assert(0<=k&&k<=n);\n        assert(valid_value(x));\n        u64 v=as_u64(x);\n\
    \        int p=k;\n        for(int level=0;level<LOG;++level){\n            bool\
    \ b=bit(v,level);\n            int o=bv[level].insert_rank(p,b);\n           \
    \ int np=b?mid[level]+o:p-o;\n            if(!b)++mid[level];\n            p=np;\n\
    \        }\n        ++n;\n    }\n    void push_back(T x){insert(n,x);}\n\n   \
    \ T erase(int k){\n        invalidate_quad();\n        assert(0<=k&&k<n);\n  \
    \      u64 x=0;\n        int p=k;\n        for(int level=0;level<LOG;++level){\n\
    \            auto [b,o]=bv[level].erase_rank(p);\n            int np=b?mid[level]+o:p-o;\n\
    \            if(b)x|=u64(1)<<(LOG-1-level);\n\n            if(!b)--mid[level];\n\
    \            p=np;\n        }\n        --n;\n        return (T)x;\n    }\n   \
    \ T pop_back(){\n        assert(n);\n        return erase(n-1);\n    }\n    T\
    \ set(int k,T x){\n        assert(0<=k&&k<n);assert(valid_value(x));\n       \
    \ if(quad_valid&&quad->access(k)==x)return x;\n        invalidate_quad();\n  \
    \      int from=k,to=k;u64 old=0,v=as_u64(x);\n        for(int level=0;level<LOG;++level){\n\
    \            bool b=bit(v,level),ob;int old_rank,new_rank;\n            if(from==to){auto\
    \ q=bv[level].set_rank(from,b);ob=q.first;old_rank=new_rank=q.second;}\n     \
    \       else{\n                auto q=bv[level].move_rank(from,to,b);ob=q.previous;old_rank=q.old_rank;new_rank=q.new_rank;\n\
    \            }\n            old=(old<<1)|u64(ob);\n            int next_from=ob?mid[level]+old_rank:from-old_rank;\n\
    \            int after_erase=mid[level]-int(!ob);\n            int next_to=b?after_erase+new_rank:to-new_rank;\n\
    \            mid[level]=after_erase+int(!b);from=next_from;to=next_to;\n     \
    \   }\n        return T(old);\n    }\n\n    int rank(T x,int l,int r)const{\n\
    \        range_check(l,r);\n        if(!valid_value(x))return 0;\n        u64\
    \ v=as_u64(x);\n\n        if(quad_valid)return quad->equal(x,l,r);\n        for(int\
    \ level=0;level<LOG && l<r;++level){\n            auto [ol,orr]=bv[level].rank1_pair(l,r);\n\
    \            if(bit(v,level)){\n                l=mid[level]+ol;\n           \
    \     r=mid[level]+orr;\n            }else{\n                l-=ol;\n        \
    \        r-=orr;\n            }\n        }\n        return r-l;\n    }\n    int\
    \ rank(T x,int r)const{return rank(x,0,r);}\n    int count(int l,int r,T x)const{return\
    \ rank(x,l,r);}\n\n    Quantile kth_smallest_with_count(int l,int r,int k)const{\n\
    \        range_check(l,r);\n        assert(0<=k&&k<r-l);\n\n        if(quad_valid){auto\
    \ q=quad->kth(l,r,k);return {q.value,q.less,q.equal};}\n        u64 x=0;int original_k=k;\n\
    \        for(int level=0;level<LOG && l<r;++level){\n            auto [ol,orr]=bv[level].rank1_pair(l,r);\n\
    \            int zl=l-ol,zr=r-orr;\n            int z=zr-zl;\n\n            if(k<z){\n\
    \                l=zl;\n                r=zr;\n            }else{\n          \
    \      x|=u64(1)<<(LOG-1-level);\n                k-=z;\n                l=mid[level]+ol;\n\
    \                r=mid[level]+orr;\n            }\n        }\n        return {T(x),original_k-k,r-l};\n\
    \    }\n    T kth_smallest(int l,int r,int k)const{return kth_smallest_with_count(l,r,k).value;}\n\
    \    T kth_largest(int l,int r,int k)const{\n        range_check(l,r);\n     \
    \   assert(0<=k&&k<r-l);\n        return kth_smallest(l,r,r-l-1-k);\n    }\n\n\
    \    int range_freq(int l,int r,T upper)const{\n        range_check(l,r);\n  \
    \      if(negative(upper))return 0;\n\n        u64 v=as_u64(upper);\n        if\
    \ constexpr(LOG<64){\n            if(v>=(u64(1)<<LOG))return r-l;\n        }\n\
    \n        if(quad_valid)return quad->less(upper,l,r);\n        int ans=0;\n  \
    \      for(int level=0;level<LOG && l<r;++level){\n            auto [ol,orr]=bv[level].rank1_pair(l,r);\n\
    \            int zl=l-ol,zr=r-orr;\n\n            if(bit(v,level)){\n        \
    \        ans+=zr-zl;\n                l=mid[level]+ol;\n                r=mid[level]+orr;\n\
    \            }else{\n                l=zl;\n                r=zr;\n          \
    \  }\n        }\n        return ans;\n    }\n    int range_freq(int l,int r,T\
    \ lower,T upper)const{\n        range_check(l,r);\n        if(!(lower<upper))return\
    \ 0;\n        auto result=count_less_pair(l,r,lower,upper);return result.second-result.first;\n\
    \    }\n\n    Counts rank_all(int l,int r,T x)const{\n        range_check(l,r);int\
    \ total=r-l;\n        if(negative(x))return {0,0,total};\n        if(!valid_value(x))return\
    \ {total,0,0};\n        if(quad_valid){auto q=quad->less_equal(x,l,r);return {q.first,q.second,total-q.first-q.second};}\n\
    \        int less=0;u64 v=as_u64(x);\n        for(int level=0;level<LOG&&l<r;++level){\n\
    \            auto [ol,orr]=bv[level].rank1_pair(l,r);\n            if(bit(v,level)){less+=(r-l)-(orr-ol);l=mid[level]+ol;r=mid[level]+orr;}\n\
    \            else{l-=ol;r-=orr;}\n        }\n        return {less,r-l,total-less-(r-l)};\n\
    \    }\n    // Counts values <lower and <upper, sharing their common bit prefix.\n\
    \    std::pair<int,int> count_less_pair(int l,int r,T lower,T upper)const{\n \
    \       range_check(l,r);assert(lower<=upper);\n        if(lower==upper){int c=range_freq(l,r,lower);return\
    \ {c,c};}\n        if(!valid_value(lower)||!valid_value(upper))return {range_freq(l,r,lower),range_freq(l,r,upper)};\n\
    \        if(quad_valid)return {quad->less(lower,l,r),quad->less(upper,l,r)};\n\
    \        u64 lo=as_u64(lower),hi=as_u64(upper);int ans=0;\n        for(int level=0;level<LOG&&l<r;++level){\n\
    \            auto [ol,orr]=bv[level].rank1_pair(l,r);\n            int zl=l-ol,zr=r-orr;bool\
    \ a=bit(lo,level),b=bit(hi,level);\n            if(a!=b)return {less_suffix(level+1,zl,zr,lo,ans),less_suffix(level+1,mid[level]+ol,mid[level]+orr,hi,ans+zr-zl)};\n\
    \            if(a){ans+=zr-zl;l=mid[level]+ol;r=mid[level]+orr;}else{l=zl;r=zr;}\n\
    \        }\n        return {ans,ans};\n    }\n    Neighbors neighbors(int l,int\
    \ r,T x)const{return neighbors_impl<true,true>(l,r,x);}\n\n    std::optional<T>\
    \ prev_value(int l,int r,T upper)const{\n        return neighbors_impl<true,false>(l,r,upper).previous;\n\
    \    }\n    std::optional<T> next_value(int l,int r,T lower)const{\n        return\
    \ neighbors_impl<false,true>(l,r,lower).next;\n    }\n};\n\n}\n\n"
  dependsOn: []
  isVerificationFile: false
  path: data-structure/DynamicWaveletMatrix.hpp
  requiredBy: []
  timestamp: '2026-09-11 22:45:40+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: data-structure/DynamicWaveletMatrix.hpp
layout: document
redirect_from:
- /library/data-structure/DynamicWaveletMatrix.hpp
- /library/data-structure/DynamicWaveletMatrix.hpp.html
title: data-structure/DynamicWaveletMatrix.hpp
---

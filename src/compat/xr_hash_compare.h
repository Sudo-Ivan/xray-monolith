#pragma once
// MSVC stdext::hash_compare compat, shared by hash_map/hash_set shims.
#include <functional>

namespace stdext
{
// MSVC's hash_compare combines hashing and ordering in one traits type.
template <class K, class Pr = std::less<K>>
class hash_compare
{
public:
    enum
    {
        bucket_size = 4,
        min_buckets = 8
    };
    size_t operator()(const K& key) const { return std::hash<K>{}(key); }
    bool operator()(const K& a, const K& b) const { return Pr{}(a, b); }
};
}

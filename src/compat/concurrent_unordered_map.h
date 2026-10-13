#pragma once
// MSVC concurrent_unordered_map.h compat: alias over std::unordered_map.
#ifndef _WIN32

#include <unordered_map>

namespace concurrency
{
template <class K, class V,
          class Hash = std::hash<K>,
          class Eq = std::equal_to<K>,
          class Alloc = std::allocator<std::pair<const K, V>>>
using concurrent_unordered_map = std::unordered_map<K, V, Hash, Eq, Alloc>;
}

#endif // _WIN32

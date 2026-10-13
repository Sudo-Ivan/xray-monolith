#pragma once
// MSVC concurrent_vector.h compat: thin wrapper over std::vector.
// Not actually thread-safe beyond what callers guarantee; enough for M1.
#ifndef _WIN32

#include <vector>

namespace concurrency
{
template <class T, class Alloc = std::allocator<T>>
class concurrent_vector : public std::vector<T, Alloc>
{
    using base = std::vector<T, Alloc>;

public:
    using typename base::iterator;
    using typename base::const_iterator;
    using typename base::size_type;
    using base::base;

    iterator grow_by(size_type n)
    {
        size_type old = this->size();
        this->resize(old + n);
        return this->begin() + old;
    }

    iterator grow_by(size_type n, const T& value)
    {
        size_type old = this->size();
        this->resize(old + n, value);
        return this->begin() + old;
    }

    iterator grow_to_at_least(size_type n)
    {
        if (this->size() < n)
            this->resize(n);
        return this->end();
    }

    size_type push_back(const T& value)
    {
        base::push_back(value);
        return this->size() - 1;
    }

    size_type push_back(T&& value)
    {
        base::push_back(std::move(value));
        return this->size() - 1;
    }
};
}

#endif // _WIN32

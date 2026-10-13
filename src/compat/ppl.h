#pragma once
// MSVC PPL (ppl.h) compat, minimal std-based implementation.
// Sequential-or-threads versions of the small subset monolith uses.
// TODO(port): swap for real TBB/oneTBB when the engine links.
#ifndef _WIN32

#include <algorithm>
#include <atomic>
#include <functional>
#include <thread>
#include <vector>

namespace concurrency
{
class task_group
{
    std::vector<std::function<void()>> pending_;
    bool canceled_ = false;

public:
    template <class F>
    void run(F&& fn)
    {
        if (!canceled_)
            fn();
    }

    template <class F>
    void run_and_wait(F&& fn)
    {
        run(std::forward<F>(fn));
        wait();
    }

    void wait() {}
    void cancel() { canceled_ = true; }
    bool is_canceling() const { return canceled_; }
};

template <typename Index, typename Body>
void parallel_for(Index first, Index last, const Body& body)
{
    const Index total = last - first;
    if (total <= 0)
        return;

    const unsigned hw = std::max(1u, std::thread::hardware_concurrency());
    const Index chunks = std::min<Index>(hw, total);
    const Index chunk = (total + chunks - 1) / chunks;

    std::vector<std::thread> workers;
    workers.reserve((size_t)chunks);
    for (Index c = 0; c < chunks; ++c)
    {
        workers.emplace_back([&, c] {
            const Index begin = first + c * chunk;
            const Index end = std::min(begin + chunk, last);
            for (Index i = begin; i < end; ++i)
                body(i);
        });
    }
    for (auto& t : workers)
        t.join();
}

template <typename It, typename Body>
void parallel_for_each(It first, It last, const Body& body)
{
    for (; first != last; ++first)
        body(*first);
}

template <typename It, typename Pr>
void parallel_sort(It first, It last, const Pr& pred)
{
    std::sort(first, last, pred);
}
}

namespace concurrency_ppl = concurrency;

#endif // _WIN32

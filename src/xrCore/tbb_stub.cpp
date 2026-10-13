// Stub implementations for the subset of legacy TBB task API the engine
// references through vendored headers. The real concurrent containers in
// src/compat use std types; these satisfy the linker only.

#include <tbb/task.h>
#include <cstdlib>

namespace
{
class stub_task : public tbb::task
{
public:
    tbb::task* execute() override { return nullptr; }
};
} // namespace

namespace tbb
{
namespace internal
{
task& allocate_root_with_context_proxy::allocate(size_t size) const
{
    void* mem = std::malloc(size);
    return *new (mem) stub_task;
}

task& allocate_child_proxy::allocate(size_t size) const
{
    void* mem = std::malloc(size);
    return *new (mem) stub_task;
}

task& allocate_additional_child_of_proxy::allocate(size_t size) const
{
    void* mem = std::malloc(size);
    return *new (mem) stub_task;
}

void allocate_additional_child_of_proxy::free(task& t) const
{
    std::free(&t);
}
} // namespace internal

void task_group_context::init()
{
}

task_group_context::~task_group_context()
{
}

namespace interface5
{
namespace internal
{
void task_base::destroy(task& t)
{
    std::free(&t);
}
} // namespace internal
} // namespace interface5

void task::note_affinity(affinity_id)
{
}
} // namespace tbb

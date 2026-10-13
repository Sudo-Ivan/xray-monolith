#pragma once
// MSVC <intrin.h> compat for GCC/Clang on x86.
#ifndef _WIN32
#include <x86intrin.h>
#include <cstring>

// MSVC __cpuid/__cpuidex as raw asm. We cannot use GCC's <cpuid.h> here:
// src/xrCore/cpuid.h shadows it on our include path.
inline void __cpuid_impl(int info[4], int function_id, int subfunction_id)
{
    unsigned int a, b, c, d;
    asm volatile("cpuid"
                 : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
                 : "a"((unsigned int)function_id), "c"((unsigned int)subfunction_id));
    info[0] = a;
    info[1] = b;
    info[2] = c;
    info[3] = d;
}

inline void __cpuid(int info[4], int function_id)
{
    __cpuid_impl(info, function_id, 0);
}

inline void __cpuidex(int info[4], int function_id, int subfunction_id)
{
    __cpuid_impl(info, function_id, subfunction_id);
}

// MSVC interlocked intrinsics -> GCC atomics
#define _InterlockedIncrement(p) __sync_add_and_fetch((p), 1)
#define _InterlockedDecrement(p) __sync_sub_and_fetch((p), 1)
#define _InterlockedAdd(p, v) __sync_add_and_fetch((p), (v))
#define _InterlockedSub(p, v) __sync_sub_and_fetch((p), (v))
#define _InterlockedAnd(p, v) __sync_and_and_fetch((p), (v))
#define _InterlockedOr(p, v) __sync_or_and_fetch((p), (v))
#define _InterlockedXor(p, v) __sync_xor_and_fetch((p), (v))
#define _InterlockedCompareExchange(p, x, c) __sync_val_compare_and_swap((p), (c), (x))
#define _InterlockedCompareExchangePointer(p, x, c) __sync_val_compare_and_swap((p), (c), (x))
#define _InterlockedExchange(p, v) __sync_lock_test_and_set((p), (v))
#define _InterlockedExchangePointer(p, v) __sync_lock_test_and_set((p), (v))
#define _ReadWriteBarrier() __atomic_thread_fence(__ATOMIC_SEQ_CST)
#define _mm_pause() __builtin_ia32_pause()

#ifndef __rdtsc
#define __rdtsc() __builtin_ia32_rdtsc()
#endif
#endif // _WIN32

// Symbol stubs for code owned by other engine modules or by xrDebugNew.cpp
// (the Windows crash handler, which is not ported yet).
#include "stdafx.h"
#include "xrDebug.h"
#include "log.h"

// build info normally generated per executable

// modded exes info normally provided by the game binary
__attribute__((weak)) int get_modded_exes_version()
{
	return 0;
}

__attribute__((weak)) xr_string get_modded_exes_version_string()
{
	return xr_string("linux-port");
}

__attribute__((weak)) LPCSTR get_modded_exes_name()
{
	return "xray-monolith-linux";
}

// console markup check lives in xrEngine
enum Console_mark : int;

// thread spawn hook from xrDebugNew.cpp
void debug_on_thread_spawn()
{
}

// std::string overloads that xrDebug.cpp does not provide
void xrDebug::fail(const char* e1, const std::string& e2, const char* file, int line, const char* function,
                   bool& ignore_always)
{
	backend("assertion failed", e1, e2.c_str(), nullptr, file, line, function, ignore_always);
}

void xrDebug::soft_fail(LPCSTR e1, const std::string& e2, LPCSTR file, int line, LPCSTR function)
{
	static bool ignore_always = false;
	backend("soft assert", e1, e2.c_str(), nullptr, file, line, function, ignore_always);
}

void xrDebug::soft_fail(LPCSTR e1, LPCSTR file, int line, LPCSTR function)
{
	static bool ignore_always = false;
	backend("soft assert", e1, nullptr, nullptr, file, line, function, ignore_always);
}

void xrDebug::soft_fail(LPCSTR e1, LPCSTR e2, LPCSTR file, int line, LPCSTR function)
{
	static bool ignore_always = false;
	backend("soft assert", e1, e2, nullptr, file, line, function, ignore_always);
}

void xrDebug::soft_fail(LPCSTR e1, LPCSTR e2, LPCSTR e3, LPCSTR file, int line, LPCSTR function)
{
	static bool ignore_always = false;
	backend("soft assert", e1, e2, e3, file, line, function, ignore_always);
}

void xrDebug::soft_fail(LPCSTR e1, LPCSTR e2, LPCSTR e3, LPCSTR e4, LPCSTR file, int line, LPCSTR function)
{
	static bool ignore_always = false;
	backend("soft assert", e1, e2, e3, file, line, function, ignore_always);
}

void xrDebug::soft_fail(LPCSTR e1, LPCSTR e2, LPCSTR e3, LPCSTR e4, LPCSTR e5, LPCSTR file, int line,
                        LPCSTR function)
{
	static bool ignore_always = false;
	backend("soft assert", e1, e2, e3, file, line, function, ignore_always);
}

// Build info and console markup. Weak so the real definitions from the game
// binary (x_ray.cpp) take precedence when linked.
__attribute__((weak)) XRCORE_API u32 build_id = 0;
__attribute__((weak)) XRCORE_API LPCSTR build_date = __DATE__;

enum Console_mark : int;
__attribute__((weak)) bool is_console_mark(Console_mark)
{
	return false;
}
bool ignore_verify = true;

namespace crash_saving
{
    void (*save_impl)() = nullptr;
	BOOL enabled = TRUE;

    void save()
    {
        if (enabled && save_impl != nullptr)
        {
            (*save_impl)();
        }
    }
}

// crash/bug-report globals normally provided by xrDebugNew.cpp (Windows-only crash handler)
typedef void (*full_memory_stats_callback_type)();
XRCORE_API full_memory_stats_callback_type g_full_memory_stats_callback = 0;
XRCORE_API string_path g_bug_report_file;

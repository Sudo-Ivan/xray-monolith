#pragma once
// Minimal shellapi compat: ShellExecute stub for the debug "open console" path.
#define SW_SHOW 5
#ifdef __cplusplus
extern "C" {
#endif
static void* ShellExecute_(void*, const char*, const char*, const char*, const char*, int) { return (void*)32; }
#define ShellExecute ShellExecute_
#ifdef __cplusplus
}
#endif

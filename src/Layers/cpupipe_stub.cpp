// xrCPU_Pipe stubs: the SSE skinning dispatch table and ttapi worker pool are
// only used by real renderers. The null render path does not call them.

#include "../xrEngine/stdafx.h"
#include "../xrCPU_Pipe/xrCPU_Pipe.h"

extern "C" {

void __cdecl xrBind_PSGP(xrDispatchTable* T, _processor_info*)
{
    Memory.mem_fill(T, 0, sizeof(xrDispatchTable));
}

DWORD ttapi_Init(_processor_info*) { return 0; }
void ttapi_Done() {}
DWORD ttapi_GetWorkersCount() { return 0; }
void ttapi_AddWorker(void (*)(void*), void*) {}
void ttapi_RunAllWorkers() {}

}

extern "C" void XR_EARLY_INIT() {}

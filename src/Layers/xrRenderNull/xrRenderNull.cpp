// xrRenderNull - no-op renderer used for headless/device bring-up on Linux.
// Sets the engine's global render interface pointers to null implementations
// so the device layer, input, and event loop can initialize without a GPU.

#include "stdafx.h"
#include "null_stubs.cpp"
#include "../xrRender/dxRenderFactory.h"

namespace
{
NullIRender_interface g_nullRender;
NullCDUInterface g_nullDU;
NullIUIRender g_nullUIRender;
NullIDebugRender g_nullDRender;

NullIRenderDeviceRender g_nullRenderDeviceRender;
NullIApplicationRender g_nullApplicationRender;
NullIConsoleRender g_nullConsoleRender;
NullIStatGraphRender g_nullStatGraphRender;
NullIStatsRender g_nullStatsRender;
NullIThunderboltDescRender g_nullThunderboltDescRender;
NullIThunderboltRender g_nullThunderboltRender;
NullIFlareRender g_nullFlareRender;
NullILensFlareRender g_nullLensFlareRender;
NullIEnvDescriptorRender g_nullEnvDescriptorRender;
NullIEnvDescriptorMixerRender g_nullEnvDescriptorMixerRender;
NullIEnvironmentRender g_nullEnvironmentRender;
NullIFontRender g_nullFontRender;
NullIImGuiRender g_nullImGuiRender;
NullIRainRender g_nullRainRender;
#ifdef DEBUG
NullIObjectSpaceRender g_nullObjectSpaceRender;
#endif
NullIWallMarkArray g_nullWallMarkArray;
NullIUIShader g_nullUIShader;
NullIUISequenceVideoItem g_nullUISequenceVideoItem;
} // namespace

dxRenderFactory RenderFactoryImpl;

#define NULL_FACTORY_IMPL(Class, Obj)                                                     \
    I##Class* dxRenderFactory::Create##Class() { return &Obj; }                            \
    void dxRenderFactory::Destroy##Class(I##Class*) {}

NULL_FACTORY_IMPL(UISequenceVideoItem, g_nullUISequenceVideoItem)
NULL_FACTORY_IMPL(UIShader, g_nullUIShader)
NULL_FACTORY_IMPL(StatGraphRender, g_nullStatGraphRender)
NULL_FACTORY_IMPL(ConsoleRender, g_nullConsoleRender)
NULL_FACTORY_IMPL(RenderDeviceRender, g_nullRenderDeviceRender)
#ifdef DEBUG
NULL_FACTORY_IMPL(ObjectSpaceRender, g_nullObjectSpaceRender)
#endif
NULL_FACTORY_IMPL(ApplicationRender, g_nullApplicationRender)
NULL_FACTORY_IMPL(WallMarkArray, g_nullWallMarkArray)
NULL_FACTORY_IMPL(StatsRender, g_nullStatsRender)
NULL_FACTORY_IMPL(FlareRender, g_nullFlareRender)
NULL_FACTORY_IMPL(ThunderboltRender, g_nullThunderboltRender)
NULL_FACTORY_IMPL(ThunderboltDescRender, g_nullThunderboltDescRender)
NULL_FACTORY_IMPL(RainRender, g_nullRainRender)
NULL_FACTORY_IMPL(LensFlareRender, g_nullLensFlareRender)
NULL_FACTORY_IMPL(ImGuiRender, g_nullImGuiRender)
NULL_FACTORY_IMPL(EnvironmentRender, g_nullEnvironmentRender)
NULL_FACTORY_IMPL(EnvDescriptorMixerRender, g_nullEnvDescriptorMixerRender)
NULL_FACTORY_IMPL(EnvDescriptorRender, g_nullEnvDescriptorRender)
NULL_FACTORY_IMPL(FontRender, g_nullFontRender)

BOOL DllMainXrRenderNull(HANDLE, DWORD ul_reason_for_call, LPVOID)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        ::Render = &g_nullRender;
        ::RenderFactory = &RenderFactoryImpl;
        ::DU = &g_nullDU;
        UIRender = &g_nullUIRender;
        DRender = &g_nullDRender;
    }
    return TRUE;
}

extern "C" bool SupportsNullRendering()
{
    return true;
}

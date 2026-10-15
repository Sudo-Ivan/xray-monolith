// xrRenderNull - no-op renderer used for headless/device bring-up on Linux.
// Sets the engine's global render interface pointers to null implementations
// so the device layer, input, and event loop can initialize without a GPU.

#include "stdafx.h"
#include "null_sdl.h"
#include "null_stubs.cpp"
#include "../xrRender/dxRenderFactory.h"
#include "../xrEngine/XR_IOConsole.h"
#include "../xrEngine/xr_ioc_cmd.h"

// Renderer cvars queried by game/engine code when the real renderer is absent.
Fvector3 ps_r2_dof = { -1.25f, 0.f, 600.f };
float ps_r2_dof_sky = 30.f;
float ps_r2_dof_kernel_size = 0.25f;
float ps_r2_dof_enable = 0.f;
float ps_r2_dof_reload = 0.f;

static void null_initconsole()
{
    CMD4(CCC_Vector3, "r2_dof", &ps_r2_dof, Fvector3().set(-1000.f, -1000.f, 0.f), Fvector3().set(1000.f, 1000.f, 1000.f));
    static CCC_Float x_r2_dof_sky("r2_dof_sky", &ps_r2_dof_sky, -10000.f, 10000.f); Console->AddCommand(&x_r2_dof_sky);
    static CCC_Float x_r2_dof_kernel_size("r2_dof_kernel_size", &ps_r2_dof_kernel_size, 0.f, 10.f); Console->AddCommand(&x_r2_dof_kernel_size);
    static CCC_Float x_r2_dof_enable("r2_dof_enable", &ps_r2_dof_enable, 0.f, 1.f); Console->AddCommand(&x_r2_dof_enable);
    static CCC_Float x_r2_dof_reload("r2_dof_reload", &ps_r2_dof_reload, 0.f, 1.f); Console->AddCommand(&x_r2_dof_reload);
}

namespace
{
NullIRender_interface g_nullRender;
NullCDUInterface g_nullDU;
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

NullIImGuiRender g_nullImGuiRender;
NullIRainRender g_nullRainRender;
#ifdef DEBUG
NullIObjectSpaceRender g_nullObjectSpaceRender;
#endif
NullIWallMarkArray g_nullWallMarkArray;
NullIUISequenceVideoItem g_nullUISequenceVideoItem;
} // namespace

dxRenderFactory RenderFactoryImpl;

#define NULL_FACTORY_IMPL(Class, Obj)                                                     \
    I##Class* dxRenderFactory::Create##Class() { return &Obj; }                            \
    void dxRenderFactory::Destroy##Class(I##Class*) {}

NULL_FACTORY_IMPL(UISequenceVideoItem, g_nullUISequenceVideoItem)
IUIShader* dxRenderFactory::CreateUIShader() { return xray_null_sdl_uishader_create(); }
void dxRenderFactory::DestroyUIShader(IUIShader* p) { xray_null_sdl_uishader_destroy(p); }
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
IFontRender* dxRenderFactory::CreateFontRender() { return xray_null_sdl_fontrender(); }
void dxRenderFactory::DestroyFontRender(IFontRender* f) { xray_null_sdl_fontrender_destroy(f); }

BOOL DllMainXrRenderNull(HANDLE, DWORD ul_reason_for_call, LPVOID)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        ::Render = &g_nullRender;
        ::RenderFactory = &RenderFactoryImpl;
        ::DU = &g_nullDU;
        UIRender = xray_null_sdl_uirender();
        DRender = &g_nullDRender;
        null_initconsole();
    }
    return TRUE;
}

extern "C" bool SupportsNullRendering()
{
    return true;
}

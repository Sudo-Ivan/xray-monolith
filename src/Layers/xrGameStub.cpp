// Minimal xrGame stubs - satisfies linker while the real xrGame module is pending.
// The engine calls xrFactory_Create for game objects; nullptr means no game data
// can be created, so gameplay still requires the real module.

#include "../xrEngine/stdafx.h"
#include "../xrEngine/Engine.h"

extern "C" {
DLL_Pure* __cdecl xrFactory_Create(CLASS_ID) { return nullptr; }
void __cdecl xrFactory_Destroy(DLL_Pure*) {}
}

BOOL DllMainXrGame(HANDLE, u32, LPVOID) { return TRUE; }

// globals the engine references from xrGame
ENGINE_API Flags32 psDeviceFlags2;
ENGINE_API int ps_ssfx_grass_interactive_unused = 0;
ENGINE_API Fvector4 ps_ssfx_grass_interactive;
ENGINE_API Fvector4 ps_ssfx_wind_trees;
ENGINE_API float g_dispersion_base = 0.f;
ENGINE_API float g_dispersion_factor = 0.f;
ENGINE_API bool g_ai_unlimited_ammo = false;
ENGINE_API int g_crosshair_color = 0;
ENGINE_API float g_fTimeFactor = 1.f;

// Cursor and key binding stubs
#include "../xrGame/UICursor.h"
#include "../xrGame/xr_level_controller.h"

CUICursor::CUICursor() { bVisible = false; vPos.set(0.f, 0.f); vPrevPos.set(0.f, 0.f); m_b_use_win_cursor = false; m_static = nullptr; }
CUICursor::~CUICursor() {}
void CUICursor::InitInternal() {}
void CUICursor::OnRender() {}
Fvector2 CUICursor::GetCursorPositionDelta() { return Fvector2().set(0.f, 0.f); }
Fvector2 CUICursor::GetCursorPosition() { return vPos; }
void CUICursor::SetUICursorPosition(Fvector2 pos) { vPos = pos; }
void CUICursor::UpdateCursorPosition(int, int) {}
void CUICursor::OnScreenResolutionChanged() {}

static CUICursor g_stub_cursor;
CUICursor& GetUICursor() { return g_stub_cursor; }

EGameActions get_binded_action(int) { return (EGameActions)0; }
void updateCurrentScope() {}

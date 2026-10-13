////////////////////////////////////////////////////////////////////////////
//	Module 		: script_render_device_script.cpp
//	Created 	: 28.06.2004
//  Modified 	: 28.06.2004
//	Author		: Dmitriy Iassenev
//	Description : Script render device script export
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "script_render_device.h"

using namespace luabind;

bool is_device_paused(CRenderDevice* d)
{
	return !!Device.Paused();
}

void set_device_paused(CRenderDevice* d, bool b)
{
	Device.Pause(b, TRUE, FALSE, "set_device_paused_script");
}

extern ENGINE_API BOOL bShowPauseString;

void set_device_paused_ex(CRenderDevice* d, bool b)
{
	Device.Pause(b, TRUE, TRUE, "set_device_paused_ex_script");
	bShowPauseString = FALSE;
}

extern ENGINE_API BOOL g_appLoaded;

bool is_app_ready()
{
	return !!g_appLoaded;
}

u32 time_global(const CRenderDevice* self)
{
	THROW(self);
	return (self->dwTimeGlobal);
}

u32 time_continual(const CRenderDevice* self)
{
	THROW(self);
	return (self->dwTimeContinual);
}

#pragma optimize("s",on)
template <>
void CScriptRenderDevice::script_register(lua_State* L)
{
	module(L)
	[
		class_<CRenderDevice>("render_device")
		.def_readonly("width", static_cast<decltype(CRenderDeviceData::dwWidth) CRenderDevice::*>(&CRenderDevice::dwWidth))
		.def_readonly("height", static_cast<decltype(CRenderDeviceData::dwHeight) CRenderDevice::*>(&CRenderDevice::dwHeight))
		.def_readonly("time_delta", static_cast<decltype(CRenderDeviceData::dwTimeDelta) CRenderDevice::*>(&CRenderDevice::dwTimeDelta))
		.def_readonly("f_time_delta", static_cast<decltype(CRenderDeviceData::fTimeDelta) CRenderDevice::*>(&CRenderDevice::fTimeDelta))
		.def_readonly("cam_pos", static_cast<decltype(CRenderDeviceData::vCameraPosition) CRenderDevice::*>(&CRenderDevice::vCameraPosition))
		.def_readonly("cam_dir", static_cast<decltype(CRenderDeviceData::vCameraDirection) CRenderDevice::*>(&CRenderDevice::vCameraDirection))
		.def_readonly("cam_top", static_cast<decltype(CRenderDeviceData::vCameraTop) CRenderDevice::*>(&CRenderDevice::vCameraTop))
		.def_readonly("cam_right", static_cast<decltype(CRenderDeviceData::vCameraRight) CRenderDevice::*>(&CRenderDevice::vCameraRight))
		//			.def_readonly("view",					static_cast<decltype(CRenderDeviceData::mView) CRenderDevice::*>(&CRenderDevice::mView))
		//			.def_readonly("projection",				static_cast<decltype(CRenderDeviceData::mProject) CRenderDevice::*>(&CRenderDevice::mProject))
		//			.def_readonly("full_transform",			static_cast<decltype(CRenderDeviceData::mFullTransform) CRenderDevice::*>(&CRenderDevice::mFullTransform))
		.def_readonly("fov", static_cast<decltype(CRenderDeviceData::fFOV) CRenderDevice::*>(&CRenderDevice::fFOV))
		.def_readonly("aspect_ratio", static_cast<decltype(CRenderDeviceData::fASPECT) CRenderDevice::*>(&CRenderDevice::fASPECT))
		.def("time_global", &time_global)
		.def("time_continual", &time_continual)
		.def_readonly("precache_frame", static_cast<decltype(CRenderDeviceData::dwPrecacheFrame) CRenderDevice::*>(&CRenderDevice::dwPrecacheFrame))
		.def_readonly("frame", static_cast<decltype(CRenderDeviceData::dwFrame) CRenderDevice::*>(&CRenderDevice::dwFrame))
		.def("is_paused", &is_device_paused)
		.def("pause", &set_device_paused)
		.def("pause_ex", &set_device_paused_ex),
		def("app_ready", &is_app_ready)
	];
}

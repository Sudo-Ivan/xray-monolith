// Generated null renderer stubs - do not edit
#include "stdafx.h"
#include "Render.h"
#include "../xrEngine/defines.h"
#include "imgui.h"
#include "Render.h"
#include "../Include/xrRender/DebugRender.h"
#include "../Include/xrRender/UIRender.h"
#include "../Include/xrRender/RenderDeviceRender.h"
#include "../Include/xrRender/ApplicationRender.h"
#include "../Include/xrRender/ConsoleRender.h"
#include "../Include/xrRender/StatGraphRender.h"
#include "../Include/xrRender/StatsRender.h"
#include "../Include/xrRender/ThunderboltDescRender.h"
#include "../Include/xrRender/ThunderboltRender.h"
#include "../Include/xrRender/LensFlareRender.h"
#include "../Include/xrRender/EnvironmentRender.h"
#include "../Include/xrRender/FontRender.h"
#include "../Include/xrRender/ImGuiRender.h"
#include "../Include/xrRender/RenderVisual.h"
#include "../Include/xrRender/RainRender.h"
#include "../Include/xrRender/ObjectSpaceRender.h"
#include "../Include/xrRender/WallMarkArray.h"
#include "../Include/xrRender/DrawUtils.h"
#include "../Include/xrRender/UIShader.h"
#include "../Include/xrRender/UISequenceVideoItem.h"
#include "../Include/xrRender/Kinematics.h"
#include "../Include/xrRender/KinematicsAnimated.h"
#include "../Include/xrRender/ParticleCustom.h"
#include "../Include/xrRender/animation_blend.h"
#include "../Include/xrRender/particles_systems_library_interface.hpp"

// ==== src/xrEngine/Render.h ====
class NullIRender_Light : public IRender_Light
{
public:
    void set_type(LT type) override {}
    void set_active(bool) override {}
    bool get_active() override { return false; }
    void set_shadow(bool) override {}
    void set_volumetric(bool) override {}
    void set_volumetric_quality(float) override {}
    void set_volumetric_intensity(float) override {}
    void set_volumetric_distance(float) override {}
    void set_position(const Fvector& P, const float eps = EPS) override {}
    void set_rotation(const Fvector& D, const Fvector& R, const float eps = EPS) override {}
    void set_cone(float angle) override {}
    void set_range(float R) override {}
    void set_virtual_size(float R) override {}
    void set_texture(LPCSTR name) override {}
    void set_color(const Fcolor& C) override {}
    void set_color(float r, float g, float b) override {}
    void set_hud_mode(bool b) override {}
    bool get_hud_mode() override { return false; }
    void set_is_playerlight(bool b) override {}
};

class NullIRender_Glow : public IRender_Glow
{
public:
    void set_active(bool) override {}
    bool get_active() override { return false; }
    void set_position(const Fvector& P, const float eps = EPS_L) override {}
    void set_direction(const Fvector& P) override {}
    void set_radius(float R) override {}
    void set_texture(LPCSTR name) override {}
    void set_color(const Fcolor& C) override {}
    void set_color(float r, float g, float b) override {}
};

class NullIRender_ObjectSpecific : public IRender_ObjectSpecific
{
public:
    void force_mode(u32 mode) override {}
    float get_luminocity() override { return 0.f; }
    float get_luminocity_hemi() override { return 0.f; }
    float* get_luminocity_hemi_cube() override { return nullptr; }
};

class NullIRender_Target : public IRender_Target
{
public:
    void set_blur(float f) override {}
    void set_gray(float f) override {}
    void set_duality_h(float f) override {}
    void set_duality_v(float f) override {}
    void set_noise(float f) override {}
    void set_noise_scale(float f) override {}
    void set_noise_fps(float f) override {}
    void set_color_base(u32 f) override {}
    void set_color_gray(u32 f) override {}
    void set_color_add(const Fvector& f) override {}
    u32 get_width() override { return 0; }
    u32 get_height() override { return 0; }
    void set_cm_imfluence(float f) override {}
    void set_cm_interpolate(float f) override {}
    void set_cm_textures(const shared_str& tex0, const shared_str& tex1) override {}
};

class NullIRender_interface : public IRender_interface
{
public:
    GenerationLevel get_generation() override { return 0; }
    bool is_sun_static() override { return false; }
    DWORD get_dx_level() override { return 0; }
    void create() override {}
    void destroy() override {}
    void reset_begin() override {}
    void reset_end() override {}
    void level_Load(IReader*) override {}
    void level_Unload() override {}
    HRESULT shader_compile(LPCSTR name, DWORD const* pSrcData, UINT SrcDataLen, LPCSTR pFunctionName, LPCSTR pTarget, DWORD Flags, void*& result) override { return 0; }
    LPCSTR getShaderPath() override { return nullptr; }
    IRender_Sector* getSector(int id) override { return nullptr; }
    IRenderVisual* getVisual(int id) override { return nullptr; }
    IRender_Sector* detectSector(const Fvector& P) override { return nullptr; }
    IRender_Target* getTarget() override { return nullptr; }
    void set_Transform(Fmatrix* M) override {}
    void set_HUD(BOOL V) override {}
    BOOL get_HUD() override { return false; }
    void set_CamAttached(BOOL V) override {}
    BOOL get_CamAttached() override { return false; }
    void set_Invisible(BOOL V) override {}
    void flush() override {}
    void set_Object(IRenderable* O) override {}
    void add_Occluder(Fbox2& bb_screenspace) override {}
    void add_Visual(IRenderVisual* V) override {}
    void add_Geometry(IRenderVisual* V) override {}
    void add_StaticWallmark(const wm_shader& S, const Fvector& P, float s, CDB::TRI* T, Fvector* V) override {}
    void add_StaticWallmark(IWallMarkArray* pArray, const Fvector& P, float s, CDB::TRI* T, Fvector* V, float ttl = 0.f, bool ignore_opt = false, bool random_rotation = true) override {}
    void add_StaticWallmark(IWallMarkArray* pArray, const Fvector& P, float s, CDB::TRI* T, Fvector* V, float ttl, bool ignore_opt, float rotation) override {}
    void clear_static_wallmarks() override {}
    void add_SkeletonWallmark(const Fmatrix* xf, IKinematics* obj, IWallMarkArray* pArray, const Fvector& start, const Fvector& dir, float size, float ttl = 0.f, bool ignore_opt = false) override {}
    IRender_ObjectSpecific* ros_create(IRenderable* parent) override { return nullptr; }
    void ros_destroy(IRender_ObjectSpecific*&) override {}
    IRender_Light* light_create() override { return nullptr; }
    IRender_Glow* glow_create() override { return nullptr; }
    IRenderVisual* model_CreateParticles(LPCSTR name) override { return nullptr; }
    IRenderVisual* model_Create(LPCSTR name, IReader* data = 0) override { return nullptr; }
    IRenderVisual* model_CreateChild(LPCSTR name, IReader* data) override { return nullptr; }
    IRenderVisual* model_Duplicate(IRenderVisual* V) override { return nullptr; }
    void model_Delete(IRenderVisual*& V, BOOL bDiscard = FALSE) override {}
    void model_Logging(BOOL bEnable) override {}
    void models_Prefetch() override {}
    void models_PrefetchOne(LPCSTR name, bool assert = true) override {}
    void models_Clear(BOOL b_complete) override {}
    bool models_Exists(LPCSTR name) override { return false; }
    void TakeScreenshot(LPCSTR path, Fvector2 dimensions, DxEncoding encoding = eDXE_A8R8G8B8) override {}
    BOOL occ_visible(vis_data& V) override { return false; }
    BOOL occ_visible(Fbox& B) override { return false; }
    BOOL occ_visible(sPoly& P) override { return false; }
    void Calculate() override {}
    void Render() override {}
    void Screenshot(ScreenshotMode mode = SM_NORMAL, LPCSTR name = 0) override {}
    void Screenshot(ScreenshotMode mode, CMemoryWriter& memory_writer) override {}
    void ScreenshotAsyncBegin() override {}
    void ScreenshotAsyncEnd(CMemoryWriter& memory_writer) override {}
    void rmNear() override {}
    void rmFar() override {}
    void rmNormal() override {}
    u32 memory_usage() override { return 0; }
    u32 active_phase() override { return 0; }
    void RenderToTarget(RRT target) override {}
    void ScreenshotImpl(ScreenshotMode mode, LPCSTR name, CMemoryWriter* memory_writer) override {}
};

// ==== src/Include/xrRender/DebugRender.h ====
class NullIDebugRender : public IDebugRender
{
public:
    void Render() override {}
    void add_lines(Fvector const *vertices, u32 const &vertex_count, u16 const *pairs, u32 const &pair_count, u32 const &color, bool bHud = false) override {}
    void NextSceneMode() override {}
    void ZEnable(bool bEnable) override {}
    void OnFrameEnd() override {}
    void SetShader(const debug_shader &shader) override {}
    void CacheSetXformWorld(const Fmatrix& M) override {}
    void CacheSetCullMode(CullMode) override {}
    void SetAmbient(u32 colour) override {}
    void SetDebugShader(dbgShaderHandle shdHandle) override {}
    void DestroyDebugShader(dbgShaderHandle shdHandle) override {}
    void dbg_DrawTRI(Fmatrix& T, Fvector& p1, Fvector& p2, Fvector& p3, u32 C) override {}
};

// ==== src/Include/xrRender/UIRender.h ====
class NullIUIRender : public IUIRender
{
public:
    void CreateUIGeom() override {}
    void DestroyUIGeom() override {}
    void SetShader(IUIShader& shader) override {}
    void SetAlphaRef(int aref) override {}
    void SetScissor(Irect* rect = NULL) override {}
    void GetActiveTextureResolution(Fvector2& res) override {}
    bool GetTextureResolution(LPCSTR name, Fvector2& res) override { return false; }
    void PushPoint(float x, float y, float z, u32 C, float u, float v) override {}
    void StartPrimitive(u32 iMaxVerts, ePrimitiveType primType, ePointType pointType) override {}
    void FlushPrimitive() override {}
    LPCSTR UpdateShaderName(LPCSTR tex_name, LPCSTR sh_name) override { return nullptr; }
    void CacheSetXformWorld(const Fmatrix& M) override {}
    void CacheSetCullMode(CullMode) override {}
};

// ==== src/Include/xrRender/RenderDeviceRender.h ====
class NullIRenderDeviceRender : public IRenderDeviceRender
{
public:
    void Copy(IRenderDeviceRender& _in) override {}
    void setGamma(float fGamma) override {}
    void setBrightness(float fGamma) override {}
    void setContrast(float fGamma) override {}
    void updateGamma() override {}
    void OnDeviceDestroy(BOOL bKeepTextures) override {}
    void ValidateHW() override {}
    void DestroyHW() override {}
    void Reset(HWND hWnd, u32& dwWidth, u32& dwHeight, float& fWidth_2, float& fHeight_2) override {}
    void SetupStates() override {}
    void OnDeviceCreate(LPCSTR shName) override {}
    void Create(HWND hWnd, u32& dwWidth, u32& dwHeight, float& fWidth_2, float& fHeight_2, bool) override
    {
        if (!psCurrentVidMode[0] || !psCurrentVidMode[1])
        {
            psCurrentVidMode[0] = 1024;
            psCurrentVidMode[1] = 768;
        }
        dwWidth = psCurrentVidMode[0];
        dwHeight = psCurrentVidMode[1];
        fWidth_2 = float(dwWidth) / 2.f;
        fHeight_2 = float(dwHeight) / 2.f;
    }
    void SetupGPU(BOOL bForceGPU_SW, BOOL bForceGPU_NonPure, BOOL bForceGPU_REF) override {}
    void overdrawBegin() override {}
    void overdrawEnd() override {}
    void DeferredLoad(BOOL E) override {}
    void ResourcesDeferredUpload() override {}
    void ResourcesDeferredUnload() override {}
    void ResourcesGetMemoryUsage(u32& m_base, u32& c_base, u32& m_lmaps, u32& c_lmaps) override {}
    void ResourcesDestroyNecessaryTextures() override {}
    void ResourcesStoreNecessaryTextures() override {}
    void ResourcesDumpMemoryUsage() override {}
    void ResourcesPrefetchCreateTexture(LPCSTR name) override {}
    bool HWSupportsShaderYUV2RGB() override { return false; }
    DeviceState GetDeviceState() override { return {}; }
    BOOL GetForceGPU_REF() override { return false; }
    u32 GetCacheStatPolys() override { return 0; }
    void Begin() override {}
    void Clear() override {}
    void End() override {}
    void ClearTarget() override {}
    void SetCacheXform(Fmatrix& mView, Fmatrix& mProject) override {}
    void SetCacheXform_prev(Fmatrix& mView, Fmatrix& mProject) override {}
    void OnAssetsChanged() override {}
};

// ==== src/Include/xrRender/ApplicationRender.h ====
class NullIApplicationRender : public IApplicationRender
{
public:
    void Copy(IApplicationRender& _in) override {}
    void LoadBegin() override {}
    void destroy_loading_shaders() override {}
    void setLevelLogo(LPCSTR pszLogoName) override {}
    void load_draw_internal(CApplication& owner) override {}
    void KillHW() override {}
};

// ==== src/Include/xrRender/ConsoleRender.h ====
class NullIConsoleRender : public IConsoleRender
{
public:
    void Copy(IConsoleRender& _in) override {}
    void OnRender(bool bGame) override {}
};

// ==== src/Include/xrRender/StatGraphRender.h ====
class NullIStatGraphRender : public IStatGraphRender
{
public:
    void Copy(IStatGraphRender& _in) override {}
    void OnDeviceCreate() override {}
    void OnDeviceDestroy() override {}
    void OnRender(CStatGraph& owner) override {}
};

// ==== src/Include/xrRender/StatsRender.h ====
class NullIStatsRender : public IStatsRender
{
public:
    void Copy(IStatsRender& _in) override {}
    void OutData1(CGameFont& F) override {}
    void OutData2(CGameFont& F) override {}
    void OutData3(CGameFont& F) override {}
    void OutData4(CGameFont& F) override {}
    void GuardVerts(CGameFont& F) override {}
    void GuardDrawCalls(CGameFont& F) override {}
    void SetDrawParams(IRenderDeviceRender* pRender) override {}
};

// ==== src/Include/xrRender/ThunderboltDescRender.h ====
class NullIThunderboltDescRender : public IThunderboltDescRender
{
public:
    void Copy(IThunderboltDescRender& _in) override {}
    void CreateModel(LPCSTR m_name) override {}
    void DestroyModel() override {}
};

// ==== src/Include/xrRender/ThunderboltRender.h ====
class NullIThunderboltRender : public IThunderboltRender
{
public:
    void Copy(IThunderboltRender& _in) override {}
    void Render(CEffect_Thunderbolt& owner) override {}
};

// ==== src/Include/xrRender/LensFlareRender.h ====
class NullIFlareRender : public IFlareRender
{
public:
    void Copy(IFlareRender& _in) override {}
    void CreateShader(LPCSTR sh_name, LPCSTR tex_name) override {}
    void DestroyShader() override {}
};

class NullILensFlareRender : public ILensFlareRender
{
public:
    void Copy(ILensFlareRender& _in) override {}
    void Render(CLensFlare& owner, BOOL bSun, BOOL bFlares, BOOL bGradient) override {}
    void OnDeviceCreate() override {}
    void OnDeviceDestroy() override {}
};

// ==== src/Include/xrRender/EnvironmentRender.h ====
class NullIEnvDescriptorRender : public IEnvDescriptorRender
{
public:
    void Copy(IEnvDescriptorRender& _in) override {}
    void OnDeviceCreate(CEnvDescriptor& owner) override {}
    void OnDeviceDestroy() override {}
};

class NullIEnvDescriptorMixerRender : public IEnvDescriptorMixerRender
{
public:
    void Copy(IEnvDescriptorMixerRender& _in) override {}
    void Destroy() override {}
    void Clear() override {}
    void lerp(IEnvDescriptorRender* inA, IEnvDescriptorRender* inB) override {}
};

class NullIEnvironmentRender : public IEnvironmentRender
{
public:
    void Copy(IEnvironmentRender& _in) override {}
    void OnFrame(CEnvironment& env) override {}
    void OnLoad() override {}
    void OnUnload() override {}
    void RenderSky(CEnvironment& env, bool only_MV = false) override {}
    void RenderClouds(CEnvironment& env) override {}
    void OnDeviceCreate() override {}
    void OnDeviceDestroy() override {}
    particles_systems::library_interface const& particles_systems_library() override { static particles_systems::library_interface* d = nullptr; return *d; }
};

// ==== src/Include/xrRender/FontRender.h ====
class NullIFontRender : public IFontRender
{
public:
    void Initialize(LPCSTR cShader, LPCSTR cTexture) override {}
    void OnRender(CGameFont& owner) override {}
};

// ==== src/Include/xrRender/ImGuiRender.h ====
class NullIImGuiRender : public IImGuiRender
{
public:
    void Copy(IImGuiRender& _in) override {}
    void Frame() override
    {
        // Fonts are added later in ide::OnAppStart, so the atlas can only be
        // built here once fonts exist. CPU-side build gives NewFrame a valid
        // atlas; the renderer-side upload is a no-op under the null backend.
        ImGuiIO& io = ImGui::GetIO();
        if (io.Fonts->Fonts.Size > 0 && !io.Fonts->IsBuilt())
        {
            io.Fonts->Build();
            io.Fonts->SetTexID((ImTextureID)(intptr_t)1);
        }
    }
    void Render(ImDrawData* data) override {}
    void OnDeviceCreate(ImGuiContext* context) override
    {
        if (context)
            ImGui::SetCurrentContext(context);
    }
    void OnDeviceDestroy() override {}
    void OnDeviceResetBegin() override {}
    void OnDeviceResetEnd() override {}
};

// ==== src/Include/xrRender/RenderVisual.h ====
class NullIRenderVisual : public IRenderVisual
{
public:
    vis_data& getVisData() override { static vis_data d = {}; return d; }
    u32 getType() override { return 0; }
#ifdef DEBUG
    shared_str getDebugName() override { return {}; }
#endif
};

// ==== src/Include/xrRender/RainRender.h ====
class NullIRainRender : public IRainRender
{
public:
    void Copy(IRainRender& _in) override {}
    void Render(CEffect_Rain& owner) override {}
    const Fsphere& GetDropBounds() const override { static Fsphere d = {}; return d; }
};

// ==== src/Include/xrRender/ObjectSpaceRender.h ====
#ifdef DEBUG
class NullIObjectSpaceRender : public IObjectSpaceRender
{
public:
    void Copy(IObjectSpaceRender &_in) override {}
    void dbgRender() override {}
    void dbgAddSphere(const Fsphere &sphere, u32 colour) override {}
    void SetShader() override {}
};
#endif

// ==== src/Include/xrRender/WallMarkArray.h ====
class NullIWallMarkArray : public IWallMarkArray
{
public:
    void Copy(IWallMarkArray& _in) override {}
    void AppendMark(LPCSTR s_textures) override {}
    void clear() override {}
    bool empty() override { return false; }
    wm_shader GenerateWallmark() override { return {}; }
};

// ==== src/Include/xrRender/DrawUtils.h ====
class NullCDUInterface : public CDUInterface
{
public:
    void DrawCross(const Fvector& p, float szx1, float szy1, float szz1, float szx2, float szy2, float szz2, u32 clr, BOOL bRot45 = false) override {}
    void DrawCross(const Fvector& p, float sz, u32 clr, BOOL bRot45 = false) override {}
    void DrawFlag(const Fvector& p, float heading, float height, float sz, float sz_fl, u32 clr, BOOL bDrawEntity) override {}
    void DrawRomboid(const Fvector& p, float radius, u32 clr) override {}
    void DrawJoint(const Fvector& p, float radius, u32 clr) override {}
    void DrawSpotLight(const Fvector& p, const Fvector& d, float range, float phi, u32 clr) override {}
    void DrawDirectionalLight(const Fvector& p, const Fvector& d, float radius, float range, u32 clr) override {}
    void DrawPointLight(const Fvector& p, float radius, u32 clr) override {}
    void DrawSound(const Fvector& p, float radius, u32 clr) override {}
    void DrawLineSphere(const Fvector& p, float radius, u32 clr, BOOL bCross) override {}
    void dbgDrawPlacement(const Fvector& p, int sz, u32 clr, LPCSTR caption = 0, u32 clr_font = 0xffffffff) override {}
    void dbgDrawVert(const Fvector& p0, u32 clr, LPCSTR caption = 0) override {}
    void dbgDrawEdge(const Fvector& p0, const Fvector& p1, u32 clr, LPCSTR caption = 0) override {}
    void dbgDrawFace(const Fvector& p0, const Fvector& p1, const Fvector& p2, u32 clr, LPCSTR caption = 0) override {}
    void DrawFace(const Fvector& p0, const Fvector& p1, const Fvector& p2, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire) override {}
    void DrawLine(const Fvector& p0, const Fvector& p1, u32 clr) override {}
    void DrawLink(const Fvector& p0, const Fvector& p1, float sz, u32 clr) override {}
    void DrawFaceNormal(const Fvector& p0, const Fvector& p1, const Fvector& p2, float size, u32 clr) override {}
    void DrawFaceNormal(const Fvector* p, float size, u32 clr) override {}
    void DrawFaceNormal(const Fvector& C, const Fvector& N, float size, u32 clr) override {}
    void DrawSelectionBox(const Fvector& center, const Fvector& size, u32* c = 0) override {}
    void DrawSelectionBoxB(const Fbox& box, u32* c = 0) override {}
    void DrawIdentSphere(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w) override {}
    void DrawIdentSpherePart(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w) override {}
    void DrawIdentCone(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w) override {}
    void DrawIdentCylinder(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w) override {}
    void DrawIdentBox(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w) override {}
    void DrawBox(const Fvector& offs, const Fvector& Size, BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w) override {}
    void DrawAABB(const Fvector& p0, const Fvector& p1, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire) override {}
    void DrawAABB(const Fmatrix& parent, const Fvector& center, const Fvector& size, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire) override {}
    void DrawOBB(const Fmatrix& parent, const Fobb& box, u32 clr_s, u32 clr_w) override {}
    void DrawSphere(const Fmatrix& parent, const Fvector& center, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire) override {}
    void DrawSphere(const Fmatrix& parent, const Fsphere& S, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire) override {}
    void DrawCylinder(const Fmatrix& parent, const Fvector& center, const Fvector& dir, float height, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire) override {}
    void DrawCone(const Fmatrix& parent, const Fvector& apex, const Fvector& dir, float height, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire) override {}
    void DrawPlane(const Fvector& center, const Fvector2& scale, const Fvector& rotate, u32 clr_s, u32 clr_w, BOOL bCull, BOOL bSolid, BOOL bWire) override {}
    void DrawPlane(const Fvector& p, const Fvector& n, const Fvector2& scale, u32 clr_s, u32 clr_w, BOOL bCull, BOOL bSolid, BOOL bWire) override {}
    void DrawRectangle(const Fvector& o, const Fvector& u, const Fvector& v, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire) override {}
    void DrawGrid() override {}
    void DrawPivot(const Fvector& pos, float sz = 5.f) override {}
    void DrawAxis(const Fmatrix& T) override {}
    void DrawObjectAxis(const Fmatrix& T, float sz, BOOL sel) override {}
    void DrawSelectionRect(const Ivector2& m_SelStart, const Ivector2& m_SelEnd) override {}
    void DrawIndexedPrimitive(int prim_type, u32 pc, const Fvector& pos, const Fvector* vb, const u32& vb_size, const u32* ib, const u32& ib_size, const u32& clr_argb, float scale = 1.0f) override {}
    void OutText(const Fvector& pos, LPCSTR text, u32 color = 0xFF000000, u32 shadow_color = 0xFF909090) override {}
    void OnDeviceDestroy() override {}
};

// ==== src/Include/xrRender/UIShader.h ====
class NullIUIShader : public IUIShader
{
public:
    void Copy(IUIShader& _in) override {}
    void create(LPCSTR sh, LPCSTR tex = nullptr, bool no_cache = false) override {}
    bool inited() override { return false; }
};

// ==== src/Include/xrRender/UISequenceVideoItem.h ====
class NullIUISequenceVideoItem : public IUISequenceVideoItem
{
public:
    void Copy(IUISequenceVideoItem& _in) override {}
    bool HasTexture() override { return false; }
    void CaptureTexture() override {}
    void ResetTexture() override {}
    BOOL video_IsPlaying() override { return false; }
    void video_Sync(u32 _time) override {}
    void video_Play(BOOL looped, u32 _time = 0xFFFFFFFF) override {}
    void video_Stop() override {}
};

// ==== src/Include/xrRender/Kinematics.h ====
class NullIKinematics : public IKinematics
{
public:
    void Bone_Calculate(CBoneData* bd, Fmatrix* parent) override {}
    void Bone_GetAnimPos(Fmatrix& pos, u16 id, u8 channel_mask, bool ignore_callbacks) override {}
    bool PickBone(const Fmatrix& parent_xform, pick_result& r, float dist, const Fvector& start, const Fvector& dir, u16 bone_id) override { return false; }
    void EnumBoneVertices(SEnumVerticesCallback& C, u16 bone_id) override {}
    u16 LL_BoneID(LPCSTR B) override { return 0; }
    u16 LL_BoneID(const shared_str& B) override { return 0; }
    LPCSTR LL_BoneName_dbg(u16 ID) override { return nullptr; }
    CInifile* LL_UserData() override { return nullptr; }
    accel* LL_Bones() override { return nullptr; }
    CBoneInstance& LL_GetBoneInstance(u16 bone_id) override { static CBoneInstance* d = nullptr; return *d; }
    CBoneData& LL_GetData(u16 bone_id) override { static CBoneData* d = nullptr; return *d; }
    const IBoneData& GetBoneData(u16 bone_id) const override { static IBoneData* d = nullptr; return *d; }
    u16 LL_BoneCount() const override { return 0; }
    u16 LL_VisibleBoneCount() override { return 0; }
    Fmatrix& LL_GetTransform(u16 bone_id) override { static Fmatrix* d = nullptr; return *d; }
    const Fmatrix& LL_GetTransform(u16 bone_id) const override { static const Fmatrix* d = nullptr; return *d; }
    Fmatrix& LL_GetTransform_R(u16 bone_id) override { static Fmatrix* d = nullptr; return *d; }
    Fobb& LL_GetBox(u16 bone_id) override { static Fobb d = {}; return d; }
    const Fbox& GetBox() const override { static Fbox d = {}; return d; }
    void LL_GetBindTransform(xr_vector<Fmatrix>& matrices) override {}
    int LL_GetBoneGroups(xr_vector<xr_vector<u16>>& groups) override { return 0; }
    u16 LL_GetBoneRoot() override { return 0; }
    void LL_SetBoneRoot(u16 bone_id) override {}
    BOOL LL_GetBoneVisible(u16 bone_id) override { return false; }
    void LL_SetBoneVisible(u16 bone_id, BOOL val, BOOL bRecursive) override {}
    u64 LL_GetBonesVisible() override { return 0; }
    void LL_SetBonesVisible(u64 mask) override {}
    IRenderVisual* GetVisualByBone(u16 bone_id) override { return nullptr; }
    IRenderVisual* GetVisualByBone(LPCSTR bone_name) override { return nullptr; }
    void CalculateBones(BOOL bForceExact = FALSE) override {}
    void CalculateBones_Invalidate() override {}
    bool CopyBonesFrom(IKinematics* from) override { return false; }
    void Callback(UpdateCallback C, void* Param) override {}
    void SetUpdateCallback(UpdateCallback pCallback) override {}
    void SetUpdateCallbackParam(void* pCallbackParam) override {}
    bool NeedUCalc() override { return false; }
    UpdateCallback GetUpdateCallback() override { return {}; }
    void* GetUpdateCallbackParam() override { return nullptr; }
    IRenderVisual* dcast_RenderVisual() override { return nullptr; }
    IKinematicsAnimated* dcast_PKinematicsAnimated() override { return nullptr; }
#ifdef DEBUG
    void DebugRender(Fmatrix& XFORM) override {}
    shared_str getDebugName() override { return {}; }
#endif
};

// ==== src/Include/xrRender/KinematicsAnimated.h ====
class NullIKinematicsAnimated : public IKinematicsAnimated
{
public:
    void OnCalculateBones() override {}
#ifdef DEBUG
    std::pair<LPCSTR,LPCSTR> LL_MotionDefName_dbg(MotionID ID) override { return {}; }
    void LL_DumpBlends_dbg() override {}
#endif
    u32 LL_PartBlendsCount(u32 bone_part_id) override { return 0; }
    CBlend* LL_PartBlend(u32 bone_part_id, u32 n) override { return nullptr; }
    void LL_IterateBlends(IterateBlendsCallback& callback) override {}
    u16 LL_MotionsSlotCount() override { return 0; }
    const shared_motions& LL_MotionsSlot(u16 idx) override { static shared_motions d = {}; return d; }
    CMotionDef* LL_GetMotionDef(MotionID id) override { return nullptr; }
    CMotion* LL_GetRootMotion(MotionID id) override { return nullptr; }
    CMotion* LL_GetMotion(MotionID id, u16 bone_id) override { return nullptr; }
    void LL_BuldBoneMatrixDequatize(const CBoneData* bd, u8 channel_mask, SKeyTable& keys) override {}
    void LL_BoneMatrixBuild(u16 bone_id, CBoneInstance& bi, const Fmatrix* parent, const SKeyTable& keys) override {}
    IBlendDestroyCallback* GetBlendDestroyCallback() override { return nullptr; }
    void SetBlendDestroyCallback(IBlendDestroyCallback* cb) override {}
    void SetUpdateTracksCalback(IUpdateTracksCallback* callback) override {}
    IUpdateTracksCallback* GetUpdateTracksCalback() override { return nullptr; }
    MotionID LL_MotionID(LPCSTR B) override { return {}; }
    u16 LL_PartID(LPCSTR B) override { return 0; }
    CBlend* LL_PlayCycle(u16 partition, MotionID motion, BOOL bMixing, float blendAccrue, float blendFalloff, float Speed, BOOL noloop, PlayCallback Callback, LPVOID CallbackParam, u8 channel = 0) override { return nullptr; }
    CBlend* LL_PlayCycle(u16 partition, MotionID motion, BOOL bMixIn, PlayCallback Callback, LPVOID CallbackParam, u8 channel = 0) override { return nullptr; }
    void LL_SetChannelFactor(u16 channel, float factor) override {}
    void UpdateTracks() override {}
    void LL_UpdateTracks(float dt, bool b_force, bool leave_blends) override {}
    MotionID ID_Cycle(LPCSTR N) override { return {}; }
    MotionID ID_Cycle_Safe(LPCSTR N) override { return {}; }
    MotionID ID_Cycle(shared_str N) override { return {}; }
    MotionID ID_Cycle_Safe(shared_str N) override { return {}; }
    CBlend* PlayCycle(LPCSTR N, BOOL bMixIn = TRUE, PlayCallback Callback = 0, LPVOID CallbackParam = 0, u8 channel = 0) override { return nullptr; }
    CBlend* PlayCycle(MotionID M, BOOL bMixIn = TRUE, PlayCallback Callback = 0, LPVOID CallbackParam = 0, u8 channel = 0) override { return nullptr; }
    CBlend* PlayCycle(u16 partition, MotionID M, BOOL bMixIn = TRUE, PlayCallback Callback = 0, LPVOID CallbackParam = 0, u8 channel = 0, float speed = 0) override { return nullptr; }
    MotionID ID_FX(LPCSTR N) override { return {}; }
    MotionID ID_FX_Safe(LPCSTR N) override { return {}; }
    CBlend* PlayFX(LPCSTR N, float power_scale) override { return nullptr; }
    CBlend* PlayFX(MotionID M, float power_scale) override { return nullptr; }
    const CPartition& partitions() const override { static CPartition d = {}; return d; }
    IRenderVisual* dcast_RenderVisual() override { return nullptr; }
    IKinematics* dcast_PKinematics() override { return nullptr; }
    float get_animation_length(MotionID motion_ID) override { return 0.f; }
};

// ==== src/Include/xrRender/ParticleCustom.h ====
class NullIParticleCustom : public IParticleCustom
{
public:
    void OnDeviceCreate() override {}
    void OnDeviceDestroy() override {}
    void UpdateParent(const Fmatrix& m, const Fvector& velocity, BOOL bXFORM) override {}
    void OnFrame(u32 dt) override {}
    void Play() override {}
    void Stop(BOOL bDefferedStop = TRUE) override {}
    BOOL IsPlaying() override { return false; }
    u32 ParticlesCount() override { return 0; }
    float GetTimeLimit() override { return 0.f; }
    const shared_str Name() override { return {}; }
    void SetHudMode(BOOL b) override {}
    BOOL GetHudMode() override { return false; }
};

// ==== src/Include/xrRender/animation_blend.h ====
class NullIBlendDestroyCallback : public IBlendDestroyCallback
{
public:
    void BlendDestroy(CBlend& blend) override {}
};

// ==== src/Include/xrRender/particles_systems_library_interface.hpp ====
class Nulllibrary_interface : public particles_systems::library_interface
{
public:
    PS::CPGDef const* const* particles_group_begin() const override { return nullptr; }
    PS::CPGDef const* const* particles_group_end() const override { return nullptr; }
    void particles_group_next(PS::CPGDef const* const*& iterator) const override {}
    shared_str const& particles_group_id(PS::CPGDef const& particles_group) const override { static shared_str const d = {}; return d; }
};

// Renderer-module globals consumed by xrGame (normally defined in xrRender/xrRender_console.cpp)
ENGINE_API Flags32 psDeviceFlags2;
ENGINE_API int ps_ssfx_grass_interactive_unused = 0;
ENGINE_API Fvector4 ps_ssfx_grass_interactive;
ENGINE_API Fvector4 ps_ssfx_wind_trees;
ENGINE_API Fvector4 ps_ssfx_int_grass_params_1 = { 1.0f, 1.0f, 1.0f, 25.0f };
ENGINE_API Fvector4 ps_ssfx_int_grass_params_2 = { 1.0f, 5.0f, 1.0f, 1.0f };

// Renderer-module console globals consumed by xrGame/xrEngine
float sil_glow_max_temp = 0.15f;
float sil_glow_shot_temp = 0.004f;
float sil_glow_cool_temp_rate = 0.01f;
Fvector sil_glow_color = { 1.f, .0f, .0f };
int ps_r2_heatvision = 0;
int heat_vision_cooldown = 1;
float heat_vision_cooldown_time = 20000.f;
int heat_vision_zombie_cold = 0;
float ps_r4_hdr10_pda_intensity = 1.0f;
int ps_r4_hdr10_pda = 0;
float hud_fov_aim_factor = 0;
Flags32 ps_actor_shadow_flags = {0};
BOOL r_optimize_calculate_bones = TRUE;
float wallmark_range_static = 100.f;
float wallmark_range_skeleton = 50.f;

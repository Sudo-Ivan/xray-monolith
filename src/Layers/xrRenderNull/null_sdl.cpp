// SDL_Renderer visual path for the null renderer.
//
// The null renderer's render interfaces discard everything, which is
// correct for headless runs. When a real SDL window exists, the UI
// layer (menu, fonts, console, imgui) is still submitted as textured
// quads through IUIRender. This file rasterizes those quads with
// SDL_Renderer and presents in the device render End() call, so the
// game is visible without a D3D pipeline.

#include "stdafx.h"
#include "null_sdl.h"
#include "../Include/xrRender/UIRender.h"
#include "../Include/xrRender/UIShader.h"
#include "imgui.h"
#include "../xrCore/LocatorAPI.h"
#include "../xrGame/GamePersistent.h"
#include "../xrEngine/GameFont.h"
#include "../xrEngine/MbHelpers.h"
#include "../Include/xrRender/FontRender.h"
#include <SDL.h>

namespace
{
SDL_Renderer* g_sdl_renderer;
SDL_Texture* g_missing_tex;
u32 g_missing_w = 64, g_missing_h = 64;

SDL_Renderer* ensure_renderer()
{
    if (g_sdl_renderer)
        return g_sdl_renderer;
    SDL_Window* win = (SDL_Window*)xray_sdl_main_window();
    if (!win)
        return nullptr;
    g_sdl_renderer = SDL_CreateRenderer(
        win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_sdl_renderer)
        g_sdl_renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    if (!g_sdl_renderer)
        return nullptr;
    SDL_SetRenderDrawBlendMode(g_sdl_renderer, SDL_BLENDMODE_BLEND);
    // 64x64 magenta checker so missing textures are obvious.
    u32 px[64 * 64];
    for (int y = 0; y < 64; ++y)
        for (int x = 0; x < 64; ++x)
            px[y * 64 + x] = ((x ^ y) & 8) ? 0xffff00ffu : 0xff000000u;
    g_missing_tex = SDL_CreateTexture(
        g_sdl_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, 64, 64);
    SDL_UpdateTexture(g_missing_tex, nullptr, px, 64 * sizeof(u32));
    SDL_SetTextureBlendMode(g_missing_tex, SDL_BLENDMODE_BLEND);
    return g_sdl_renderer;
}

// ---- DDS decode (DXT1, DXT5, A8R8G8B8, X8R8G8B8, R8G8B8) ----

#pragma pack(push, 1)
struct dds_header
{
    u32 magic;
    u32 size;
    u32 flags;
    u32 height;
    u32 width;
    u32 pitch;
    u32 depth;
    u32 mipmaps;
    u32 reserved[11];
    u32 pf_size;
    u32 pf_flags;
    u32 fourcc;
    u32 rgb_bits;
    u32 r_mask;
    u32 g_mask;
    u32 b_mask;
    u32 a_mask;
    u32 caps[5];
};
#pragma pack(pop)

static void dxt1_block(const u8* src, u32* dst, int dst_pitch, bool allow_transparent)
{
    u16 c0, c1;
    memcpy(&c0, src, 2);
    memcpy(&c1, src + 2, 2);
    u8 r[4], g[4], b[4], a[4] = {255, 255, 255, 255};
    r[0] = ((c0 >> 11) & 0x1f) * 255 / 31;
    g[0] = ((c0 >> 5) & 0x3f) * 255 / 63;
    b[0] = (c0 & 0x1f) * 255 / 31;
    r[1] = ((c1 >> 11) & 0x1f) * 255 / 31;
    g[1] = ((c1 >> 5) & 0x3f) * 255 / 63;
    b[1] = (c1 & 0x1f) * 255 / 31;
    if (c0 > c1 || !allow_transparent)
    {
        r[2] = (2 * r[0] + r[1]) / 3; g[2] = (2 * g[0] + g[1]) / 3; b[2] = (2 * b[0] + b[1]) / 3;
        r[3] = (r[0] + 2 * r[1]) / 3; g[3] = (g[0] + 2 * g[1]) / 3; b[3] = (b[0] + 2 * b[1]) / 3;
    }
    else
    {
        r[2] = (r[0] + r[1]) / 2; g[2] = (g[0] + g[1]) / 2; b[2] = (b[0] + b[1]) / 2;
        r[3] = g[3] = b[3] = 0; a[3] = 0;
    }
    u32 idx;
    memcpy(&idx, src + 4, 4);
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x)
        {
            int ci = (idx >> (2 * (4 * y + x))) & 3;
            dst[y * dst_pitch + x] =
                (a[ci] << 24) | (r[ci] << 16) | (g[ci] << 8) | b[ci];
        }
}

static void dxt5_block(const u8* src, u32* dst, int dst_pitch)
{
    u8 a0 = src[0], a1 = src[1];
    u8 a[8] = {a0, a1};
    if (a0 > a1)
        for (int i = 0; i < 6; ++i)
            a[2 + i] = u8(((7 - i) * a0 + i * a1) / 7);
    else
    {
        for (int i = 0; i < 4; ++i)
            a[2 + i] = u8(((5 - i) * a0 + i * a1) / 5);
        a[6] = 0; a[7] = 255;
    }
    u64 abits = 0;
    memcpy(&abits, src + 2, 6);
    u32 col[16];
    dxt1_block(src + 8, col, 4, false);
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x)
        {
            int ai = (abits >> (3 * (4 * y + x))) & 7;
            dst[y * dst_pitch + x] = (a[ai] << 24) | (col[y * 4 + x] & 0x00ffffffu);
        }
}

static u32 unpack_bits(u32 v, u32 mask)
{
    if (!mask) return 0;
    int shift = 0;
    while (!(mask & 1)) { mask >>= 1; ++shift; }
    u32 raw = (v >> shift) & mask;
    u32 max = mask;
    return raw * 255 / max;
}

static SDL_Texture* load_dds(LPCSTR tex, u32& out_w, u32& out_h)
{
    string_path path;
    strconcat(sizeof(path), path, tex, ".dds");
    IReader* r = FS.r_open("$game_textures$", path);
    if (!r)
        r = FS.r_open(path);
    if (!r)
        return nullptr;
    xr_vector<u8> data(r->length());
    r->r(data.data(), (int)r->length());
    FS.r_close(r);
    if (data.size() < sizeof(dds_header))
        return nullptr;
    const dds_header* h = (const dds_header*)data.data();
    if (h->magic != 0x20534444)
        return nullptr;
    u32 w = h->width, hgt = h->height;
    if (!w || !hgt || w > 8192 || hgt > 8192)
        return nullptr;
    xr_vector<u32> px(w * hgt);
    const u8* img = data.data() + 128;
    const u32 fcc = h->fourcc;
    if (fcc == 0x31545844) // DXT1
    {
        int bw = (w + 3) / 4, bh = (hgt + 3) / 4;
        for (int by = 0; by < bh; ++by)
            for (int bx = 0; bx < bw; ++bx)
            {
                const u8* blk = img + (by * bw + bx) * 8;
                for (int y = 0; y < 4 && by * 4 + y < (int)hgt; ++y)
                    for (int x = 0; x < 4 && bx * 4 + x < (int)w; ++x)
                    {
                        u32 tmp[16];
                        dxt1_block(blk, tmp, 4, true);
                        px[(by * 4 + y) * w + bx * 4 + x] = tmp[y * 4 + x];
                    }
            }
    }
    else if (fcc == 0x35545844 || fcc == 0x33545844) // DXT5 / DXT3
    {
        int bw = (w + 3) / 4, bh = (hgt + 3) / 4;
        for (int by = 0; by < bh; ++by)
            for (int bx = 0; bx < bw; ++bx)
            {
                const u8* blk = img + (by * bw + bx) * 16;
                for (int y = 0; y < 4 && by * 4 + y < (int)hgt; ++y)
                    for (int x = 0; x < 4 && bx * 4 + x < (int)w; ++x)
                    {
                        u32 tmp[16];
                        dxt5_block(blk, tmp, 4);
                        px[(by * 4 + y) * w + bx * 4 + x] = tmp[y * 4 + x];
                    }
            }
    }
    else if (h->rgb_bits == 32 || h->rgb_bits == 24 || h->rgb_bits == 16 || h->rgb_bits == 8)
    {
        u32 bpp = h->rgb_bits / 8;
        u32 pitch = h->pitch;
        if (pitch < w * bpp || u64(pitch) * hgt > u64(data.size() - 128))
            pitch = w * bpp;
        for (u32 y = 0; y < hgt; ++y)
            for (u32 x = 0; x < w; ++x)
            {
                const u8* p = img + y * pitch + x * bpp;
                u32 v = 0;
                memcpy(&v, p, bpp > 4 ? 4 : bpp);
                // alpha-only atlases need white rgb so vertex colors modulate correctly
                u32 rr = h->r_mask ? unpack_bits(v, h->r_mask) : 255;
                u32 gg = h->g_mask ? unpack_bits(v, h->g_mask) : 255;
                u32 bb = h->b_mask ? unpack_bits(v, h->b_mask) : 255;
                u32 aa = h->a_mask ? unpack_bits(v, h->a_mask)
                                   : (h->r_mask || h->g_mask || h->b_mask ? 255 : unpack_bits(v, 0xff));
                px[y * w + x] = (aa << 24) | (rr << 16) | (gg << 8) | bb;
            }
    }
    else
        return nullptr;
    out_w = w;
    out_h = hgt;
    SDL_Texture* t = SDL_CreateTexture(
        ensure_renderer(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, (int)w, (int)hgt);
    if (!t)
        return nullptr;
    SDL_UpdateTexture(t, nullptr, px.data(), w * sizeof(u32));
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    return t;
}

// ---- shader ----

class SdlUIShader : public IUIShader
{
public:
    SDL_Texture* tex = nullptr;
    u32 w = 0, h = 0;
    bool ok = false;
    xr_string key;

    void Copy(IUIShader& _in) override
    {
        const SdlUIShader& o = (const SdlUIShader&)_in;
        tex = o.tex; w = o.w; h = o.h; ok = o.ok; key = o.key;
    }
    void create(LPCSTR sh, LPCSTR tex_name, bool) override
    {
        (void)sh;
        key = tex_name ? tex_name : "";
        if (!ensure_renderer())
            return;
        tex = load_dds(tex_name, w, h);
        if (!tex)
        {
            tex = g_missing_tex;
            w = g_missing_w;
            h = g_missing_h;
        }
        ok = true;
    }
    bool inited() override { return ok; }
};

// ---- UI render ----

class SdlUIRender : public IUIRender
{
public:
    SDL_Texture* cur_tex = nullptr;
    Fvector2 cur_res = {0, 0};
    xr_vector<SDL_Vertex> verts;
    xr_vector<int> indices;
    ePrimitiveType prim = ptNone;
    ePointType ptype = pttNone;

    void CreateUIGeom() override { ensure_renderer(); }
    void DestroyUIGeom() override {}

    void SetShader(IUIShader& shader) override
    {
        const SdlUIShader& s = (const SdlUIShader&)shader;
        cur_tex = s.tex ? s.tex : g_missing_tex;
        cur_res.set((float)s.w, (float)s.h);
    }
    void SetAlphaRef(int) override {}
    void SetScissor(Irect* rect) override
    {
        SDL_Renderer* r = ensure_renderer();
        if (!r) return;
        if (rect)
        {
            SDL_Rect sc = {rect->x1, rect->y1, rect->x2 - rect->x1, rect->y2 - rect->y1};
            SDL_RenderSetClipRect(r, &sc);
        }
        else
            SDL_RenderSetClipRect(r, nullptr);
    }
    void GetActiveTextureResolution(Fvector2& res) override { res = cur_res; }
    bool GetTextureResolution(LPCSTR name, Fvector2& res) override
    {
        u32 w = 0, h = 0;
        SDL_Texture* t = load_dds(name, w, h);
        if (!t)
            return false;
        res.set((float)w, (float)h);
        SDL_DestroyTexture(t);
        return true;
    }
    void PushPoint(float x, float y, float z, u32 C, float u, float v) override
    {
        (void)z;
        SDL_Vertex sv;
        sv.position.x = x;
        sv.position.y = y;
        sv.color.r = (C >> 16) & 0xff;
        sv.color.g = (C >> 8) & 0xff;
        sv.color.b = C & 0xff;
        sv.color.a = C >> 24;
        sv.tex_coord.x = u;
        sv.tex_coord.y = v;
        verts.push_back(sv);
    }
    void StartPrimitive(u32 iMaxVerts, ePrimitiveType primType, ePointType pointType) override
    {
        prim = primType;
        ptype = pointType;
        verts.clear();
        verts.reserve(iMaxVerts);
    }
    void FlushPrimitive() override
    {
        SDL_Renderer* r = ensure_renderer();
        if (!r || verts.empty())
            return;
        indices.clear();
        if (prim == ptTriList)
            for (size_t i = 0; i + 2 < verts.size(); i += 3)
            {
                indices.push_back((int)i);
                indices.push_back((int)i + 1);
                indices.push_back((int)i + 2);
            }
        else if (prim == ptTriStrip)
            for (size_t i = 0; i + 2 < verts.size(); ++i)
            {
                int a = (int)i, b = (int)i + 1, c = (int)i + 2;
                if (i & 1)
                {
                    indices.push_back(b);
                    indices.push_back(a);
                }
                else
                {
                    indices.push_back(a);
                    indices.push_back(b);
                }
                indices.push_back(c);
            }
        else if (prim == ptLineList)
        {
            for (size_t i = 0; i + 1 < verts.size(); i += 2)
                SDL_RenderDrawLineF(r, verts[i].position.x, verts[i].position.y,
                                    verts[i + 1].position.x, verts[i + 1].position.y);
            verts.clear();
            return;
        }
        else if (prim == ptLineStrip)
        {
            for (size_t i = 0; i + 1 < verts.size(); ++i)
                SDL_RenderDrawLineF(r, verts[i].position.x, verts[i].position.y,
                                    verts[i + 1].position.x, verts[i + 1].position.y);
            verts.clear();
            return;
        }
        if (indices.empty())
        {
            verts.clear();
            return;
        }
        SDL_RenderGeometry(r, cur_tex, verts.data(), (int)verts.size(),
                           indices.data(), (int)indices.size());
        verts.clear();
    }
    LPCSTR UpdateShaderName(LPCSTR tex_name, LPCSTR sh_name) override { return sh_name; }
    void CacheSetXformWorld(const Fmatrix&) override {}
    void CacheSetCullMode(CullMode) override {}
};

} // namespace

extern ENGINE_API Fvector2 g_current_font_scale;
class dxFontRender : public IFontRender
{
public:
    SDL_Texture* tex = nullptr;
    u32 tw = 0, th = 0;

    void Initialize(LPCSTR cShader, LPCSTR cTexture) override
    {
        (void)cShader;
        if (ensure_renderer())
            tex = load_dds(cTexture, tw, th);
    }

    void OnRender(CGameFont& owner) override
    {
        SDL_Renderer* r = ensure_renderer();
        if (!r || !tex)
            return;
        if (!(owner.uFlags & CGameFont::fsValid))
        {
            owner.vTS.set((int)tw, (int)th);
            owner.fTCHeight = owner.fHeight / float(owner.vTS.y);
            owner.uFlags |= CGameFont::fsValid;
        }
        xr_vector<SDL_Vertex> verts;
        xr_vector<int> indices;
        for (u32 i = 0; i < owner.strings.size(); ++i)
        {
            CGameFont::String& PS = owner.strings[i];
            wide_char wsStr[MAX_MB_CHARS];
            int len = owner.IsMultibyte()
                ? mbhMulti2Wide(wsStr, NULL, MAX_MB_CHARS, PS.string)
                : xr_strlen(PS.string);
            if (!len)
                continue;
            float X = float(iFloor(PS.x));
            float Y = float(iFloor(PS.y));
            float S = PS.height * g_current_font_scale.y;
            float Y2 = Y + S;
            const float hs = fis_zero(owner.fHeight) ? 1.0f : (PS.height / owner.fHeight);
            if (PS.align)
            {
                float fSize = owner.IsMultibyte() ? owner.SizeOf_(wsStr) : owner.SizeOf_(PS.string);
                if (!fis_zero(owner.GetHeight()))
                    fSize *= PS.height / owner.GetHeight();
                if (PS.align == CGameFont::alCenter)
                    X -= (iFloor(fSize * 0.5f)) * g_current_font_scale.x;
                else if (PS.align == CGameFont::alRight)
                    X -= iFloor(fSize);
            }
            u32 clr = PS.c, clr2 = PS.c;
            if (owner.uFlags & CGameFont::fsGradient)
            {
                u32 _R = color_get_R(clr) / 2;
                u32 _G = color_get_G(clr) / 2;
                u32 _B = color_get_B(clr) / 2;
                u32 _A = color_get_A(clr);
                clr2 = color_rgba(_R, _G, _B, _A);
            }
            for (int j = 0; j < len; ++j)
            {
                Fvector l;
                l = owner.IsMultibyte()
                    ? owner.GetCharTC(wsStr[1 + j])
                    : owner.GetCharTC((u16)(u8)PS.string[j]);
                float scw = l.z * hs * g_current_font_scale.x;
                float fTCWidth = l.z / owner.vTS.x;
                if (!fis_zero(l.z))
                {
                    float tu = l.x / owner.vTS.x + (0.5f / owner.vTS.x);
                    float tv = l.y / owner.vTS.y + (0.5f / owner.vTS.y);
                    int base = (int)verts.size();
                    auto quad = [&](float x, float y, u32 c, float u, float v) {
                        SDL_Vertex sv;
                        sv.position.x = x; sv.position.y = y;
                        sv.color.r = (c >> 16) & 0xff;
                        sv.color.g = (c >> 8) & 0xff;
                        sv.color.b = c & 0xff;
                        sv.color.a = c >> 24;
                        sv.tex_coord.x = u; sv.tex_coord.y = v;
                        verts.push_back(sv);
                    };
                    quad(X, Y2, clr2, tu, tv + owner.fTCHeight);
                    quad(X, Y, clr, tu, tv);
                    quad(X + scw, Y2, clr2, tu + fTCWidth, tv + owner.fTCHeight);
                    quad(X + scw, Y, clr, tu + fTCWidth, tv);
                    indices.push_back(base + 0);
                    indices.push_back(base + 1);
                    indices.push_back(base + 3);
                    indices.push_back(base + 0);
                    indices.push_back(base + 3);
                    indices.push_back(base + 2);
                }
                X += scw * owner.vInterval.x;
                if (owner.IsMultibyte())
                {
                    X -= 2;
                    if (IsNeedSpaceCharacter(wsStr[1 + j]))
                        X += owner.fXStep;
                }
            }
        }
        if (!indices.empty())
            SDL_RenderGeometry(r, tex, verts.data(), (int)verts.size(),
                               indices.data(), (int)indices.size());
    }
};

namespace
{
SdlUIRender g_sdl_uirender;
} // namespace

IUIRender* xray_null_sdl_uirender() { return &g_sdl_uirender; }
IUIShader* xray_null_sdl_uishader_create() { return xr_new<SdlUIShader>(); }
void xray_null_sdl_uishader_destroy(IUIShader* sh) { xr_delete((SdlUIShader*&)sh); }
IFontRender* xray_null_sdl_fontrender() { return xr_new<dxFontRender>(); }
void xray_null_sdl_fontrender_destroy(IFontRender* f) { xr_delete((dxFontRender*&)f); }

void xray_null_sdl_frame_begin()
{
    SDL_Renderer* r = ensure_renderer();
    if (!r)
        return;
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderClear(r);
}

static void dump_frame(SDL_Renderer* r, const char* path)
{
    int w, h;
    SDL_RenderGetLogicalSize(r, &w, &h);
    if (!w || !h)
    {
        SDL_GetRendererOutputSize(r, &w, &h);
    }
    xr_vector<u8> px(w * h * 4);
    if (SDL_RenderReadPixels(r, nullptr, SDL_PIXELFORMAT_RGBA32, px.data(), w * 4) != 0)
        return;
    FILE* f = fopen(path, "wb");
    if (!f)
        return;
    fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (int i = 0; i < w * h; ++i)
        fwrite(px.data() + i * 4, 1, 3, f);
    fclose(f);
}
void xray_null_sdl_frame_end()
{
    // The real renderers invoke the PP-UI pass from their combine phase;
    // DoRenderDialogs (menu, console, cursor) only runs through it.
    if (g_pGamePersistent)
        g_pGamePersistent->OnRenderPPUI_main();
    SDL_Renderer* r = ensure_renderer();
    if (r)
        SDL_RenderPresent(r);
    static int fr = 0;
    static const char* dump_path = getenv("XRAY_FRAME_DUMP");
    static int dump_at = dump_path ? atoi(getenv("XRAY_FRAME_DUMP_AT") ? getenv("XRAY_FRAME_DUMP_AT") : "30") : -1;
    if (r && dump_at > 0 && ++fr >= dump_at)
    {
        dump_at = 0;
        dump_frame(r, dump_path);
    }
}

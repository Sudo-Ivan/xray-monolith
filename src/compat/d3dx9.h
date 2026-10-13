#pragma once

// Minimal D3DX9 helpers used by the engine (FVF vertex size, declaration
// helpers, matrix inverse). d3d9compat.hpp supplies the D3D9 types.

#include "../Common/d3d9compat.hpp"

typedef Fmatrix D3DXMATRIX;

inline D3DXMATRIX* D3DXMatrixInverse(D3DXMATRIX* out, float* det, const D3DXMATRIX* m)
{
    if (det)
        *det = 1.f; // Fmatrix has no det(); 4x3 invert only
    ((Fmatrix*)out)->invert(*((const Fmatrix*)m));
    return out;
}

// FVF vertex size in bytes - mirror of D3DXGetFVFVertexSize
inline u32 D3DXGetFVFVertexSize(u32 fvf)
{
    u32 size = 0;
    switch (fvf & D3DFVF_POSITION_MASK)
    {
    case D3DFVF_XYZ: size += 12; break;
    case D3DFVF_XYZRHW: size += 16; break;
    case D3DFVF_XYZB1: size += 16; break;
    case D3DFVF_XYZB2: size += 20; break;
    case D3DFVF_XYZB3: size += 24; break;
    case D3DFVF_XYZB4: size += 28; break;
    case D3DFVF_XYZB5: size += 32; break;
    case D3DFVF_XYZW: size += 24; break;
    }
    if (fvf & D3DFVF_NORMAL)
        size += 12;
    if (fvf & D3DFVF_PSIZE)
        size += 4;
    if (fvf & D3DFVF_DIFFUSE)
        size += 4;
    if (fvf & D3DFVF_SPECULAR)
        size += 4;
    u32 tex_count = (fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
    for (u32 i = 0; i < tex_count; ++i)
    {
        u32 dims = (fvf >> (16 + i * 2)) & 0x3;
        static const u32 dim_size[] = {0, 8, 12, 16};
        size += dim_size[dims];
    }
    return size;
}

// FVF -> vertex element declaration (mirrors D3DXDeclaratorFromFVF)
inline u32 D3DXDeclaratorFromFVF(u32 fvf, D3DVERTEXELEMENT9* decl)
{
    u32 n = 0;
    u16 offset = 0;
    auto emit = [&](u8 type, u8 usage, u8 usage_idx, u16 size) {
        decl[n].Stream = 0;
        decl[n].Offset = offset;
        decl[n].Type = type;
        decl[n].Method = (u8)D3DDECLMETHOD_DEFAULT;
        decl[n].Usage = usage;
        decl[n].UsageIndex = usage_idx;
        offset += size;
        ++n;
    };

    u32 pos = fvf & D3DFVF_POSITION_MASK;
    if (pos == D3DFVF_XYZ)
        emit(D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_POSITION, 0, 12);
    else if (pos == D3DFVF_XYZRHW)
        emit(D3DDECLTYPE_FLOAT4, D3DDECLUSAGE_POSITIONT, 0, 16);
    else if (pos == D3DFVF_XYZW)
        emit(D3DDECLTYPE_FLOAT4, D3DDECLUSAGE_POSITION, 0, 24);
    else if (pos >= D3DFVF_XYZB1 && pos <= D3DFVF_XYZB5)
    {
        u32 blends = pos - D3DFVF_XYZB1 + 1;
        emit(D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_POSITION, 0, 12);
        u32 wsize = blends * 4;
        emit(D3DDECLTYPE_FLOAT1 + (blends - 1), D3DDECLUSAGE_BLENDWEIGHT, 0, (u16)wsize);
        if (blends < 5)
            emit(D3DDECLTYPE_UBYTE4, D3DDECLUSAGE_BLENDINDICES, 0, 4);
        else
            emit(D3DDECLTYPE_UBYTE4, D3DDECLUSAGE_BLENDINDICES, 0, 0);
    }

    if (fvf & D3DFVF_NORMAL)
        emit(D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_NORMAL, 0, 12);
    if (fvf & D3DFVF_PSIZE)
        emit(D3DDECLTYPE_FLOAT1, D3DDECLUSAGE_PSIZE, 0, 4);
    if (fvf & D3DFVF_DIFFUSE)
        emit(D3DDECLTYPE_D3DCOLOR, D3DDECLUSAGE_COLOR, 0, 4);
    if (fvf & D3DFVF_SPECULAR)
        emit(D3DDECLTYPE_D3DCOLOR, D3DDECLUSAGE_COLOR, 1, 4);

    u32 tex_count = (fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
    for (u32 i = 0; i < tex_count; ++i)
    {
        u32 dims = (fvf >> (16 + i * 2)) & 0x3;
        static const u8 dim_type[] = {0, D3DDECLTYPE_FLOAT2, D3DDECLTYPE_FLOAT3, D3DDECLTYPE_FLOAT4};
        static const u16 dim_size[] = {0, 8, 12, 16};
        if (dims == 0)
            continue;
        emit(dim_type[dims], D3DDECLUSAGE_TEXCOORD, (u8)i, dim_size[dims]);
    }

    decl[n].Stream = 0xFF;
    decl[n].Offset = 0;
    decl[n].Type = D3DDECLTYPE_UNUSED;
    decl[n].Method = 0;
    decl[n].Usage = 0;
    decl[n].UsageIndex = 0;
    return n + 1;
}

inline u32 D3DXGetDeclLength(const D3DVERTEXELEMENT9* decl)
{
    if (!decl)
        return 0;
    u32 n = 0;
    while (decl[n].Stream != 0xFF && n < MAXD3DDECLLENGTH + 1)
        ++n;
    return n;
}

inline u32 D3DXGetDeclVertexSize(const D3DVERTEXELEMENT9* decl, u32 stream)
{
    if (!decl)
        return 0;
    u32 size = 0;
    static const u16 type_size[] = {
        4, 8, 12, 16, // FLOAT1..FLOAT4
        4, 4,         // D3DCOLOR, UBYTE4
        4, 8,         // SHORT2, SHORT4
        4,            // UBYTE4N
        4, 8,         // SHORT2N, SHORT4N
        8,            // USHORT2N
        16,           // USHORT4N
        8, 4,         // UDEC3, DEC3N
        4,            // FLOAT16_2
        8,            // FLOAT16_4
        0             // UNUSED
    };
    for (u32 i = 0; decl[i].Stream != 0xFF && i < MAXD3DDECLLENGTH + 1; ++i)
    {
        if (decl[i].Stream == stream)
            size += type_size[decl[i].Type];
    }
    return size;
}

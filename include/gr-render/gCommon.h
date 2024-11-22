#pragma once

#include <unordered_map>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

#include <gr-math/gmath.h>

#define GR_ASSERT(msg) assert(false && msg)

enum class BufferType {
    VBO,
    EBO,
    PBO 
};

enum class BufferUsage {
    STATIC,
    DYNAMIC
};

enum class PrimitiveType {
    POINTS,
    LINES,
    LINE_LOOP,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLES_STRIP,
    TRIANGLES_FAN
};

namespace grr {
    class gRenderbuffer;
    class gVertexArray;
    class gFramebuffer;
    class gTexture;
    class gRender;
    class gShader;

    enum RenderState : grm::u64 {
        GR_FALSE               = 1 << 1,
        GR_TRUE                = 1 << 2,
        GR_BACKGROUND_COLOR    = 1 << 3,
        GR_BACKGROUND          = 1 << 4,
        GR_DEPTH_BUFFER        = 1 << 5,
        GR_COLOR_BUFFER        = 1 << 6,
        GR_CULL_FACE           = 1 << 7,
        GR_CULL                = 1 << 8,
        GR_FRONT               = 1 << 9,
        GR_BACK                = 1 << 10,
        GR_VIEWPORT            = 1 << 11,
        GR_DEPTH               = 1 << 12,
        GR_DEPTH_MASK          = 1 << 13,
        GR_DEPTH_FUNC          = 1 << 14,
        GR_DEPTH_ALWAYS        = 1 << 15,
        GR_DEPTH_NEVER         = 1 << 16,
        GR_DEPTH_LESS          = 1 << 17,
        GR_DEPTH_EQUAL         = 1 << 18,
        GR_DEPTH_LEQUAL        = 1 << 19,
        GR_DEPTH_GREATER       = 1 << 20,
        GR_DEPTH_NOTEQUAL      = 1 << 21,
        GR_DEPTH_GEQUAL        = 1 << 22,
        GR_MULTISAMPLE         = 1 << 23,
        GR_FRAMEBUFFER_SRGB    = 1 << 24,
        GR_BLEND               = 1 << 25,
        GR_SRC_ALPHA           = 1 << 26,
        GR_ONE_MINUS_SRC_ALPHA = 1 << 27
    };

    enum BufferBindingTarget {
        GR_ARRAY_BUFFER              = 1 << 1,
        // GR_COPY_READ_BUFFER          = 1 << 3,
        // GR_COPY_WRITE_BUFFER         = 1 << 4,
        // GR_DRAW_INDIRECT_BUFFER      = 1 << 5,
        GR_ELEMENT_ARRAY_BUFFER      = 1 << 6,
        // GR_PIXEL_PACK_BUFFER         = 1 << 7,
        // GR_PIXEL_UNPACK_BUFFER       = 1 << 8,
        // GR_TEXTURE_BUFFER            = 1 << 11, // OpenGL ES 3.1
        // GR_TRANSFORM_FEEDBACK_BUFFER = 1 << 12,
        // GR_UNIFORM_BUFFER            = 1 << 13,
    };

    enum TextureFormat : grm::u16 {
        TextureFormat_SRGB = 1 << 0,
        TextureFormat_RGB = 1 << 1,
        TextureFormat_RGB332 = 1 << 2,
        TextureFormat_RGB444 = 1 << 3,
        TextureFormat_RGB565 = 1 << 4,
        TextureFormat_RGB888 = 1 << 5,

        TextureFormat_SRGBA = 1 << 6,
        TextureFormat_RGBA = 1 << 8,
        TextureFormat_RGBA4444 = 1 << 9,
        TextureFormat_RGBA8888 = 1 << 10,
        TextureFormat_DepthComponent = 1 << 11
    };

    enum gTextureFlags : grm::u32 {
        gTextureFlags_None                = 1 << 0,
        gTextureFlags_MipMaps             = 1 << 1,
        gTextureFlags_Texture             = 1 << 2,
        gTextureFlags_Cubemap             = 1 << 3,
        gTextureFlags_Filter_Linear       = 1 << 4,
        gTextureFlags_Filter_Trilinear    = 1 << 5,
        gTextureFlags_Filter_Bilinear     = 1 << 6,
        gTextureFlags_Cubemap_Positive_X  = 1 << 7,
        gTextureFlags_Cubemap_Negative_X  = 1 << 8,
        gTextureFlags_Cubemap_Positive_Y  = 1 << 9,
        gTextureFlags_Cubemap_Negative_Y  = 1 << 10,
        gTextureFlags_Cubemap_Positive_Z  = 1 << 11,
        gTextureFlags_Cubemap_Negative_Z  = 1 << 12,
        gTextureFlags_Clamp_Repeat        = 1 << 14,
        gTextureFlags_Clamp_Border        = 1 << 15,
        gTextureFlags_Clamp_Edge        = 1 << 16,
    };

    enum gFramebufferFlags : grm::u32 {
        gFramebufferFlags_Color_Attachiment0 = 1 << 0,
        gFramebufferFlags_Color_Attachiment1 = 1 << 1,
        gFramebufferFlags_Color_Attachiment2 = 1 << 2,
        gFramebufferFlags_Color_Attachiment3 = 1 << 3,
        gFramebufferFlags_Color_Attachiment4 = 1 << 4,
        gFramebufferFlags_Color_Attachiment5 = 1 << 5,
        gFramebufferFlags_Depth_Attachiment  = 1 << 7,
        gFramebufferFlags_Cubemap_Positive_X  = 1 << 8,
        gFramebufferFlags_Cubemap_Negative_X  = 1 << 9,
        gFramebufferFlags_Cubemap_Positive_Y  = 1 << 10,
        gFramebufferFlags_Cubemap_Negative_Y  = 1 << 11,
        gFramebufferFlags_Cubemap_Positive_Z  = 1 << 12,
        gFramebufferFlags_Cubemap_Negative_Z  = 1 << 13,
        gFramebufferFlags_Cubemap             = 1 << 14,
        gFramebufferFlags_Texture             = 1 << 15,
    };

    enum RenderbufferType {
        RenderbufferType_Depth_Component     = 1 << 0,
        RenderbufferType_Depth_Component16   = 1 << 1,
        RenderbufferType_Depth_Component24   = 1 << 2,
        RenderbufferType_Depth_Component32F  = 1 << 3,
        RenderbufferType_Depth24_Stencil8    = 1 << 4,
        RenderbufferType_Depth_Stencil       = 1 << 5,
        RenderbufferType_Depth32F_Stencil8   = 1 << 6,
        RenderbufferType_Stencil             = 1 << 7
    };
};



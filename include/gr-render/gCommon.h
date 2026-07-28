#pragma once

#include <cstdint>

using BufferType_       =       uint32_t;
using BufferID          =       uint32_t;

using PrimitiveType_    =       uint32_t;

using TextureFlags_     =       uint32_t;

typedef uint32_t TextureID;
typedef uint32_t TextureFlags;

using gTextureCubemapFace_  =   uint32_t;

// Shader
typedef uint32_t UniformID;

typedef uint32_t ShaderID;

typedef uint32_t RenderbufferID;

typedef uint32_t VertexID;

// ***** Renderbuffer ***** //

#define GR_INVALID_ID (uint32_t)-1

enum BufferType : BufferType_
{
    VBO     =   0,  // Vertex Buffer Object
    EBO     =   1,  // Element Buffer Object
    FBO     =   2,  // Frame Buffer Object
    RBO     =   3,  // Render Buffer Object
    TBO     =   4  // Texture Buffer Object
};

enum class BufferUsage {
    STATIC,
    DYNAMIC
};

enum PrimitiveType : PrimitiveType_
{
    POINTS              = 0,
    LINES               = 1,
    LINE_LOOP           = 2,
    LINE_STRIP          = 3,
    TRIANGLES           = 4,
    TRIANGLES_STRIP     = 5,
    TRIANGLES_FAN       = 6
};

typedef struct TextureFormatInfo
{
    std::uint32_t internalformat;
    std::uint32_t format;
    std::uint32_t type;
} TextureFormatInfo;

// SetEnable
#define GR_CULL_FACE            0
#define GR_DEPTH                1
#define GR_MULTISAMPLE          2
#define GR_FRAMEBUFFER_SRGB     3
#define GR_BLEND                4

// depth func
#define GR_DEPTH_FUNC_ALWAYS    0
#define GR_DEPTH_FUNC_NEVER     1
#define GR_DEPTH_FUNC_LESS      2
#define GR_DEPTH_FUNC_EQUAL     3
#define GR_DEPTH_FUNC_LEQUAL    4
#define GR_DEPTH_FUNC_GREATER   5
#define GR_DEPTH_FUNC_NOTEQUAL  6
#define GR_DEPTH_FUNC_GEQUAL    7

typedef uint32_t GEnum;

namespace gr
{
    enum RenderState : uint64_t {
        GR_FALSE               = 1 << 1,
        GR_TRUE                = 1 << 2,
        GR_BACKGROUND          = 1 << 4,
        GR_DEPTH_BUFFER        = 1 << 5,
        GR_COLOR_BUFFER        = 1 << 6,
        GR_CULL                = 1 << 8,
        GR_FRONT               = 1 << 9,
        GR_BACK                = 1 << 10,
        // GR_DEPTH_MASK          = 1 << 13,
        // GR_DEPTH_FUNC          = 1 << 14,
        // GR_SRC_ALPHA           = 1 << 26,
        GR_ONE_MINUS_SRC_ALPHA = 1 << 27
    };
};



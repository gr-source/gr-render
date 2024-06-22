#pragma once

#include <unordered_map>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

#include <gr-math/gmath.h>

#include "gVertex2D.h"
#include "gVertex3D.h"
#include "gTypes.h"

#define GR_ASSERT(msg) assert(false && msg)

namespace grr {
    class gVertexArray;
    class gTexture;
    class gRender;
    class gShader;

    enum RenderState : u32 {
        GR_FALSE            = 1 << 1,
        GR_TRUE             = 1 << 2,
        GR_BACKGROUND_COLOR = 1 << 3,
        GR_BACKGROUND       = 1 << 4,
        GR_DEPTH_BUFFER     = 1 << 5,
        GR_COLOR_BUFFER     = 1 << 6,
        GR_CULL_FACE        = 1 << 7,
        GR_CULL             = 1 << 8,
        GR_FRONT            = 1 << 9,
        GR_BACK             = 1 << 10,
        GR_VIEWPORT         = 1 << 11,
        GR_DEPTH            = 1 << 12,
        GR_DEPTH_MASK       = 1 << 13,
        GR_DEPTH_FUNC       = 1 << 14,
        GR_DEPTH_ALWAYS     = 1 << 15,
        GR_DEPTH_NEVER      = 1 << 16,
        GR_DEPTH_LESS       = 1 << 17,
        GR_DEPTH_EQUAL      = 1 << 18,
        GR_DEPTH_LEQUAL     = 1 << 19,
        GR_DEPTH_GREATER    = 1 << 20,
        GR_DEPTH_NOTEQUAL   = 1 << 21,
        GR_DEPTH_GEQUAL     = 1 << 22,
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

    enum TextureFormat : u16 {
        TextureFormat_RGB = 1 << 1,
        TextureFormat_RGB332 = 1 << 2,
        TextureFormat_RGB444 = 1 << 3,
        TextureFormat_RGB565 = 1 << 4,
        TextureFormat_RGB888 = 1 << 5,

        TextureFormat_RGBA = 1 << 8,
        TextureFormat_RGBA4444 = 1 << 9,
        TextureFormat_RGBA8888 = 1 << 10
    };

    enum BufferType : u32 {
        BufferType_VBO = 1 << 1,
        BufferType_EBO = 1 << 2,
        BufferType_DEFAULT = 1 << 1
    };

    enum PrimitiveType {
        PrimitiveType_Points = 0,
        PrimitiveType_Lines = 1,
        PrimitiveType_LineLoop = 2,
        PrimitiveType_LineStrip = 3,
        PrimitiveType_Triangles = 4,
        PrimitiveType_TriangleStrip = 5,
        PrimitiveType_TriangleFan = 6
    };
};



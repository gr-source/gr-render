#pragma once

#include "gCommon.h"

#define TEXTURE_TYPE_2D               0
#define TEXTURE_TYPE_CUBE             1

#define TEXTURE_FILTER_NEAREST                    0
#define TEXTURE_FILTER_LINEAR                     1

#define TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST    2
#define TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST     3
#define TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR     4
#define TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR      5

#define TEXTURE_WRAP_REPEAT           0
#define TEXTURE_WRAP_CLAMP            1
#define TEXTURE_WRAP_MIRROR           2

#define TEXTURE_FORMAT_RED      0
#define TEXTURE_FORMAT_RG16F    1
#define TEXTURE_FORMAT_RG       2
#define TEXTURE_FORMAT_RGB16F   3
#define TEXTURE_FORMAT_RGB      4
#define TEXTURE_FORMAT_RGB8     5
#define TEXTURE_FORMAT_RGBA16F  6
#define TEXTURE_FORMAT_RGBA     7
#define TEXTURE_FORMAT_RGBA8    8
#define TEXTURE_FORMAT_SRGB     9
#define TEXTURE_FORMAT_SRGBA    10

#define TEXTURE_FORMAT_DEPTH 11

namespace gr
{
    typedef struct texture
    {
        uint32_t id = GR_INVALID_ID;

        uint32_t width = 0;
        uint32_t height = 0;
        union {
            uint32_t flags = 0;
            struct
            {
                uint32_t type     : 2; 
                uint32_t filter   : 4;
                uint32_t wrap     : 2;
                uint32_t format   : 5;
                uint32_t reserved : 19;
            };
        };

        void create();

        void destroy();

        void bind() const;

        void unbind() const;

        void applyTextureBuffer(const void* pixels);

        void applyTextureSettings();
    } texture;

} // namespace gr

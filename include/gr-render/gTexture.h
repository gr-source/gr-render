#pragma once

#include "gCommon.h"

#define TEXTURE_TYPE_2D               0
#define TEXTURE_TYPE_CUBE             1

#define TEXTURE_FILTER_NEAREST           0
#define TEXTURE_FILTER_LINEAR            1
#define TEXTURE_FILTER_NEAREST_MIPMAP    2
#define TEXTURE_FILTER_LINEAR_MIPMAP     3

#define TEXTURE_WRAP_REPEAT           0
#define TEXTURE_WRAP_CLAMP            1
#define TEXTURE_WRAP_MIRROR           2

namespace gr
{
    typedef struct Texture
    {
        uint32_t width;
        uint32_t height;
        TextureID id;
        union {
            TextureFlags flags;
            struct
            {
                uint32_t type     : 2; 
                uint32_t filter   : 4;
                uint32_t wrap     : 2;
                uint32_t format   : 5;
                uint32_t reserved : 19;
            };
        };
    } Texture;

    void BindTexture(Texture* texture);

    void UnbindTexture(Texture* texture);

    void ApplyTextureBufffer(Texture* texture, void* pixels);

    void ApplyTextureSettings(Texture* texture);

    class gTexture
    {
    public:
        static void Unbind(u32 index);

        gTexture();
        ~gTexture();

        void updateBuffer(u32 width, u32 height, void* pixels);

        void bind(u32 index);

        void unbind();

        u32 getTargetTexture() const;

        TextureID getTextureID() const;

        bool isValid() const;

        bool isCubemap() const;

        bool isTexture2D() const;
        
        void set_filtering(TextureFlags_ flags);

        void set_clamping(TextureFlags_ flags);

        void set_texture(TextureFlags_ flags);

        void set_format(TextureFormat format);

        void set_face(gTextureCubemapFace_ face);

        void generate_mipmaps();

    private:
        static const TextureFormatInfo TextureFormatInfoMapping[20];

        u32 m_height;
        
        u32 m_width;

        TextureID textureID;

        TextureFlags_ texture_flags;

        u32 m_active;

        TextureFormat m_format;

        void apply_clamping() const;

        void apply_filtering() const;

        void apply_mipmaps() const;
    };
} // namespace gr

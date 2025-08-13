#pragma once

#include "gCommon.h"

namespace grr {
    class gTexture {
    public:
        static void Unbind(grm::uint32 index);

        static gTexture *GetCurrent();

        gTexture();
        ~gTexture();

        void updateBuffer(grm::uint32 width, grm::uint32 height, void* pixels);

        void bind(grm::uint32 index);

        void unbind();

        grm::uint32 getTargetTexture() const;

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

        static gTexture *s_current;

        grm::uint32 m_height;
        
        grm::uint32 m_width;

        TextureID textureID;

        TextureFlags_ texture_flags;

        grm::uint32 m_active;

        TextureFormat m_format;

        void apply_clamping() const;

        void apply_filtering() const;

        void apply_mipmaps() const;
    };
} // namespace gr

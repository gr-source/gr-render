#pragma once

#include "gCommon.h"

namespace grr {
    class gTexture {
    public:
        static gTexture* Create(grm::u32 width, grm::u32 height, gTextureFlags flags, TextureFormat format, void* pixels);

        static void PushTexture(gTexture *texture, grm::u32 width, grm::u32 height, gTextureFlags flags, TextureFormat format, void* pixels);

        static void Bind(const gTexture *texture, int idx);

        static void Unbind();

        gTexture();
        ~gTexture();

        void bind(int texture = 0);

        void unbind();

        const grm::u32& getID() const;

        const bool isValid() const;
        
    private:
        grm::u32 m_height;
        grm::u32 m_width;
        grm::u32 m_index;

        TextureFormat m_format;

        static std::unordered_map<grm::u32, grm::u32> m_textureMap;
    };
} // namespace gr

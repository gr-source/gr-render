#pragma once

#include "gCommon.h"

namespace grr {
    class gTexture {
    public:
        static gTexture* Create(u32 width, u32 height, TextureFormat format, void* pixels);

        static void PushTexture(gTexture *texture, u32 width, u32 height, TextureFormat format, void* pixels);

        static void Bind(const gTexture *texture, int idx);

        static void Unbind();

        gTexture();
        ~gTexture();

        void bind(int texture = 0);

        void unbind();

        const u32& getID() const;

        const bool isValid() const;
        
    private:
        u32 m_height;
        u32 m_width;
        u32 m_index;

        TextureFormat m_format;

        static std::unordered_map<u32, u32> m_textureMap;
    };
} // namespace gr

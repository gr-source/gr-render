#pragma once

#include "gCommon.h"

namespace grr {
    class gTexture {
    public:
        static gTexture* Create();

        static void UpdateTexture(grm::u32 width, grm::u32 height, grm::u32 flags, TextureFormat format, void* pixels);

        static void Bind(gTexture *texture, int idx = 0);

        static void Unbind();

        static gTexture *GetCurrent();

        gTexture();
        ~gTexture();

        grm::u32 getID() const;

        bool isValid() const;
        
        grm::u32 getTargetTexture();

        static void Release();

    private:
        static std::unordered_map<grm::u32, grm::u32> m_textureMap;
        
        static gTexture *s_current;

        grm::u32 m_height;
        
        grm::u32 m_width;

        grm::u32 m_index;

        grm::u32 m_flags;

        TextureFormat m_format;
    };
} // namespace gr

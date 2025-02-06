#pragma once

#include "gCommon.h"

namespace grr {
    class gTexture {
    public:
        static void Unbind(grm::u32 index);

        static gTexture *GetCurrent();

        gTexture();
        ~gTexture();

        void updateBuffer(grm::u32 width, grm::u32 height, grm::u32 flags, TextureFormat format, void* pixels);

        void bind(grm::u32 index);

        void unbind();

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

        grm::u32 m_active;

        TextureFormat m_format;
    };
} // namespace gr

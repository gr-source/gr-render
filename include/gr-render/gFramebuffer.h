#pragma once

#include "gRenderbuffer.h"

namespace grr {
    class gFramebuffer {
    public:
        static grm::u32 Create();

        static void Bind(grm::u32 id);

        static void SetRenderbuffer(gFramebufferFlags attachment);

        static void SetTexture(gFramebufferFlags attachment, gFramebufferFlags textarget = gFramebufferFlags_Texture);

        static void Unbind();

        static void Destroy(grm::u32 id);

        static void GetPixels(grm::u8 attachmentID, int x, int y, grm::u32 width, grm::u32 height, TextureFormat format, void *pixels);
        
        static void Release();
        
    private:
        static grm::u32 s_current;

        static std::unordered_map<gFramebufferFlags, grm::u32> m_apiFramebuffer;
    };

} // namespace grr

#pragma once

#include "gRenderbuffer.h"

namespace grr {
    class gFramebuffer {
    public:
        static grm::uint32 Create();

        static void Bind(grm::uint32 id);

        static void SetRenderbuffer(gFramebufferFlags attachment);

        static void SetTexture(gFramebufferFlags attachment, gFramebufferFlags textarget = gFramebufferFlags_Texture);

        static void Unbind();

        static void Destroy(grm::uint32 id);

        static void GetPixels(grm::uint8 attachmentID, int x, int y, grm::uint32 width, grm::uint32 height, TextureFormat format, void *pixels);
        
        static void Release();
        
    private:
        static grm::uint32 s_current;

        static std::unordered_map<gFramebufferFlags, grm::uint32> m_apiFramebuffer;
    };

} // namespace grr

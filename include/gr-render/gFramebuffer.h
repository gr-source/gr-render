#pragma once

#include "gRenderbuffer.h"

namespace grr
{
    class gFramebuffer
    {
    public:
        static u32 Create();

        static void Bind(u32 id);

        static void SetRenderbuffer(gFramebufferFlags attachment);

        static void SetTexture(grr::gTexture* texture, gFramebufferFlags attachment, gFramebufferFlags textarget = gFramebufferFlags_Texture);

        static void Unbind();

        static void Destroy(u32 id);

        static void GetPixels(u8 attachmentID, int x, int y, u32 width, u32 height, TextureFormat format, void *pixels);
        
        static void Release();
        
    private:
        static u32 s_current;

        static std::unordered_map<gFramebufferFlags, u32> m_apiFramebuffer;
    };

} // namespace grr

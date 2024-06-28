#pragma once

#include "gCommon.h"

namespace grr {
    class gFramebuffer {
    public:
        static u32 Create();

        static void Bind(u32 id);

        static void SetRenderbuffer(FramebufferType attachment, u32 id);

        static void SetTexture(FramebufferType attachment, u32 id);

        static void Unbind();

        static void Destroy(u32 id);
    private:
        static u32 s_current;

        static std::unordered_map<FramebufferType, u32> m_apiFramebuffer;
    };

} // namespace grr

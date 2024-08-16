#pragma once

#include "gRenderbuffer.h"

namespace grr {
    class gFramebuffer {
    public:
        static grm::u32 Create();

        static void Bind(grm::u32 id);

        static void SetRenderbuffer(FramebufferType attachment, grm::u32 id);

        static void SetTexture(FramebufferType attachment, grm::u32 id);

        static void Unbind();

        static void Destroy(grm::u32 id);
    private:
        static grm::u32 s_current;

        static std::unordered_map<FramebufferType, grm::u32> m_apiFramebuffer;
    };

} // namespace grr

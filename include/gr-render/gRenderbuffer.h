#pragma once

#include "gCommon.h"

namespace grr {
    class gRenderbuffer {
    public:
        static grm::u32 Create();

        static void Bind(grm::u32 id);

        static void SetBufferStorage(RenderbufferType internalFormat, grm::u32 width, grm::u32 height);

        static void Unbind();

        static void Destoy(grm::u32 id);

        static grm::u32 &GetCurrent();

        static void Release();

    private:
        static grm::u32 s_current;

        static std::unordered_map<RenderbufferType, grm::u32> s_renderbufferApi;

        friend class gFramebuffer;
    };
} // namespace grr

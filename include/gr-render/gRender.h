#pragma once

#include "gCommon.h"

namespace grr {
    class gRender {
    public:

        static void SetRenderState(RenderState state, void* value);

        static void SetRenderState(RenderState state, grm::u32 value);

        static std::string getRenderStateName(RenderState state);

        static bool Initialize();

        static void Release();

    private:
        static std::unordered_map<BufferBindingTarget, grm::u32> m_bufferMap;

        static std::unordered_map<grm::u32, grm::u32> m_renderStateMap;
    };
}
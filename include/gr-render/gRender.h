#pragma once

#include "gColor.h"

#include "gFramebuffer.h"
#include "gVertexArray.h"
#include "gTexture.h"
#include "gShader.h"
#include "gError.h"

#include <sstream>
#include <iomanip>
#include <string>

#define MAX_BLOCK_BUFFER 1024

namespace grr {
    class gRender {
    public:

        static void SetRenderState(RenderState state, void* value);

        static void SetRenderState(RenderState state, grm::u32 value);

        static const std::string getRenderStateName(RenderState state);

        static const bool Initialize();
    private:
        static std::unordered_map<BufferBindingTarget, grm::u32> m_bufferMap;

        static std::unordered_map<PrimitiveType, grm::u32> m_primitiveMap;

        static std::unordered_map<grm::u32, grm::u32> m_renderStateMap;

    };
}
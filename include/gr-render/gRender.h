#pragma once

#include "gCommon.h"

namespace grr {
    class gRender
    {
    public:
        static void SetBackgroundColor(const Color& color);

        static void SetViewport(const Rect& bounds);

        static void SetEnable(GEnum state, bool value);

        static void SetRenderState(RenderState state, u32 value);

        static std::string getRenderStateName(RenderState state);

        static bool Initialize();

        static void Release();

    private:
        static std::unordered_map<BufferBindingTarget, u32> m_bufferMap;

        static std::unordered_map<u32, u32> m_renderStateMap;

        static gRender& GetInstance();

        // fields
        Color s_BackgroundColor;

        Rect s_ViewportBounds;

        u32 s_StateMask;

        // methods
        gRender();

        void setBackgroundColor(const Color& color);

        void setViewport(const Rect& bounds);

        void setEnable(GEnum state, bool value);
    };
}

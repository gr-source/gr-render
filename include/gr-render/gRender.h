#pragma once

#include "gCommon.h"

#include <gr-math/gmath.hpp>

#include <unordered_map>

namespace gr {
    class gRender
    {
    public:
        static void SetBackgroundColor(const Color& color);

        static void SetViewport(const Rect& bounds);

        static void SetEnable(GEnum state, bool value);

        static void SetDepthFunc(GEnum func);

        static void SetRenderState(RenderState state, u32 value);

        static bool Initialize();

    private:
        static gRender& GetInstance();

        // fields
        Color s_BackgroundColor;

        Rect s_ViewportBounds;

        uint32_t s_StateMask;

        uint32_t s_DepthFunc;

        // methods
        gRender();

        void setBackgroundColor(const Color& color);

        void setViewport(const Rect& bounds);

        void setEnable(GEnum state, bool value);

        void setDepthFunc(GEnum func);
    };
}

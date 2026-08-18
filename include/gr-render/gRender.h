#pragma once

#include "gCommon.h"

#include "color.hpp"
#include "rect.hpp"

namespace gr {
    class gRender
    {
    public:
        static void SetBackgroundColor(const Color& color);

        static void SetViewport(const Rect& viewport);

        static void SetEnable(GEnum state, bool value);

        static void SetDepthFunc(GEnum func);

        static void SetRenderState(RenderState state, uint32_t value);

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

        void setViewport(const Rect& viewport);

        void setEnable(GEnum state, bool value);

        void setDepthFunc(GEnum func);
    };
}

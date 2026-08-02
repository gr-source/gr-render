#pragma once

#include "gCommon.h"

#include <unordered_map>

namespace gr {
    class gRender
    {
    public:
        static void SetBackgroundColor(const float color[4]);

        static void SetViewport(const float bounds[4]);

        static void SetEnable(GEnum state, bool value);

        static void SetDepthFunc(GEnum func);

        static void SetRenderState(RenderState state, uint32_t value);

        static bool Initialize();

    private:
        static gRender& GetInstance();

        // fields
        float s_BackgroundColor[4];

        float s_ViewportBounds[4];

        uint32_t s_StateMask;

        uint32_t s_DepthFunc;

        // methods
        gRender();

        void setBackgroundColor(const float color[4]);

        void setViewport(const float bounds[4]);

        void setEnable(GEnum state, bool value);

        void setDepthFunc(GEnum func);
    };
}

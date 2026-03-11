#include "gRender.h"

#include "gFramebuffer.h"
#include "gVertexArray.h"
#include "gTexture.h"
#include "gShader.h"
#include "gError.h"
#include "gl.h"

static const GLenum GL_ENABLE_DISABLE_MAP[] = {
    GL_CULL_FACE,
    GL_DEPTH_TEST,
    GL_MULTISAMPLE,
    GL_FRAMEBUFFER_SRGB,
    GL_BLEND
};

namespace grr {
    std::unordered_map<BufferBindingTarget, u32> gRender::m_bufferMap {
        {BufferBindingTarget::GR_ARRAY_BUFFER, GL_ARRAY_BUFFER},
        {BufferBindingTarget::GR_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER}
    };

    std::unordered_map<u32, u32> gRender::m_renderStateMap = {
        {GR_DEPTH_ALWAYS, GL_ALWAYS},
        {GR_DEPTH_NEVER, GL_NEVER},
        {GR_DEPTH_LESS, GL_LESS},
        {GR_DEPTH_EQUAL, GL_EQUAL},
        {GR_DEPTH_LEQUAL, GL_LEQUAL},
        {GR_DEPTH_GREATER, GL_GREATER},
        {GR_DEPTH_NOTEQUAL, GL_NOTEQUAL},
        {GR_FALSE, GL_FALSE},
        {GR_TRUE, GL_TRUE},
        {GR_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
    };

    void gRender::SetBackgroundColor(const Color &color)
    {
        GetInstance().setBackgroundColor(color);
    }

    void gRender::SetViewport(const Rect &bounds)
    {
        GetInstance().setViewport(bounds);
    }

    void gRender::SetEnable(GEnum state, bool value)
    {
        GetInstance().setEnable(state, value);
    }

    void gRender::SetRenderState(RenderState state, u32 value)
    {
        switch (state) {
        case GR_BACKGROUND: {
            GLbitfield filter = 0;
            if ((value & GR_DEPTH_BUFFER) == GR_DEPTH_BUFFER) {
                filter |= GL_DEPTH_BUFFER_BIT;
            }
            if ((value & GR_COLOR_BUFFER) == GR_COLOR_BUFFER) {
                filter |= GL_COLOR_BUFFER_BIT;
            }
            return GL_CALL(glClear(filter));
        }
        case GR_CULL: {
            if ((value & GR_FRONT) == GR_FRONT && (value & GR_BACK) == GR_BACK) {
                return GL_CALL(glCullFace(GL_FRONT_AND_BACK));
            } else if ((value & GR_BACK) == GR_BACK) {
                return GL_CALL(glCullFace(GL_BACK));
            }
            return GL_CALL(glCullFace(GL_FRONT));
        }
        case GR_DEPTH_MASK: {
            GL_CALL(glDepthMask(m_renderStateMap[value]));
            break;
        }
        case GR_DEPTH_FUNC: {
            return GL_CALL(glDepthFunc(m_renderStateMap[value]));
        }
        case GR_SRC_ALPHA: {
            glBlendFunc(GL_SRC_ALPHA, m_renderStateMap[value]);
            break;
        }
        default:
            std::cout << "Invalid Va: " << getRenderStateName(state) << std::endl;
        }
    }

    std::string gRender::getRenderStateName(RenderState state) {
        #define GET_ENUM_NAME(e) case e: return #e
        switch (state) {
        GET_ENUM_NAME(GR_FALSE);
        GET_ENUM_NAME(GR_TRUE);
        GET_ENUM_NAME(GR_BACKGROUND);
        GET_ENUM_NAME(GR_DEPTH_BUFFER);
        GET_ENUM_NAME(GR_COLOR_BUFFER);
        GET_ENUM_NAME(GR_CULL);
        GET_ENUM_NAME(GR_FRONT);
        GET_ENUM_NAME(GR_BACK);
        default:
            return "undefined";
        }
        #undef GET_ENUM_NAME
    }

    bool gRender::Initialize() {
        #if !GR_OPENGLES3
        if (glewInit() != GLEW_OK) {
            return false;
        }
        #endif
        return true;
    }

    void gRender::Release() {
        m_bufferMap.clear();

        m_renderStateMap.clear();

        gFramebuffer::Release();
    }

    gRender& gRender::GetInstance()
    {
        static gRender instance;
        return instance;
    }

    gRender::gRender()
        :
            s_BackgroundColor(Color::black),
            s_ViewportBounds{0.0f, 0.0f, 0.0f, 0.0f},
            s_StateMask(0)
    {}

    void gRender::setBackgroundColor(const Color& color)
    {
        if (color.r != s_BackgroundColor.r || color.g != s_BackgroundColor.g || 
            color.b != s_BackgroundColor.b || color.a != s_BackgroundColor.a) 
        {
            GL_CALL(glClearColor(color.r, color.g, color.b, color.a));
            s_BackgroundColor = color;
        }
    }

    void gRender::setViewport(const Rect& bounds)
    {
        if (bounds.x != s_ViewportBounds.x || bounds.y != s_ViewportBounds.y || 
            bounds.w != s_ViewportBounds.w || bounds.h != s_ViewportBounds.h)
        {
            GL_CALL(glViewport(bounds.x, bounds.y, bounds.w, bounds.h));
            s_ViewportBounds = bounds;
        }
    }

    void gRender::setEnable(GEnum state, bool value)
    {
        uint32_t bit = 1ULL << state;

        uint8_t enabled = s_StateMask & bit;
        if (enabled != value)
        {
            if (value)
            {
                s_StateMask |= bit;

                glEnable(GL_ENABLE_DISABLE_MAP[state]);
            } else
            {
                s_StateMask &= ~bit;

                glDisable(GL_ENABLE_DISABLE_MAP[state]);
            }
        }
    }
}






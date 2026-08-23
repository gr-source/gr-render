#include "gRender.h"

#include "gl.h"

static constexpr GLenum GL_ENABLE_DISABLE_MAP[] = {
    GL_CULL_FACE,
    GL_DEPTH_TEST,
    GL_MULTISAMPLE,
    GL_FRAMEBUFFER_SRGB,
    GL_BLEND
};

namespace gr
{
    /*
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
    */

    void gRender::SetBackgroundColor(const Color& color)
    {
        GetInstance().setBackgroundColor(color);
    }

    void gRender::SetViewport(const Rect& viewport)
    {
        GetInstance().setViewport(viewport);
    }

    void gRender::SetEnable(GEnum state, bool value)
    {
        GetInstance().setEnable(state, value);
    }


    void gRender::SetDepthFunc(GEnum func)
    {
        GetInstance().setDepthFunc(func);
    }

    void gRender::SetRenderState(RenderState state, uint32_t value)
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
                      /*
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
                           */
        default:
            break;
        }
    }

    bool gRender::Initialize(std::string& error)
    {
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            error = std::string((const char*)glewGetErrorString(err));
        }

        return GLEW_OK != err;
    }

    gRender& gRender::GetInstance()
    {
        static gRender instance;
        return instance;
    }

    gRender::gRender()
        :
            s_BackgroundColor{0.0f, 0.0f, 0.0f, 0.0f},
            s_ViewportBounds{0.0f, 0.0f, 0.0f, 0.0f},
            s_StateMask(0)
    {}

    void gRender::setBackgroundColor(const Color& color)
    {
        if (
                color[0] != s_BackgroundColor[0] ||
                color[1] != s_BackgroundColor[1] || 
                color[2] != s_BackgroundColor[2] ||
                color[3] != s_BackgroundColor[3]) 
        {
            GL_CALL(glClearColor(color[0], color[1], color[2], color[3]));
            s_BackgroundColor[0] = color[0];
            s_BackgroundColor[1] = color[1];
            s_BackgroundColor[2] = color[2];
            s_BackgroundColor[3] = color[3];
        }
    }

    void gRender::setViewport(const Rect& viewport)
    {
        if (
                viewport[0] != s_ViewportBounds[0] ||
                viewport[1] != s_ViewportBounds[1] ||
                viewport[2] != s_ViewportBounds[2] ||
                viewport[3] != s_ViewportBounds[3])
        {
            GL_CALL(glViewport(viewport[0], viewport[1], viewport[2], viewport[3]));
            s_ViewportBounds[0] = viewport[0];
            s_ViewportBounds[1] = viewport[1];
            s_ViewportBounds[2] = viewport[2];
            s_ViewportBounds[3] = viewport[3];
        }
    }

    void gRender::setEnable(GEnum state, bool value)
    {
        uint32_t bit = 1ULL << state;

        bool enabled = (s_StateMask & bit) != 0;
        if (enabled != value)
        {
            if (value)
            {
                s_StateMask |= bit;

                GL_CALL(glEnable(GL_ENABLE_DISABLE_MAP[state]));
            } else
            {
                s_StateMask &= ~bit;

                GL_CALL(glDisable(GL_ENABLE_DISABLE_MAP[state]));
            }
        }
    }


    void gRender::setDepthFunc(GEnum func)
    {
        if (func == s_DepthFunc)
            return;

        GLenum glFunc;

        switch (func)
        {
        case GR_DEPTH_FUNC_ALWAYS:
            glFunc = GL_ALWAYS;
            break;

        case GR_DEPTH_FUNC_NEVER:
            glFunc = GL_NEVER;
            break;

        case GR_DEPTH_FUNC_LESS:
            glFunc = GL_LESS;
            break;

        case GR_DEPTH_FUNC_EQUAL:
            glFunc = GL_EQUAL;
            break;

        case GR_DEPTH_FUNC_LEQUAL:
            glFunc = GL_LEQUAL;
            break;

        case GR_DEPTH_FUNC_GREATER:
            glFunc = GL_GREATER;
            break;

        case GR_DEPTH_FUNC_NOTEQUAL:
            glFunc = GL_NOTEQUAL;
            break;

        case GR_DEPTH_FUNC_GEQUAL:
            glFunc = GL_GEQUAL;
            break;

        default:
            return;
        }

        GL_CALL(glDepthFunc(glFunc));
        s_DepthFunc = func;
    }
}






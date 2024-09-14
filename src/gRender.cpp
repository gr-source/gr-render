#include "gRender.h"

#include "gColor.h"

#include "gFramebuffer.h"
#include "gVertexArray.h"
#include "gTexture.h"
#include "gShader.h"
#include "gError.h"
#include "gl.h"

namespace grr {
    std::unordered_map<BufferBindingTarget, grm::u32> gRender::m_bufferMap {
        {BufferBindingTarget::GR_ARRAY_BUFFER, GL_ARRAY_BUFFER},
        {BufferBindingTarget::GR_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER}
    };

    std::unordered_map<grm::u32, grm::u32> gRender::m_renderStateMap = {
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

    void gRender::SetRenderState(RenderState state, void *value) {
        switch (state) {
        case GR_BACKGROUND_COLOR: {
            gColor* color = (gColor*)value;
            GL_CALL(glClearColor(color->r, color->g, color->b, color->a));
            break;
        }
        case GR_VIEWPORT: {
            iVector2* size = (iVector2*)value;
            GL_CALL(glViewport(0, 0, size->x, size->y));
            break;
        }
        default:
            std::cout << "Invalid V: " << getRenderStateName(state) << std::endl;
            break;
        }
    }

    void gRender::SetRenderState(RenderState state, grm::u32 value) {
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
        case GR_CULL_FACE: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_CULL_FACE));
            }
            return GL_CALL(glDisable(GL_CULL_FACE));
        }
        case GR_CULL: {
            if ((value & GR_FRONT) == GR_FRONT && (value & GR_BACK) == GR_BACK) {
                return GL_CALL(glCullFace(GL_FRONT_AND_BACK));
            } else if ((value & GR_BACK) == GR_BACK) {
                return GL_CALL(glCullFace(GL_BACK));
            }
            return GL_CALL(glCullFace(GL_FRONT));
        }
        case GR_DEPTH: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_DEPTH_TEST));
            }
            return GL_CALL(glDisable(GL_DEPTH_TEST));
        }
        case GR_DEPTH_MASK: {
            GL_CALL(glDepthMask(m_renderStateMap[value]));
            break;
        }
        case GR_DEPTH_FUNC: {
            return GL_CALL(glDepthFunc(m_renderStateMap[value]));
        }
        case GR_MULTISAMPLE: {
            if (value == GR_TRUE) {
                return glEnable(GL_MULTISAMPLE);
            }
            return glDisable(GL_MULTISAMPLE);
        }
        case GR_FRAMEBUFFER_SRGB: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_FRAMEBUFFER_SRGB));
            }
            return GL_CALL(glDisable(GL_FRAMEBUFFER_SRGB));
        }
        case GR_BLEND: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_BLEND));
            }
            return GL_CALL(glDisable(GL_BLEND));
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
        GET_ENUM_NAME(GR_BACKGROUND_COLOR);
        GET_ENUM_NAME(GR_BACKGROUND);
        GET_ENUM_NAME(GR_DEPTH_BUFFER);
        GET_ENUM_NAME(GR_COLOR_BUFFER);
        GET_ENUM_NAME(GR_CULL_FACE);
        GET_ENUM_NAME(GR_CULL);
        GET_ENUM_NAME(GR_FRONT);
        GET_ENUM_NAME(GR_BACK);
        GET_ENUM_NAME(GR_DEPTH);
        GET_ENUM_NAME(GR_VIEWPORT);
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

        gRenderbuffer::Release();

        gTexture::Release();

        gVertexArray::Release();
    }

    // void gRender::GetPixelData() {
    //     glReadPixels()
    // }
}

#include "gRenderbuffer.h"
#include "gl.h"

namespace grr {
    u32 gRenderbuffer::s_current = 0;

    std::unordered_map<RenderbufferType, u32> gRenderbuffer::s_renderbufferApi {
        {RenderbufferType_Depth_Component,    GL_DEPTH_COMPONENT},
        {RenderbufferType_Depth_Component16,  GL_DEPTH_COMPONENT16},
        {RenderbufferType_Depth_Component24,  GL_DEPTH_COMPONENT24},
        {RenderbufferType_Depth_Component32F, GL_DEPTH_COMPONENT32F},
        {RenderbufferType_Depth24_Stencil8,   GL_DEPTH24_STENCIL8},
        {RenderbufferType_Depth_Stencil,      GL_DEPTH_STENCIL},
        {RenderbufferType_Depth32F_Stencil8,  GL_DEPTH32F_STENCIL8},
        {RenderbufferType_Stencil,            GL_STENCIL}
    };

    u32 gRenderbuffer::Create() {
        u32 id;
        GL_CALL(glCreateRenderbuffers(1, &id));

        return id;
    }

    void gRenderbuffer::Bind(u32 id) {  
        GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, id));

        s_current = id;
    }

    void gRenderbuffer::SetBufferStorage(RenderbufferType internalFormat, u32 width, u32 height) {
        GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, s_renderbufferApi[internalFormat], width, height));
    }

    void gRenderbuffer::Unbind() {
        GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

        s_current = 0;
    }

    void gRenderbuffer::Destoy(u32 id) {
        if (!id) {
            return;
        }

        GL_CALL(glDeleteRenderbuffers(1, &id));
    }

} // namespace grr

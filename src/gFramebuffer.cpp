#include "gFramebuffer.h"

#include "gRenderbuffer.h"
#include "gTexture.h"
#include "gl.h"


namespace grr {
    u32 gFramebuffer::s_current = 0;

    std::unordered_map<FramebufferType, u32> gFramebuffer::m_apiFramebuffer{
        {FramebufferType_Color_Attachiment0, GL_COLOR_ATTACHMENT0},
        {FramebufferType_Color_Attachiment1, GL_COLOR_ATTACHMENT1},
        {FramebufferType_Color_Attachiment2, GL_COLOR_ATTACHMENT2},
        {FramebufferType_Color_Attachiment3, GL_COLOR_ATTACHMENT3},
        {FramebufferType_Color_Attachiment4, GL_COLOR_ATTACHMENT4},
        {FramebufferType_Color_Attachiment5, GL_COLOR_ATTACHMENT5},
        {FramebufferType_Depth_Attachiment,  GL_DEPTH_ATTACHMENT}
    };

    u32 gFramebuffer::Create() {
        u32 id;
        GL_CALL(glCreateFramebuffers(1, &id));
        return id;
    }

    void gFramebuffer::Bind(u32 id) {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));

        s_current = id;
    }

    void gFramebuffer::SetRenderbuffer(FramebufferType attachment, u32 id) {
        GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, m_apiFramebuffer[attachment], GL_RENDERBUFFER, id));
    }

    void gFramebuffer::SetTexture(FramebufferType attachment, u32 id) {
        GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, m_apiFramebuffer[attachment], GL_TEXTURE_2D, id, 0));
    }

    void gFramebuffer::Unbind() {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        s_current = 0;
    }

    void gFramebuffer::Destroy(u32 id) {
        if (!id) {
            return;
        }

        GL_CALL(glDeleteFramebuffers(1, &id));
    }

} // namespace grr

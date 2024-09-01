#include "gFramebuffer.h"

#include "gRenderbuffer.h"
#include "gTexture.h"
#include "gl.h"


namespace grr {
    grm::u32 gFramebuffer::s_current = 0;

    std::unordered_map<gFramebufferFlags, grm::u32> gFramebuffer::m_apiFramebuffer{
        {gFramebufferFlags_Color_Attachiment0, GL_COLOR_ATTACHMENT0},
        {gFramebufferFlags_Color_Attachiment1, GL_COLOR_ATTACHMENT1},
        {gFramebufferFlags_Color_Attachiment2, GL_COLOR_ATTACHMENT2},
        {gFramebufferFlags_Color_Attachiment3, GL_COLOR_ATTACHMENT3},
        {gFramebufferFlags_Color_Attachiment4, GL_COLOR_ATTACHMENT4},
        {gFramebufferFlags_Color_Attachiment5, GL_COLOR_ATTACHMENT5},
        {gFramebufferFlags_Depth_Attachiment,  GL_DEPTH_ATTACHMENT},
        {gFramebufferFlags_Cubemap_Positive_X, GL_TEXTURE_CUBE_MAP_POSITIVE_X},
        {gFramebufferFlags_Cubemap_Negative_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
        {gFramebufferFlags_Cubemap_Positive_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
        {gFramebufferFlags_Cubemap_Negative_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
        {gFramebufferFlags_Cubemap_Positive_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
        {gFramebufferFlags_Cubemap_Negative_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z},
        {gFramebufferFlags_Texture,            GL_TEXTURE_2D}
    };

    grm::u32 gFramebuffer::Create() {
        grm::u32 id;
        GL_CALL(glCreateFramebuffers(1, &id));
        return id;
    }

    void gFramebuffer::Bind(grm::u32 id) {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));

        s_current = id;
    }

    void gFramebuffer::SetRenderbuffer(gFramebufferFlags attachment) {
        auto &rbo = gRenderbuffer::GetCurrent();

        assert(rbo && "Invalid render buffer.");

        GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, m_apiFramebuffer[attachment], GL_RENDERBUFFER, rbo));
    }

    void gFramebuffer::SetTexture(gFramebufferFlags attachment, gFramebufferFlags textarget) {
        auto texture = grr::gTexture::GetCurrent();
        assert(texture != nullptr && "Invalid texture");

        GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, m_apiFramebuffer[attachment], m_apiFramebuffer[textarget], texture->getID(), 0));
    }

    void gFramebuffer::Unbind() {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        s_current = 0;
    }

    void gFramebuffer::Destroy(grm::u32 id) {
        if (!id) {
            return;
        }

        GL_CALL(glDeleteFramebuffers(1, &id));
    }

} // namespace grr

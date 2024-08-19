#include "gTexture.h"

#include "gl.h"

namespace grr {
    std::unordered_map<grm::u32, grm::u32> gTexture::m_textureMap{
    };

    gTexture *gTexture::s_current = nullptr;

    gTexture *gTexture::Create(grm::u32 width, grm::u32 height, grm::u32 flags, TextureFormat format, void *pixels) {
        gTexture* texture = new gTexture();
        
        UpdateTexture(texture, width, height, flags, format, pixels);

        return texture;
    }

    void gTexture::UpdateTexture(gTexture *texture, grm::u32 width, grm::u32 height, grm::u32 flags, TextureFormat format, void *pixels) {
        texture->m_height = height;
        texture->m_width = width;
        texture->m_format = format;

        auto target = (flags & gTextureFlags_Cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);

        if (!texture->isValid())
            GL_CALL(glGenTextures(1, &texture->m_index));

        GL_CALL(glBindTexture(target, texture->m_index));

        switch (format) {
        case TextureFormat_SRGB:
            GL_CALL(glTexImage2D(target, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RGB:
            GL_CALL(glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RGB332:
            GL_CALL(glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE_3_3_2, pixels));
            break;
        case TextureFormat_RGB565:
            GL_CALL(glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels));
            break;
        case TextureFormat_RGB444:
            GL_CALL(glTexImage2D(target, 0, GL_RGB4, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_4_4_4_4, pixels));
            break;
        case TextureFormat_RGB888:
            GL_CALL(glTexImage2D(target, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_SRGBA:
            GL_CALL(glTexImage2D(target, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RGBA:
            GL_CALL(glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RGBA4444:
            GL_CALL(glTexImage2D(target, 0, GL_RGBA4, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, pixels));
            break;
        case TextureFormat_RGBA8888:
            GL_CALL(glTexImage2D(target, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
            break;
        default:
            break;
        }

        if (flags & gTextureFlags_MipMaps)
            GL_CALL(glGenerateMipmap(target));

        // uv da textura
        GL_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT));
        
        // filtro de textura
        GL_CALL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        if (flags & gTextureFlags_MipMaps) {
            if (flags & gTextureFlags_Filter_Linear) {
                GL_CALL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            } else if (flags & gTextureFlags_Filter_Bilinear) {
                GL_CALL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
            } else if (flags & gTextureFlags_Filter_Trilinear) {
                GL_CALL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
            }
        }

        GL_CALL(glBindTexture(target, 0));
    }

    void gTexture::Bind(gTexture *texture, int idx) {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + idx));

        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        if (texture)
            GL_CALL(glBindTexture(GL_TEXTURE_2D, texture->m_index));

        s_current = texture;
    }

    void gTexture::Unbind() {
        GL_CALL(glActiveTexture(GL_TEXTURE0));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        s_current = nullptr;
    }

    gTexture *gTexture::GetCurrent() {
        return s_current;
    }

    gTexture::gTexture() : m_width(0), m_height(0), m_index(0) {}

    gTexture::~gTexture() {
        if (m_index) {
            glDeleteTextures(1, &m_index);
        }
    }

    const grm::u32& gTexture::getID() const {
        return m_index;
    }

    const bool gTexture::isValid() const {
        return m_index;
    }
} // namespace gr


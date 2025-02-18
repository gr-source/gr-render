#include "gTexture.h"

#include "gl.h"

namespace grr {
    std::unordered_map<grm::u32, grm::u32> gTexture::m_textureMap{
    };

    gTexture *gTexture::s_current = nullptr;

    void gTexture::Unbind(grm::u32 index) {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + index));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    gTexture *gTexture::GetCurrent() {
        return s_current;
    }

    gTexture::gTexture() : m_width(0), m_height(0), m_index(0), m_active(0) {}


    void gTexture::updateBuffer(grm::u32 width, grm::u32 height, grm::u32 flags, TextureFormat format, void *pixels) {
        m_height = height;
        m_width = width;
        m_format = format;
        m_flags = flags;

        if (!isValid())
            GL_CALL(glGenTextures(1, &m_index));

        GL_CALL(glBindTexture(getTargetTexture(), m_index));

        GLenum internalTarget = getTargetTexture();
        if (flags & gTextureFlags_Cubemap) {
            if (flags & gTextureFlags_Cubemap_Positive_X) {
                internalTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            } else if (flags & gTextureFlags_Cubemap_Negative_X) {
                internalTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
            }  else if (flags & gTextureFlags_Cubemap_Positive_Y) {
                internalTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
            }  else if (flags & gTextureFlags_Cubemap_Negative_Y) {
                internalTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
            }  else if (flags & gTextureFlags_Cubemap_Positive_Z) {
                internalTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
            }  else if (flags & gTextureFlags_Cubemap_Negative_Z) {
                internalTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
            }
        }

        switch (format) {
        case TextureFormat_SRGB:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RGB:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RGB332:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE_3_3_2, pixels));
            break;
        case TextureFormat_RGB565:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pixels));
            break;
        case TextureFormat_RGB444:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGB4, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_4_4_4_4, pixels));
            break;
        case TextureFormat_RGB888:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_SRGBA:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RGBA:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RGBA4444:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGBA4, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, pixels));
            break;
        case TextureFormat_RGBA8888:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_DepthComponent:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels));
            break;
        case TextureFormat_RED_INTEGER:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_INT, pixels));
            break;
        case TextureFormat_RGB16F:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, pixels));
            break;
        case TextureFormat_RGB32F:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, pixels));
            break;
        case TextureFormat_RGBA16F:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, pixels));
            break;
        case TextureFormat_RGBA32F:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RGBA32F, width, height, 0, GL_RGB, GL_FLOAT, pixels));
            break;
        case TextureFormat_RED:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RG:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, pixels));
            break;
        case TextureFormat_RG16F:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, pixels));
            break;
        case TextureFormat_RG32F:
            GL_CALL(glTexImage2D(internalTarget, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, pixels));
            break;
        default:
            break;
        }

        if (flags & gTextureFlags_MipMaps)
            GL_CALL(glGenerateMipmap(getTargetTexture()));

        // texture clamping
        if (flags & gTextureFlags_Clamp_Edge) {
            GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        } else if (flags & gTextureFlags_Clamp_Border) {
            GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
            GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        } else {
            GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_WRAP_S, GL_REPEAT));
            GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_WRAP_T, GL_REPEAT));
        }
        
        // filtro de textura
        GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        if (flags & gTextureFlags_MipMaps) {
            if (flags & gTextureFlags_Filter_Linear) {
                GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            } else if (flags & gTextureFlags_Filter_Bilinear) {
                GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
            } else if (flags & gTextureFlags_Filter_Trilinear) {
                GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
            }
        }
        GL_CALL(glBindTexture(getTargetTexture(), 0));
    }

    gTexture::~gTexture() {
        if (m_index) {
            glDeleteTextures(1, &m_index);
        }
    }

    void gTexture::bind(grm::u32 index) {
        m_active = GL_TEXTURE0 + index;
        
        GL_CALL(glActiveTexture(m_active));

        GL_CALL(glBindTexture(getTargetTexture(), m_index));

        s_current = this;
    }

    void gTexture::unbind() {
        GL_CALL(glActiveTexture(m_active));
        GL_CALL(glBindTexture(getTargetTexture(), 0));
    }

    grm::u32 gTexture::getID() const {
        return m_index;
    }

    bool gTexture::isValid() const {
        return m_index != 0;
    }

    grm::u32 gTexture::getTargetTexture() {
        return m_flags & gTextureFlags_Cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    }

    void gTexture::Release() {
        s_current = nullptr;

        m_textureMap.clear();
    }
} // namespace gr

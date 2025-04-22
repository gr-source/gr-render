#include "gTexture.h"

#include "gCommon.h"
#include "gl.h"

namespace grr
{

    const TextureFormatInfo gTexture::TextureFormatInfoMapping[20] = {
        {GL_RGB,                GL_RGB,             GL_UNSIGNED_BYTE},          // TextureFormat_RGB        - 0
        {GL_SRGB,               GL_RGB,             GL_UNSIGNED_BYTE},          // TextureFormat_SRGB       - 1
        {GL_RGB,                GL_RGB,             GL_UNSIGNED_BYTE_3_3_2},    // TextureFormat_RGB332     - 2
        {GL_RGB4,               GL_RGB,             GL_UNSIGNED_SHORT_4_4_4_4}, // TextureFormat_RGB444     - 3
        {GL_RGB,                GL_RGB,             GL_UNSIGNED_SHORT_5_6_5},   // TextureFormat_RGB565     - 4
        {GL_RGB8,               GL_RGB,             GL_UNSIGNED_BYTE},          // TextureFormat_RGB888     - 5
        {GL_SRGB_ALPHA,         GL_RGBA,            GL_UNSIGNED_BYTE},          // TextureFormat_SRGBA      - 6
        {GL_RGBA,               GL_RGBA,            GL_UNSIGNED_BYTE},          // TextureFormat_RGBA       - 7
        {GL_RGBA4,              GL_RGBA,            GL_UNSIGNED_SHORT_4_4_4_4}, // TextureFormat_RGBA4444
        {GL_RGBA8,              GL_RGBA,            GL_UNSIGNED_BYTE},          // TextureFormat_RGBA8888
        {GL_DEPTH_COMPONENT,    GL_DEPTH_COMPONENT, GL_FLOAT},                  // TextureFormat_DepthComponent
        {GL_R32I,               GL_RED_INTEGER,     GL_INT},                    // TextureFormat_RED_INTEGER
        {GL_RGB16F,             GL_RGB,             GL_FLOAT},                  // TextureFormat_RGB16F
        {GL_RGB32F,             GL_RGB,             GL_FLOAT},                  // TextureFormat_RGB32F
        {GL_RGBA16F,            GL_RGB,             GL_FLOAT},                  // TextureFormat_RGBA16F
        {GL_RGBA32F,            GL_RGB,             GL_FLOAT},                  // TextureFormat_RGBA32F
        {GL_RED,                GL_RED,             GL_UNSIGNED_BYTE},          // TextureFormat_RED
        {GL_RG,                 GL_RG,              GL_UNSIGNED_BYTE},          // TextureFormat_RG
        {GL_RG16F,              GL_RG,              GL_FLOAT},                  // TextureFormat_RG16F
        {GL_RG32F,              GL_RG,              GL_FLOAT}                   // TextureFormat_RG32F
    };

    gTexture *gTexture::s_current = nullptr;

    void gTexture::Unbind(grm::u32 index) {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + index));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    gTexture *gTexture::GetCurrent() {
        return s_current;
    }

    gTexture::gTexture() : m_width(0), m_height(0), textureID(InvalidTextureID), m_active(0), texture_flags(0), m_format(TextureFormat_RGB)
    {
        set_format(TextureFormat_RGB);
        set_texture(gTextureFlags_Texture);
        set_clamping(gTextureFlags_Clamp_Repeat);
        set_filtering(gTextureFlags_Filter_Linear);
    }

    void gTexture::updateBuffer(grm::u32 width, grm::u32 height, void *pixels)
    {
        m_width = width;
        m_height = height;

        if (!isValid())
            GL_CALL(glGenTextures(1, &textureID));

        GL_CALL(glBindTexture(getTargetTexture(), textureID));

        GLenum internalTarget = getTargetTexture();
        if (isCubemap())
        {
            if (texture_flags & gTextureFlags_Cubemap_Positive_X)
                internalTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            else if (texture_flags & gTextureFlags_Cubemap_Negative_X)
                internalTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
            else if (texture_flags & gTextureFlags_Cubemap_Positive_Y)
                internalTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
            else if (texture_flags & gTextureFlags_Cubemap_Negative_Y)
                internalTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
            else if (texture_flags & gTextureFlags_Cubemap_Positive_Z)
                internalTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
            else if (texture_flags & gTextureFlags_Cubemap_Negative_Z)
                internalTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        }

        {
            auto &info = TextureFormatInfoMapping[m_format];

            GL_CALL(glTexImage2D(internalTarget, 0, info.internalformat, width, height, 0, info.format, info.type, pixels));
        }

        // apply mipmaps
        apply_mipmaps();

        // texture clamping
        apply_clamping();

        // filtro de textura
        apply_filtering();

        GL_CALL(glBindTexture(getTargetTexture(), 0));
    }


    gTexture::~gTexture()
    {
        if (textureID == InvalidTextureID)
        {
            return;
        }
        glDeleteTextures(1, &textureID);
    }

    void gTexture::bind(grm::u32 index) {
        m_active = GL_TEXTURE0 + index;
        
        GL_CALL(glActiveTexture(m_active));

        GL_CALL(glBindTexture(getTargetTexture(), textureID));

        s_current = this;
    }

    void gTexture::unbind() {
        GL_CALL(glActiveTexture(m_active));
        GL_CALL(glBindTexture(getTargetTexture(), 0));
    }

    grm::u32 gTexture::getTargetTexture() const
    {
        return isCubemap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    }

    TextureID gTexture::getTextureID() const
    {
        return textureID;
    }

    bool gTexture::isValid() const
    {
        return textureID != InvalidTextureID;
    }

    bool gTexture::isCubemap() const
    {
        return texture_flags & gTextureFlags_Cubemap;
    }

    bool gTexture::isTexture2D() const
    {
        return texture_flags & gTextureFlags_Texture;
    }

    void gTexture::set_filtering(TextureFlags_ flags)
    {
        texture_flags &= ~(gTextureFlags_Filter_Linear | gTextureFlags_Filter_Nearest | gTextureFlags_Filter_Trilinear | gTextureFlags_Filter_Bilinear);

        texture_flags |= flags;
    }

    void gTexture::set_clamping(TextureFlags_ flags)
    {
        texture_flags &= ~(gTextureFlags_Clamp_Repeat | gTextureFlags_Clamp_Border | gTextureFlags_Clamp_Edge);

        texture_flags |= flags;
    }

    void gTexture::set_texture(TextureFlags_ flags)
    {
        if ((flags & gTextureFlags_Texture) && (flags & gTextureFlags_Cubemap))
            flags = gTextureFlags_Texture;

        texture_flags &= ~(gTextureFlags_Texture | gTextureFlags_Cubemap);

        texture_flags |= flags;
    }

    void gTexture::set_format(TextureFormat format)
    {
        m_format = format;
    }

    void gTexture::set_face(gTextureCubemapFace_ flags)
    {
        texture_flags &= ~(gTextureCubemapFace_All);

        texture_flags |= flags;
    }

    void gTexture::generate_mipmaps()
    {
        texture_flags |= gTextureFlags_MipMaps;
    }

    // ********** private ********** //
    void gTexture::apply_clamping() const
    {
        GLint wraps = GL_REPEAT;
        GLint wrapr = GL_REPEAT;
        GLint wrapt = GL_REPEAT;
        
        if (texture_flags & gTextureFlags_Clamp_Edge)
        {
            wraps = GL_CLAMP_TO_EDGE;
            wrapr = GL_CLAMP_TO_EDGE;
            wrapt = GL_CLAMP_TO_EDGE;
        } else if (texture_flags & gTextureFlags_Clamp_Border)
        {
            wraps = GL_CLAMP_TO_BORDER;
            wrapr = GL_CLAMP_TO_BORDER;
            wrapt = GL_CLAMP_TO_BORDER;
        }

        GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_WRAP_S, wraps));
        GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_WRAP_R, wrapr));
        GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_WRAP_T, wrapt));
    }

    void gTexture::apply_filtering() const
    {
        GLint minFilter = GL_LINEAR;
        GLint magFilter = GL_LINEAR;

        if (texture_flags & gTextureFlags_MipMaps)
        {
            if (texture_flags & gTextureFlags_Filter_Trilinear)
            {
                minFilter = GL_LINEAR_MIPMAP_LINEAR;
            }
            else if (texture_flags & gTextureFlags_Filter_Bilinear)
            {
                minFilter = GL_LINEAR_MIPMAP_NEAREST;
            }
            else
            {
                minFilter = GL_LINEAR;
            }
        }
        else {
            if (texture_flags & gTextureFlags_Filter_Linear)
            {
                minFilter = GL_LINEAR;
            }
            else if (texture_flags & gTextureFlags_Filter_Nearest)
            {
                minFilter = GL_NEAREST;
            }
        }

        if (texture_flags & gTextureFlags_Filter_Linear)
        {
            magFilter = GL_LINEAR;
        } 
        else if (texture_flags & gTextureFlags_Filter_Nearest)
        {
            magFilter = GL_NEAREST;
        }

        GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_MIN_FILTER, minFilter));
        GL_CALL(glTexParameteri(getTargetTexture(), GL_TEXTURE_MAG_FILTER, magFilter));
    }

    void gTexture::apply_mipmaps() const
    {

        if (texture_flags & gTextureFlags_MipMaps)
            GL_CALL(glGenerateMipmap(getTargetTexture()));
    }

} // namespace gr

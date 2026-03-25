#include "gTexture.h"

#include "gCommon.h"
#include "gl.h"

namespace gr
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

    // TODO: Isso 'e' muito engraçado kkkk
    void GetGLFormat(uint32_t myFormat, GLint* internalFormat, GLenum* format, GLenum* type) {
        *type = GL_UNSIGNED_BYTE;

        switch(myFormat) {
            case TextureFormat_RGB:
                *internalFormat = GL_RGB;
                *format = GL_RGB;
                break;
            case TextureFormat_RGBA:
                *internalFormat = GL_RGBA;
                *format = GL_RGBA;
                break;
            case TextureFormat_RGBA4444:
                *internalFormat = GL_RGBA4; 
                *format = GL_RGBA; 
                *type = GL_UNSIGNED_SHORT_4_4_4_4; 
                break;
            case TextureFormat_RGBA8888:
                *internalFormat = GL_RGBA8; 
                *format = GL_RGBA; 
                break;
            case TextureFormat_SRGB:
                *internalFormat = GL_SRGB;
                *format = GL_RGB;
                break;
            case TextureFormat_SRGBA:
                *internalFormat = GL_SRGB8_ALPHA8;
                *format = GL_RGBA;
                break;
            case TextureFormat_RGB444:
                *internalFormat = GL_RGB4; 
                *format = GL_RGB; 
                *type = GL_UNSIGNED_SHORT_4_4_4_4;
                break;
            case TextureFormat_RGB565:
                *internalFormat = GL_RGB565; 
                *format = GL_RGB; 
                *type = GL_UNSIGNED_SHORT_5_6_5; 
                break;
            case TextureFormat_RGB888:
                *internalFormat = GL_RGB8;
                *format = GL_RGB; 
                break;
            case TextureFormat_RGB16F:
                *internalFormat = GL_RGB16F;
                *format = GL_RGB;
                *type = GL_HALF_FLOAT;
                break;
            case TextureFormat_RGB32F:  
                *internalFormat = GL_RGB32F;
                *format = GL_RGB;
                *type = GL_FLOAT;
                break;
            case TextureFormat_RGBA16F:  
                *internalFormat = GL_RGBA16F;
                *format = GL_RGBA;
                *type = GL_HALF_FLOAT;
                break;
            case TextureFormat_RGBA32F:  
                *internalFormat = GL_RGBA32F;
                *format = GL_RGBA;
                *type = GL_FLOAT;
                break;
            case TextureFormat_RED:
                *internalFormat = GL_R8; 
                *format = GL_RED; 
                break;
            case TextureFormat_RG:
                *internalFormat = GL_RG8; 
                *format = GL_RG; 
                break;
            case TextureFormat_RG16F:
                *internalFormat = GL_RG16F; 
                *format = GL_RG; 
                *type = GL_HALF_FLOAT; 
                break;
            case TextureFormat_RG32F:
                *internalFormat = GL_RG32F; 
                *format = GL_RG; 
                *type = GL_FLOAT; 
                break;
            case TextureFormat_DepthComponent: 
                *internalFormat = GL_DEPTH_COMPONENT;
                *format = GL_DEPTH_COMPONENT;
                *type = GL_FLOAT;
                break;
            case TextureFormat_RED_INTEGER:
                *internalFormat = GL_R8; 
                *format = GL_RED_INTEGER; 
                *type = GL_UNSIGNED_INT; 
                break;
            default:
                *internalFormat = GL_RGBA;
                *format = GL_RGBA;
                break;
        }
    }

    void BindTexture(Texture *texture)
    {
        GLenum target = (texture->type == TEXTURE_TYPE_CUBE) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

        glBindTexture(target, texture->id);
    }

    void UnbindTexture(Texture *texture)
    {
        GLenum target = (texture->type == TEXTURE_TYPE_CUBE) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

        glBindTexture(target, 0);
    }

    void ApplyTextureBuffer(Texture *texture, void* pixels)
    {
        GLint internalFmt;
        GLenum fmt, type;
        GetGLFormat(texture->format, &internalFmt, &fmt, &type);

        GLenum target = (texture->type == TEXTURE_TYPE_CUBE) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
        if (texture->type == TEXTURE_TYPE_CUBE)
        {
            void **faces = (void **)pixels;
            for (int i = 0; i < 6; i++)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFmt, 
                             texture->width, texture->height, 0, fmt, type, faces[i]);
            }
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFmt, texture->width, texture->height, 0, fmt, type, pixels);
        }

        if (texture->filter >= TEXTURE_FILTER_NEAREST_MIPMAP)
        {
            glGenerateMipmap(target);
        }
    }

    void ApplyTextureSettings(Texture *texture)
    {
        GLenum target = (texture->type == TEXTURE_TYPE_CUBE) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

        GLint glFilter;
        switch(texture->filter)
        {
            case TEXTURE_FILTER_NEAREST:        glFilter = GL_NEAREST; break;
            case TEXTURE_FILTER_LINEAR:         glFilter = GL_LINEAR; break;
            case TEXTURE_FILTER_NEAREST_MIPMAP: glFilter = GL_NEAREST_MIPMAP_NEAREST; break;
            case TEXTURE_FILTER_LINEAR_MIPMAP:  glFilter = GL_LINEAR_MIPMAP_LINEAR; break;
            default: glFilter = GL_LINEAR;
        }
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, glFilter);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, (glFilter == GL_NEAREST) ? GL_NEAREST : GL_LINEAR);

        GLint glWrap;
        switch(texture->wrap) {
            case TEXTURE_WRAP_REPEAT: glWrap = GL_REPEAT; break;
            case TEXTURE_WRAP_CLAMP:  glWrap = GL_CLAMP_TO_EDGE; break;
            case TEXTURE_WRAP_MIRROR: glWrap = GL_MIRRORED_REPEAT; break;
            default: glWrap = GL_REPEAT;
        }
        
        glTexParameteri(target, GL_TEXTURE_WRAP_S, glWrap);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, glWrap);
        
        if(texture->type == TEXTURE_TYPE_CUBE) {
            glTexParameteri(target, GL_TEXTURE_WRAP_R, glWrap);
        }
    }

    void gTexture::Unbind(u32 index) {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + index));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    gTexture::gTexture() : m_width(0), m_height(0), textureID(GR_INVALID_ID), m_active(0), texture_flags(0), m_format(TextureFormat_RGB)
    {
        set_format(TextureFormat_RGB);
        set_texture(gTextureFlags_Texture);
        set_clamping(gTextureFlags_Clamp_Repeat);
        set_filtering(gTextureFlags_Filter_Linear);
    }

    void gTexture::updateBuffer(u32 width, u32 height, void *pixels)
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
        if (textureID == GR_INVALID_ID)
            return;
        glDeleteTextures(1, &textureID);
    }

    void gTexture::bind(u32 index) {
        m_active = GL_TEXTURE0 + index;
        
        GL_CALL(glActiveTexture(m_active));

        GL_CALL(glBindTexture(getTargetTexture(), textureID));
    }

    void gTexture::unbind() {
        GL_CALL(glActiveTexture(m_active));
        GL_CALL(glBindTexture(getTargetTexture(), 0));
    }

    u32 gTexture::getTargetTexture() const
    {
        return isCubemap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    }

    TextureID gTexture::getTextureID() const
    {
        return textureID;
    }

    bool gTexture::isValid() const
    {
        return textureID != GR_INVALID_ID;
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

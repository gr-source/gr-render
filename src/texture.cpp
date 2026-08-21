#include "texture.hpp"

#include "gl.h"

namespace gr
{

    // TODO: Isso 'e' muito engraçado kkkk
    void GetGLFormat(uint32_t myFormat, GLint* internalFormat, GLenum* format, GLenum* type) {
        *type = GL_UNSIGNED_BYTE;

        switch(myFormat)
        {
    
            case TEXTURE_FORMAT_RGB:
                *internalFormat = GL_RGB;
                *format = GL_RGB;
                break;
            case TEXTURE_FORMAT_RGBA:
                *internalFormat = GL_RGBA;
                *format = GL_RGBA;
                break;/*
            case TextureFormat_RGBA4444:
                *internalFormat = GL_RGBA4; 
                *format = GL_RGBA; 
                *type = GL_UNSIGNED_SHORT_4_4_4_4; 
                break; */
            case TEXTURE_FORMAT_RGBA8:
                *internalFormat = GL_RGBA8; 
                *format = GL_RGBA; 
                break;
            case TEXTURE_FORMAT_SRGB:
                *internalFormat = GL_SRGB;
                *format = GL_RGB;
                break;
            case TEXTURE_FORMAT_SRGBA:
                *internalFormat = GL_SRGB8_ALPHA8;
                *format = GL_RGBA;
                break;/*
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
                */
            case TEXTURE_FORMAT_RGB8:
                *internalFormat = GL_RGB8;
                *format = GL_RGB; 
                break;
            case TEXTURE_FORMAT_RGB16F:
                *internalFormat = GL_RGB16F;
                *format = GL_RGB;
                *type = GL_HALF_FLOAT;
                break;/*
            case TextureFormat_RGB32F:  
                *internalFormat = GL_RGB32F;
                *format = GL_RGB;
                *type = GL_FLOAT;
                break;*/
            case TEXTURE_FORMAT_RGBA16F:  
                *internalFormat = GL_RGBA16F;
                *format = GL_RGBA;
                *type = GL_HALF_FLOAT;
                break;/*
            case TextureFormat_RGBA32F:  
                *internalFormat = GL_RGBA32F;
                *format = GL_RGBA;
                *type = GL_FLOAT;
                break;*/
            case TEXTURE_FORMAT_RED:
                *internalFormat = GL_R8; 
                *format = GL_RED; 
                break;
            case TEXTURE_FORMAT_RG:
                *internalFormat = GL_RG8; 
                *format = GL_RG; 
                break;
            case TEXTURE_FORMAT_RG16F:
                *internalFormat = GL_RG16F; 
                *format = GL_RG; 
                *type = GL_HALF_FLOAT; 
                break;/*
            case TextureFormat_RG32F:
                *internalFormat = GL_RG32F; 
                *format = GL_RG; 
                *type = GL_FLOAT; 
                break;
                */
            case TEXTURE_FORMAT_DEPTH: 
                *internalFormat = GL_DEPTH_COMPONENT;
                *format = GL_DEPTH_COMPONENT;
                *type = GL_FLOAT;
                break;
                /*
            case TextureFormat_RED_INTEGER:
                *internalFormat = GL_R8; 
                *format = GL_RED_INTEGER; 
                *type = GL_UNSIGNED_INT; 
                break;
                */
            default:
                *internalFormat = GL_RGBA;
                *format = GL_RGBA;
                break;
        }
    }

    void texture::create()
    {
        if (id == GR_INVALID_ID)
            GL_CALL(glGenTextures(1, &id));
    }

    void texture::destroy()
    {
        if (id != GR_INVALID_ID)
            GL_CALL(glDeleteTextures(1, &id));

        id = GR_INVALID_ID;
    }

    void texture::bind() const
    {
        GLenum target = (type == TEXTURE_TYPE_CUBE) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

        glBindTexture(target, id);
    }

    void texture::unbind() const
    {
        GLenum target = (type == TEXTURE_TYPE_CUBE) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

        glBindTexture(target, 0);
    }

    void texture::applyTextureBuffer(const void* pixels)
    {
        GLint internalFmt;
        GLenum fmt, type;
        GetGLFormat(format, &internalFmt, &fmt, &type);

        GLenum target = (type == TEXTURE_TYPE_CUBE) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
        if (type == TEXTURE_TYPE_CUBE)
        {
            void **faces = (void **)pixels;
            for (int i = 0; i < 6; i++)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFmt, 
                             width, height, 0, fmt, type, faces[i]);
            }
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFmt, width, height, 0, fmt, type, pixels);
        }

        if (filter >= TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST)
        {
            glGenerateMipmap(target);
        }
    }

    void texture::applyTextureSettings()
    {
        GLenum target = (type == TEXTURE_TYPE_CUBE) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

        switch (filter)
        {
            case TEXTURE_FILTER_NEAREST:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;

            case TEXTURE_FILTER_LINEAR:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;

            case TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;

            case TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;

            case TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;

            case TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
        }

        GLint glWrap;
        switch(wrap) {
            case TEXTURE_WRAP_REPEAT: glWrap = GL_REPEAT; break;
            case TEXTURE_WRAP_CLAMP:  glWrap = GL_CLAMP_TO_EDGE; break;
            case TEXTURE_WRAP_MIRROR: glWrap = GL_MIRRORED_REPEAT; break;
            default: glWrap = GL_REPEAT;
        }
        
        glTexParameteri(target, GL_TEXTURE_WRAP_S, glWrap);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, glWrap);
        
        if(type == TEXTURE_TYPE_CUBE) {
            glTexParameteri(target, GL_TEXTURE_WRAP_R, glWrap);
        }
    }
} // namespace gr

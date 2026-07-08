#include "shader.hpp"

#include "gCommon.h"
#include "gl.h"

#include <cstddef>
#include <string.h>

namespace gr
{
    Shader::Shader() : shaderID(GR_INVALID_ID), m_uniforms(nullptr), m_count(0), m_capacity(0), m_buffer_size(0)
    {}

    Shader::~Shader()
    {
        if (m_uniforms != nullptr)
            free(m_uniforms);
    }

    bool Shader::create(const char **fragment, size_t numFragments, const char **vertex, size_t numVertex, std::string* error)
    {
        // Fragment shader
        uint32_t shader_fragment = GL_CALL(glCreateShader(GL_FRAGMENT_SHADER));
        GL_CALL(glShaderSource(shader_fragment, numFragments, fragment, nullptr));
        GL_CALL(glCompileShader(shader_fragment));

        // Check erros
        GLint success;
        GL_CALL(glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            GLint logSize = 0;
            GL_CALL(glGetShaderiv(shader_fragment, GL_INFO_LOG_LENGTH, &logSize));

            error->resize(logSize, '\0');
            GL_CALL(glGetShaderInfoLog(shader_fragment, error->size(), nullptr, error->data()));

            GL_CALL(glDeleteShader(shader_fragment));
            return false;
        }

        // Vertex shader
        auto shader_vertex = GL_CALL(glCreateShader(GL_VERTEX_SHADER));
        GL_CALL(glShaderSource(shader_vertex, numVertex, vertex, nullptr));
        GL_CALL(glCompileShader(shader_vertex));

        // Check erros
        GL_CALL(glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            GLint logSize = 0;
            GL_CALL(glGetShaderiv(shader_vertex, GL_INFO_LOG_LENGTH, &logSize));

            error->resize(logSize, '\0');
            GL_CALL(glGetShaderInfoLog(shader_vertex, error->size(), nullptr, error->data()));
            // error("vertex: %s", infoLog);
            GL_CALL(glDeleteShader(shader_vertex));
            return false;
        }

        // delete program
        if (shaderID != GR_INVALID_ID)
        {
            GL_CALL(glUseProgram(0));
            GL_CALL(glDeleteProgram(shaderID));
        }

        shaderID = GL_CALL(glCreateProgram());

        // attach
        GL_CALL(glAttachShader(shaderID, shader_fragment));
        GL_CALL(glAttachShader(shaderID, shader_vertex));

        // link
        GL_CALL(glLinkProgram(shaderID));

        GL_CALL(glGetProgramiv(shaderID, GL_LINK_STATUS, &success));
        if (!success)
        {
            GLint logSize = 0;
            GL_CALL(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize));

            error->resize(logSize, '\0');
            GL_CALL(glGetProgramInfoLog(shaderID, error->size(), nullptr, error->data()));
            // error("link: %s", infoLog);

            GL_CALL(glDeleteShader(shader_fragment));
            GL_CALL(glDeleteShader(shader_vertex));
            GL_CALL(glDeleteProgram(shaderID));
            return false;
        }

        GL_CALL(glDeleteShader(shader_fragment));
        GL_CALL(glDeleteShader(shader_vertex));

        return true;
    }

    void Shader::destroy()
    {
        if (m_uniforms != nullptr)
            free(m_uniforms);

        if (shaderID != GR_INVALID_ID)
            GL_CALL(glDeleteShader(shaderID));

        m_capacity = 0;
        m_count = 0;

        m_uniforms = nullptr;

        shaderID = GR_INVALID_ID;
    }

    UniformID Shader::registry(const char *name, uint32_t count, UniformType type)
    {
        UniformID id = findUniform(name);
        if (id != GR_INVALID_ID)
            return id;
     
        if (m_count >= m_capacity)
            grow();

        int location = glGetUniformLocation(shaderID, name);
        if (location == -1)
            return GR_INVALID_ID;

        size_t stride = 0;
        switch (type)
        {
            case UniformType::SAMPLERCUBE:
            case UniformType::SAMPLER2D:
            case UniformType::BOOL:
            case UniformType::INT:
                stride = sizeof(GLint);
                break;
            case UniformType::FLOAT:
                stride = sizeof(GLfloat);
                break;
            case UniformType::VEC2:
                stride = sizeof(Vector2);
                break;
            case UniformType::VEC3:
                stride = sizeof(Vector3);
                break;
            case UniformType::VEC4:
                stride = sizeof(Vector4);
                break;
            case UniformType::MAT3:
                stride = sizeof(Matrix3x3);
                break;
            case UniformType::MAT4:
                stride = sizeof(Matrix4x4);
                break;
            default:
                break;
        }

        UniformID uniformID = m_count++;

        auto &uniform = m_uniforms[uniformID];
        strncpy(uniform.name, name, sizeof(uniform.name));

        uniform.location = location;
        uniform.type = type;
        uniform.count = count;
        uniform.size = stride;
        uniform.offset = m_buffer_size;

        m_buffer_size += stride;

        return uniformID;
    }

    void Shader::setUniform(const char *name, const void *data)
    {
        UniformID id = findUniform(name);
        if (id == GR_INVALID_ID)
            return;

        return SetUniform(id, data);
    }

    void Shader::SetUniform(UniformID id, const void *data)
    {
        assert(id < m_count);

        auto &uniform = m_uniforms[id];

        switch (uniform.type)
        {
            case UniformType::SAMPLERCUBE:
            case UniformType::SAMPLER2D:
            case UniformType::BOOL:
            case UniformType::INT:
                GL_CALL(glUniform1iv(uniform.location, uniform.count, (const GLint *)data));
                return;
            case UniformType::FLOAT:
                GL_CALL(glUniform1fv(uniform.location, uniform.count, (const GLfloat *)data));
                return;
            case UniformType::VEC2:
                GL_CALL(glUniform2fv(uniform.location, uniform.count, (const GLfloat *)data));
                return;
            case UniformType::VEC3:
                GL_CALL(glUniform3fv(uniform.location, uniform.count, (const GLfloat *)data));
                return;
            case UniformType::VEC4:
                GL_CALL(glUniform4fv(uniform.location, uniform.count, (const GLfloat *)data));
                return;
            case UniformType::MAT3:
                GL_CALL(glUniformMatrix3fv(uniform.location, uniform.count, GL_FALSE, (const GLfloat *)data));
                return;
            case UniformType::MAT4:
                GL_CALL(glUniformMatrix4fv(uniform.location, uniform.count, GL_FALSE, (const GLfloat *)data));
                return;
            default:
                assert(false && "Unknown uniform type.");
                return;
        }
    }

    void Shader::bind()
    {
        GL_CALL(glUseProgram(shaderID));
    }

    void Shader::unbind()
    {
        glUseProgram(0);
    }

    UniformID Shader::findUniform(const char *name)
    {
        for (size_t i=0;i<m_count;i++)
        {
            if (strcmp(m_uniforms[i].name, name) == 0)
                return i;
        }
        return GR_INVALID_ID;
    }

    void Shader::grow()
    {
        if (m_capacity)
        {
            m_capacity *= 2;

            m_uniforms = (ShaderUniform*)realloc(m_uniforms, m_capacity * sizeof(ShaderUniform));
        } else
        {
            m_capacity = 1;

            m_uniforms = (ShaderUniform*)malloc(m_capacity * sizeof(ShaderUniform));
        }
    }
}



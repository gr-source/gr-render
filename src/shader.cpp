#include "shader.hpp"

#include "gl.h"

#include <cassert>

#include <cstring>

namespace gr
{
    bool shader::create(const char **fragment, size_t numFragments, const char **vertex, size_t numVertex, std::string* error)
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

    void shader::destroy()
    {
        if (uniforms != nullptr)
            free(uniforms);

        if (shaderID != GR_INVALID_ID)
            GL_CALL(glDeleteShader(shaderID));

        shaderID = GR_INVALID_ID;

        capacity = 0;
        count = 0;

        if (uniforms != nullptr)
            free(uniforms);

        uniforms = nullptr;
    }

    UniformID shader::registry(const char *name, uint32_t numElements, UniformType type)
    {
        UniformID found = findUniform(name);
        if (found != GR_INVALID_ID)
            return found;
     
        if (count >= capacity)
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
                stride = sizeof(GLfloat) * 2;
                break;
            case UniformType::VEC3:
                stride = sizeof(GLfloat) * 3;
                break;
            case UniformType::VEC4:
                stride = sizeof(GLfloat) * 4;
                break;
            case UniformType::MAT3:
                stride = sizeof(GLfloat) * 3 * 3;
                break;
            case UniformType::MAT4:
                stride = sizeof(GLfloat) * 4 * 4;
                break;
            default:
                break;
        }

        UniformID uniformID = static_cast<UniformID>(count++);

        auto &uniform = uniforms[uniformID];
        strncpy(uniform.name, name, sizeof(uniform.name));

        uniform.location = location;
        uniform.type = type;
        uniform.count = numElements;
        uniform.size = stride;
        uniform.offset = bufferSize;

        bufferSize += stride;

        return uniformID;
    }

    void shader::setUniform(const char *name, const void *data)
    {
        UniformID id = findUniform(name);
        if (id == GR_INVALID_ID)
            return;

        return SetUniform(id, data);
    }

    void shader::SetUniform(UniformID id, const void *data)
    {
        assert(id < count);
        if (id >= count)
            return;

        auto &uniform = uniforms[id];

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

    void shader::bind() const
    {
        GL_CALL(glUseProgram(shaderID));
    }

    void shader::unbind() const
    {
        glUseProgram(0);
    }

    UniformID shader::findUniform(const char *name)
    {
        for (size_t i=0;i<count;i++)
        {
            if (strcmp(uniforms[i].name, name) == 0)
                return i;
        }
        return GR_INVALID_ID;
    }

    void shader::grow()
    {
        capacity = capacity > 0 ? capacity * 2 : 2;

        uniforms = (uniform*)realloc(uniforms, capacity * sizeof(uniform));
    }
}



#include "gShader.h"

#include "gl.h"
#include "gColor.h"

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

extern std::string m_error;

namespace grr {
    gShader* gShader::m_instance = nullptr;

    gShader::gShader() : m_id(0) {}

    gShader *gShader::Create(const char **fragments, const char **vertex) {
        gShader* shader = new gShader();
        shader->m_id = GL_CALL(glCreateProgram());
        
        // Create Fragment shader object
        u32 fragmentShaderID = GL_CALL(glCreateShader(GL_FRAGMENT_SHADER));
        GL_CALL(glShaderSource(fragmentShaderID, NELEMS(fragments), fragments, nullptr));
        GL_CALL(glCompileShader(fragmentShaderID));
        if (gShader::checkerrors(fragmentShaderID, true)) {
            GL_CALL(glDeleteShader(fragmentShaderID));

            shader->destroy();

            return nullptr;
        }

        // Create vertex shader object
        u32 vertexShaderID = GL_CALL(glCreateShader(GL_VERTEX_SHADER));
        GL_CALL(glShaderSource(vertexShaderID, NELEMS(vertex), vertex, nullptr));
        GL_CALL(glCompileShader(vertexShaderID));
        if (gShader::checkerrors(vertexShaderID, true)) {
            GL_CALL(glDeleteShader(fragmentShaderID));
            GL_CALL(glDeleteShader(vertexShaderID));

            shader->destroy();

            return nullptr;
        }

        // attach shader object in to program
        GL_CALL(glAttachShader(shader->m_id, fragmentShaderID));
        GL_CALL(glAttachShader(shader->m_id, vertexShaderID));

        // link shaders object in program
        GL_CALL(glLinkProgram(shader->m_id));
        if (gShader::checkerrors(shader->m_id, false)) {
            GL_CALL(glDeleteShader(fragmentShaderID));
            GL_CALL(glDeleteShader(vertexShaderID));

            shader->destroy();

            return nullptr;
        }

        // clean shader object
        GL_CALL(glDeleteShader(fragmentShaderID));
        GL_CALL(glDeleteShader(vertexShaderID));

        return shader;
    }

    gShader *gShader::GetCurrent() {
        return gShader::m_instance;
    }

    void gShader::Register(const std::string &name) {
        GLint location = GL_CALL(glGetUniformLocation(m_instance->m_id, name.c_str()));
        if (location != -1) {
            m_instance->m_uniformMap[name] = location;
        } else {
            std::cout << "Uniform do not search: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, u16 count, const Matrix4x4& data) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniformMatrix4fv(it->second, static_cast<GLsizei>(count), GL_FALSE, data.data));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, u16 count, const Vector3& data) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniform3fv(it->second, static_cast<GLsizei>(count), data.data));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, u8 count, const int *value) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniform1iv(it->second, static_cast<GLsizei>(count), static_cast<const GLint*>(value)));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, u16 count, const gColor& data) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniform4fv(it->second, static_cast<GLsizei>(count), data.data));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, int data) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniform1i(it->second, static_cast<const GLint>(data)));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, Vector3 data) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniform3fv(it->second, 1, reinterpret_cast<const GLfloat*>(data.data)));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, Vector4 data) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniform4fv(it->second, 1, reinterpret_cast<const GLfloat*>(data.data)));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, gColor data) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniform4fv(it->second, 1, reinterpret_cast<const GLfloat*>(data.data)));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    const bool gShader::isValid() const {
        return m_id != 0;
    }

    void gShader::bind() {
        GL_CALL(glUseProgram(m_id));
        gShader::m_instance = this;
    }

    void gShader::unbind() {
        GL_CALL(glUseProgram(0));
        gShader::m_instance = nullptr;
    }

    void gShader::destroy() {
        if (m_id) {
            GL_CALL(glDeleteProgram(m_id));
        }
        delete this;
    }

    const bool gShader::checkerrors(u32 shader, bool compile) {
        GLint status;
        if (compile) {
            GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
            if (status == GL_TRUE) {
                return false;
            }
            GLint length;
            GL_CALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));

            GLint type;
            GL_CALL(glGetShaderiv(shader, GL_SHADER_TYPE, &type));

            std::string logMessage(length, '\0');
            GL_CALL(glGetShaderInfoLog(shader, logMessage.size(), nullptr, logMessage.data()));

            m_error += "================================\n";
            m_error += "Failed to compile shader - ";
            switch (type) {
            case GL_FRAGMENT_SHADER:
                m_error += ".frag\n\n";
                break;
            case GL_VERTEX_SHADER:
                m_error += ".vert\n\n";
                break;
            default:
                break;
            }
            m_error += logMessage + "\n";
            m_error += "================================\n";
            return true;
        }

        GL_CALL(glGetProgramiv(shader, GL_LINK_STATUS, &status));
        if (status == GL_TRUE) {
            return false;
        }
        std::string logMessage(512, '\0');

        GL_CALL(glGetProgramInfoLog(shader, logMessage.size(), nullptr, logMessage.data()));
        m_error += "Failed to link shader program - \n\n";
        m_error += logMessage + '\n';
        return true;
    }
} // namespace grr

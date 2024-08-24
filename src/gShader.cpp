#include "gShader.h"

#include "gl.h"
#include "gColor.h"

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

extern std::string m_error;

namespace grr {
    gShader* gShader::m_instance = nullptr;

    gShader::gShader() : m_id(0), m_bValid(false) {}

    gShader::~gShader() {
        if (m_id) {
            GL_CALL(glDeleteProgram(m_id));
        }
    }

    gShader *gShader::Create(const char **fragments, const char **vertex) {
        gShader* shader = new gShader();

        gShader::Bind(shader);
        gShader::SetupShader(fragments, vertex);
        gShader::Unbind();

        return shader;
    }

    void gShader::SetupShader(const char **fragments, const char **vertex) {
        auto shader = GetCurrent();
        assert(shader || fragments || vertex);

        if (!shader->isValid())
            shader->m_id = GL_CALL(glCreateProgram());
        
        // Create Fragment shader object
        grm::u32 fragmentShaderID = GL_CALL(glCreateShader(GL_FRAGMENT_SHADER));
        GL_CALL(glShaderSource(fragmentShaderID, NELEMS(fragments), fragments, nullptr));
        GL_CALL(glCompileShader(fragmentShaderID));
        if (gShader::checkerrors(fragmentShaderID, true)) {
            GL_CALL(glDeleteShader(fragmentShaderID));
            return;
        }

        // Create vertex shader object
        grm::u32 vertexShaderID = GL_CALL(glCreateShader(GL_VERTEX_SHADER));
        GL_CALL(glShaderSource(vertexShaderID, NELEMS(vertex), vertex, nullptr));
        GL_CALL(glCompileShader(vertexShaderID));
        if (gShader::checkerrors(vertexShaderID, true)) {
            GL_CALL(glDeleteShader(fragmentShaderID));
            GL_CALL(glDeleteShader(vertexShaderID));
            return;
        }

        // attach shader object in to program
        GL_CALL(glAttachShader(shader->m_id, fragmentShaderID));
        GL_CALL(glAttachShader(shader->m_id, vertexShaderID));

        // link shaders object in program
        GL_CALL(glLinkProgram(shader->m_id));
        if (gShader::checkerrors(shader->m_id, false)) {
            GL_CALL(glDeleteShader(fragmentShaderID));
            GL_CALL(glDeleteShader(vertexShaderID));

            return;
        }

        // clean shader object
        GL_CALL(glDeleteShader(fragmentShaderID));
        GL_CALL(glDeleteShader(vertexShaderID));

        shader->m_bValid = true;
    }
    
    void gShader::Bind(gShader *shader) {
        GL_CALL(glUseProgram(shader->m_id));
        gShader::m_instance = shader;
    }

    void gShader::Unbind() {
        GL_CALL(glUseProgram(0));
        gShader::m_instance = nullptr;
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
    void gShader::SetUniform(const std::string& name, grm::u16 count, const Matrix4x4& data) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniformMatrix4fv(it->second, static_cast<GLsizei>(count), GL_FALSE, data.data));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, grm::u16 count, const Vector3& data) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniform3fv(it->second, static_cast<GLsizei>(count), data.data));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, grm::u16 count, const int *value) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniform1iv(it->second, static_cast<GLsizei>(count), static_cast<const GLint*>(value)));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, grm::u16 count, const Matrix4x4 *value) {
        auto it = m_instance->m_uniformMap.find(name);
        if (m_instance->m_uniformMap.end() != it) {
            GL_CALL(glUniformMatrix4fv(it->second, static_cast<GLsizei>(count), GL_FALSE, reinterpret_cast<const GLfloat *>(value)));
        } else {
            std::cout << "Invalid Uniform: " << name << std::endl;
        }
    }

    template <>
    void gShader::SetUniform(const std::string& name, grm::u16 count, const gColor& data) {
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

    bool gShader::isValid() const {
        return m_bValid && m_id;
    }

    const bool gShader::checkerrors(grm::u32 shader, bool compile) {
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

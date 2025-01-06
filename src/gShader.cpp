#include "gShader.h"

#include "gl.h"

#include <cstring>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

extern std::ostringstream m_error;

namespace grr {
    std::unordered_map<std::string, std::unique_ptr<Uniform>> gShader::m_uniformMap;

    gShader *gShader::m_current = nullptr;

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
        gShader::m_current = shader;
    }

    void gShader::Unbind() {
        GL_CALL(glUseProgram(0));
        gShader::m_current = nullptr;
    }

    gShader *gShader::GetCurrent() {
        return gShader::m_current;
    }

    bool gShader::SetUniform(const std::string& name, const void *data) {
        if (!data) {
            m_error << "Invalid source." << std::endl;
            return false;
        }

        if (m_uniformMap.find(name) == m_uniformMap.end()) {
            m_error << "Uniform do not found." << std::endl;
            return false;
        }
        auto &uniform = m_uniformMap[name];
        if (!uniform->getData()) {
            return false;
        }
        uniform->setData(data);
        return true;
    }

    /*
    void gShader::Flush() {
        auto shader = GetCurrent();
        if (!shader) {
            return;
        }

        for (auto &&[name, uniform] : m_uniformMap) {
            if (!uniform->isValid()) { continue; }

            auto location = glGetUniformLocation(shader->m_id, name.c_str());
            if (location == -1) { continue; }

            switch (uniform->getType()) {
            case UniformType::INT:
                GL_CALL(glUniform1iv(location, (uniform->getSize() / sizeof(GLint)), static_cast<const GLint *>(uniform->getData())));
                break;
            case UniformType::VEC2:
                GL_CALL(glUniform2fv(location, (uniform->getSize() / sizeof(Vector2)), static_cast<const GLfloat *>(uniform->getData())));
                break;
            case UniformType::VEC3:
                GL_CALL(glUniform3fv(location, (uniform->getSize() / sizeof(Vector3)), static_cast<const GLfloat *>(uniform->getData())));
                break;
            case UniformType::VEC4:
                GL_CALL(glUniform4fv(location, (uniform->getSize() / sizeof(Vector4)), static_cast<const GLfloat *>(uniform->getData())));
                break;
            case UniformType::MAT3:
                GL_CALL(glUniformMatrix3fv(location, (uniform->getSize() / sizeof(Matrix3x3)), GL_FALSE, static_cast<const GLfloat *>(uniform->getData())));
                break;
            case UniformType::MAT4:
                GL_CALL(glUniformMatrix4fv(location, (uniform->getSize() / sizeof(Matrix4x4)), GL_FALSE, static_cast<const GLfloat *>(uniform->getData())));
                break;
            default:
                continue;
            }
        }
    }
    */

    bool gShader::isValid() const {
        return m_bValid && m_id;
    }

    bool gShader::registry(const std::string &name, UniformType type, grm::u32 count, const void *data) {
        if (m_uniformMap.find(name) != m_uniformMap.end()) {
            return true;
        }

        if (count <= 0) {
            return false;
        }

        m_uniformMap[name] = std::make_unique<Uniform>(name, type, count, data);
        
        return true;
    }

    bool gShader::checkerrors(grm::u32 shader, bool compile) {
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

            m_error << "================================" << std::endl;
            m_error << "Failed to compile shader - ";
            switch (type) {
            case GL_FRAGMENT_SHADER:
                m_error << ".frag" << std::endl << std::endl;
                break;
            case GL_VERTEX_SHADER:
                m_error << ".vert" << std::endl << std::endl;
                break;
            default:
                break;
            }
            m_error << logMessage << std::endl;
            m_error << "================================" << std::endl;
            return true;
        }

        GL_CALL(glGetProgramiv(shader, GL_LINK_STATUS, &status));
        if (status == GL_TRUE) {
            return false;
        }
        std::string logMessage(512, '\0');

        GL_CALL(glGetProgramInfoLog(shader, logMessage.size(), nullptr, logMessage.data()));
        m_error << "Failed to link shader program:" << std::endl << std::endl;
        m_error << logMessage << std::endl;
        return true;
    }
} // namespace grr

Uniform::Uniform(const std::string &name, UniformType type, grm::u32 count, const void *data) : m_name(name), m_type(type), m_size(0), m_data(nullptr) {
    if (count <= 0) { return; }

    switch (type) {
    case UniformType::INT:
        m_size = count * sizeof(GLint);
        break;
    case UniformType::VEC2:
        m_size = count * sizeof(Vector2);
        break;
    case UniformType::VEC3:
        m_size = count * sizeof(Vector3);
        break;
    case UniformType::VEC4:
        m_size = count * sizeof(Vector4);
        break;
    case UniformType::MAT3:
        m_size = count * sizeof(Matrix3x3);
        break;
    case UniformType::MAT4:
        m_size = count * sizeof(Matrix4x4);
        break;
    default:
        return;
    }

    m_data = std::malloc(m_size);
    if (data) {
        std::memcpy(m_data, data, m_size);
    }
}

Uniform::~Uniform() {
    if (m_data) {
        std::free(m_data);
    }
}

bool Uniform::isValid() const {
    return m_data != nullptr && m_size > 0;
}

const void *Uniform::getData() const {
    return m_data;
}

void Uniform::setData(const void *data) {
    std::memcpy(m_data, data, m_size);
}

UniformType Uniform::getType() const {
    return m_type;
}

grm::size Uniform::getSize() const {
    return m_size;
}

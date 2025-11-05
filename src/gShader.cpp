#include "gShader.h"

#include "gCommon.h"
#include "gl.h"

#include "gError.h"

#include <string.h>

grr::gShader::gShader() : shaderID(-1), valid(false), m_uniforms(nullptr), m_count(0)
{
    m_uniforms = ((Uniform *)malloc(sizeof(Uniform) * MAX_UNIFORM));
    for (size_t i=0;i<MAX_UNIFORM;i++)
    {
        auto &uniform = m_uniforms[i];

        uniform.name = nullptr;
        uniform.data = nullptr;
        uniform.index = InvalidID;
        uniform.dirty = false;
        uniform.stride = 0;
    }
}

grr::gShader::~gShader()
{
    for (size_t i=0;i<m_count;i++) {
        Uniform &uniform = m_uniforms[i];

        if (uniform.data != NULL) {
            free(uniform.data);
        }

        if (uniform.name != NULL) {
            free(uniform.name);
        }
    }
    if (m_uniforms) {
        free(m_uniforms);
    }
}

void grr::gShader::build(const char **fragment, int nfrag, const char **vertex, int nvert) {
    // Fragment shader
    auto shader_fragment = GL_CALL(glCreateShader(GL_FRAGMENT_SHADER));
    GL_CALL(glShaderSource(shader_fragment, nfrag, fragment, nullptr));
    GL_CALL(glCompileShader(shader_fragment));

    // Check erros
    GLint success;
    GL_CALL(glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success));
    if (!success) {
        char infoLog[1024];
        GL_CALL(glGetShaderInfoLog(shader_fragment, 1024, nullptr, infoLog));
        error("fragment: %s", infoLog);
        GL_CALL(glDeleteShader(shader_fragment));
        valid = false;
        return;
    }

    // Vertex shader
    auto shader_vertex = GL_CALL(glCreateShader(GL_VERTEX_SHADER));
    GL_CALL(glShaderSource(shader_vertex, nvert, vertex, nullptr));
    GL_CALL(glCompileShader(shader_vertex));

    // Check erros
    GL_CALL(glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success));
    if (!success) {
        char infoLog[1024];
        GL_CALL(glGetShaderInfoLog(shader_vertex, 1024, nullptr, infoLog));
        error("vertex: %s", infoLog);
        GL_CALL(glDeleteShader(shader_vertex));
        valid = false;
        return;
    }

    // delete program
    if (shaderID != -1)
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
    if (!success) {
        char infoLog[1024];
        GL_CALL(glGetProgramInfoLog(shaderID, 1024, nullptr, infoLog));
        error("link: %s", infoLog);

        GL_CALL(glDeleteShader(shader_fragment));
        GL_CALL(glDeleteShader(shader_vertex));
        GL_CALL(glDeleteProgram(shaderID));
        valid = false;
        return;
    }

    GL_CALL(glDeleteShader(shader_fragment));
    GL_CALL(glDeleteShader(shader_vertex));

    valid = true;
}

bool grr::gShader::isValid() const
{
    return valid && shaderID != -1;
}

UniformID grr::gShader::registry(const char *name, uint32_t count, UniformVariable_ variable)
{
    UniformID id = findUniform(name);

    if (id != InvalidID)
    {
        return id;
    }
 
    if (m_count + 1 >= MAX_UNIFORM)
    {
        error("%s, Max uniform exceded.", name);
        return InvalidID;
    }

    id = glGetUniformLocation(shaderID, name);
    if (id == -1)
    {
        error("%s, Uniform not found.", name);
        return InvalidID;
    }

    size_t stride = 0;
    switch (variable)
    {
        case UniformVariable::SAMPLERCUBE:
        case UniformVariable::SAMPLER2D:
        case UniformVariable::BOOL:
        case UniformVariable::INT:
            stride = count * sizeof(GLint);
            break;
        case UniformVariable::FLOAT:
            stride = count * sizeof(GLfloat);
            break;
        case UniformVariable::VEC2:
            stride = count * sizeof(Vector2);
            break;
        case UniformVariable::VEC3:
            stride = count * sizeof(Vector3);
            break;
        case UniformVariable::VEC4:
            stride = count * sizeof(Vector4);
            break;
        case UniformVariable::MAT3:
            stride = count * sizeof(Matrix3x3);
            break;
        case UniformVariable::MAT4:
            stride = count * sizeof(Matrix4x4);
            break;
        default:
            error("Type is not valid: ", name);
            return -1;
    }

    auto uniformID = m_count;
    m_count++;

    auto &uniform = m_uniforms[uniformID];
    if (uniform.name != nullptr)
    {
        free(uniform.name);
    }

    uniform.name = strdup(name);
    uniform.index = id;
    uniform.data = nullptr;
    uniform.variable = variable;
    uniform.dirty = false;
    uniform.stride = stride;
    if (uniform.data != nullptr)
    {
        free(uniform.data);
    }
    
    uniform.data = malloc(stride);

    return uniformID;
}

bool grr::gShader::setUniform(const char *name, const void *data)
{
    UniformID id = findUniform(name);
    if (id == InvalidID)
    {
        return false;
    }

    return setUniform(id, data);
}

bool grr::gShader::setUniform(UniformID id, const void *data)
{
    auto &uniform = m_uniforms[id];

    if (data == NULL || memcpy(uniform.data, data, uniform.stride) == NULL)
    {
        return false;
    }

    uniform.dirty = true;
    return true;
}

void grr::gShader::flush()
{
    for (UniformID id =0;id<m_count;id++)
    {
        auto& uniform = m_uniforms[id];

        if (!uniform.dirty)
            continue;

        uniform.dirty = false;

        switch (uniform.variable)
        {
            case UniformVariable::SAMPLERCUBE:
            case UniformVariable::SAMPLER2D:
            case UniformVariable::BOOL:
            case UniformVariable::INT:
                GL_CALL(glUniform1iv(uniform.index, (uniform.stride / sizeof(GLint)), (const GLint *)uniform.data));
                break;
            case UniformVariable::FLOAT:
                GL_CALL(glUniform1fv(uniform.index, (uniform.stride / sizeof(GLfloat)), (const GLfloat *)uniform.data));
                break;
            case UniformVariable::VEC2:
                GL_CALL(glUniform2fv(uniform.index, (uniform.stride / sizeof(Vector2)), (const GLfloat *)uniform.data));
                break;
            case UniformVariable::VEC3:
                GL_CALL(glUniform3fv(uniform.index, (uniform.stride / sizeof(Vector3)), (const GLfloat *)uniform.data));
                break;
            case UniformVariable::VEC4:
                GL_CALL(glUniform4fv(uniform.index, (uniform.stride / sizeof(Vector4)), (const GLfloat *)uniform.data));
                break;
            case UniformVariable::MAT3:
                GL_CALL(glUniformMatrix3fv(uniform.index, (uniform.stride / sizeof(Matrix3x3)), GL_FALSE, (const GLfloat *)uniform.data));
                break;
            case UniformVariable::MAT4:
                GL_CALL(glUniformMatrix4fv(uniform.index, (uniform.stride / sizeof(Matrix4x4)), GL_FALSE, (const GLfloat *)uniform.data));
                break;
            default:
                break;
        }
    }
}

void grr::gShader::bind()
{
    GL_CALL(glUseProgram(shaderID));
}

void grr::gShader::unbind()
{
    glUseProgram(0);
}

void grr::gShader::cleanUniform() {
    for (size_t i=0;i<MAX_UNIFORM;i++) {
        auto &uniform = m_uniforms[i];
        if (uniform.name != nullptr) {
            free(uniform.name);
            uniform.name = nullptr;
        }

        if (uniform.data != nullptr) {
            free(uniform.data);
            uniform.data = nullptr;
        }
        uniform.stride = 0;

        uniform.index = -1;
        uniform.dirty = false;
    }
    m_count = 0;
}

grr::Uniform *grr::gShader::getUniform() const
{
    return m_uniforms;
}

size_t grr::gShader::getUniformCount() const
{
    return m_count;
}

UniformID grr::gShader::findUniform(const char *name)
{
    for (size_t i=0;i<MAX_UNIFORM;i++)
    {
        if (m_uniforms[i].name == NULL)
        {
            continue;
        }


        if (strcmp(m_uniforms[i].name, name) == 0)
        {
            return i;
        }
    }
    return InvalidID;
}

const char *grr::gShader::getName(UniformID id) const
{
    return id == InvalidID ? NULL : m_uniforms[id].name;
}



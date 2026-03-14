#include "gShader.h"

#include "gCommon.h"
#include "gl.h"

#include "gError.h"

#include <cstddef>
#include <string.h>

grr::gShader::gShader() : shaderID(GR_INVALID_ID), valid(false), m_uniforms(nullptr), m_count(0), m_capacity(0)
{}

grr::gShader::~gShader()
{
    for (size_t i=0;i<m_count;i++)
    {
        ShaderUniform &uniform = m_uniforms[i];

        if (uniform.name != NULL)
            free(uniform.name);
    }

    if (m_uniforms != nullptr)
        free(m_uniforms);
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

UniformID grr::gShader::registry(const char *name, uint32_t count, UniformType type)
{
    UniformID id = findUniform(name);
    if (id != GR_INVALID_ID)
        return id;
 
    if (m_count >= m_capacity)
        reallocate();

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
            stride = count * sizeof(GLint);
            break;
        case UniformType::FLOAT:
            stride = count * sizeof(GLfloat);
            break;
        case UniformType::VEC2:
            stride = count * sizeof(Vector2);
            break;
        case UniformType::VEC3:
            stride = count * sizeof(Vector3);
            break;
        case UniformType::VEC4:
            stride = count * sizeof(Vector4);
            break;
        case UniformType::MAT3:
            stride = count * sizeof(Matrix3x3);
            break;
        case UniformType::MAT4:
            stride = count * sizeof(Matrix4x4);
            break;
        default:
            error("Type is not valid: ", name);
            return -1;
    }

    UniformID uniformID = m_count++;

    auto &uniform = m_uniforms[uniformID];
    uniform.name = strdup(name);
    uniform.id = location;
    uniform.type = type;
    uniform.stride = stride;

    return uniformID;
}

void grr::gShader::setUniform(const char *name, const void *data)
{
    UniformID id = findUniform(name);
    if (id == GR_INVALID_ID)
        return;

    return SetUniform(id, data);
}

void grr::gShader::SetUniform(UniformID id, const void *data)
{
    auto &uniform = m_uniforms[id];

    switch (uniform.type)
    {
        case UniformType::SAMPLERCUBE:
        case UniformType::SAMPLER2D:
        case UniformType::BOOL:
        case UniformType::INT:
            GL_CALL(glUniform1iv(uniform.id, (uniform.stride / sizeof(GLint)), (const GLint *)data));
            break;
        case UniformType::FLOAT:
            GL_CALL(glUniform1fv(uniform.id, (uniform.stride / sizeof(GLfloat)), (const GLfloat *)data));
            break;
        case UniformType::VEC2:
            GL_CALL(glUniform2fv(uniform.id, (uniform.stride / sizeof(Vector2)), (const GLfloat *)data));
            break;
        case UniformType::VEC3:
            GL_CALL(glUniform3fv(uniform.id, (uniform.stride / sizeof(Vector3)), (const GLfloat *)data));
            break;
        case UniformType::VEC4:
            GL_CALL(glUniform4fv(uniform.id, (uniform.stride / sizeof(Vector4)), (const GLfloat *)data));
            break;
        case UniformType::MAT3:
            GL_CALL(glUniformMatrix3fv(uniform.id, (uniform.stride / sizeof(Matrix3x3)), GL_FALSE, (const GLfloat *)data));
            break;
        case UniformType::MAT4:
            GL_CALL(glUniformMatrix4fv(uniform.id, (uniform.stride / sizeof(Matrix4x4)), GL_FALSE, (const GLfloat *)data));
            break;
        default:
            break;
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

grr::ShaderUniform *grr::gShader::GetUniform(UniformID id) const
{
    return m_uniforms + id;
}

size_t grr::gShader::getUniformCount() const
{
    return m_count;
}

UniformID grr::gShader::findUniform(const char *name)
{
    for (size_t i=0;i<m_count;i++)
    {
        if (strcmp(m_uniforms[i].name, name) == 0)
            return i;
    }
    return GR_INVALID_ID;
}

void grr::gShader::reallocate()
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



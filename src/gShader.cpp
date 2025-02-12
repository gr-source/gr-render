#include "gShader.h"

#include "gl.h"

#include <stdlib.h>
#include <string.h>

#include "gError.h"

grr::gShader::gShader() : programID(glCreateProgram()), valid(false), uniform_list((Uniform *)calloc(MAX_UNIFORM, sizeof(Uniform))), numUniform(0) {
    for (size_t i=0;i<MAX_UNIFORM;i++) {
        auto &uniform = uniform_list[i];

        uniform.name = nullptr;
        uniform.data = nullptr;
        uniform.index = -1;
        uniform.dirty = false;
    }
}

grr::gShader::~gShader() {
    for (size_t i=0;i<numUniform;i++) {
        Uniform &uniform = uniform_list[i];

        if (uniform.data != NULL) {
            free(uniform.data);
        }

        if (uniform.name != NULL) {
            free(uniform.name);
        }
    }
    if (uniform_list) {
        free(uniform_list);
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
    if (programID != 0) {
        GL_CALL(glUseProgram(0));
        GL_CALL(glDeleteProgram(programID));
    }

    programID = GL_CALL(glCreateProgram());

    // attach
    GL_CALL(glAttachShader(programID, shader_fragment));
    GL_CALL(glAttachShader(programID, shader_vertex));

    // link
    GL_CALL(glLinkProgram(programID));

    GL_CALL(glGetProgramiv(programID, GL_LINK_STATUS, &success));
    if (!success) {
        char infoLog[1024];
        GL_CALL(glGetProgramInfoLog(programID, 1024, nullptr, infoLog));
        error("link: %s", infoLog);

        GL_CALL(glDeleteShader(shader_fragment));
        GL_CALL(glDeleteShader(shader_vertex));
        GL_CALL(glDeleteProgram(programID));
        valid = false;
        return;
    }

    GL_CALL(glDeleteShader(shader_fragment));
    GL_CALL(glDeleteShader(shader_vertex));

    valid = true;
}

bool grr::gShader::isValid() const {
    return valid;
}

int grr::gShader::registry(const char *name, UniformType type) {
    int id = findUniform(name);

    if (id != -1) { return id; }
 
    if (numUniform + 1 >= MAX_UNIFORM) {
        error("%s, Max uniform exceded.", name);
        return -1;
    }

    id = glGetUniformLocation(programID, name);
    if (id == -1) {
        error("%s, Uniform not found.", name);
        return -1;
    }


    auto &uniform = uniform_list[numUniform];
    uniform.name = strdup(name);
    uniform.index = id;
    uniform.data = nullptr;
    uniform.type = type;
    uniform.dirty = false;

    return numUniform++;
}

void grr::gShader::setUniform(const char *name, uint32_t count, const void *data) {
    int id = findUniform(name);
    if (id == -1) { return; }

    auto &uniform = uniform_list[id];

    size_t stride = 0;
    switch (uniform.type) {
    case UniformType_int:
        stride = count * sizeof(GLint);
        break;
    case UniformType_float:
        stride = count * sizeof(GLfloat);
        break;
    case UniformType_vec2:
        stride = count * sizeof(Vector2);
        break;
    case UniformType_vec3:
        stride = count * sizeof(Vector3);
        break;
    case UniformType_vec4:
        stride = count * sizeof(Vector4);
        break;
    case UniformType_mat3:
        stride = count * sizeof(Matrix3x3);
        break;
    case UniformType_mat4:
        stride = count * sizeof(Matrix4x4);
        break;
    default:
        return;
    }

    if (uniform.count != count) {
        if (uniform.data != nullptr) {
            free(uniform.data);
        }
        uniform.data = malloc(stride);
        uniform.count = count;
    }

    memcpy(uniform.data, data, stride);

    uniform.dirty = true;
}

void grr::gShader::flush() {
    auto temp = numUniform;
    while (temp--) {
        auto& uniform = uniform_list[temp];

        if (!uniform.dirty) {
            continue;
        }

        uniform.dirty = false;

        switch (uniform.type) {
        case UniformType_int:
            GL_CALL(glUniform1iv(uniform.index, uniform.count, (const GLint *)uniform.data));
            break;
        case UniformType_float:
            GL_CALL(glUniform1fv(uniform.index, uniform.count, (const GLfloat *)uniform.data));
            break;
        case UniformType_vec2:
            GL_CALL(glUniform2fv(uniform.index, uniform.count, (const GLfloat *)uniform.data));
            break;
        case UniformType_vec3:
            GL_CALL(glUniform3fv(uniform.index, uniform.count, (const GLfloat *)uniform.data));
            break;
        case UniformType_vec4:
            GL_CALL(glUniform4fv(uniform.index, uniform.count, (const GLfloat *)uniform.data));
            break;
        case UniformType_mat3:
            GL_CALL(glUniformMatrix3fv(uniform.index, uniform.count, GL_FALSE, (const GLfloat *)uniform.data));
            break;
        case UniformType_mat4:
            GL_CALL(glUniformMatrix4fv(uniform.index, uniform.count, GL_FALSE, (const GLfloat *)uniform.data));
            break;
        default:
            break;
        }
    }
}

void grr::gShader::bind() {
    GL_CALL(glUseProgram(programID));
}

void grr::gShader::unbind() {
    glUseProgram(0);
}

void grr::gShader::cleanUniform() {
    for (size_t i=0;i<MAX_UNIFORM;i++) {
        auto &uniform = uniform_list[i];
        if (uniform.name != nullptr) {
            free(uniform.name);
            uniform.name = nullptr;
        }

        if (uniform.data != nullptr) {
            free(uniform.data);
            uniform.data = nullptr;
        }
        uniform.count = 0;

        uniform.index = -1;
        uniform.dirty = false;
    }
    numUniform = 0;
}

grr::Uniform *grr::gShader::getUniform() const
{
    return uniform_list;
}

size_t grr::gShader::getUniformCount() const
{
    return numUniform;
}

int grr::gShader::findUniform(const char *name) {
    for (size_t i=0;i<MAX_UNIFORM;i++) {
        if (uniform_list[i].name == NULL) {
            continue;
        }

        if (strcmp(uniform_list[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

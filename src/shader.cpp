#include "shader.hpp"

#include "gl.h"

grr::Shader::Shader() : mID(0) {
    mID = GL_CALL(glCreateProgram());
}

grr::Shader::~Shader() {
    if (mID != 0) {
        GL_CALL(glDeleteProgram(mID));
    }
}

#include "gError.h"

#include "gl.h"

std::ostringstream m_error;

std::string grr::gError::GetError() {
    auto temp = m_error.str();
    m_error.clear();
    return temp;
}


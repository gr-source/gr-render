#include "gError.h"

std::string m_error;

std::string grr::gError::GetError() {
    auto temp = m_error;
    m_error.clear();
    return m_error;
}


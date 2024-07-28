#include "gError.h"

std::string m_error;

const std::string & grr::gError::GetError() {
    return m_error;
}

void grr::gError::Clean() {
    m_error.clear();
}

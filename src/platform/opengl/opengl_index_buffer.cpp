#include "platform/opengl/opengl_index_buffer.hpp"

#include "gl.h"

namespace gr
{
    opengl_index_buffer::opengl_index_buffer(const void* data, uint32_t size) : m_id(0)
    {
        glGenBuffers(1, &m_id);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    opengl_index_buffer::~opengl_index_buffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void opengl_index_buffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void opengl_index_buffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

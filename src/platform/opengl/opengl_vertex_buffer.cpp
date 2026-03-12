#include "platform/opengl/opengl_vertex_buffer.hpp"

#include "gl.h"
#include <iostream>

namespace gr
{
    opengl_vertex_buffer::opengl_vertex_buffer(const void* data, uint32_t size, buffer_usage usage) : m_id(0), m_size(size)
    {
        glGenBuffers(1, &m_id);

        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage == buffer_usage::static_draw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_usage = usage;
    }

    opengl_vertex_buffer::~opengl_vertex_buffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void opengl_vertex_buffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void opengl_vertex_buffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void opengl_vertex_buffer::SetData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        if (size > m_size)
        {
            m_size = size;

            glBufferData(GL_ARRAY_BUFFER, size, data, m_usage == buffer_usage::static_draw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

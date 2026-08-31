#include "uniformBuffer.hpp"

#include "gl.h"

namespace engine::renderer
{
UniformBuffer::UniformBuffer(size_t size) : m_handle(0)
{
    GL_CALL(glCreateBuffers(1, &m_handle));
    GL_CALL(glNamedBufferStorage(m_handle, size, nullptr, GL_DYNAMIC_STORAGE_BIT));
}

UniformBuffer::~UniformBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_handle));
}

void UniformBuffer::attach(uint32_t binding)
{
    GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_handle));
}

void UniformBuffer::detach()
{
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);
}

void UniformBuffer::update(const void* data, size_t size)
{
    GL_CALL(glNamedBufferSubData(m_handle, 0, size, data));
}

}


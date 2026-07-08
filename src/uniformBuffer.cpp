#include "uniformBuffer.hpp"

#include "gl.h"

namespace engine::renderer
{
UniformBuffer::UniformBuffer() : m_handle(0), m_size(0), m_binding(0)
{}

UniformBuffer::~UniformBuffer()
{}

void UniformBuffer::Create(uint32_t binding, size_t size)
{
    GL_CALL(glCreateBuffers(1, &m_handle));
    GL_CALL(glNamedBufferStorage(m_handle, size, nullptr, GL_DYNAMIC_STORAGE_BIT));
    GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_handle));

    m_size = size;
    m_binding = binding;
}

void UniformBuffer::Destroy()
{
    GL_CALL(glDeleteBuffers(1, &m_handle));
}

void UniformBuffer::update(const void* data, size_t size)
{
    GL_CALL(glNamedBufferSubData(m_handle, 0, size, data));
}

}


#include "gVertexArray.h"

#include "gCommon.h"
#include "gl.h"
#include <cstddef>
#include <cstdint>

namespace grr
{
    gVertexArray::gVertexArray() : vertexID(InvalidVertexID)
    {
        GL_CALL(glGenVertexArrays(1, &vertexID));
    }

    gVertexArray::~gVertexArray()
    {
        if (vertexID != InvalidVertexID)
        {
            glDeleteVertexArrays(1, &vertexID);
        }
    }

    gVertexArray *gVertexArray::m_instance = nullptr;

    grm::u32 gVertexArray::s_currentBuffer = 0;

    std::array<std::uint32_t, 5> gVertexArray::bufferMappings = {
        GL_ARRAY_BUFFER,            // BufferType::VBO
        GL_ELEMENT_ARRAY_BUFFER,    // BufferType::EBO
        GL_FRAMEBUFFER,             // BufferType::FBO
        GL_RENDERBUFFER,            // BufferType::RBO
        GL_TEXTURE_BUFFER           // BufferType::TBO
    };

    std::array<std::uint32_t, 7> gVertexArray::primitiveMappings {
        GL_POINTS,
        GL_LINES,
        GL_LINE_LOOP,
        GL_LINE_STRIP,
        GL_TRIANGLES,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN
    };

    std::unordered_map<BufferID, grm::u32> gVertexArray::m_bufferIndex;

    BufferID gVertexArray::CreateBuffer(BufferType_ target, const void *data, std::size_t size)
    {
        BufferID bufferID = 0;
        GL_CALL(glGenBuffers(1, &bufferID));
        if (!bufferID)
        {
            return InvalidBufferID;
        }

        if (data != nullptr && size > 0)
        {
            GL_CALL(glBindBuffer(bufferMappings[target], bufferID));
            GL_CALL(glBufferData(bufferMappings[target], size, data, GL_STATIC_DRAW));
            GL_CALL(glBindBuffer(bufferMappings[target], 0));
        }

        m_bufferIndex.emplace(bufferID, bufferMappings[target]);

        return bufferID;
    }

    void gVertexArray::DeleteBuffer(BufferID index)
    {
        auto it = m_bufferIndex.find(index);
        if (it == m_bufferIndex.end())
        {
            return;
        }

        GL_CALL(glDeleteBuffers(1, &index));

        m_bufferIndex.erase(it);
    }

    void gVertexArray::Bind(grm::u32 index)
    {
        s_currentBuffer = m_bufferIndex[index];

        GL_CALL(glBindBuffer(m_bufferIndex[index], index));
    }

    void gVertexArray::SetAttrib(grm::u8 index, grm::u16 size, grm::u16 stride, const void *pointer) {
        GL_CALL(glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(size), GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), pointer));
        GL_CALL(glEnableVertexAttribArray(static_cast<GLuint>(index)));
    }

    void gVertexArray::SetAttribI(grm::u8 index, grm::u16 size, grm::u16 stride, const void *pointer) {
        GL_CALL(glVertexAttribIPointer(static_cast<GLuint>(index), static_cast<GLint>(size), GL_INT, static_cast<GLsizei>(stride), pointer));
        GL_CALL(glEnableVertexAttribArray(static_cast<GLuint>(index)));
    }

    void gVertexArray::SetAttribDivisor(grm::u8 index, grm::u8 divisor) {
        GL_CALL(glVertexAttribDivisor(static_cast<GLuint>(index), divisor));
    }

    void gVertexArray::UpdateResizeBuffer(grm::u32 size, BufferUsage usage) {
        int arraySize = 0;
        GL_CALL(glGetBufferParameteriv(s_currentBuffer, GL_BUFFER_SIZE,  &arraySize));

        GLenum num = 0;

        switch (usage) {
        case BufferUsage::STATIC:
            num = GL_STATIC_DRAW;
            break;
        case BufferUsage::DYNAMIC:
            num = GL_DYNAMIC_DRAW;
            break;
        default:
            GR_ASSERT("Invalid.");
            break;
        }
        
        while (size != arraySize)
        {
            GL_CALL(glBufferData(s_currentBuffer, size, nullptr, num));

            GL_CALL(glGetBufferParameteriv(s_currentBuffer, GL_BUFFER_SIZE,  &arraySize));
        }
    }

    void gVertexArray::SetBufferUpdate(grm::u32 offset, grm::u32 size, const void *data) {
        GL_CALL(glBufferSubData(s_currentBuffer, offset, size, data));
    }

    void gVertexArray::DrawElementsInstanced(PrimitiveType primitive, grm::u32 count, const void *indices, grm::u32 primcount) {
        GL_CALL(glDrawElementsInstanced(primitiveMappings[primitive], count, GL_UNSIGNED_INT, indices, primcount));
    }

    void gVertexArray::DrawElements(PrimitiveType primitive, grm::u32 count, const void* indices) {
        GL_CALL(glDrawElements(primitiveMappings[primitive], count, GL_UNSIGNED_INT, indices));
    }

    void gVertexArray::DrawArrays(PrimitiveType primitive, grm::u32 count)
    {
        GL_CALL(glDrawArrays(primitiveMappings[primitive], 0, count));
    }

    void gVertexArray::DrawArraysInstanced(PrimitiveType primitive, grm::u32 count, grm::u32 primcount) {
        glDrawArraysInstanced(primitiveMappings[primitive], 0, count, primcount);
    }

    void gVertexArray::bind()
    {
        GL_CALL(glBindVertexArray(vertexID));

        m_instance = this;
    }

    void gVertexArray::unbind()
    {
        glBindVertexArray(0);

        m_instance = nullptr;
    }

    bool gVertexArray::is_valid() const
    {
        return vertexID != InvalidVertexID;
    }

    const VertexID &gVertexArray::getID() const
    {
        return vertexID;
    }
    
} // namespace grr

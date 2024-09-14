#include "gVertexArray.h"

#include "gl.h"

namespace grr {
    gVertexArray::gVertexArray() : m_vao(0) {}

    gVertexArray* gVertexArray::m_instance = nullptr;

    grm::u32 gVertexArray::s_currentBuffer = 0;

    std::unordered_map<BufferType, grm::u32> gVertexArray::m_bufferTypeMap {
        {BufferType_VBO, GL_ARRAY_BUFFER},
        {BufferType_PBO, GL_ARRAY_BUFFER},
        {BufferType_EBO, GL_ELEMENT_ARRAY_BUFFER}
    };

    std::unordered_map<PrimitiveType, grm::u32> gVertexArray::m_primitiveMap {
        {PrimitiveType_Points, GL_POINTS},
        {PrimitiveType_Lines, GL_LINES},
        {PrimitiveType_LineLoop, GL_LINE_LOOP},
        {PrimitiveType_LineStrip, GL_LINE_STRIP},
        {PrimitiveType_Triangles, GL_TRIANGLES},
        {PrimitiveType_TriangleStrip, GL_TRIANGLE_STRIP},
        {PrimitiveType_TriangleFan, GL_TRIANGLE_FAN}
    };

    std::unordered_map<grm::u32, grm::u32> gVertexArray::m_bufferIndex;

    gVertexArray *gVertexArray::Create() {
        gVertexArray* vertexArray = new gVertexArray();

        GL_CALL(glGenVertexArrays(1, &vertexArray->m_vao));

        return vertexArray;
    }

    grm::u32 gVertexArray::CreateBuffer(BufferType target) {
        grm::u32 m_index = 0;
        GL_CALL(glGenBuffers(1, &m_index));
        if (!m_index) {
            std::cout << "falied to create buffers" << std::endl;
            return 0;
        }

        GL_CALL(glBindBuffer(m_bufferTypeMap[target], m_index));

        m_bufferIndex.emplace(m_index, m_bufferTypeMap[target]);

        return m_index;
    }

    void gVertexArray::DeleteBuffer(grm::u32 index) {
        auto it = m_bufferIndex.find(index);
        if (it == m_bufferIndex.end()) {
            return;
        }
        GL_CALL(glDeleteBuffers(1, &index));

        m_bufferIndex.erase(it);
    }

    void gVertexArray::Bind(grm::u32 index) {
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

    void gVertexArray::UpdateResizeBuffer(grm::u32 size, grm::u32 flags) {
        int arraySize = 0;
        GL_CALL(glGetBufferParameteriv(s_currentBuffer, GL_BUFFER_SIZE,  &arraySize));

        GLenum usage;

        switch (flags) {
        case gBufferFlags_Static:
            usage = GL_STATIC_DRAW;
            break;
        case gBufferFlags_Dynamic:
            usage = GL_DYNAMIC_DRAW;
            break;
        default:
            GR_ASSERT("Invalid.");
            break;
        }
        
        while (size != arraySize) {
            GL_CALL(glBufferData(s_currentBuffer, size, nullptr, usage));

            GL_CALL(glGetBufferParameteriv(s_currentBuffer, GL_BUFFER_SIZE,  &arraySize));
        }
    }

    void gVertexArray::SetBufferUpdate(grm::u32 offset, grm::u32 size, const void *data) {
        GL_CALL(glBufferSubData(s_currentBuffer, offset, size, data));
    }

    void gVertexArray::DrawElementsInstanced(PrimitiveType primitive, grm::u32 count, const void *indices, grm::u32 primcount) {
        GL_CALL(glDrawElementsInstanced(m_primitiveMap[primitive], count, GL_UNSIGNED_INT, indices, primcount));
    }

    void gVertexArray::DrawElements(PrimitiveType primitive, grm::u32 count, const void* indices) {
        GL_CALL(glDrawElements(m_primitiveMap[primitive], count, GL_UNSIGNED_INT, indices));
    }

    void gVertexArray::DrawArrays(PrimitiveType primitive, grm::u32 count) {
        GL_CALL(glDrawArrays(m_primitiveMap[primitive], 0, count));
    }

    void gVertexArray::DrawArraysInstanced(PrimitiveType primitive, grm::u32 count, grm::u32 primcount) {
        glDrawArraysInstanced(m_primitiveMap[primitive], 0, count, primcount);
    }

    void gVertexArray::bind() {
        GL_CALL(glBindVertexArray(m_vao));

        m_instance = this;
    }

    void gVertexArray::unbind() {
        glBindVertexArray(0);

        m_instance = nullptr;
    }

    void gVertexArray::destroy() {
        if (m_vao != -1) {
            GL_CALL(glDeleteVertexArrays(1, &m_vao));
        }
        delete this;
    }

    bool gVertexArray::isValid() const {
        return m_vao;
    }
    
    grm::u32 gVertexArray::getID() const {
        return m_vao;
    }
    
    void gVertexArray::Release() {
        m_instance = nullptr;

        s_currentBuffer = 0;

        m_bufferTypeMap.clear();

        m_primitiveMap.clear();

        m_bufferIndex.clear();
    }
} // namespace grr

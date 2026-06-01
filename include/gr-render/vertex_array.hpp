#pragma once

#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace gr
{
    class vertex_array
    {
    protected:
        std::vector<vertex_buffer*> m_vertex_buffers;

        index_buffer* m_index_buffer;

    public:
        static vertex_array* create();

        vertex_array() : m_index_buffer(nullptr) {}

        virtual ~vertex_array() {}

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(vertex_buffer* vbo) = 0;

        virtual void SetIndexBuffer(index_buffer* ibo) = 0;

        inline std::vector<vertex_buffer*> GetVertexBuffers()
        {
            return m_vertex_buffers;
        }
    };
}



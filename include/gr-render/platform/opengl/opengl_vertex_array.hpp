#pragma once

#include "vertex_array.hpp"

#include <stdint.h>

namespace gr
{
    class opengl_vertex_array : public vertex_array
    {
    public:
        opengl_vertex_array();
        virtual ~opengl_vertex_array();

        virtual void Bind() const override;

        virtual void Unbind() const override;

        virtual void AddVertexBuffer(std::shared_ptr<vertex_buffer>& vbo) override;

        virtual void SetIndexBuffer(std::shared_ptr<index_buffer>& ibo) override;

    private:
        std::vector<std::shared_ptr<vertex_buffer>> m_vertex_buffers;

        std::shared_ptr<index_buffer> m_index_buffer;

        uint32_t m_vertex_buffer_index;

        uint32_t m_id;
    };
}

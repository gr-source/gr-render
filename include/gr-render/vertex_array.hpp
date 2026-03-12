#pragma once

#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace gr
{
    class vertex_array
    {
    public:
        static std::shared_ptr<vertex_array> create();

        virtual ~vertex_array() {}

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(std::shared_ptr<vertex_buffer>& vbo) = 0;

        virtual void SetIndexBuffer(std::shared_ptr<index_buffer>& ibo) = 0;
    };
}



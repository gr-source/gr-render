#pragma once

#include "vertex_buffer.hpp"
namespace gr
{
    class opengl_vertex_buffer : public vertex_buffer
    {
    public:
        opengl_vertex_buffer(const void* data, uint32_t size, buffer_usage usage);
        virtual ~opengl_vertex_buffer() override;

        virtual void Bind() override;

        virtual void Unbind() override;

        virtual void SetData(const void* data, uint32_t size) override;

    private:
        uint32_t m_size;

        uint32_t m_id;
    };
}

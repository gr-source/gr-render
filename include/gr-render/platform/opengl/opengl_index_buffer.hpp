#pragma once

#include "index_buffer.hpp"
namespace gr
{
    class opengl_index_buffer : public index_buffer
    {
    public:
        opengl_index_buffer(const void *data, uint32_t size);
        virtual ~opengl_index_buffer() override;

        virtual void Bind() override;

        virtual void Unbind() override;

    private:
        uint32_t m_id;
    };
}

#include "vertex_buffer.hpp"

#include "platform/opengl/opengl_vertex_buffer.hpp"

namespace gr
{
    std::shared_ptr<vertex_buffer> vertex_buffer::create(const void *data, uint32_t size, buffer_usage usage)
    {
        return std::make_shared<opengl_vertex_buffer>(data, size, usage);
    }
}

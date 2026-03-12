#include "index_buffer.hpp"

#include "platform/opengl/opengl_index_buffer.hpp"

namespace gr
{
    std::shared_ptr<index_buffer> index_buffer::create(const void *data, uint32_t size)
    {
        return std::make_shared<opengl_index_buffer>(data, size);
    }
}

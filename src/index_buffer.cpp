#include "index_buffer.hpp"

#include "platform/opengl/opengl_index_buffer.hpp"

namespace gr
{
    index_buffer* index_buffer::create(const void *data, uint32_t size)
    {
        return new opengl_index_buffer(data, size);
    }
}

#include "vertex_array.hpp"

#include "platform/opengl/opengl_vertex_array.hpp"

namespace gr
{
    std::shared_ptr<vertex_array> vertex_array::create()
    {
        return std::make_shared<opengl_vertex_array>();
    }
}

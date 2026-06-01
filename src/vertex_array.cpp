#include "vertex_array.hpp"

#include "platform/opengl/opengl_vertex_array.hpp"

namespace gr
{
    vertex_array* vertex_array::create()
    {
        return new opengl_vertex_array();
    }
}

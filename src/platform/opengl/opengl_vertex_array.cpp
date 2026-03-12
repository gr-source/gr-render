#include "platform/opengl/opengl_vertex_array.hpp"

#include "gl.h"


static GLenum shader_data_type_to_opengl_base_type(gr::shader_data_type type)
{
    switch (type)
    {
        case gr::shader_data_type::Float:
            return GL_FLOAT;
        case gr::shader_data_type::Float2:
            return GL_FLOAT;
        case gr::shader_data_type::Float3:
            return GL_FLOAT;
        case gr::shader_data_type::Float4:
            return GL_FLOAT;
    }
    return 0; 
}

namespace gr
{
    opengl_vertex_array::opengl_vertex_array() : m_id(0), m_vertex_buffer_index(0)
    {
        glGenVertexArrays(1, &m_id);
    }

    opengl_vertex_array::~opengl_vertex_array()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void opengl_vertex_array::Bind() const
    {
        glBindVertexArray(m_id);
    }

    void opengl_vertex_array::Unbind() const
    {
        glBindVertexArray(0);
    }

    void opengl_vertex_array::AddVertexBuffer(std::shared_ptr<vertex_buffer>& vbo)
    {
        Bind();
        vbo->Bind();

        const auto& layout = vbo->GetLayout();
        
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(m_vertex_buffer_index);
            
            glVertexAttribPointer(
                m_vertex_buffer_index,
                element.get_component_count(),
                shader_data_type_to_opengl_base_type(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.get_stride(),
                (const void*)element.offset
            );
            glVertexAttribDivisor(m_vertex_buffer_index, element.instanced);
            
            m_vertex_buffer_index++;
        }

        m_vertex_buffers.push_back(vbo);
    }

    void opengl_vertex_array::SetIndexBuffer(std::shared_ptr<index_buffer>& ibo)
    {
        Bind();
        ibo->Bind();

        m_index_buffer = ibo;
    }
}

#pragma once

#include "gCommon.h"

#include <stdint.h>

namespace gr
{
    class Shader
    {
    public:
        Shader();
        ~Shader();

        bool create(const char **fragment, size_t numFragments, const char **vertex, size_t numvertex, std::string* error);

        void destroy();

        UniformID registry(const char *name, uint32_t count, UniformType type);

        template <typename T>
        void set_uniform(const char *name, const T &data);

        void setUniform(const char *name, const void *data);

        void SetUniform(UniformID id, const void *data);

        void bind();

        void unbind();

        inline ShaderUniform *GetUniforms() const
        {
            return m_uniforms;
        }

        inline size_t GetUniformCount() const
        {
            return m_count;
        }

        inline size_t GetUniformBufferSize() const
        {
            return m_buffer_size;
        }

        UniformID findUniform(const char *name);

    private:
        ShaderID shaderID;

        ShaderUniform *m_uniforms;

        size_t m_buffer_size;

        size_t m_capacity;

        size_t m_count;

        void grow();
    };

    template <typename T>
    inline void Shader::set_uniform(const char *name, const T &data)
    {
        setUniform(name, &data);
    }
};






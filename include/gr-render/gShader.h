#pragma once

#include "gCommon.h"
#include "shader.hpp"

#include <stdint.h>

namespace grr
{
    class gShader
    {
    public:
        gShader();
        ~gShader();

        void build(const char **fragment, int nfrag, const char **vertex, int nvert);

        bool isValid() const;

        UniformID registry(const char *name, uint32_t count, UniformType type);

        template <typename T>
        void set_uniform(const char *name, const T &data);

        void setUniform(const char *name, const void *data);

        void SetUniform(UniformID id, const void *data);

        void bind();

        void unbind();

        ShaderUniform *GetUniform(UniformID id) const;

        size_t getUniformCount() const;

        UniformID findUniform(const char *name);

    private:
        ShaderID shaderID;

        ShaderUniform *m_uniforms;

        size_t m_capacity;

        size_t m_count;

        bool valid;

        void reallocate();
    };

    template <typename T>
    inline void gShader::set_uniform(const char *name, const T &data)
    {
        setUniform(name, &data);
    }
};






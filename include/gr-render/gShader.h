#pragma once

#include "gCommon.h"
#include "shader.hpp"

#include <stdint.h>

#define MAX_UNIFORM 200

namespace grr {
    class gShader {
    public:
        gShader();
        ~gShader();

        void build(const char **fragment, int nfrag, const char **vertex, int nvert);

        bool isValid() const;

        UniformID registry(const char *name, uint32_t count, UniformVariable_ variable);

        template <typename T>
        bool set_uniform(const char *name, const T &data);

        bool setUniform(const char *name, const void *data);

        bool setUniform(UniformID id, const void *data);

        void flush();
        
        void bind();

        void unbind();

        void cleanUniform();

        Uniform *getUniform() const;

        size_t getUniformCount() const;

        UniformID findUniform(const char *name);

        const char *getName(UniformID id) const;
        
    private:
        ShaderID shaderID;

        Uniform *uniform_list;

        size_t numUniform;

        bool valid;
    };

    template <typename T>
    inline bool gShader::set_uniform(const char *name, const T &data)
    {
        return setUniform(name, &data);
    }
};






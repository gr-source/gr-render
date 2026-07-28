#pragma once

#include "gCommon.h"

#include <cstddef>
#include <string>

namespace gr
{
    enum class UniformType
    {
        BOOL,
        INT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT3,
        MAT4,
        SAMPLER2D,
        SAMPLERCUBE
    };

    typedef struct uniform
    {
        UniformType type;
        UniformID location;

        uint32_t offset;
        uint32_t size;
        uint32_t count;

        char name[256];
    } uniform;

    typedef struct shader
    {
        ShaderID shaderID = GR_INVALID_ID;

        uniform *uniforms = nullptr;

        size_t bufferSize = 0;

        size_t capacity = 0;

        size_t count = 0;

        bool create(const char **fragment, size_t numFragments, const char **vertex, size_t numvertex, std::string* error);

        void destroy();

        UniformID registry(const char *name, uint32_t numElements, UniformType type);

        template <typename T>
        void set_uniform(const char *name, const T &data);

        void setUniform(const char *name, const void *data);

        void SetUniform(UniformID id, const void *data);

        void bind() const;

        void unbind() const;

        UniformID findUniform(const char *name);

        void grow();
    } shader;

    template <typename T>
    inline void shader::set_uniform(const char *name, const T &data)
    {
        setUniform(name, &data);
    }
};






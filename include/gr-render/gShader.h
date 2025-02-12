#pragma once

#include "gCommon.h"

#include <memory>

#include <stdint.h>

#define MAX_UNIFORM 200

namespace grr {
    enum UniformType {
        UniformType_none   = 1 << 0,
        UniformType_int    = 1 << 1,
        UniformType_float  = 1 << 2,
        UniformType_vec2   = 1 << 3,
        UniformType_vec3   = 1 << 4,
        UniformType_vec4   = 1 << 5,
        UniformType_mat3   = 1 << 6,
        UniformType_mat4   = 1 << 7,
    };

    struct Uniform {
        char *name;
        UniformType type;
        int index;
        uint32_t count;
        void *data;
        bool dirty;
    };
    
    class gShader {
    public:
        gShader();
        ~gShader();

        void build(const char **fragment, int nfrag, const char **vertex, int nvert);

        bool isValid() const;

        int registry(const char *name, UniformType type);

        void setUniform(const char *name, uint32_t count, const void *data);

        void flush();
        
        void bind();

        void unbind();

        void cleanUniform();

        Uniform *getUniform() const;

        size_t getUniformCount() const;

    private:
        uint32_t programID;

        Uniform *uniform_list;

        size_t numUniform;

        bool valid;

        int findUniform(const char *name);
    };
};






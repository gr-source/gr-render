#pragma once

#include "gCommon.h"

#include <memory>

#include <stdint.h>

#define MAX_UNIFORM 200

namespace grr {
    class gShader {
    public:
        gShader();
        ~gShader();

        void build(const char **fragment, int nfrag, const char **vertex, int nvert);

        bool isValid() const;

        int registry(const char *name, uint32_t count, UniformType type);

        void setUniform(const char *name, const void *data);

        void setUniform(int id, const void *data);

        void flush();
        
        void bind();

        void unbind();

        void cleanUniform();

        Uniform *getUniform() const;

        size_t getUniformCount() const;

        int findUniform(const char *name);

        const char *getName(int id) const;
        
    private:
        uint32_t programID;

        Uniform *uniform_list;

        size_t numUniform;

        bool valid;

    };
};






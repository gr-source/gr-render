#pragma once

#include "gCommon.h"

namespace grr {
    class gShader {
    public:
        gShader();
        ~gShader();

        static gShader *Create(const char **fragments, const char **vertex);

        static void Create(gShader *shader, const char **fragments, const char **vertex);

        static gShader* GetCurrent();

        static void Register(const std::string& name);

        template <typename T>
        static void SetUniform(const std::string& name, grm::u16 count, const T& data);

        template <typename T>
        static void SetUniform(const std::string &name, grm::u16 count, const T *value);

        template <typename T>
        static void SetUniform(const std::string& name, T data);

        const bool isValid() const;

        void bind();

        void unbind();

    private:
        grm::u32 m_id;

        std::unordered_map<std::string, grm::u32> m_uniformMap;

        static gShader* m_instance;

        static const bool checkerrors(grm::u32 shader, bool compile);
    };
}
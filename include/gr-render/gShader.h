#pragma once

#include "gCommon.h"

namespace grr {
    class gShader {
    public:
        gShader();
        ~gShader();

        static gShader *Create(const char **fragments, const char **vertex);

        static void SetupShader(const char **fragments, const char **vertex);

        static void Bind(gShader *shader);

        static void Unbind();

        static gShader* GetCurrent();

        static void Register(const std::string& name);

        template <typename T>
        static void SetUniform(const std::string& name, grm::u16 count, const T& data);

        template <typename T>
        static void SetUniform(const std::string &name, grm::u16 count, const T *value);

        template <typename T>
        static void SetUniform(const std::string& name, T data);

        bool isValid() const;

    private:
        std::unordered_map<std::string, grm::u32> m_uniformMap;

        grm::u32 m_id;

        bool m_bValid;

        static gShader* m_instance;

        static const bool checkerrors(grm::u32 shader, bool compile);
    };
}
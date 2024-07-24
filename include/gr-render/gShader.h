#pragma once

#include "gCommon.h"

namespace grr {
    class gShader {
    public:
        gShader();

        static gShader* Create(const std::vector<const char*>& fragments, const std::vector<const char*>& vertex);

        static gShader* GetCurrent();

        static void Register(const std::string& name);

        template <typename T>
        static void SetUniform(const std::string& name, u16 count, const T& data);

        template <typename T>
        static void SetUniform(const std::string& name, T data);

        const bool isValid() const;

        void bind();

        void unbind();

        void destroy();

    private:
        u32 m_id;

        std::unordered_map<std::string, u32> m_uniformMap;

        static gShader* m_instance;

        static void checkerrors(u32 shader, bool compile);
    };
}
#pragma once

#include "gCommon.h"

#include <memory>

enum class UniformType {
    NONE,
    VEC2,
    VEC3,
    VEC4,
    MAT3,
    MAT4,
    INT
};

class Uniform {
public:
    Uniform(const std::string &name, UniformType type, grm::u32 count, const void *data);
    ~Uniform();

    bool isValid() const;

    const void *getData() const;

    void setData(const void *data);

    UniformType getType() const;

    grm::size getSize() const;

private:
    std::string m_name;
    
    UniformType m_type;

    grm::size m_size;

    void *m_data;
};

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

        template <typename T>
        static bool Register(const std::string& name, UniformType type, grm::u32 count, const T *data);

        template <typename T>
        static bool Register(const std::string& name, UniformType type, grm::u32 count, T data);

        static bool SetUniform(const std::string &name, const void *data);

        template <typename T>
        static bool SetUniform(const std::string &name, const T *data);

        template <typename T>
        static bool SetUniform(const std::string &name, T data);

        static void Flush();

        bool isValid() const;

    private:
        static std::unordered_map<std::string, std::unique_ptr<Uniform>> m_uniformMap;

        static gShader *m_current;

        grm::u32 m_id;

        bool m_bValid;


        static bool registry(const std::string& name, UniformType type, grm::u32 count, const void *data);

        static bool checkerrors(grm::u32 shader, bool compile);
    };

    template <typename T>
    inline bool gShader::Register(const std::string &name, UniformType type, grm::u32 count, const T *data) {
        return registry(name, type, count, reinterpret_cast<const void *>(data));
    }

    template <typename T>
    inline bool gShader::Register(const std::string &name, UniformType type, grm::u32 count, T data) {
        return registry(name, type, count, reinterpret_cast<const void *>(&data));
    }

    template <typename T>
    inline bool gShader::SetUniform(const std::string &name, const T *data) {
        return gShader::SetUniform(name, reinterpret_cast<const void *>(data));
    }

    template <typename T>
    inline bool gShader::SetUniform(const std::string &name, T data) {
        return gShader::SetUniform(name, reinterpret_cast<const void *>(&data));
    }
}
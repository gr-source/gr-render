#pragma once

#include <cstdint>
#include <string>

namespace gr
{
    enum class shader_data_type
    {
        Float, Float2, Float3, Float4
    };

    inline static uint32_t shader_data_type_size(shader_data_type type)
    {
        switch (type)
        {
            case shader_data_type::Float:
                return 4;
            case shader_data_type::Float2:
                return 4 * 2;
            case shader_data_type::Float3:
                return 4 * 3;
            case shader_data_type::Float4:
                return 4 * 4;
        }
        return 0;
    }

    struct buffer_element
    {
        std::string name;
        shader_data_type type;
        uint32_t size;
        size_t offset;
        bool normalized;
        bool instanced;

        buffer_element(shader_data_type type, const std::string& name, bool instanced = false, bool normalized = false)
            : name(name), type(type), size(shader_data_type_size(type)), offset(0), instanced(instanced), normalized(normalized) {}

        inline uint32_t get_component_count() const
        {
            switch (type) {
                case shader_data_type::Float:
                    return 1;
                case shader_data_type::Float2:
                    return 2;
                case shader_data_type::Float3:
                    return 3;
                case shader_data_type::Float4:
                    return 4;
            }
            return 0;
        }
    };
}

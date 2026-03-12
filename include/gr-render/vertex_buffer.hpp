#pragma once

#include <cstdint>
#include <memory>

#include "buffer_layout.hpp"

namespace gr
{
    enum class buffer_usage { static_draw, dynamic_draw };

    class vertex_buffer
    {
    public:
        static std::shared_ptr<vertex_buffer> create(const void* data, uint32_t size, buffer_usage usage);

        virtual ~vertex_buffer() {}

        virtual void Bind() = 0;

        virtual void Unbind() = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;

        inline void SetLayout(const buffer_layout& layout)
        {
            m_layout = layout;
        }

        inline const buffer_layout& GetLayout() const
        {
            return m_layout;
        }

    private:
        buffer_layout m_layout;

    protected:
        buffer_usage m_usage;
    };
}

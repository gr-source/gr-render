#pragma once

#include <cstdint>
#include <memory>

namespace gr
{
    class index_buffer
    {
    public:
        static std::shared_ptr<index_buffer> create(const void* data, uint32_t size);

        virtual ~index_buffer() {}

        virtual void Bind() = 0;

        virtual void Unbind() = 0;
    };
}

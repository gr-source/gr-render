#pragma once

#include <cstddef>
#include <cstdint>

namespace engine::renderer
{
class UniformBuffer
{
public:
    UniformBuffer(size_t size);
    ~UniformBuffer();

    void attach(uint32_t binding);

    void detach();

    template <typename T>
    void Update(const T& data)
    {
        update((const void*)&data, sizeof(T));
    }

private:
    uint32_t m_handle;

    void update(const void* buffer, size_t size);
};
}



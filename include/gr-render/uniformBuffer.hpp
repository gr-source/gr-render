#pragma once

#include <cstddef>
#include <cstdint>

namespace engine::renderer
{
class UniformBuffer
{
public:
    UniformBuffer();
    ~UniformBuffer();

    void Create(uint32_t binding, size_t size);

    void Destroy();

    template <typename T>
    void Update(const T& data)
    {
        update((const void*)&data, sizeof(T));
    }

private:
    uint32_t m_handle;

    uint32_t m_binding;

    size_t m_size;

    void update(const void* buffer, size_t size);
};
}



#pragma once

#include "gCommon.h"

#include <array>

#define GR_MAX_BLOCK_BUFFER 1024

namespace grr
{
    class gVertexArray
    {
    public:
        gVertexArray();
        ~gVertexArray();
        
        static BufferID CreateBuffer(BufferType_ target, const void *data = nullptr, std::size_t size = 0);

        static void DeleteBuffer(BufferID index);

        static void Bind(u32 index);

        static void SetAttrib(u8 index, u16 size, u16 stride, const void* pointer);

        static void SetAttribI(u8 index, u16 size, u16 stride, const void* pointer);

        static void SetAttribDivisor(u8 index, u8 divisor);

        static void UpdateResizeBuffer(u32 size, BufferUsage usage);

        static void SetBufferUpdate(u32 offset, u32 size, const void* data);

        static void DrawElements(PrimitiveType primitive, u32 count, const void* indices);

        static void DrawElementsInstanced(PrimitiveType primitive, u32 count, const void* indices, u32 primcount);

        static void DrawArrays(PrimitiveType primitive, u32 count);

        static void DrawArraysInstanced(PrimitiveType primitive, u32 count, u32 primcount);

        void bind();

        void unbind();

        bool is_valid() const;

        const VertexID &getID() const;

    private:
        static gVertexArray *m_instance;

        static u32 s_currentBuffer;

        static std::array<std::uint32_t, 5> bufferMappings;

        static std::array<u32, 7> primitiveMappings;

        static std::unordered_map<BufferID, u32> m_bufferIndex;

        VertexID vertexID;
    };
} // namespace grr

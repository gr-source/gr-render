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

        static void Bind(grm::uint32 index);

        static void SetAttrib(grm::uint8 index, grm::uint16 size, grm::uint16 stride, const void* pointer);

        static void SetAttribI(grm::uint8 index, grm::uint16 size, grm::uint16 stride, const void* pointer);

        static void SetAttribDivisor(grm::uint8 index, grm::uint8 divisor);

        static void UpdateResizeBuffer(grm::uint32 size, BufferUsage usage);

        static void SetBufferUpdate(grm::uint32 offset, grm::uint32 size, const void* data);

        static void DrawElements(PrimitiveType primitive, grm::uint32 count, const void* indices);

        static void DrawElementsInstanced(PrimitiveType primitive, grm::uint32 count, const void* indices, grm::uint32 primcount);

        static void DrawArrays(PrimitiveType primitive, grm::uint32 count);

        static void DrawArraysInstanced(PrimitiveType primitive, grm::uint32 count, grm::uint32 primcount);

        void bind();

        void unbind();

        bool is_valid() const;

        const VertexID &getID() const;

    private:
        static gVertexArray *m_instance;

        static grm::uint32 s_currentBuffer;

        static std::array<std::uint32_t, 5> bufferMappings;

        static std::array<grm::uint32, 7> primitiveMappings;

        static std::unordered_map<BufferID, grm::uint32> m_bufferIndex;

        VertexID vertexID;
    };
} // namespace grr

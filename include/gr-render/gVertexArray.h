#pragma once

#include "gCommon.h"

#include <array>

#define GR_MAX_BLOCK_BUFFER 1024

namespace grr {
    class gVertexArray {
    public:
        gVertexArray();
        ~gVertexArray();
        
        static gVertexArray *Create();

        static BufferID CreateBuffer(BufferType_ target, const void *data = nullptr, std::size_t size = 0);

        static void DeleteBuffer(BufferID index);

        static void Bind(grm::u32 index);

        static void SetAttrib(grm::u8 index, grm::u16 size, grm::u16 stride, const void* pointer);

        static void SetAttribI(grm::u8 index, grm::u16 size, grm::u16 stride, const void* pointer);

        static void SetAttribDivisor(grm::u8 index, grm::u8 divisor);

        static void UpdateResizeBuffer(grm::u32 size, BufferUsage usage);

        static void SetBufferUpdate(grm::u32 offset, grm::u32 size, const void* data);

        static void DrawElements(PrimitiveType primitive, grm::u32 count, const void* indices);

        static void DrawElementsInstanced(PrimitiveType primitive, grm::u32 count, const void* indices, grm::u32 primcount);

        static void DrawArrays(PrimitiveType primitive, grm::u32 count);

        static void DrawArraysInstanced(PrimitiveType primitive, grm::u32 count, grm::u32 primcount);

        void bind();

        void unbind();

        bool isValid() const;

        grm::u32 getID() const;

        static void Destroy(gVertexArray *target);
        
    private:
        static gVertexArray *m_instance;

        static grm::u32 s_currentBuffer;

        static std::array<std::uint32_t, 5> bufferMappings;

        static std::array<grm::u32, 7> primitiveMappings;

        static std::unordered_map<grm::u32, grm::u32> m_bufferIndex;

        grm::u32 m_vao;
    };
} // namespace grr

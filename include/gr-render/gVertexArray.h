#pragma once

#include "gCommon.h"

#define GR_MAX_BLOCK_BUFFER 1024

namespace grr {
    class gVertexArray {
    public:
        gVertexArray();
        
        static gVertexArray* Create();

        static u32 CreateBuffer(BufferType target);

        static void DeleteBuffer(u32 index);

        static void Bind(u32 index);

        static void SetAttrib(u8 index, u16 size, u16 stride, const void* pointer);
        static void SetAttribI(u8 index, u16 size, u16 stride, const void* pointer);

        static void UpdateResize(u32 size);

        static void SetBufferUpdate(u32 offset, u32 size, const void* data);

        static void DrawElementsInstanced(PrimitiveType primitive, u32 count, const void* indices, u32 primcount);

        static void DrawElements(PrimitiveType primitive, u32 count, const void* indices);

        static void DrawArrays(PrimitiveType primitive, u32 count);

        void bind();

        void unbind();

        void destroy();
    private:
        u32 m_vao;

        static gVertexArray* m_instance;

        static u32 s_currentBuffer;

        static std::unordered_map<BufferType, u32> m_bufferTypeMap;

        static std::unordered_map<PrimitiveType, u32> m_primitiveMap;

        static std::unordered_map<u32, u32> m_bufferIndex;
    };
} // namespace grr

#pragma once

#include "gCommon.h"

#define GR_MAX_BLOCK_BUFFER 1024

namespace grr {
    class gVertexArray {
    public:
        gVertexArray();
        
        static gVertexArray* Create();

        static grm::u32 CreateBuffer(BufferType target);

        static void DeleteBuffer(grm::u32 index);

        static void Bind(grm::u32 index);

        static void SetAttrib(grm::u8 index, grm::u16 size, grm::u16 stride, const void* pointer);

        static void SetAttribI(grm::u8 index, grm::u16 size, grm::u16 stride, const void* pointer);

        static void SetAttribDivisor(grm::u8 index, grm::u8 divisor);

        static void UpdateResizeBuffer(grm::u32 size, grm::u32 flags);

        static void SetBufferUpdate(grm::u32 offset, grm::u32 size, const void* data);

        static void DrawElements(PrimitiveType primitive, grm::u32 count, const void* indices);

        static void DrawElementsInstanced(PrimitiveType primitive, grm::u32 count, const void* indices, grm::u32 primcount);

        static void DrawArrays(PrimitiveType primitive, grm::u32 count);

        static void DrawArraysInstanced(PrimitiveType primitive, grm::u32 count, grm::u32 primcount);

        void bind();

        void unbind();

        void destroy();

        bool isValid() const;

        grm::u32 getID() const;

        static void Release();
        
    private:
        grm::u32 m_vao;

        static gVertexArray* m_instance;

        static grm::u32 s_currentBuffer;

        static std::unordered_map<BufferType, grm::u32> m_bufferTypeMap;

        static std::unordered_map<PrimitiveType, grm::u32> m_primitiveMap;

        static std::unordered_map<grm::u32, grm::u32> m_bufferIndex;
    };
} // namespace grr

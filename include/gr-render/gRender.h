#pragma once

#include "gColor.h"

#include "gFramebuffer.h"
#include "gVertexArray.h"
#include "gTexture.h"
#include "gShader.h"
#include "gError.h"

#include <sstream>
#include <iomanip>
#include <string>

#define MAX_BLOCK_BUFFER 1024

namespace grr {
    class gRender {
    public:

        static void SetRenderState(RenderState state, void* value);

        static void SetRenderState(RenderState state, u32 value);

        static const std::string getRenderStateName(RenderState state);

        static const bool OpenGLInit();

        static void SetResolution(u32 width, u32 height);

        static void Render2D();

        static void RenderTexture2D(const Matrix3x3& model, gTexture* texture);

        static void RenderPrimitive2D(PrimitiveType primitive, u32 numVertice, void* vertice);
        
        static void RenderPrimitive2D(PrimitiveType primitive, u32 numVertice, void* vertice, u32 numIndice, void* indice);

        static void RenderPrimitive2D(PrimitiveType primitive, u32 numVertice, void* vertice, u32 numIndice, void* indice, u32 numPrim, void *prim);

        static void Render3D(const Matrix4x4& projection, const Matrix4x4& model);

        static void RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void* vertice);

        static void RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void* vertice, u32 numIndice, void* indice);

        static void RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void* vertice, u32 numIndice, void* indice, u32 numPrim, void *prim);

        static void OpenGLShutdown();
    private:
        static std::unordered_map<BufferBindingTarget, u32> m_bufferMap;

        static std::unordered_map<PrimitiveType, u32> m_primitiveMap;

        static std::unordered_map<u32, u32> m_renderStateMap;

        static gShader* m_shader2D;
        static gShader* m_shader3D;
        static gShader* m_shaderInstanced3D;

        static gVertexArray* m_vertexArray2D;
        static gVertexArray* m_vertexArray3D;

        static u32 m_height;
        static u32 m_width;

        static u32 buffers2D[3];

        static u32 buffers3D[3];

        static void CreateVertexArray2D();

        static void CreateVertexArray3D();

    };
}
#include "gRender.h"

#include "gShaderSource.h"
#include "gVertexArray.h"
#include "gTexture.h"
#include "gShader.h"
#include "gl.h" 

#include "gColor.h"

#define VBO_INDEX 0
#define EBO_INDEX 1
#define PBO_INDEX 2

namespace grr {
    std::unordered_map<BufferBindingTarget, u32> gRender::m_bufferMap {
        {BufferBindingTarget::GR_ARRAY_BUFFER, GL_ARRAY_BUFFER},
        {BufferBindingTarget::GR_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER}
    };

    std::unordered_map<u32, u32> gRender::m_renderStateMap = {
        {GR_DEPTH_ALWAYS, GL_ALWAYS},
        {GR_DEPTH_NEVER, GL_NEVER},
        {GR_DEPTH_LESS, GL_LESS},
        {GR_DEPTH_EQUAL, GL_EQUAL},
        {GR_DEPTH_LEQUAL, GL_LEQUAL},
        {GR_DEPTH_GREATER, GL_GREATER},
        {GR_DEPTH_NOTEQUAL, GL_NOTEQUAL},
        {GR_FALSE, GL_FALSE},
        {GR_TRUE, GL_TRUE},
        {GR_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
    };

    gVertexArray* gRender::m_vertexArray2D = nullptr;
    gVertexArray* gRender::m_vertexArray3D = nullptr;

    gShader* gRender::m_shader2D = nullptr;
    gShader* gRender::m_shader3D = nullptr;
    gShader* gRender::m_shaderInstanced3D = nullptr;

    u32 gRender::m_height = 0;
    u32 gRender::m_width = 0;

    u32 gRender::buffers2D[3]{0, 0, 0};
    u32 gRender::buffers3D[3]{0, 0, 0};

    void gRender::SetRenderState(RenderState state, void *value) {
        switch (state) {
        case GR_BACKGROUND_COLOR: {
            gColor* color = (gColor*)value;
            GL_CALL(glClearColor(color->r, color->g, color->b, color->a));
            break;
        }
        case GR_VIEWPORT: {
            iVector2* size = (iVector2*)value;
            GL_CALL(glViewport(0, 0, size->x, size->y));
            break;
        }
        default:
            std::cout << "Invalid V: " << getRenderStateName(state) << std::endl;
            break;
        }
    }

    void gRender::SetRenderState(RenderState state, u32 value) {
        switch (state) {
        case GR_BACKGROUND: {
            GLbitfield filter = 0;
            if ((value & GR_DEPTH_BUFFER) == GR_DEPTH_BUFFER) {
                filter |= GL_DEPTH_BUFFER_BIT;
            }
            if ((value & GR_COLOR_BUFFER) == GR_COLOR_BUFFER) {
                filter |= GL_COLOR_BUFFER_BIT;
            }
            return GL_CALL(glClear(filter));
        }
        case GR_CULL_FACE: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_CULL_FACE));
            }
            return GL_CALL(glDisable(GL_CULL_FACE));
        }
        case GR_CULL: {
            if ((value & GR_FRONT) == GR_FRONT && (value & GR_BACK) == GR_BACK) {
                return GL_CALL(glCullFace(GL_FRONT_AND_BACK));
            } else if ((value & GR_BACK) == GR_BACK) {
                return GL_CALL(glCullFace(GL_BACK));
            }
            return GL_CALL(glCullFace(GL_FRONT));
        }
        case GR_DEPTH: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_DEPTH_TEST));
            }
            return GL_CALL(glDisable(GL_DEPTH_TEST));
        }
        case GR_DEPTH_MASK: {
            GL_CALL(glDepthMask(m_renderStateMap[value]));
            break;
        }
        case GR_DEPTH_FUNC: {
            return GL_CALL(glDepthFunc(m_renderStateMap[value]));
        }
        case GR_MULTISAMPLE: {
            if (value == GR_TRUE) {
                return glEnable(GL_MULTISAMPLE);
            }
            return glDisable(GL_MULTISAMPLE);
        }
        case GR_FRAMEBUFFER_SRGB: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_FRAMEBUFFER_SRGB));
            }
            return GL_CALL(glDisable(GL_FRAMEBUFFER_SRGB));
        }
        case GR_BLEND: {
            if (value == GR_TRUE) {
                return GL_CALL(glEnable(GL_BLEND));
            }
            return GL_CALL(glDisable(GL_BLEND));
        }
        case GR_SRC_ALPHA: {
            glBlendFunc(GL_SRC_ALPHA, m_renderStateMap[value]);
            break;
        }
        default:
            std::cout << "Invalid Va: " << getRenderStateName(state) << std::endl;
        }
    }

    const std::string gRender::getRenderStateName(RenderState state) {
        #define GET_ENUM_NAME(e) case e: return #e
        switch (state) {
        GET_ENUM_NAME(GR_FALSE);
        GET_ENUM_NAME(GR_TRUE);
        GET_ENUM_NAME(GR_BACKGROUND_COLOR);
        GET_ENUM_NAME(GR_BACKGROUND);
        GET_ENUM_NAME(GR_DEPTH_BUFFER);
        GET_ENUM_NAME(GR_COLOR_BUFFER);
        GET_ENUM_NAME(GR_CULL_FACE);
        GET_ENUM_NAME(GR_CULL);
        GET_ENUM_NAME(GR_FRONT);
        GET_ENUM_NAME(GR_BACK);
        GET_ENUM_NAME(GR_DEPTH);
        GET_ENUM_NAME(GR_VIEWPORT);
        default:
            return "undefined";
        }
        #undef GET_ENUM_NAME
    }

    const bool gRender::OpenGLInit() {
        #if !GR_OPENGLES3
        if (glewInit() != GLEW_OK) {
            return false;
        }
        #endif

        #if GR_OPENGLES3
        const std::string glVersion = "#version 300 es";
        #else
        const std::string glVersion = "#version 330 core";
        #endif

        CreateVertexArray2D();
        // m_shader2D = gShader::Create({(glVersion + fragmentShader2D).c_str()}, {(glVersion + vertexShader2D).c_str()});

        CreateVertexArray3D();
        // m_shader3D = gShader::Create({(glVersion + fragmentShader3D).c_str()}, {(glVersion + vertexShader3D).c_str()});

        // m_shaderInstanced3D = gShader::Create({(glVersion + fragmentShader3D).c_str()}, {(glVersion + vertexShaderInstance3D).c_str()});

        return true;
    }

    void gRender::SetResolution(u32 width, u32 height) {
        m_height = height;
        m_width = width;
    }

    void gRender::Render2D() {
        Matrix4x4 projection = Math::orthographic(0.0f, m_width, 0.0f, m_height, -1.0f, 1.0f);
        
        m_shader2D->bind();
        gShader::SetUniform("projection", 1, projection);
    }

    void gRender::RenderTexture2D(const Matrix3x3 &model, gTexture *texture) {
    }

    void gRender::RenderPrimitive2D(PrimitiveType primitive, u32 numVertice, void *vertice) {
        m_vertexArray2D->bind();

        gVertexArray::Bind(buffers2D[VBO_INDEX]);
        gVertexArray::UpdateResize(numVertice * sizeof(gVertex2D));
        gVertexArray::SetBufferUpdate(0, numVertice * sizeof(gVertex2D), vertice);

        gVertexArray::DrawArrays(primitive, numVertice);
    }

    void gRender::RenderPrimitive2D(PrimitiveType primitive, u32 numVertice, void *vertice, u32 numIndice, void *indice) {
        m_vertexArray2D->bind();

        gVertexArray::Bind(buffers2D[EBO_INDEX]);
        gVertexArray::UpdateResize(numIndice * sizeof(u32));
        gVertexArray::SetBufferUpdate(0, numIndice * sizeof(u32), indice);

        gVertexArray::Bind(buffers2D[VBO_INDEX]);
        gVertexArray::UpdateResize(numVertice * sizeof(gVertex2D));
        gVertexArray::SetBufferUpdate(0, numVertice * sizeof(gVertex2D), vertice);

        gVertexArray::DrawElements(primitive, numIndice, nullptr);
    }

    void gRender::RenderPrimitive2D(PrimitiveType primitive, u32 numVertice, void *vertice, u32 numIndice, void *indice, u32 numPrim, void *prim) {
        m_vertexArray2D->bind();

        gVertexArray::Bind(buffers2D[EBO_INDEX]);
        gVertexArray::UpdateResize(numIndice * sizeof(u32));
        gVertexArray::SetBufferUpdate(0, numIndice * sizeof(u32), indice);

        gVertexArray::Bind(buffers2D[VBO_INDEX]);
        gVertexArray::UpdateResize(numVertice * sizeof(gVertex2D));
        gVertexArray::SetBufferUpdate(0, numVertice * sizeof(gVertex2D), vertice);

        gVertexArray::Bind(buffers2D[PBO_INDEX]);
        gVertexArray::UpdateResize(numPrim * sizeof(Matrix4x4));
        gVertexArray::SetBufferUpdate(0, numPrim * sizeof(Matrix4x4), prim);

        gVertexArray::DrawElements(primitive, numIndice, nullptr);
    }

    void gRender::Render3D(const Matrix4x4& projection, const Matrix4x4& view) {
        m_shaderInstanced3D->bind();
        gShader::SetUniform("projection", 1, projection);
        gShader::SetUniform("view", 1, view);
    }

    void gRender::RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void *vertice) {
        m_vertexArray3D->bind();

        gVertexArray::Bind(buffers3D[VBO_INDEX]);
        gVertexArray::UpdateResize(numVertice * sizeof(gVertex3D));
        gVertexArray::SetBufferUpdate(0, numVertice * sizeof(gVertex3D), vertice);

        gVertexArray::DrawArrays(primitive, numVertice);
    }

    void gRender::RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void *vertice, u32 numIndice, void *indice) {
        m_vertexArray3D->bind();

        gVertexArray::Bind(buffers3D[EBO_INDEX]);
        gVertexArray::UpdateResize(numIndice * sizeof(u32));
        gVertexArray::SetBufferUpdate(0, numIndice * sizeof(u32), indice);

        gVertexArray::Bind(buffers3D[VBO_INDEX]);
        gVertexArray::UpdateResize(numVertice * sizeof(gVertex3D));
        gVertexArray::SetBufferUpdate(0, numVertice * sizeof(gVertex3D), vertice);

        gVertexArray::DrawElements(primitive, numIndice, nullptr);
    }

    void gRender::RenderPrimitive3D(PrimitiveType primitive, u32 numVertice, void *vertice, u32 numIndice, void *indice, u32 numPrim, void *prim) {
        m_vertexArray3D->bind();

        gVertexArray::Bind(buffers3D[EBO_INDEX]);
        gVertexArray::UpdateResize(numIndice * sizeof(u32));
        gVertexArray::SetBufferUpdate(0, numIndice * sizeof(u32), indice);

        gVertexArray::Bind(buffers3D[VBO_INDEX]);
        gVertexArray::UpdateResize(numVertice * sizeof(gVertex3D));
        gVertexArray::SetBufferUpdate(0, numVertice * sizeof(gVertex3D), vertice);

        gVertexArray::Bind(buffers3D[PBO_INDEX]);
        gVertexArray::UpdateResize(numPrim * sizeof(Matrix4x4));
        gVertexArray::SetBufferUpdate(0, numPrim * sizeof(Matrix4x4), prim);

        gVertexArray::DrawElementsInstanced(PrimitiveType_Triangles, numIndice, nullptr, numPrim);
    }

    void gRender::CreateVertexArray2D() {
        m_vertexArray2D = gVertexArray::Create();
        if (!m_vertexArray2D) {
            GR_ASSERT("falied to create vertex array 2D.");
        }

        buffers2D[EBO_INDEX] = gVertexArray::CreateBuffer(BufferType_EBO);
        if (!buffers2D[EBO_INDEX]) {
            GR_ASSERT("Created buffer falied.");
        }

        buffers2D[VBO_INDEX] = gVertexArray::CreateBuffer(BufferType_VBO);
        if (!buffers2D[VBO_INDEX]) {
            GR_ASSERT("Created buffer falied.");
        }

        buffers2D[PBO_INDEX] = gVertexArray::CreateBuffer(BufferType_PBO);
        if (!buffers2D[PBO_INDEX]) {
            GR_ASSERT("Created buffer falied.");
        }

        m_vertexArray2D->bind();

        m_vertexArray2D->unbind();
    }

    void gRender::CreateVertexArray3D() {
        // Create Buffer
        m_vertexArray3D = gVertexArray::Create();
        if (!m_vertexArray3D) {
            GR_ASSERT("Falied to create vao buffer");
        }

        buffers3D[EBO_INDEX] = gVertexArray::CreateBuffer(BufferType_EBO);
        if (!buffers3D[EBO_INDEX]) {
            GR_ASSERT("Falied to create ebo buffer.");
        }

        buffers3D[VBO_INDEX] = gVertexArray::CreateBuffer(BufferType_VBO);
        if (!buffers3D[VBO_INDEX]) {
            GR_ASSERT("Falied to create vbo buffer.");
        }

        buffers3D[PBO_INDEX] = gVertexArray::CreateBuffer(BufferType_PBO);
        if (!buffers3D[PBO_INDEX]) {
            GR_ASSERT("Falied to create prim buffer.");
        }

        // Config buffer
        m_vertexArray3D->bind();

        gVertexArray::Bind(buffers3D[EBO_INDEX]);

        gVertexArray::Bind(buffers3D[VBO_INDEX]);
        gVertexArray::SetAttrib(0, 3, sizeof(gVertex3D), reinterpret_cast<void*>(offsetof(gVertex3D, position)));

        gVertexArray::Bind(buffers3D[PBO_INDEX]);

        for (int i = 0; i < 4; ++i) {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), (GLvoid*)(sizeof(Vector4) * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        m_vertexArray3D->unbind();
    }

    void gRender::OpenGLShutdown() {
        if (m_shader2D) {
            delete m_shader2D;
        }
        if (m_shader3D) {
            delete m_shader3D;
        }

        if (m_vertexArray2D) {
            m_vertexArray2D->destroy();
        }
        if (m_vertexArray3D) {
            m_vertexArray3D->destroy();
        }
        
        GL_CALL(glDeleteBuffers(3, buffers2D));
        GL_CALL(glDeleteBuffers(3, buffers3D));
    }
}

#undef VBO_INDEX
#undef EBO_INDEX
#undef PBO_INDEX



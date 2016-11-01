#pragma once

#include "renderer/Geometry.h"

#ifdef _OPENGL_RENDER_
#include "renderer/DataTypes.h"
#include "renderer/RenderTechnique.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for geometry management. GL render only.
    */
    class CGeometryGL final : public CGeometry
    {
    public:

        CGeometryGL(const CRenderTechnique* technique);
        ~CGeometryGL();
        
        void                init()      override;
        void                free()      override;
        void                draw()      override;
        void                refresh()   override;

    private:

        s32                 computeVertexSize(const ShaderDataList& shaderDataList);
        void                initBufferData(const ShaderDataList& shaderDataList);

        u32                 m_arrayId;
        u32                 m_instanceAmount;

        bool                m_initialized;

    private:

        static void         genVertexArray(u32& buffer);
        static void         bindVertexArray(u32 buffer);
        static void         deleteVertexArray(u32& buffer);

        static void         initVertexAttribPointer(s32 attrib, EDataType type, u32 count, u32 size = 0, u32 offset = 0);
        static void         vertexAttribArray(s32 attrib, bool enable);
        static void         vertexAttribDivisior(s32 attrib, u32 value);

        static void         drawElements(EPrimitivesTopology mode, u32 count, u32 primCount);
        static void         drawArrays(EPrimitivesTopology mode, u32 first, u32 count, u32 primCount);

        static u32          s_currentArray;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_

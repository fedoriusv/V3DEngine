#ifndef _V3D_GEOMETRY_GL_H_
#define _V3D_GEOMETRY_GL_H_

#include "renderer/Geometry.h"
#include "renderer/DataTypes.h"

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
    class CGeometryGL : public CGeometry
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

        bool                m_initialized;

    private:

        static void         genVertexArray(u32& buffer);
        static void         bindVertexArray(u32 buffer);
        static void         deleteVertexArray(u32& buffer);

        static void         initVertexAttribPointer(s32 attrib, EDataType type, u32 count, u32 size = 0, u32 offset = 0);
        static void         vertexAttribArray(s32 attrib, bool enable);
        static void         vertexAttribDivisior(s32 attrib, u32 value);

        static void         drawElements(EPrimitivesMode mode, u32 count, u32 primCount);
        static void         drawArrays(EPrimitivesMode mode, u32 first, u32 count, u32 primCount);

        static u32          s_currentArray;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_GL_H_

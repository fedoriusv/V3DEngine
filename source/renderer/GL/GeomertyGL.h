#ifndef _V3D_GEOMETRY_GL__H_
#define _V3D_GEOMETRY_GL_H_

#include "renderer/Geometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugDrawGL;

    class CGeometryGL : public CGeometry
    {
    public:

        CGeometryGL(const RenderTechniquePtr& technique);
        virtual             ~CGeometryGL();
        
        void                init()    override;
        void                draw()    override;
        void                free()    override;
        void                refresh() override;

    private:

        friend              CDebugDrawGL;

        v3d::u32            m_arrayId;

        static void         genBuffers(v3d::u32& buffer);
        static void         bindBuffers(const v3d::u32 target, const v3d::u32 buffer);
        static void         deleteBuffers(v3d::u32& buffer);

        static void         bufferData(const v3d::u32 target, const v3d::u32 size, void* data);
        static void         bufferSubData(const v3d::u32 target, const v3d::u32 offset, const v3d::u32 size, void* data);

        static void*        mapBuffer(const v3d::u32 target, const v3d::u32 access);
        static bool         unmapBuffer(const v3d::u32 target);
        static void*        mapBufferRange(const v3d::u32 target, const v3d::u32 offset, const v3d::u32 size, const v3d::u32 flags);

        static void         getBufferPointer(const v3d::u32 target, const v3d::u32 pname, void** params);

        static void         genVertexArray(v3d::u32& buffer);
        static void         bindVertexArray(const v3d::u32 buffer);
        static void         deleteVertexArray(v3d::u32& buffer);

        static void         initVertexAttribPointer(const v3d::u32 vertexAttrib, const v3d::u32 size);

        static void         drawElements(const u32 mode, const u32 count);
        static void         drawArrays(const u32 mode, const u32 first, const u32 count);

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_GEOMETRY_GL_H_

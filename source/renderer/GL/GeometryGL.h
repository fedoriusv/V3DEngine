#ifndef _V3D_GEOMETRY_GL__H_
#define _V3D_GEOMETRY_GL_H_

#include "renderer/Geometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugGeometryGL;
    class CDebugLightGL;

    class CGeometryGL : public CGeometry
    {
    public:

        CGeometryGL(const RenderTechniquePtr& technique);
        ~CGeometryGL();
        
        void                init()    override;
        void                draw()    override;
        void                free()    override;
        void                refresh() override;

    private:

        friend              CDebugGeometryGL;
        friend              CDebugLightGL;

        v3d::u32            m_arrayId;

        s32                 computeVertexSize(const AttributeList& attributes);
        void                initBufferData(const AttributeList& attributes);

        static void         genBuffers(u32& buffer);
        static void         bindBuffers(const u32 target, const u32 buffer);
        static void         deleteBuffers(u32& buffer);

        static void         bufferData(const u32 target, const u32 size, void* data);
        static void         bufferSubData(const u32 target, const u32 offset, const u32 size, void* data);

        static void*        mapBuffer(const u32 target, const u32 access);
        static bool         unmapBuffer(const u32 target);
        static void*        mapBufferRange(const u32 target, const u32 offset, const u32 size, const u32 flags);

        static void         getBufferPointer(const u32 target, const u32 pname, void** params);

        static void         genVertexArray(u32& buffer);
        static void         bindVertexArray(const u32 buffer);
        static void         deleteVertexArray(u32& buffer);

        static void         initVertexAttribPointer(const v3d::u32 vertexAttrib, const u32 count, bool normalized = false, const u32 size = 0, const u32 offset = 0);

        static void         drawElements(const u32 mode, const u32 count);
        static void         drawArrays(const u32 mode, const u32 first, const u32 count);

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_GEOMETRY_GL_H_

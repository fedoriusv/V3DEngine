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
        static void         bindBuffers(EGeometryTarget target, u32 buffer);
        static void         deleteBuffers(u32& buffer);

        static void         bufferData(EGeometryTarget target, u32 size, void* data);
        static void         bufferSubData(EGeometryTarget target, u32 offset, u32 size, void* data);

        static void*        mapBuffer(EGeometryTarget target, u32 access);
        static bool         unmapBuffer(EGeometryTarget target);
        static void*        mapBufferRange(EGeometryTarget target, u32 offset, u32 size, u32 flags);

        static void         getBufferPointer(EGeometryTarget target, u32 pname, void** params);

        static void         genVertexArray(u32& buffer);
        static void         bindVertexArray(u32 buffer);
        static void         deleteVertexArray(u32& buffer);

        static void         initVertexAttribPointer(u32 vertexAttrib, u32 count, bool normalized = false, u32 size = 0, u32 offset = 0);

        static void         drawElements(EDrawMode mode, u32 count);
        static void         drawElementsInstanced(EDrawMode mode, u32 count, u32 primCount);
        static void         drawArrays(EDrawMode mode, u32 first, u32 count);
        static void         drawArraysInstanced(EDrawMode mode, u32 first, u32 count, u32 primCount);

        static u32          s_currentBuffer[eTargetCount];
        static u32          s_currentArray;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_GEOMETRY_GL_H_

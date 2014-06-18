#ifndef _V3D_GEOMETRY_GL__H_
#define _V3D_GEOMETRY_GL_H_

#include "renderer/Geometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CGeometryGL : public CGeometry
    {
    public:

        CGeometryGL(const RenderTechniquePtr& technique);
        virtual      ~CGeometryGL();
        
        void         init()    override;
        void         draw()    override;
        void         free()    override;
        void         refresh() override;

    private:

        void         initAttribute(u32 pass);

        v3d::u32     m_arrayId;

        void         genBuffers(v3d::u32& buffer);
        void         bindBuffers(const v3d::u32 target, const v3d::u32 buffer);
        void         deleteBuffers(v3d::u32& buffer);

        void         bufferData(const v3d::u32 target, const v3d::u32 size, void* data);
        void         bufferSubData(const v3d::u32 target, const v3d::u32 offset, const v3d::u32 size, void* data);

        void*        mapBuffer(const v3d::u32 target, const v3d::u32 access);
        bool         unmapBuffer(const v3d::u32 target);
        void*        mapBufferRange(const v3d::u32 target, const v3d::u32 offset, const v3d::u32 size, const v3d::u32 flags);

        void         getBufferPointer(const v3d::u32 target, const v3d::u32 pname, void** params);

        void         genVertexArray(v3d::u32& buffer);
        void         bindVertexArray(const v3d::u32 buffer);
        void         deleteVertexArray(v3d::u32& buffer);

        void         initVertexAttribPointer(const v3d::u32 vertexAttrib, const v3d::u32 size);
        void         refreshBufferData();

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_GEOMETRY_GL_H_

#ifndef _V3D_GEOMETRY_GL_H_
#define _V3D_GEOMETRY_GL_H_

#include "renderer/Geometry.h"
#include "renderer/DataTypes.h"

namespace v3d
{
namespace renderer
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
        u32                 m_verticesId;
        u32                 m_indicesId;

        bool                m_initialized;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_GL_H_

#ifndef _V3D_RENDER_LOD_H_
#define _V3D_RENDER_LOD_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderLOD
    {
    public:

        CRenderLOD();
        ~CRenderLOD();

        void    setGeometryDistance(const f32 lod);
        f32     getGeometryDistance() const;

    private:

        f32     m_geometryLod;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderLOD> RenderLODPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_LOD_H_
#ifndef _V3D_MESH_H_
#define _V3D_MESH_H_

#include "Object.h"
#include "Material.h"
#include "Geometry.h"

namespace v3d
{
namespace scene
{
    class CModel;
}
namespace renderer
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CMesh : public CObject
    {
    public:

        CMesh();
        virtual             ~CMesh();

        void                setMaterial(const MaterialPtr& material);
        const MaterialPtr&  getMaterial() const;

        const GeometryPtr&  getGeomerty() const;

        void                update(f64 time);
        void                render();

    private:

        friend              scene::CModel;

        MaterialPtr         m_material;
        GeometryPtr         m_geometry;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CMesh> MeshPtr;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MESH_H_

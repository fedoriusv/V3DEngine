#ifndef _V3D_MESH_H_
#define _V3D_MESH_H_

#include "Node.h"
#include "renderer/Renderable.h"
#include "stream/Resource.h"
#include "utils/Cloneable.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager;

    class CMesh : public CNode, public renderer::Renderable, public stream::IResource, public utils::TCloneable<CMesh*>
    {
    public:

        CMesh();
        virtual     ~CMesh();

        void        init(const stream::IStreamPtr& stream)  override;
        bool        load()                                  override;

        void        init()                                  override;
        void        render()                                override;
        void        update(s32 dt)                          override;

        CMesh*      clone() const                           override;

    private:

        CMesh(const CMesh&);

        void        loadGeometry(const stream::IStreamPtr& stream);
        void        loadMaterial(const stream::IStreamPtr& stream);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace scene
} //namespace v3d

#endif //_V3D_MESH_H_

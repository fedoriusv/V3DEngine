#ifndef _V3D_MESH_H_
#define _V3D_MESH_H_

#include "Node.h"
#include "renderer/Renderable.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager;

    class CMesh : public CNode, public renderer::CRenderable, public stream::CResource
    {
    public:

        explicit    CMesh();
        virtual     ~CMesh();

        void        init(const stream::IStreamPtr& stream)  override;
        bool        load()                                  override;

        void        init()                                  override;
        void        render()                                override;
        void        update(s32 dt)                          override;

    private:

        void        loadGeometry(const stream::IStreamPtr& stream);
        void        loadMaterial(const stream::IStreamPtr& stream);
        void        build();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
#endif //_V3D_MESH_H_

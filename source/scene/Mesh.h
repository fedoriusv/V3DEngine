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

    class CMesh : public CNode, public renderer::CRenderable
    {
    public:

        explicit CMesh(const std::string& file);
        virtual     ~CMesh();

        void        init()         override;
        void        render()       override;
        void        update(s32 dt) override;

    private:

        void        build();

        std::string m_file;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CMesh> MeshPtr;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
#endif //_V3D_MESH_H_

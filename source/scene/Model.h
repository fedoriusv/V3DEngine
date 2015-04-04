#ifndef _V3D_MODEL_H_
#define _V3D_MODEL_H_

#include "Node.h"
#include "renderer/Mesh.h"
#include "resources/ModelData.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager;

    class CModel : public CNode
    {
    public:

        CModel(const std::string& file, const std::string& techique);
        virtual                         ~CModel();

        void                            render()         override;
        void                            update(s32 time) override;
        void                            init()           override;

        void                            addMesh(const renderer::MeshPtr& mesh);
        const renderer::MeshPtr&        getMesh(u32 id) const;
        u32                             getMeshCount() const;

    private:

        std::vector<renderer::MeshPtr>  m_meshes;
        resources::ModelDataPtr         m_data;

        std::string                     m_file;
        std::string                     m_techique;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CModel> ModelPtr;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_H_

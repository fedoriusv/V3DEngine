#ifndef _V3D_MODEL_H_
#define _V3D_MODEL_H_

#include "Node.h"
#include "stream/Resource.h"
#include "utils/Cloneable.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager;
    class CScene;

    class CModel : public CNode, public stream::IResource, public utils::TCloneable<CModel*>
    {
    public:

        CModel();
        virtual             ~CModel();

        friend              CScene;

        void                init(const stream::IStreamPtr& stream)  override;
        bool                load()                                  override;

        void                init()         override;
        void                update(s32 dt) override;

        bool                setRenderTechniqueForAllMeshes(const std::string& file);

        CModel*             clone() override;

    private:

        CModel(const CModel&);

        const NodeList&     getNodeList() const;

        NodeList            m_nodeList;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
#endif //_V3D_MODEL_H_

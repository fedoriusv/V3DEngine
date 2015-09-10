#ifndef _V3D_MODEL_H_
#define _V3D_MODEL_H_

#include "Node.h"
#include "stream/Resource.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager;

    class CModel : public CNode, public stream::CResource
    {
    public:

        CModel();
        virtual          ~CModel();

        void            init(const stream::IStreamPtr& stream)  override;
        bool            load()                                  override;

        void            init()         override;
        void            update(s32 dt) override;

        bool            setRenderTechniqueForAllMeshes(const std::string& file);

    private:

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
#endif //_V3D_MODEL_H_

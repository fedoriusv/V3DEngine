#ifndef _V3D_MODEL_H_
#define _V3D_MODEL_H_

#include "Node.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager;

    class CModel : public CNode
    {
    public:

        explicit CModel(const std::string& file);
        virtual                 ~CModel();

        void                    init()         override;
        void                    update(s32 dt) override;

    private:

        std::string             m_file;
        std::vector<CNode*>     m_nodesList;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
#endif //_V3D_MODEL_H_

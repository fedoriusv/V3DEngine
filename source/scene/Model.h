#ifndef _V3D_MODEL_H_
#define _V3D_MODEL_H_

#include "Node.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModel : public CNode
    {
    public:

        CModel();
        virtual                   ~CModel();

        void                      render()         override;
        void                      update(f64 time) override;
        void                      init()           override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CModel>         ModelPtr;
    typedef std::map<std::string, ModelPtr> ModelMap;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_H_

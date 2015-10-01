#ifndef _V3D_MODEL_MANAGER_H_
#define _V3D_MODEL_MANAGER_H_

#include "utils/Singleton.h"
#include "Model.h"
#include "stream/ResourceLoader.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager : public utils::TSingleton<CModelManager>, public stream::TResourceLoader<CModel>
    {
    private:

        friend utils::TSingleton<CModelManager>;
        friend CModel;

        CModelManager();
        ~CModelManager();

    public:

        CModel*     load(const std::string& name, const std::string& alias = "") override;

    private:

        u32         m_modelInc;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_MANAGER_H_
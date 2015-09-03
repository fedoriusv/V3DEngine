#ifndef _V3D_MODEL_MANAGER_H_
#define _V3D_MODEL_MANAGER_H_

#include "Singleton.h"
#include "Model.h"
#include "stream/ResourceLoader.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager : public TSingleton<CModelManager>, public stream::TResourceLoader<CModel>
    {
    private:

        friend TSingleton<CModelManager>;

        CModelManager();
        ~CModelManager();

    public:

        const CModel*   load(const std::string& name, const std::string& alias = "") override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_MANAGER_H_
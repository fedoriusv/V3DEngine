#ifndef _V3D_MODEL_MANAGER_H_
#define _V3D_MODEL_MANAGER_H_

#include "Singleton.h"
#include "resources/ModelData.h"
#include "stream/ResourceLoader.h"
#include "decoders/ResourceDecoder.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager : public Singleton<CModelManager>, public stream::TResourceLoader<resources::ModelDataPtr>
    {
    public:

        CModelManager();
        virtual                         ~CModelManager();

        const resources::ModelDataPtr   load(const std::string& name, const std::string& alias = "") override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_MANAGER_H_
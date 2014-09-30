#ifndef _V3D_MODEL_MANAGER_H_
#define _V3D_MODEL_MANAGER_H_

#include "Model.h"
#include "Singleton.h"
#include "resources/ResourceDecoder.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelManager : public Singleton<CModelManager>
    {
    public:

        CModelManager();
        virtual                             ~CModelManager();

        void                                add(const ModelPtr& model);
        ModelPtr                            get(const std::string& name);
        ModelPtr                            load(const std::string& name);

        void                                unload(const std::string& name);
        void                                unload(const ModelPtr& model);

        void                                unloadAll();

        void                                registerDecoder(resources::DecoderPtr decoder);
        void                                unregisterDecoder(resources::DecoderPtr& decoder);

        void                                registerPath(const std::string& path);
        void                                unregisterPath(const std::string& path);

    private:

        ModelMap                            m_modelList;
        std::vector<resources::DecoderPtr>  m_decoders;
        std::vector<std::string>            m_pathes;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_MANAGER_H_
#ifndef _V3D_MODEL_MANAGER_H_
#define _V3D_MODEL_MANAGER_H_

#include "Singleton.h"
#include "resources/ModelData.h"
#include "decoders/ResourceDecoder.h"

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

        void                                add(const resources::ModelDataPtr& model);
        const resources::ModelDataPtr&      get(const std::string& name);
        resources::ModelDataPtr             load(const std::string& name);

        void                                unload(const std::string& name);
        void                                unload(const resources::ModelDataPtr& model);

        void                                unloadAll();

        void                                registerDecoder(decoders::DecoderPtr decoder);
        void                                unregisterDecoder(decoders::DecoderPtr& decoder);

        void                                registerPath(const std::string& path);
        void                                unregisterPath(const std::string& path);

    private:

        resources::ModelDataMap             m_modelList;
        std::vector<decoders::DecoderPtr>   m_decoders;
        std::vector<std::string>            m_pathes;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_MANAGER_H_
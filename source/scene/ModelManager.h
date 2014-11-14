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
        virtual             ~CModelManager();

        resources::ModelDataPtr load(const std::string& name);

        void                    registerDecoder(decoders::DecoderPtr decoder);
        void                    unregisterDecoder(decoders::DecoderPtr& decoder);

        void                    registerPath(const std::string& path);
        void                    unregisterPath(const std::string& path);

    private:

        std::vector<decoders::DecoderPtr>   m_decoders;
        std::vector<std::string>            m_pathes;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_MANAGER_H_
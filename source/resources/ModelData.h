#ifndef _V3D_MODEL_DATA_H_
#define _V3D_MODEL_DATA_H_

#include "stream/Resource.h"

namespace v3d
{
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelData : public stream::CResource
    {

    public:

        CModelData();
        virtual     ~CModelData();

        void       init(const stream::IStreamPtr& stream)  override;
        bool       load()                                  override;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CModelData>         ModelDataPtr;
    typedef std::map<std::string, ModelDataPtr> ModelDataMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_DATA_H_
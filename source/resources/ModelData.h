#ifndef _V3D_MODEL_DATA_H_
#define _V3D_MODEL_DATA_H_

#include "stream/Resource.h"
#include "renderer/RenderTechnique.h"

namespace v3d
{
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelData : public stream::CResource
    {

    public:

        CModelData();
        virtual             ~CModelData();

        void                init(const stream::IStreamPtr& stream)  override;
        bool                load()                                  override;

        u32                 getCountMeshes() const;
        stream::ResourcePtr readMeshResource(const renderer::RenderTechniquePtr& technique) const;

        const std::string&  getName()   const;
        s32                 getID()     const;

    private:

        u32                 m_countMeshes;
        s32                 m_id;
        std::string         m_name;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CModelData>         ModelDataPtr;
    typedef std::map<std::string, ModelDataPtr> ModelDataMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_DATA_H_
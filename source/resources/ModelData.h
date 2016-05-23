#ifndef _V3D_MODEL_DATA_H_
#define _V3D_MODEL_DATA_H_

#include "Vertex.h"
#include "scene/Node.h"
#include "stream/Resource.h"

namespace v3d
{
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    DEPRECATED class CModelData : public stream::IResource
    {

    public:

        struct SNodeData
        {
            SNodeData(const stream::IStreamPtr& stream, scene::ENodeType type)
            : _stream(stream)
            , _type(type)
            , _parrentIdx(-1) {}

            scene::ENodeType    _type;
            stream::IStreamPtr  _stream;
            s32                 _parrentIdx;
        };

        CModelData();
        virtual                         ~CModelData();

        void                            init(const stream::IStreamPtr& stream)  override;
        bool                            load()                                  override;

        u32                             getCountNodes() const;

        const std::string&              getName()   const;
        s32                             getID()     const;

        const std::vector<SNodeData>&   getNodesList() const;

    private:

        void                            loadGeometry(const stream::IStreamPtr& stream);
        void                            loadMaterial(const stream::IStreamPtr& stream);

        u32                             m_countNodes;
        s32                             m_id;
        std::string                     m_name;

        std::vector<SNodeData>          m_nodesList;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CModelData>         ModelDataPtr;
    typedef std::map<std::string, ModelDataPtr> ModelDataMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_DATA_H_
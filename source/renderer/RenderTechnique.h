#ifndef _V3D_RENDER_TECHNIQUE_H_
#define _V3D_RENDER_TECHNIQUE_H_

#include "stream/Resource.h"
#include "RenderPass.h"
#include "tinyxml2.h"


namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderTechnique : public stream::CResource
    {
    public:

        CRenderTechnique();
        virtual              ~CRenderTechnique();

        const RenderPassPtr& getRenderPass(u32 id) const;
        u32                  getRenderPassCount()  const;
        void                 addRenderPass(const RenderPassPtr& pass);

        const RenderPassList& getRenderPassList() const;


        void                 init(stream::IStream* stream)   override;
        bool                 load()                          override;

        void                 refresh();


    private:

        bool                 parse(tinyxml2::XMLElement* root);

        RenderPassList       m_renderPassList;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderTechnique>            RenderTechniquePtr;
    typedef std::map<std::string, RenderTechniquePtr>    RenderTechniqueMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_TECHNIQUE_H_

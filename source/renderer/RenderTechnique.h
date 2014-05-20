#ifndef _F3D_RENDER_TECHNIQUE_H_
#define _F3D_RENDER_TECHNIQUE_H_

#include "stream/Resource.h"
#include "RenderPass.h"
#include "tinyxml2.h"

namespace f3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef  std::vector<CRenderPassPtr>    CRenderPassList;

    class CRenderTechique : public stream::CResource
    {
    public:

        CRenderTechique();
        virtual             ~CRenderTechique();

        CRenderPassPtr      getRenderPass(u32 id) const;
        u32                 getRenderPassCount()  const;
        void                addRenderPass(const CRenderPassPtr& pass);

        void                load();
        void                refresh();


    private:

        bool                parse(tinyxml2::XMLElement* root);

        CRenderPassList     m_renderPassList;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderTechique> CRenderTechiquePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_RENDER_TECHNIQUE_H_
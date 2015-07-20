#ifndef _V3D_RENDER_TECHNIQUE_MANAGER_H_
#define _V3D_RENDER_TECHNIQUE_MANAGER_H_

#include "renderer/RenderTechnique.h"
#include "stream/ResourceLoader.h"
#include "Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderTechniqueManager : public Singleton<CRenderTechniqueManager>, public stream::TResourceLoader<renderer::RenderTechniquePtr>
    {
    public:

        CRenderTechniqueManager();
        virtual                             ~CRenderTechniqueManager();

        void                                add(const renderer::RenderTechniquePtr& technique);
        const renderer::RenderTechniquePtr  load(const std::string& name, const std::string& alias = "") override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_TECHNIQUE_MANAGER_H_
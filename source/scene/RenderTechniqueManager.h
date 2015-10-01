#ifndef _V3D_RENDER_TECHNIQUE_MANAGER_H_
#define _V3D_RENDER_TECHNIQUE_MANAGER_H_

#include "renderer/RenderTechnique.h"
#include "stream/ResourceLoader.h"
#include "utils/Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderTechniqueManager : public utils::TSingleton<CRenderTechniqueManager>, public stream::TResourceLoader<const renderer::CRenderTechnique>
    {
    private:

        friend utils::TSingleton<CRenderTechniqueManager>;

        CRenderTechniqueManager();
        virtual                             ~CRenderTechniqueManager();

    public:

        void                                add(const renderer::CRenderTechnique* technique);
        const renderer::CRenderTechnique*   load(const std::string& name, const std::string& alias = "") override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_TECHNIQUE_MANAGER_H_
#ifndef _V3D_RENDER_TECHNIQUE_MANAGER_H_
#define _V3D_RENDER_TECHNIQUE_MANAGER_H_

#include "renderer/RenderTechnique.h"
#include "Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderTechniqueManager : public Singleton<CRenderTechniqueManager>
    {
    public:

        CRenderTechniqueManager();
        virtual                             ~CRenderTechniqueManager();

        void                                add(const renderer::RenderTechniquePtr& technique);
        const renderer::RenderTechniquePtr& get(const std::string& name);
        const renderer::RenderTechniquePtr  load(const std::string& name);

        void                                unload(const std::string& name);
        void                                unload(const renderer::RenderTechniquePtr& technique);

        void                                unloadAll();

        void                                registerPath(const std::string& path);
        void                                unregisterPath(const std::string& path);
        
    private:

        renderer::RenderTechniqueMap        m_renderTechniques;
        std::vector<std::string>            m_pathes;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_TECHNIQUE_MANAGER_H_
#ifndef _V3D_RENDER_TARGET_MANAGER_H_
#define _V3D_RENDER_TARGET_MANAGER_H_

#include "renderer/RenderTarget.h"
#include "utils/Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderTargetManager : public utils::TSingleton<CRenderTargetManager>
    {
    private:

        friend utils::TSingleton<CRenderTargetManager>;

        CRenderTargetManager();
        ~CRenderTargetManager();

    public:

        void                            add(const renderer::RenderTargetPtr& target);
        const renderer::RenderTargetPtr get(const std::string& name);

        void                            unload(const std::string& name);
        void                            unload(const renderer::RenderTargetPtr& target);
        void                            unloadAll();

        renderer::RenderTargetIter      begin();
        renderer::RenderTargetCIter     begin() const;

        renderer::RenderTargetIter      end();
        renderer::RenderTargetCIter     end() const;

    private:

        renderer::RenderTargetMap       m_renderTargets;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_TARGET_MANAGER_H_
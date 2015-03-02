#ifndef _V3D_RENDER_TARGET_MANAGER_H_
#define _V3D_RENDER_TARGET_MANAGER_H_

#include "renderer/RenderTarget.h"
#include "Singleton.h"

namespace v3d
{
    namespace scene
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class CRenderTargetManager : public Singleton<CRenderTargetManager>
        {
        public:

            CRenderTargetManager();
            virtual                          ~CRenderTargetManager();

            void                             add(const renderer::RenderTargetPtr& target);
            const renderer::RenderTargetPtr  get(const std::string& name);

            void                             unload(const std::string& name);
            void                             unload(const renderer::RenderTargetPtr& target);

            void                             unloadAll();

        private:

            renderer::RenderTargetMap        m_renderTargets;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}

#endif //_V3D_RENDER_TARGET_MANAGER_H_
#ifndef _V3D_RENDER_TARGET_MANAGER_H_
#define _V3D_RENDER_TARGET_MANAGER_H_

#include "renderer/Target.h"
#include "utils/Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTargetManager : public utils::TSingleton<CTargetManager>
    {
    private:

        friend utils::TSingleton<CTargetManager>;

        CTargetManager();
        ~CTargetManager();

    public:

        void                            add(const renderer::TargetPtr& target);
        const renderer::TargetPtr       get(const std::string& name);

        void                            unload(const std::string& name);
        void                            unload(const renderer::TargetPtr& target);
        void                            unloadAll();

        renderer::TargetIter            begin();
        renderer::TargetCIter           cbegin() const;

        renderer::TargetIter            end();
        renderer::TargetCIter           cend() const;

    private:

        renderer::TargetMap             m_renderTargets;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace scene
} //namespace v3d

#endif //_V3D_RENDER_TARGET_MANAGER_H_

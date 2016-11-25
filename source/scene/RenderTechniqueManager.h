#pragma once

#include "renderer/RenderTechnique.h"
#include "stream/ResourceLoader.h"
#include "utils/Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderTechniqueManager : public utils::TSingleton<RenderTechniqueManager>, public stream::TResourceLoader<const renderer::RenderTechnique*>
    {
    private:

        friend utils::TSingleton<RenderTechniqueManager>;

        RenderTechniqueManager();
        ~RenderTechniqueManager();

    public:

        void                                add(const renderer::RenderTechnique* technique);
        const renderer::RenderTechnique*   load(const std::string& name, const std::string& alias = "") override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace scene
} //namespace v3d

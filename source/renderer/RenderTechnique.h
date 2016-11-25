#pragma once

#include "stream/Resource.h"
#include "utils/Cloneable.h"
#include "RenderPass.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderTechnique final : public stream::IResource, public utils::TCloneable<RenderTechnique*>
    {
    public:

        RenderTechnique();
        virtual                 ~RenderTechnique();

        const RenderPassPtr     getRenderPass(u32 id) const;
        RenderPassPtr           getRenderPass(u32 id);

        const RenderPassList&   getRenderPassList() const;
        void                    addRenderPass(const RenderPassPtr pass);
        u32                     getRenderPassCount()  const;

        void                    setCurrentPass(u32 pass);
        u32                     getCurrentPass() const;

        const std::string&      getName() const;


        void                    init(const stream::IStreamPtr stream) override;
        bool                    load() override;

        RenderTechnique*        clone() const override;

    protected:

        RenderTechnique(const RenderTechnique& technique);
        RenderTechnique&        operator=(const RenderTechnique& technique);

    private:

        bool                    parse(tinyxml2::XMLElement* root);

        RenderPassList          m_renderPassList;
        u32                     m_currentPass;

        std::string             m_name;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<RenderTechnique>            RenderTechniquePtr;
    typedef std::map<std::string, RenderTechniquePtr>    RenderTechniqueMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

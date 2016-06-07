#ifndef _V3D_RENDER_TECHNIQUE_H_
#define _V3D_RENDER_TECHNIQUE_H_

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

    class CRenderTechnique : public stream::IResource, public utils::TCloneable<CRenderTechnique*>
    {
    public:

        CRenderTechnique();
        virtual                 ~CRenderTechnique();

        const RenderPassPtr&    getRenderPass(u32 id) const;
        const RenderPassList&   getRenderPassList() const;
        void                    addRenderPass(const RenderPassPtr& pass);
        u32                     getRenderPassCount()  const;

        void                    setCurrentPass(u32 pass);
        u32                     getCurrentPass() const;

        const std::string&      getName() const;


        void                    init(const stream::IStreamPtr& stream) override;
        bool                    load()                                 override;

        CRenderTechnique*       clone() override;

    protected:

        CRenderTechnique(const CRenderTechnique& technique);
        CRenderTechnique&       operator=(const CRenderTechnique& technique);

    private:

        bool                    parse(tinyxml2::XMLElement* root);

        RenderPassList          m_renderPassList;
        u32                     m_currentPass;

        std::string             m_name;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderTechnique>            RenderTechniquePtr;
    typedef std::map<std::string, RenderTechniquePtr>    RenderTechniqueMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDER_TECHNIQUE_H_

#ifndef _V3D_RENDER_PASS_H_
#define _V3D_RENDER_PASS_H_

#include "ShaderProgram.h"
#include "ShaderData.h"
#include "RenderState.h"
#include "RenderLOD.h"
#include "RenderAdvanced.h"
#include "RenderTarget.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderer;
    class CRenderTechnique;

    class CRenderPass
    {
    public:

        CRenderPass();
        ~CRenderPass();

        const ShaderProgramPtr& getShaderProgram() const;
        void                    setShaderProgram(const ShaderProgramPtr& program);

        const ShaderDataPtr&    getUserShaderData() const;
        void                    setUserShaderData(const ShaderDataPtr& data);

        const ShaderDataPtr&    getDefaultShaderData() const;
        void                    setDefaultShaderData(const ShaderDataPtr& data);

        const RenderStatePtr&   getRenderState() const;
        void                    setRenderState(const RenderStatePtr& state);

        const RenderLODPtr&     getRenderLOD() const;
        void                    setRenderLOD(const RenderLODPtr& lod);

        const RenderAdvancedPtr& getRenderAdvanced() const;
        void                    setRenderAdvanced(const RenderAdvancedPtr& advanced);

        const RenderTargetPtr&  getRenderTarget(u32 index) const;
        void                    setRenderTarget(u32 index, const RenderTargetPtr& target);
        void                    addRenderTarget(const RenderTargetPtr& target);
        u32                     getRenderTargetCount() const;

        void                    bind(u32 target = 0);
        void                    unbind(u32 target = 0);

        bool                    isCurrent() const;

    private:

        friend                  CRenderer;
        friend                  CRenderTechnique;

        void                    init();

        bool                    parse            (const tinyxml2::XMLElement* root);

        bool                    parseUniforms    (const tinyxml2::XMLElement* root);
        bool                    parseAttributes  (const tinyxml2::XMLElement* root);
        bool                    parseSamplers    (const tinyxml2::XMLElement* root);
        bool                    parseShaders     (const tinyxml2::XMLElement* root);
        bool                    parseRenderTarget(const tinyxml2::XMLElement* root);
        bool                    parseRenderState (const tinyxml2::XMLElement* root);
        bool                    parseRenderLOD   (const tinyxml2::XMLElement* root);
        bool                    parseRenderAdvanced(const tinyxml2::XMLElement* root);

        const std::string       attachIndexToUniform(const std::string& name, s32 idx);

        ShaderProgramPtr        m_program;
        ShaderDataPtr           m_userShaderData;
        ShaderDataPtr           m_defaultShaderData;
        RenderStatePtr          m_renderState;
        RenderLODPtr            m_lods;
        RenderAdvancedPtr       m_advanced;
        RenderTargetList        m_renderTargets;

        bool                    m_enable;
        bool                    m_current;
        std::string             m_name;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderPass>    RenderPassPtr;
    typedef  std::vector<RenderPassPtr>     RenderPassList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_PASS_H_

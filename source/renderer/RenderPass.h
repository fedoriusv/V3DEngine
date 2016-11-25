#pragma once

#include "ShaderProgram.h"
#include "ShaderData.h"
#include "RenderState.h"
#include "RenderLOD.h"
#include "Target.h"
#include "utils/Cloneable.h"

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
    class RenderTechnique;
    class RenderPass;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<RenderPass>    RenderPassPtr;
    typedef std::weak_ptr<RenderPass>      RenderPassWPtr;
    typedef std::vector<RenderPassPtr>     RenderPassList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Render pass class.
    * Parse and stored all shaders info and targets
    * Client thread
    */
    class RenderPass final : public utils::TCloneable<RenderPassPtr>
    {
    public:

        RenderPass();
        ~RenderPass();

        const ShaderProgramPtr  getShaderProgram() const;
        void                    setShaderProgram(const ShaderProgramPtr& program);

        const ShaderDataPtr     getUserShaderData() const;
        void                    setUserShaderData(const ShaderDataPtr& data);

        const ShaderDataPtr     getDefaultShaderData() const;
        void                    setDefaultShaderData(const ShaderDataPtr& data);

        const RenderStatePtr    getRenderState() const;
        void                    setRenderState(const RenderStatePtr& state);

        const RenderLODPtr      getRenderLOD() const;
        void                    setRenderLOD(const RenderLODPtr& lod);

        const TargetPtr         getTarget(u32 index) const;
        const TargetPtr         getTarget(const std::string& target) const;

        bool                    addTarget(const TargetPtr& target);
        bool                    removeTarget(const TargetPtr& target);

        bool                    addTarget(const std::string& target);
        bool                    removeTarget(const std::string& target);

        u32                     getTargetCount() const;

        void                    bind(u32 target = 0);
        void                    unbind(u32 target = 0);

        RenderPassPtr           clone() const override;

    private:

        RenderPass(const RenderPass& pass);
        RenderPass&             operator=(const RenderPass& pass);

        friend                  CRenderer;
        friend                  RenderTechnique;
        friend                  std::_Ref_count_obj<RenderPass>;

        void                    init();

        bool                    parse(const tinyxml2::XMLElement* root);

        bool                    parseUniforms(const tinyxml2::XMLElement* root);
        bool                    parseAttributes(const tinyxml2::XMLElement* root);
        bool                    parseFragdata(const tinyxml2::XMLElement* root);
        bool                    parseSamplers(const tinyxml2::XMLElement* root);
        bool                    parseShaders(const tinyxml2::XMLElement* root);
        bool                    parseRenderTarget(const tinyxml2::XMLElement* root);
        bool                    parseRenderState(const tinyxml2::XMLElement* root);
        bool                    parseRenderLOD(const tinyxml2::XMLElement* root);

        const std::string       attachIndexToUniform(const std::string& name, s32 idx);

        ShaderDataPtr           m_userShaderData;
        ShaderDataPtr           m_defaultShaderData;
        RenderStatePtr          m_renderState;
        RenderLODPtr            m_lods;
        TargetList              m_targetList;
        ShaderProgramPtr        m_program;

        bool                    m_enable;
        std::string             m_name;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

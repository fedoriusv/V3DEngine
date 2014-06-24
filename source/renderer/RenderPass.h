#ifndef _V3D_RENDER_PASS_H_
#define _V3D_RENDER_PASS_H_

#include "tinyxml2.h"

#include "Object.h"
#include "ShaderProgram.h"
#include "ShaderData.h"
#include "RenderState.h"


namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderTechnique;

    class CRenderPass : public CObject
    {
    public:

        CRenderPass();
        virtual             ~CRenderPass();

        ShaderProgramPtr    getShaderProgram() const;
        void                setShaderProgram(const ShaderProgramPtr& program);

        ShaderDataPtr       getShaderData() const;
        void                setShaderData(const ShaderDataPtr& data);

        RenderStatePtr      getRenderState() const;
        void                setRenderState(const RenderStatePtr& state);

        void                bind();

    private:

        friend              CRenderTechnique;

        void                init();

        bool                parse            (tinyxml2::XMLElement* root);

        bool                parseUniforms    (tinyxml2::XMLElement* root);
        bool                parseAttributes  (tinyxml2::XMLElement* root);
        bool                parseSamplers    (tinyxml2::XMLElement* root);
        bool                parseShaders     (tinyxml2::XMLElement* root);
        bool                parseRenderTarget(tinyxml2::XMLElement* root);
        bool                parseRenderState (tinyxml2::XMLElement* root);

        const std::string   attachIndexToUniform(const std::string& name, s32 idx);

        ShaderProgramPtr    m_program;
        ShaderDataPtr       m_shaderData;
        RenderStatePtr      m_renderState;
        //RenderTarget

        bool                m_enable;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderPass>    RenderPassPtr;
    typedef  std::vector<RenderPassPtr>     RenderPassList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_PASS_H_

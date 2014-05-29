#ifndef _F3D_RENDER_PASS_H_
#define _F3D_RENDER_PASS_H_

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

    class CRenderTechique;

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

    private:

        friend              CRenderTechique;

        void                init();

        bool                parse            (tinyxml2::XMLElement* root);

        bool                parseUniforms    (tinyxml2::XMLElement* root);
        bool                parseAttributes  (tinyxml2::XMLElement* root);
        bool                parseSamplers    (tinyxml2::XMLElement* root);
        bool                parseShaders     (tinyxml2::XMLElement* root);
        bool                parseRenderTarget(tinyxml2::XMLElement* root);
        bool                parseRenderState (tinyxml2::XMLElement* root);

        ShaderProgramPtr    m_program;
        ShaderDataPtr       m_shaderData;
        RenderStatePtr      m_renderState;
        //RenderTarget

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderPass>    RenderPassPtr;
    typedef  std::vector<RenderPassPtr>     RenderPassList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_RENDER_PASS_H_
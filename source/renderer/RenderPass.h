#ifndef _V3D_RENDER_PASS_H_
#define _V3D_RENDER_PASS_H_

#include "Object.h"
#include "ShaderProgram.h"
#include "ShaderData.h"
#include "RenderState.h"
#include "RenderLOD.h"
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

    class CRenderPass : public CObject
    {
    public:

        CRenderPass();
        virtual                 ~CRenderPass();

        const ShaderProgramPtr& getShaderProgram() const;
        void                    setShaderProgram(const ShaderProgramPtr& program);

        const ShaderDataPtr&    getShaderData() const;
        void                    setShaderData(const ShaderDataPtr& data);

        const RenderStatePtr&   getRenderState() const;
        void                    setRenderState(const RenderStatePtr& state);

        const RenderLODPtr&     getRenderLOD() const;
        void                    setRenderLOD(const RenderLODPtr& lod);

        void                    bind();

    private:

        friend                  CRenderer;
        friend                  CRenderTechnique;

        void                    init();

        bool                    parse            (tinyxml2::XMLElement* root);

        bool                    parseUniforms    (tinyxml2::XMLElement* root);
        bool                    parseAttributes  (tinyxml2::XMLElement* root);
        bool                    parseSamplers    (tinyxml2::XMLElement* root);
        bool                    parseShaders     (tinyxml2::XMLElement* root);
        bool                    parseRenderTarget(tinyxml2::XMLElement* root);
        bool                    parseRenderState (tinyxml2::XMLElement* root);
        bool                    parseRenderLOD   (tinyxml2::XMLElement* root);
        bool                    parseUserUniform (tinyxml2::XMLElement* element, const std::string& name, CShaderUniform::EDataType type);

        const std::string       attachIndexToUniform(const std::string& name, s32 idx);
        void                    parseArrayValue(const std::string& val, f32* array, u32 count);

        ShaderProgramPtr        m_program;
        ShaderDataPtr           m_shaderData;
        RenderStatePtr          m_renderState;
        RenderLODPtr            m_lods;
        RenderTargetPtr         m_renderTarget;

        bool                    m_enable;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderPass>    RenderPassPtr;
    typedef  std::vector<RenderPassPtr>     RenderPassList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_PASS_H_

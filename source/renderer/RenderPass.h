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

    class CRenderPass
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

        bool                    parse            (const tinyxml2::XMLElement* root);

        bool                    parseUniforms    (const tinyxml2::XMLElement* root);
        bool                    parseAttributes  (const tinyxml2::XMLElement* root);
        bool                    parseSamplers    (const tinyxml2::XMLElement* root);
        bool                    parseShaders     (const tinyxml2::XMLElement* root);
        bool                    parseRenderTarget(const tinyxml2::XMLElement* root);
        bool                    parseRenderState (const tinyxml2::XMLElement* root);
        bool                    parseRenderLOD   (const tinyxml2::XMLElement* root);
        bool                    parseUserUniform (const tinyxml2::XMLElement* element, const std::string& name, CShaderUniform::EDataType type);

        const std::string       attachIndexToUniform(const std::string& name, s32 idx);
        void                    parseArrayValue(const std::string& val, f32* array, u32 count);

        ShaderProgramPtr        m_program;
        ShaderDataPtr           m_shaderData;
        RenderStatePtr          m_renderState;
        RenderLODPtr            m_lods;
        RenderTargetPtr         m_renderTarget;

        bool                    m_enable;
        std::string             m_name;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderPass>    RenderPassPtr;
    typedef  std::vector<RenderPassPtr>     RenderPassList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_PASS_H_

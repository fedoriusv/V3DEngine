#ifndef _V3D_SHADER_SAMPLER_H_
#define _V3D_SHADER_SAMPLER_H_

#include "common.h"
#include "RenderTarget.h"
#include "Texture.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderer;
    
    /**
    * Shader Sampler.
    */
    class CShaderSampler final
    {
    public:

        enum class ESamplerType
        {
            eUserSampler,
            eTextureSampler,
            eRenderTargetSampler
        };

        CShaderSampler();
        CShaderSampler(const CShaderSampler& sampler);
        CShaderSampler& operator=(const CShaderSampler& sampler);

        ~CShaderSampler();

        const std::string&      getName() const;
        s32                     getID() const;
        ESamplerType            getType() const;

        const TexturePtr        getTexture() const;
        TexturePtr              getTexture();

        const RenderTargetWPtr  getTarget() const;
        RenderTargetWPtr        getTarget();

        bool                    parse(const tinyxml2::XMLElement* root);

        void                    setID(s32 id);

    protected:

        friend                  CRenderer;

        void                    setName(const std::string& name);

        ESamplerType            m_type;
        std::string             m_name;

        RenderTargetWPtr        m_target;
        TexturePtr              m_texture;

        s32                     m_id;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::vector<CShaderSampler*>    SamplerList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_SAMPLER_H_

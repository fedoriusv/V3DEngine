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
    class CShaderSampler
    {
    public:

        enum ESamplerType
        {
            eUserSampler = -1,
            eTextureSampler,
            eRenderTargetSampler
        };

        CShaderSampler();
        ~CShaderSampler();

        CShaderSampler&         operator=(const CShaderSampler& other);

        void                    setAttribute(const std::string& attribute);
        const std::string&      getAttribute() const;

        void                    setID(s32 id);
        s32                     getID() const;

        ESamplerType            getType() const;

        bool                    parse(const tinyxml2::XMLElement* root);

    protected:

        friend                  CRenderer;

        const CTexture*         getTexture() const;
        CTexture*               getTexture();
        const RenderTargetPtr   getTarget() const;

        ESamplerType            m_type;
        std::string             m_attribute;

        RenderTargetWPtr        m_target;
        const CTexture*         m_texture;

        s32                     m_id;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::vector<CShaderSampler*>    SamplerList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_SAMPLER_H_

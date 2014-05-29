#ifndef _F3D_SHADER_SAMPLER_H_
#define _F3D_SHADER_SAMPLER_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderSampler
    {
    public:

        CShaderSampler();
        virtual         ~CShaderSampler();

        void            setSampler(const std::string& attribute);

    protected:

        std::string     m_attribute;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderSampler>            ShaderSamplerPtr;
    typedef std::map<std::string, ShaderSamplerPtr>    SamplerList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_SAMPLER_H_
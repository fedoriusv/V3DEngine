#ifndef _V3D_SHADER_SAMPLER_H_
#define _V3D_SHADER_SAMPLER_H_

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
        virtual                 ~CShaderSampler();

        void                    setSampler(const std::string& attribute);
        const std::string&      getAttribute() const;

    protected:

        std::string             m_attribute;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderSampler>    SamplerPtr;
    typedef std::vector<SamplerPtr>            SamplerList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_SAMPLER_H_

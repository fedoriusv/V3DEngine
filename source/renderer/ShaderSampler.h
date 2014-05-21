#ifndef _F3D_SHADER_SAMPLER_H_
#define _F3D_SHADER_SAMPLER_H_

#include "Object.h"

namespace f3d
{
namespace renderer
{
    class CShaderSampler : public CObject
    {
    public:

        CShaderSampler();
        virtual ~CShaderSampler();

        void    setSampler(const std::string& attribute);

    protected:

        std::string m_attribute;
    };
}
}

#endif //_F3D_SHADER_SAMPLER_H_
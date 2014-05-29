#ifndef _F3D_SHADER_DATA_H_
#define _F3D_SHADER_DATA_H_

#include "Object.h"
#include "ShaderUniform.h"
#include "ShaderAttribute.h"
#include "ShaderSampler.h"


namespace f3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderData : public CObject
    {
    public:

        CShaderData();
        virtual                 ~CShaderData();

        void                    setUniformInt    (const std::string& name, const u32             value);
        void                    setUniformFloat  (const std::string& name, const f32             value);
        void                    setUniformVector2(const std::string& name, const core::Vector2D& vector);
        void                    setUniformVector3(const std::string& name, const core::Vector3D& vector);
        void                    setUniformVector4(const std::string& name, const core::Vector4D& vector);
        void                    setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix);
        void                    setUniformMatrix4(const std::string& name, const core::Vector4D& matrix);

        const AttributeList&    getAttributeList() const;

    protected:

        bool                    isExistUniform(const std::string& name);
        void                    addDefaultUniform(const std::string& name, EShaderDataType type, EDefaultUniformData data);

        bool                    isExistAttribute(const std::string& name);
        void                    addAttribute(const std::string& name, EShaderAttribute type);

        bool                    isExistSampler(const std::string& name);
        void                    addSampler(const std::string& name);

        AttributeList           m_attributeList;
        UniformList             m_uniformList;
        SamplerList             m_samplerList;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderData> ShaderDataPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_DATA_H_
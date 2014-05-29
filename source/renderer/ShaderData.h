#ifndef _F3D_SHADER_DATA_H_
#define _F3D_SHADER_DATA_H_

#include "Object.h"
#include "ShaderUniform.h"
#include "ShaderAttribute.h"
#include "ShaderSampler.h"


namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;

    class CShaderData : public CObject
    {
    public:

        CShaderData();
        virtual                         ~CShaderData();

        void                            setUniformInt    (const std::string& name, const u32             value);
        void                            setUniformFloat  (const std::string& name, const f32             value);
        void                            setUniformVector2(const std::string& name, const core::Vector2D& vector);
        void                            setUniformVector3(const std::string& name, const core::Vector3D& vector);
        void                            setUniformVector4(const std::string& name, const core::Vector4D& vector);
        void                            setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix);
        void                            setUniformMatrix4(const std::string& name, const core::Vector4D& matrix);

        const AttributeList&            getAttributeList() const;

        static const std::string&       getShaderDataNameByType(EShaderDataType type);
        static const EShaderDataType    getShaderDataTypeByName(const std::string& name);

    protected:

        friend                          CRenderPass;

        bool                            isExistUniform(const std::string& name);
        void                            addDefaultUniform(const std::string& name, EShaderDataType type, EDefaultUniformData data);

        bool                            isExistAttribute(const std::string& name);
        void                            addAttribute(const std::string& name, EShaderAttribute type);

        bool                            isExistSampler(const std::string& name);
        void                            addSampler(const std::string& name);

        AttributeList                   m_attributeList;
        UniformList                     m_uniformList;
        SamplerList                     m_samplerList;

    private:

        static const std::string       s_shaderDataType[EShaderDataType::eDataTypeCount];

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderData> ShaderDataPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_DATA_H_
#ifndef _V3D_SHADER_DATA_H_
#define _V3D_SHADER_DATA_H_

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

    class CDebugGeometry;
    class CDebugLight;

    class CShaderData : public CObject
    {
    public:

        CShaderData();
        virtual                         ~CShaderData();

        void                            addUniformInt    (const std::string& name, const s32             value);
        void                            addUniformFloat  (const std::string& name, const f32             value);
        void                            addUniformVector2(const std::string& name, const core::Vector2D& vector);
        void                            addUniformVector3(const std::string& name, const core::Vector3D& vector);
        void                            addUniformVector4(const std::string& name, const core::Vector4D& vector);
        void                            addUniformMatrix3(const std::string& name, const core::Matrix3D& matrix);
        void                            addUniformMatrix4(const std::string& name, const core::Matrix4D& matrix);

        const AttributeList&            getAttributeList() const;
        const UniformList&              getUniformList() const;
        const UniformList&              getDefaultUniformList() const;
        const SamplerList&              getSamplerList() const;

        static const std::string&       getDataNameByType(CShaderUniform::EDataType type);
        static const CShaderUniform::EDataType getDataTypeByName(const std::string& name);

        bool                            isExistUniform(const std::string& name);
        bool                            isExistUniform(EUniformData type);
        bool                            isExistAttribute(const std::string& name);
        bool                            isExistSampler(const std::string& name);

    protected:

        friend                          CRenderPass;

        friend                          CDebugGeometry;
        friend                          CDebugLight;

        void                            addUniform(const std::string& name, CShaderUniform::EDataType type);
        void                            addDefaultUniform(const std::string& name, EUniformData data);
        void                            addAttribute(const std::string& name, CShaderAttribute::EShaderAttribute type);
        void                            addSampler(const std::string& name);

        AttributeList                   m_attributeList;
        UniformList                     m_uniformList;
        UniformList                     m_defaultUniformList;
        SamplerList                     m_samplerList;

    private:

        static const std::string       s_dataType[CShaderUniform::eTypeCount];

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderData> ShaderDataPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_DATA_H_

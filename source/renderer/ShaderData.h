#ifndef _V3D_SHADER_DATA_H_
#define _V3D_SHADER_DATA_H_

#include "Object.h"
#include "ShaderUniform.h"
#include "ShaderAttribute.h"
#include "ShaderSampler.h"
#include "ShaderDefaultUniform.h"


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

        void                            setUniformInt    (const std::string& name, const u32             value);
        void                            setUniformFloat  (const std::string& name, const f32             value);
        void                            setUniformVector2(const std::string& name, const core::Vector2D& vector);
        void                            setUniformVector3(const std::string& name, const core::Vector3D& vector);
        void                            setUniformVector4(const std::string& name, const core::Vector4D& vector);
        void                            setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix);
        void                            setUniformMatrix4(const std::string& name, const core::Matrix4D& matrix);

        const AttributeList&            getAttributeList() const;

        static const std::string&       getDataNameByType(CShaderUniform::EDataType type);
        static const CShaderUniform::EDataType getDataTypeByName(const std::string& name);

        bool                            isExistUniform(const std::string& name);
        bool                            isExistUniform(CShaderDefaultUniform::EUniformData type);
        bool                            isExistAttribute(const std::string& name);
        bool                            isExistSampler(const std::string& name);

    protected:

        friend                          CRenderPass;

        friend                          CDebugGeometry;
        friend                          CDebugLight;

        void                            addUniform(const std::string& name, CShaderUniform::EDataType type);
        void                            addDefaultUniform(const std::string& name, CShaderDefaultUniform::EUniformData data);
        void                            addAttribute(const std::string& name, CShaderAttribute::EShaderAttribute type);
        void                            addSampler(const std::string& name);

        AttributeList                   m_attributeList;
        UniformList                     m_uniformList;
        DefaultUniformList              m_defaultUniformList;
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

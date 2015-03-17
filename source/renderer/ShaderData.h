#ifndef _V3D_SHADER_DATA_H_
#define _V3D_SHADER_DATA_H_

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

    class CShaderData
    {
    public:

        CShaderData();
        ~CShaderData();

        void                        setUniformInt    (const std::string& name, const s32             value);
        void                        setUniformFloat  (const std::string& name, const f32             value);
        void                        setUniformVector2(const std::string& name, const core::Vector2D& vector);
        void                        setUniformVector3(const std::string& name, const core::Vector3D& vector);
        void                        setUniformVector4(const std::string& name, const core::Vector4D& vector);
        void                        setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix);
        void                        setUniformMatrix4(const std::string& name, const core::Matrix4D& matrix);

        const AttributeList&        getAttributeList() const;
        const UniformList&          getUniformList() const;
        const SamplerList&          getSamplerList() const;

        static const std::string&   getDataNameByType(EDataType type);
        static EDataType            getDataTypeByName(const std::string& name);

        bool                        isExistUniform(const std::string& name);
        bool                        isExistUniform(CShaderUniform::EUniformData type);
        bool                        isExistAttribute(const std::string& name);
        bool                        isExistSampler(const std::string& name);

    protected:

        friend                      CRenderPass;
        friend                      CDebugGeometry;
        friend                      CDebugLight;

        void                        addUniform(const UniformPtr& uniform);
        void                        addAttribute(const AttributePtr& attribute);
        void                        addSampler(const SamplerPtr& sampler);

        UniformList                 m_uniformList;
        AttributeList               m_attributeList;
        SamplerList                 m_samplerList;

    private:

        static const std::string    _dataType[EDataType::eDataTypeCount];

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderData> ShaderDataPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_DATA_H_

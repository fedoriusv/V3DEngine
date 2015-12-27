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

        void                        setUniform(const std::string& name, const s32             value);
        void                        setUniform(const std::string& name, const f32             value);
        void                        setUniform(const std::string& name, const core::Vector2D& vector);
        void                        setUniform(const std::string& name, const core::Vector3D& vector);
        void                        setUniform(const std::string& name, const core::Vector4D& vector);
        void                        setUniform(const std::string& name, const core::Matrix3D& matrix);
        void                        setUniform(const std::string& name, const core::Matrix4D& matrix);

        void                        setAttribute(const std::string& name, u32 divisor, const std::vector<s32>&            data);
        void                        setAttribute(const std::string& name, u32 divisor, const std::vector<f32>&            data);
        void                        setAttribute(const std::string& name, u32 divisor, const std::vector<core::Vector2D>& data);
        void                        setAttribute(const std::string& name, u32 divisor, const std::vector<core::Vector3D>& data);
        void                        setAttribute(const std::string& name, u32 divisor, const std::vector<core::Vector4D>& data);

        const AttributeList&        getAttributeList() const;
        const UniformList&          getUniformList() const;
        const SamplerList&          getSamplerList() const;

        AttributeList&              getAttributeList();
        UniformList&                getUniformList();
        SamplerList&                getSamplerList();

        bool                        isExistUniform(const std::string& name);
        bool                        isExistUniform(CShaderUniform::EUniformData type);
        bool                        isExistAttribute(const std::string& name);
        bool                        isExistSampler(const std::string& name);

        u32                         getVertexFormatMask() const;

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
        u32                         m_vertexFormatMask;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderData>    ShaderDataPtr;
    typedef std::weak_ptr<CShaderData>      ShaderDataWPtr;
    typedef std::vector<ShaderDataWPtr>     ShaderDataList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_DATA_H_

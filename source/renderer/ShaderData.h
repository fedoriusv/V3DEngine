#ifndef _V3D_SHADER_DATA_H_
#define _V3D_SHADER_DATA_H_

#include "ShaderUniform.h"
#include "ShaderSampler.h"


namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;
    class ShaderAttribute;

    class CShaderData final
    {
    public:

        CShaderData();
        CShaderData(const CShaderData& data);
        CShaderData& operator=(const CShaderData& data);

        ~CShaderData();

        void                    setUniform(const std::string& name, const s32             value);
        void                    setUniform(const std::string& name, const f32             value);
        void                    setUniform(const std::string& name, const core::Vector2D& vector);
        void                    setUniform(const std::string& name, const core::Vector3D& vector);
        void                    setUniform(const std::string& name, const core::Vector4D& vector);
        void                    setUniform(const std::string& name, const core::Matrix3D& matrix);
        void                    setUniform(const std::string& name, const core::Matrix4D& matrix);

        void                    setAttribute(const std::string& name, u32 divisor, const std::vector<s32>&            data);
        void                    setAttribute(const std::string& name, u32 divisor, const std::vector<f32>&            data);
        void                    setAttribute(const std::string& name, u32 divisor, const std::vector<core::Vector2D>& data);
        void                    setAttribute(const std::string& name, u32 divisor, const std::vector<core::Vector3D>& data);
        void                    setAttribute(const std::string& name, u32 divisor, const std::vector<core::Vector4D>& data);

        const AttributeList&    getAttributeList() const;
        const AttributeList&    getFragDataList() const;
        const UniformList&      getUniformList() const;
        const SamplerList&      getSamplerList() const;

        AttributeList&          getAttributeList();
        AttributeList&          getFragDataList();
        UniformList&            getUniformList();
        SamplerList&            getSamplerList();

        bool                    isExistUniform(const std::string& name);
        bool                    isExistUniform(CShaderUniform::EUniformData type);
        bool                    isExistAttribute(const std::string& name);
        bool                    isExistSampler(const std::string& name);
        bool                    isExistFragData(const std::string& name);

        u32                     getVertexFormatMask() const;

    protected:

        friend                  CRenderPass;

        void                    addUniform(const CShaderUniform* uniform);
        void                    addAttribute(const ShaderAttribute* attribute);
        void                    addSampler(const CShaderSampler* sampler);
        void                    addFragData(const ShaderAttribute* fragData);

        void                    clear();

        UniformList             m_uniformList;
        AttributeList           m_attributeList;
        AttributeList           m_fragDataList;
        SamplerList             m_samplerList;
        u32                     m_vertexFormatMask;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderData>    ShaderDataPtr;
    typedef std::weak_ptr<CShaderData>      ShaderDataWPtr;
    typedef std::vector<ShaderDataWPtr>     ShaderDataList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_DATA_H_

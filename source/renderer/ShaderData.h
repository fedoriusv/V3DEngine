#ifndef _V3D_SHADER_DATA_H_
#define _V3D_SHADER_DATA_H_

#include "ShaderUniform.h"
#include "ShaderSampler.h"
#include "ShaderAttribute.h"
#include "ShaderUniform.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderPass;
    class ShaderAttribute;

    /**
    * ShaderData
    * Class provides shader data management. 
    * Used in client thread, provide transfer to render thread
    */
    class ShaderData final
    {
    public:

        ShaderData();
        ShaderData(const ShaderData& data);
        ShaderData& operator=(const ShaderData& data);

        ~ShaderData();

        void                    setUniform(const std::string& name, const s32 value);
        void                    setUniform(const std::string& name, const f32 value);
        void                    setUniform(const std::string& name, const core::Vector2D& vector);
        void                    setUniform(const std::string& name, const core::Vector3D& vector);
        void                    setUniform(const std::string& name, const core::Vector4D& vector);
        void                    setUniform(const std::string& name, const core::Matrix3D& matrix);
        void                    setUniform(const std::string& name, const core::Matrix4D& matrix);

        //TODO: getUniform()

        void                    setAttribute(const std::string& name, const std::vector<s32>& data);
        void                    setAttribute(const std::string& name, const std::vector<f32>&  data);
        void                    setAttribute(const std::string& name, const std::vector<core::Vector2D>& data);
        void                    setAttribute(const std::string& name, const std::vector<core::Vector3D>& data);
        void                    setAttribute(const std::string& name, const std::vector<core::Vector4D>& data);

        const AttributeList&    getAttributeList() const;
        const AttributeList&    getFragDataList() const;
        const UniformList&      getUniformList() const;
        const SamplerList&      getSamplerList() const;

        AttributeList&          getAttributeList();
        AttributeList&          getFragDataList();
        UniformList&            getUniformList();
        SamplerList&            getSamplerList();

        bool                    isExistUniform(const std::string& name);
        bool                    isExistUniform(ShaderUniform::EUniformData type);
        bool                    isExistAttribute(const std::string& name);
        bool                    isExistSampler(const std::string& name);
        bool                    isExistFragData(const std::string& name);

        u32                     getVertexFormatMask() const;

    protected:

        friend                  RenderPass;

        void                    addUniform(const ShaderUniform* uniform);
        void                    addAttribute(const ShaderAttribute* attribute);
        void                    addSampler(const CShaderSampler* sampler);
        void                    addFragData(const ShaderAttribute* fragData);

        void                    clear();

        std::recursive_mutex    m_mutex;

        UniformList             m_uniformList;
        AttributeList           m_attributeList;
        AttributeList           m_fragDataList;
        SamplerList             m_samplerList;
        u32                     m_vertexFormatMask;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<ShaderData>    ShaderDataPtr;
    typedef std::weak_ptr<ShaderData>      ShaderDataWPtr;
    typedef std::vector<ShaderDataWPtr>    ShaderDataList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_DATA_H_

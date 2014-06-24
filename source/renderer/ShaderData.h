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

    class CRenderJob;
    class CRenderPass;
    class CDebugDraw;

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

        static const std::string&       getShaderDataNameByType(EShaderDataType type);
        static const EShaderDataType    getShaderDataTypeByName(const std::string& name);

        bool                            isExistUniform(const std::string& name);
        bool                            isExistUniform(EDefaultUniformData type);
        bool                            isExistAttribute(const std::string& name);
        bool                            isExistSampler(const std::string& name);

    protected:

        friend                          CRenderPass;
        friend                          CRenderJob;
        friend                          CDebugDraw;

        void                            setUniformInt    (EDefaultUniformData type, const u32             value,  const s32 idx = -1);
        void                            setUniformFloat  (EDefaultUniformData type, const f32             value,  const s32 idx = -1);
        void                            setUniformVector2(EDefaultUniformData type, const core::Vector2D& vector, const s32 idx = -1);
        void                            setUniformVector3(EDefaultUniformData type, const core::Vector3D& vector, const s32 idx = -1);
        void                            setUniformVector4(EDefaultUniformData type, const core::Vector4D& vector, const s32 idx = -1);
        void                            setUniformMatrix3(EDefaultUniformData type, const core::Matrix3D& matrix, const s32 idx = -1);
        void                            setUniformMatrix4(EDefaultUniformData type, const core::Matrix4D& matrix, const s32 idx = -1);

        const std::string               findByUniformData(EDefaultUniformData type);
        void                            addDefaultUniform(const std::string& name, EShaderDataType type, EDefaultUniformData data);

        void                            addAttribute(const std::string& name, EShaderAttribute type);

        void                            addSampler(const std::string& name);

        AttributeList                   m_attributeList;
        UniformList                     m_uniformList;
        SamplerList                     m_samplerList;

    private:

        static const std::string       s_shaderDataType[EShaderDataType::eShaderDataCount];

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderData> ShaderDataPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_DATA_H_

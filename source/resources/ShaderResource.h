#pragma once

#include "stream/Resource.h"
#include "renderer/ShaderDataTypes.h"
#include "renderer/Texture.h"

namespace v3d
{
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EShaderType : s16
    {
        eShaderUnknown = -1,
        eVertex,
        eFragment,
        eGeometry,
        eTessellationControl,
        eTessellationEvaluation,
        eCompute,

        eShaderCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * ShaderResource class
    */
    class ShaderResource : public stream::IResource
    {

    public:

        enum EShaderDataRepresent : u16
        {
            eBitecode,
            eBitecodeSpirV,
            eSourceGLSL,
            eSourceHLSL
        };

        struct Reflection
        {
            struct Channel
            {
                std::string                                 name;
                u32                                         location;

                u32                                         rowCount;
                u32                                         colCount;
                renderer::ShaderDataType::EShaderDataType   type;
            };

            struct TextureParameter
            {
                std::string                                 name;
                u32                                         set;
                u32                                         binding;
                renderer::ETextureTarget                    target;
                bool                                        depth;
            };

            struct UniformParameter
            {
                std::string                                 name;
                s32                                         block;

                u32                                         rowCount;
                u32                                         colCount;
                renderer::ShaderDataType::EShaderDataType   type;
            };

            struct UniformBlockParameter
            {
                std::string                                 name;
                s32                                         id;
                u32                                         set;
                u32                                         binding;
            };

            std::vector<Channel>                            channelsIn;
            std::vector<Channel>                            channelsOut;
            std::vector<TextureParameter>                   samplers;
            std::vector<UniformParameter>                   uniforms;
            std::vector<UniformBlockParameter>              constantBuffers;

        };

        ShaderResource();
        ~ShaderResource();

        void                        init(const stream::IStreamPtr& stream) override;
        bool                        load() override;

        const std::string&  getName() const;
        const std::string&  getBody() const;

        static const std::string&   getShaderTypeNameByType(EShaderType type);
        static EShaderType          getShaderTypeByName(const std::string& name);

    private:

        std::string                 m_name;

        Reflection                  m_reflaction;

        union
        {
            std::string*            m_source;
            std::vector<u32>*       m_bitecode;
        };

        u64                         m_hash;

        EShaderDataRepresent        m_dataRepresent;
        EShaderType                 m_shaderType;

        static const std::string    s_shaderTypeName[EShaderType::eShaderCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace resources
} //namespace v3d

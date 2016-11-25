#pragma once

#include "common.h"
#include "renderer/ShaderDataTypes.h"
#include "renderer/Renderer.h"
#include "resources/Shader.h"
#include "renderer/Texture.h"

#ifdef USE_SPIRV

namespace v3d
{
namespace utils
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class SpirVCompileWrapper
    {
    public:

        enum ECompileError
        {
            eNoErrors = 0,
            eInvalidShaderType,
            eInvalidCompiler,
            eInvalidStage,
            eCompilationError,
            eInternalError,
            eNullResultObject,
            eInvalidAssembly
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

        SpirVCompileWrapper(renderer::ERenderType vendor, const resources::ShaderDefinesList& defines);
        ~SpirVCompileWrapper();

        ECompileError                   compile(const std::string& source, resources::EShaderType type, std::vector<u32>& bytecode);
        Reflection                      reflection(const std::vector<u32>& bytecode);

        const std::string&              errorMessages();

    private:

        std::string                     m_errors;
        resources::ShaderDefinesList    m_defines;
        renderer::ERenderType           m_vendor;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d

#endif //USE_SPIRV

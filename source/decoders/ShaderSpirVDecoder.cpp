#include "ShaderSpirVDecoder.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"
#include "Engine.h"
#include "resources/ShaderData.h"

#ifdef USE_SPIRV
#include "shaderc/shaderc.hpp"
#include "spirv_glsl.hpp"
#endif //USE_SPIRV

namespace v3d
{
namespace decoders
{
ShaderSpirVDecoder::ShaderSpirVDecoder(ESpirVResource resource, bool reflections)
    : m_resourceType(resource)
    , m_reflections(reflections)
{
}

ShaderSpirVDecoder::ShaderSpirVDecoder(std::initializer_list<std::string> supportedExtensions, ESpirVResource resource, bool reflections)
    : ResourceDecoder(supportedExtensions)
    , m_resourceType(resource)
    , m_reflections(reflections)
{
}

ShaderSpirVDecoder::~ShaderSpirVDecoder()
{
}

stream::IResource* ShaderSpirVDecoder::decode(const stream::IStreamPtr stream)
{
    if (!stream)
    {
        LOG_ERROR("ShaderSpirVDecoder::decode: Empty stream");
        return nullptr;
    }

#ifdef USE_SPIRV
    if (stream->size() > 0)
    {
        stream->seekBeg(0);

        std::string source;
        stream->read(source);

        resources::ShaderData* shaderData = nullptr;

       const  std::string& file = std::static_pointer_cast<stream::FileStream>(stream)->getName();
        bool validShaderType = true;
        bool needCompile = true;
        auto getShaderType = [&validShaderType](const std::string& name) -> shaderc_shader_kind
        {
            //

            return shaderc_shader_kind::shaderc_spirv_assembly;
        };
        shaderc_shader_kind shaderType = getShaderType(file);

        if (!validShaderType)
        {
            LOG_ERROR("ShaderSpirVDecoder::decode: Invalid shader type or unsupport");
            return nullptr;
        }

        auto getStringType = [](shaderc_shader_kind shaderType) ->std::string
        {
            switch(shaderType)
            {
            case shaderc_glsl_vertex_shader:
                return "vertex";

            case shaderc_glsl_fragment_shader:
                return "fragment";

            case shaderc_glsl_compute_shader:
                return "compute";

            case shaderc_glsl_geometry_shader:
                return "geometry";

            case shaderc_glsl_tess_control_shader:
                return "tess_control";

            case shaderc_glsl_tess_evaluation_shader:
                return "tess_eval";

            default:
                return "unkown";
            }
        };
        std::string stringType = getStringType(shaderType);

        if (needCompile)
        {
            shaderc::CompileOptions options;
            options.SetOptimizationLevel(shaderc_optimization_level_zero);
            if (ENGINE_RENDERER->getRenderType() == platform::ERenderType::eRenderVulkan)
            {
                options.SetTargetEnvironment(shaderc_target_env_vulkan, 0);
                options.SetSourceLanguage(shaderc_source_language_glsl);
            }
            else if (ENGINE_RENDERER->getRenderType() == platform::ERenderType::eRenderOpenGL)
            {
                options.SetTargetEnvironment(shaderc_target_env_opengl, 0);
                options.SetSourceLanguage(shaderc_source_language_glsl);
            }
            else if (ENGINE_RENDERER->getRenderType() == platform::ERenderType::eRenderDirect3D)
            {
                options.SetTargetEnvironment(shaderc_target_env_default, 0);
                options.SetSourceLanguage(shaderc_source_language_hlsl);
            }
            else
            {
                ASSERT(false, "unknown render");
            }

            shaderc::Compiler compiler;
            shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, shaderType, nullptr, options);
            if (!compiler.IsValid())
            {
                LOG_ERROR("ShaderSpirVDecoder::decode: CompileGlslToSpv is invalid");
                return nullptr;
            }

            shaderc_compilation_status status = result.GetCompilationStatus();
            auto getCompileError = [](shaderc_compilation_status status) -> std::string
            {
                switch (status)
                {
                case shaderc_compilation_status_invalid_stage:
                    return "shaderc_compilation_status_invalid_stage";

                case shaderc_compilation_status_compilation_error:
                    return "shaderc_compilation_status_compilation_error";

                case shaderc_compilation_status_internal_error:
                    return "shaderc_compilation_status_internal_error";

                case shaderc_compilation_status_null_result_object:
                    return "shaderc_compilation_status_null_result_object";

                case shaderc_compilation_status_invalid_assembly:
                    return "shaderc_compilation_status_invalid_assembly";

                default:
                    return "unknown";
                }
            };
            if (!status)
            {
                LOG_ERROR("ShaderSpirVDecoder::decode: Shader [%s]%s, compile error %s", stringType.c_str(), file.c_str(), getCompileError(status).c_str());
            }

            if (result.GetNumWarnings() > 0)
            {
                //TODO:
            }

            if (result.GetNumErrors() > 0)
            {
                LOG_ERROR("ShaderSpirVDecoder::decode: hader [%s]%s shader error messages %s", stringType.c_str(), file.c_str(), result.GetErrorMessage().c_str());
                return nullptr;
            }

            u32 byteCodeSize = (result.cend() - result.cbegin());
            const u32* byteCode = result.cbegin();
            stream::MemoryStreamPtr memory = stream::StreamManager::createMemoryStream(byteCode, byteCodeSize * sizeof(u32));

            if (m_reflections)
            {
                std::vector<u32> spirv(byteCodeSize);
                //std::copy(result.cbegin(), result.cend(), spirv);
                ShaderSpirVDecoder::parseReflactions(spirv, memory);
            }

            shaderData = new resources::ShaderData();
            shaderData->init(memory);
        }
        else
        {
            stream::MemoryStreamPtr memory = stream::StreamManager::createMemoryStream(source.c_str(), source.size());
            if (m_reflections)
            {
                u32 byteCodeSize = source.size() / sizeof(u32);
                std::vector<u32> spirv(byteCodeSize);
                const c8* data = source.data();
                for (u32 i = 0; i < byteCodeSize; ++i)
                {
                    spirv[i] = reinterpret_cast<u32>(data + (i * sizeof(u32)));
                }
                ShaderSpirVDecoder::parseReflactions(spirv, memory);
            }

            shaderData = new resources::ShaderData();
            shaderData->init(memory);
        }


        return shaderData;
    }
#endif //USE_SPIRV
    return nullptr;
}

void ShaderSpirVDecoder::parseReflactions(const std::vector<u32>& spirv, stream::IStreamPtr stream)
{
    spirv_cross::CompilerGLSL glsl(spirv);
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    //TODO: SPRIV-cross
}

} //namespace decoders
} //namespace v3d

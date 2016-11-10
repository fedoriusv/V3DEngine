#include "ShaderSpirVDecoder.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"
#include "Engine.h"
#include "resources/ShaderData.h"

#ifdef USE_SPIRV
#include "shaderc/shaderc.hpp"
#endif //USE_SPIRV

namespace v3d
{
namespace decoders
{
ShaderSpirVDecoder::ShaderSpirVDecoder(ESourceLanguage lang, bool reflections)
    : m_lang(lang)
    , m_reflections(reflections)
{
}

ShaderSpirVDecoder::ShaderSpirVDecoder(std::initializer_list<std::string> supportedExtensions, ESourceLanguage lang, bool reflections)
    : ResourceDecoder(supportedExtensions)
    , m_lang(lang)
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

        std::string file = std::static_pointer_cast<stream::FileStream>(stream)->getName();
        bool validShaderType = true;
        bool needCompile = true;
        auto getShaderType = [&validShaderType](const std::string& name) -> shaderc_shader_kind
        {
            //
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
            options.SetSourceLanguage(m_lang == ESourceLanguage::eHLSLLang ? shaderc_source_language_hlsl : shaderc_source_language_glsl);
            options.SetOptimizationLevel(shaderc_optimization_level_zero);
            if (ENGINE_RENDERER->getRenderType() == platform::ERenderType::eRenderVulkan)
            {
                options.SetTargetEnvironment(shaderc_target_env_vulkan, 0);
            }
            else
            {
                options.SetTargetEnvironment(shaderc_target_env_opengl, 0);
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

            u32 byteCodeSize = (result.cend() - result.cbegin()) * sizeof(u32);
            const u32* byteCode = result.cbegin();
            stream::MemoryStreamPtr spirv = stream::StreamManager::createMemoryStream(byteCode, byteCodeSize);

            if (m_reflections)
            {
                shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(source, shaderType, nullptr, options);
                if (!compiler.IsValid())
                {
                    LOG_ERROR("ShaderSpirVDecoder::decode: CompileGlslToSpvAssembly is invalid");
                    return nullptr;
                }

                shaderc_compilation_status status = result.GetCompilationStatus();
                if (!status)
                {
                    LOG_ERROR("ShaderSpirVDecoder::decode: Shader [%s]%s, Assembly error %s", stringType.c_str(), file.c_str(), getCompileError(status).c_str());
                }

                //TODO: check on error and log

                u32 assemblySize = (result.cend() - result.cbegin());
                std::string assembly(result.cbegin(), assemblySize);

                ShaderSpirVDecoder::parseReflactions(assembly, spirv);
            }

            shaderData = new resources::ShaderData();
            shaderData->init(spirv);
        }
        else
        {
            stream::MemoryStreamPtr spirv = stream::StreamManager::createMemoryStream(source.c_str(), source.size());
            if (m_reflections)
            {
                //TODO:
            }

            shaderData = new resources::ShaderData();
            shaderData->init(spirv);
        }


        return shaderData;
    }
#endif //USE_SPIRV
    return nullptr;
}

void ShaderSpirVDecoder::parseReflactions(const std::string& assembly, stream::IStreamPtr stream)
{
    //TODO: SPRIV-cross
}

} //namespace decoders
} //namespace v3d

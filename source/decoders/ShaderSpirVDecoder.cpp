#include "ShaderSpirVDecoder.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"
#include "Engine.h"
#include "resources/ShaderResource.h"

#ifdef USE_SPIRV
#include "shaderc/shaderc.hpp"
#include "spirv_glsl.hpp"
#endif //USE_SPIRV

namespace v3d
{
namespace decoders
{

using namespace renderer;
using namespace resources;

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

        ShaderResource* shaderData = nullptr;

       const  std::string& file = std::static_pointer_cast<stream::FileStream>(stream)->getName();
        bool validShaderType = true;
        auto getShaderType = [&validShaderType](const std::string& name) -> shaderc_shader_kind
        {
            std::string fileExtension = "";

            const size_t pos = name.find_last_of('.');
            if (pos != std::string::npos)
            {
                fileExtension = std::string(name.begin() + pos + 1, name.end());
            }

            if (fileExtension == "vert")
            {
                return shaderc_shader_kind::shaderc_glsl_vertex_shader;
            }
            else if (fileExtension == "frag")
            {
                return shaderc_shader_kind::shaderc_glsl_fragment_shader;
            }
            else if (fileExtension == "geom")
            {
                return shaderc_shader_kind::shaderc_glsl_geometry_shader;
            }
            else if (fileExtension == "comp")
            {
                return shaderc_shader_kind::shaderc_glsl_compute_shader;
            }
            else if (fileExtension == "tesc")
            {
                return shaderc_shader_kind::shaderc_glsl_tess_control_shader;
            }
            else if (fileExtension == "tese")
            {
                return shaderc_shader_kind::shaderc_glsl_tess_evaluation_shader;
            }

            validShaderType = false;
            return shaderc_shader_kind::shaderc_spirv_assembly;
        };

        shaderc_shader_kind shaderType = getShaderType(file);

        if (!validShaderType)
        {
            LOG_ERROR("ShaderSpirVDecoder::decode: Invalid shader type or unsupport");
            return nullptr;
        }

        auto getStringType = [](shaderc_shader_kind shaderType) -> std::string
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
                return "unknown";
            }
        };
        std::string stringType = getStringType(shaderType);

        if (m_resourceType == ESpirVResource::eSpirVSource)
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
            shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, shaderType, stringType.c_str(), options);
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

            if (status != shaderc_compilation_status_success)
            {
                LOG_ERROR("ShaderSpirVDecoder::decode: Shader [%s]%s, compile error %s", stringType.c_str(), file.c_str(), getCompileError(status).c_str());
            }

            if (result.GetNumErrors() > 0)
            {
                LOG_ERROR("ShaderSpirVDecoder::decode: header [%s]%s shader error messages:\n%s", stringType.c_str(), file.c_str(), result.GetErrorMessage().c_str());
                return nullptr;
            }

            if (result.GetNumWarnings() > 0)
            {
                LOG_WARNING("ShaderSpirVDecoder::decode: header [%s]%s shader warnings messages:\n%s", stringType.c_str(), file.c_str(), result.GetErrorMessage().c_str());
            }

            u64 byteCodeSize = (result.cend() - result.cbegin());
            u64 byteCodeSizeInByte = byteCodeSize * sizeof(u32);
            const u32* byteCode = result.cbegin();

            stream::IStreamPtr memory = stream::StreamManager::createMemoryStream();
            auto getInnerShaderType = [](shaderc_shader_kind shaderType) -> resources::EShaderType
            {
                switch (shaderType)
                {
                case shaderc_shader_kind::shaderc_glsl_vertex_shader:
                    return resources::EShaderType::eVertex;

                case shaderc_shader_kind::shaderc_glsl_fragment_shader:
                    return resources::EShaderType::eFragment;

                case shaderc_shader_kind::shaderc_glsl_geometry_shader:
                    return resources::EShaderType::eGeometry;

                case shaderc_shader_kind::shaderc_glsl_tess_control_shader:
                    return resources::EShaderType::eTessellationControl;

                case shaderc_shader_kind::shaderc_glsl_tess_evaluation_shader:
                    return resources::EShaderType::eTessellationEvaluation;

                case shaderc_shader_kind::shaderc_glsl_compute_shader:
                    return resources::EShaderType::eCompute;

                default:
                    return resources::EShaderType::eShaderUnknown;
                }
            };
            resources::EShaderType innerShaderType = getInnerShaderType(shaderType);
            memory->write<resources::EShaderType>(innerShaderType);

            u16 represent = static_cast<u16>(ShaderResource::eBitecodeSpirV);
            memory->write<u16>(represent);

            memory->write<u64>(byteCodeSizeInByte);
            memory->write(byteCode, byteCodeSize * sizeof(u32), 1);
            memory->write<bool>(m_reflections);

            if (m_reflections)
            {
                std::vector<u32> spirv(byteCodeSize);
                std::copy(result.cbegin(), result.cend(), spirv.begin());
                ShaderSpirVDecoder::parseReflactions(spirv, memory);
            }

            shaderData = new ShaderResource();
            shaderData->init(memory);
        }
        else
        {
            stream::IStreamPtr memory = stream::StreamManager::createMemoryStream();
            u16 represent = static_cast<u16>(ShaderResource::eSourceGLSL);
            memory->write<u16>(represent);
            memory->write(source);
            memory->write<bool>(m_reflections);

            if (m_reflections)
            {
                u64 byteCodeSize = source.size() / sizeof(u32);
                std::vector<u32> spirv(byteCodeSize);
                memcpy(&spirv[0], source.data(), source.size());
                ShaderSpirVDecoder::parseReflactions(spirv, memory);
            }

            shaderData = new ShaderResource();
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

    auto getInnerDataType = [](const spirv_cross::SPIRType& type) -> ShaderDataType::EShaderDataType
    {
        switch (type.basetype)
        {
        case spirv_cross::SPIRType::Boolean:
        case spirv_cross::SPIRType::Int:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return ShaderDataType::eInt;
                }
                else if (type.vecsize == 2)
                {
                    return ShaderDataType::eVector2i;
                }
                else if (type.vecsize == 3)
                {
                    return ShaderDataType::eVector3i;
                }
                else if (type.vecsize == 4)
                {
                    return ShaderDataType::eVector4i;
                }
            }
            return ShaderDataType::eUnknown;

        case spirv_cross::SPIRType::UInt:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return ShaderDataType::eUint;
                }
                else if (type.vecsize == 2)
                {
                    return ShaderDataType::eVector2u;
                }
                else if (type.vecsize == 3)
                {
                    return ShaderDataType::eVector3u;
                }
                else if (type.vecsize == 4)
                {
                    return ShaderDataType::eVector4u;
                }
            }
            return ShaderDataType::eUnknown;

        case spirv_cross::SPIRType::Int64:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return ShaderDataType::eInt64;
                }
                else if (type.vecsize == 2)
                {
                    return ShaderDataType::eVector2u64;
                }
                else if (type.vecsize == 3)
                {
                    return ShaderDataType::eVector3u64;
                }
                else if (type.vecsize == 4)
                {
                    return ShaderDataType::eVector4u64;
                }
            }
            return ShaderDataType::eUnknown;

        case spirv_cross::SPIRType::UInt64:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return ShaderDataType::eUint64;
                }
                else if (type.vecsize == 2)
                {
                    return ShaderDataType::eVector2u64;
                }
                else if (type.vecsize == 3)
                {
                    return ShaderDataType::eVector3u64;
                }
                else if (type.vecsize == 4)
                {
                    return ShaderDataType::eVector4u64;
                }
            }
            return ShaderDataType::eUnknown;

        case spirv_cross::SPIRType::Float:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return ShaderDataType::eFloat;
                }
                else if (type.vecsize == 2)
                {
                    return ShaderDataType::eVector2f;
                }
                else if (type.vecsize == 3)
                {
                    return ShaderDataType::eVector3f;
                }
                else if (type.vecsize == 4)
                {
                    return ShaderDataType::eVector4f;
                }
            }
            else if (type.columns == 3 && type.vecsize == 3)
            {
                return ShaderDataType::eMatrix3f;
            }
            else if (type.columns == 4 && type.vecsize == 4)
            {
                return ShaderDataType::eMatrix4f;
            }
            return ShaderDataType::eUnknown;

        case spirv_cross::SPIRType::Double:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return ShaderDataType::eDouble;
                }
                else if (type.vecsize == 2)
                {
                    return ShaderDataType::eVector2d;
                }
                else if (type.vecsize == 3)
                {
                    return ShaderDataType::eVector3d;
                }
                else if (type.vecsize == 4)
                {
                    return ShaderDataType::eVector4d;
                }
            }
            else if (type.columns == 3 && type.vecsize == 3)
            {
                return ShaderDataType::eMatrix3d;
            }
            else if (type.columns == 4 && type.vecsize == 4)
            {
                return ShaderDataType::eMatrix4d;
            }
            return ShaderDataType::eUnknown;

        case spirv_cross::SPIRType::Char:
        case spirv_cross::SPIRType::Unknown:
        case spirv_cross::SPIRType::Void:
        case spirv_cross::SPIRType::AtomicCounter:
        case spirv_cross::SPIRType::Struct:
        case spirv_cross::SPIRType::Image:
        case spirv_cross::SPIRType::SampledImage:
        case spirv_cross::SPIRType::Sampler:
        default:
            return ShaderDataType::eUnknown;
        };

        return ShaderDataType::eUnknown;
    };

    auto getInnerTextureTarget = [](const spirv_cross::SPIRType& type) -> ETextureTarget
    {
        switch (type.image.dim)
        {
        case spv::Dim1D:
            if (type.image.arrayed)
            {
                return ETextureTarget::eTexture1DArray;
            }
            else
            {
                return ETextureTarget::eTexture1D;
            }

        case spv::Dim2D:
            if (type.image.arrayed)
            {
                return ETextureTarget::eTexture2DArray;
            }
            else
            {
                if (type.image.ms)
                {
                    return ETextureTarget::eTexture2DMSAA;
                }
                return ETextureTarget::eTexture2D;
            }

        case spv::Dim3D:
            if (type.image.ms)
            {
                return ETextureTarget::eTexture3DMSAA;
            }
            return ETextureTarget::eTexture3D;

        case spv::DimCube:
            if (type.image.arrayed)
            {
                return ETextureTarget::eTextureCubeMapArray;
            }
            else
            {
                return ETextureTarget::eTextureCubeMap;
            }

        case spv::DimRect:
            return ETextureTarget::eTextureRectangle;

        case spv::DimBuffer:
            return ETextureTarget::eTextureBuffer;

        default:
            return ETextureTarget::eTextureNull;
        }

        return ETextureTarget::eTextureNull;
    };

    u32 inputChannelCount = static_cast<u32>(resources.stage_inputs.size());
    stream->write<u32>(inputChannelCount);
    for (auto& inputChannel : resources.stage_inputs)
    {
        const std::string& name = glsl.get_name(inputChannel.id);
        ASSERT(!name.empty(), "empty name");

        u32 location = glsl.get_decoration(inputChannel.id, spv::DecorationLocation);

        const spirv_cross::SPIRType& type = glsl.get_type(inputChannel.type_id);
        ShaderDataType::EShaderDataType innerType = getInnerDataType(type);
        u32 col = type.columns;
        u32 row = type.vecsize;

        stream->write(name);
        stream->write<u32>(location);
        stream->write<ShaderDataType::EShaderDataType>(innerType);
        stream->write<u32>(col);
        stream->write<u32>(row);
    }

    u32 outputChannelCount = static_cast<u32>(resources.stage_outputs.size());
    stream->write<u32>(outputChannelCount);
    for (auto& outputChannel : resources.stage_outputs)
    {
        const std::string& name = glsl.get_name(outputChannel.id);
        ASSERT(!name.empty(), "empty name");

        u32 location = glsl.get_decoration(outputChannel.id, spv::DecorationLocation);

        const spirv_cross::SPIRType& type = glsl.get_type(outputChannel.type_id);
        ShaderDataType::EShaderDataType innerType = getInnerDataType(type);
        u32 col = type.columns;
        u32 row = type.vecsize;

        stream->write(name);
        stream->write<u32>(location);
        stream->write<ShaderDataType::EShaderDataType>(innerType);
        stream->write<u32>(col);
        stream->write<u32>(row);
    }

    u32 unifromBufferCount = static_cast<u32>(resources.uniform_buffers.size());
    stream->write<u32>(unifromBufferCount);
    s32 buffID = 0;
    for (auto& buffer : resources.uniform_buffers)
    {
        const std::string& name = glsl.get_name(buffer.id);
        ASSERT(!name.empty(), "empty name");

        u32 binding = glsl.get_decoration(buffer.id, spv::DecorationBinding);
        u32 set = glsl.get_decoration(buffer.id, spv::DecorationDescriptorSet);
        const spirv_cross::SPIRType& block_type = glsl.get_type(buffer.type_id);

        stream->write<s32>(buffID);
        stream->write(name);
        stream->write<u32>(set);
        stream->write<u32>(binding);

        u32 posMembers = stream->tell();

        u32 countMembers = 0;
        stream->write<u32>(countMembers);

        u32 index = 0;
        while (true)
        {
            const std::string& member_name = glsl.get_member_name(buffer.base_type_id, index);
            if (member_name.empty())
            {
                break;
            }
            const spirv_cross::SPIRType& type = glsl.get_type(block_type.member_types[index]);
            ShaderDataType::EShaderDataType innerType = getInnerDataType(type);
            u32 col = type.columns;
            u32 row = type.vecsize;
            ++index;

            stream->write<s32>(buffID);
            stream->write(member_name);
            stream->write<ShaderDataType::EShaderDataType>(innerType);
            stream->write<u32>(col);
            stream->write<u32>(row);
        }

        u32 posCurr = stream->tell();
        countMembers = index;
        stream->seekBeg(posMembers);
        stream->write<u32>(countMembers);
        stream->seekBeg(posCurr);

        ++buffID;
    }

    u32 samplersCount = static_cast<u32>(resources.sampled_images.size());
    stream->write<u32>(samplersCount);
    for (auto& image : resources.sampled_images)
    {
        const std::string& name = glsl.get_name(image.id);
        ASSERT(!name.empty(), "empty name");

        u32 binding = glsl.get_decoration(image.id, spv::DecorationBinding);
        u32 set = glsl.get_decoration(image.id, spv::DecorationDescriptorSet);

        const spirv_cross::SPIRType& type = glsl.get_type(image.type_id);
        ETextureTarget innerType = getInnerTextureTarget(type);
        bool depth = type.image.depth;

        stream->write(name);
        stream->write<u32>(set);
        stream->write<u32>(binding);
        stream->write<ETextureTarget>(innerType);
        stream->write<bool>(depth);
    }
}

} //namespace decoders
} //namespace v3d

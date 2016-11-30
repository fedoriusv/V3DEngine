#include "SpirVCompiler.h"

#ifdef USE_SPIRV
#include "shaderc/shaderc.hpp"
#include "spirv_glsl.hpp"

namespace v3d
{
namespace utils
{

SpirVCompileWrapper::SpirVCompileWrapper(renderer::ERenderType vendor, const resources::ShaderDefinesList& defines)
    : m_defines(defines)
    , m_vendor(vendor)
{
}

SpirVCompileWrapper::~SpirVCompileWrapper()
{
}

SpirVCompileWrapper::ECompileError SpirVCompileWrapper::compile(const std::string& source, resources::EShaderType type, std::vector<u32>& bytecode)
{
    m_errors.clear();

    bool validShaderType = true;
    auto getShaderType = [&validShaderType](resources::EShaderType type) -> shaderc_shader_kind
    {
        switch (type)
        {
        case resources::EShaderType::eVertex:
            return shaderc_shader_kind::shaderc_glsl_vertex_shader;
        case resources::EShaderType::eFragment:
            return shaderc_shader_kind::shaderc_glsl_fragment_shader;
        case resources::EShaderType::eGeometry:
            return shaderc_shader_kind::shaderc_glsl_geometry_shader;
        case resources::EShaderType::eCompute:
            return shaderc_shader_kind::shaderc_glsl_compute_shader;
        case resources::EShaderType::eTessellationControl:
            return shaderc_shader_kind::shaderc_glsl_tess_control_shader;
        case resources::EShaderType::eTessellationEvaluation:
            return shaderc_shader_kind::shaderc_glsl_tess_evaluation_shader;
        default:
            return shaderc_shader_kind::shaderc_spirv_assembly;
        }

        validShaderType = false;
        return shaderc_shader_kind::shaderc_spirv_assembly;
    };

    shaderc_shader_kind shaderType = getShaderType(type);
    if (!validShaderType)
    {
        return SpirVCompileWrapper::eInvalidShaderType;
    }

    shaderc::CompileOptions options;
    options.SetOptimizationLevel(shaderc_optimization_level_zero);
    if (m_vendor == renderer::ERenderType::eRenderVulkan)
    {
        options.SetTargetEnvironment(shaderc_target_env_vulkan, 0);
        options.SetSourceLanguage(shaderc_source_language_glsl);
    }
    else if (m_vendor == renderer::ERenderType::eRenderOpenGL)
    {
        options.SetTargetEnvironment(shaderc_target_env_opengl, 0);
        options.SetSourceLanguage(shaderc_source_language_glsl);
    }
    else if (m_vendor == renderer::ERenderType::eRenderDirect3D)
    {
        options.SetTargetEnvironment(shaderc_target_env_default, 0);
        options.SetSourceLanguage(shaderc_source_language_hlsl);
    }
    else
    {
        ASSERT(false, "unknown render");
    }

    for (auto& define : m_defines)
    {
        options.AddMacroDefinition(define.first, define.second);
    }

    shaderc::Compiler compiler;
    shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, shaderType, "shader", options);
    if (!compiler.IsValid())
    {
        return SpirVCompileWrapper::eInvalidCompiler;
    }

    shaderc_compilation_status status = result.GetCompilationStatus();
    auto getCompileError = [](shaderc_compilation_status status) -> ECompileError
    {
        switch (status)
        {
        case shaderc_compilation_status_invalid_stage:
            return SpirVCompileWrapper::eInvalidStage;

        case shaderc_compilation_status_compilation_error:
            return SpirVCompileWrapper::eCompilationError;

        case shaderc_compilation_status_internal_error:
            return SpirVCompileWrapper::eInternalError;

        case shaderc_compilation_status_null_result_object:
            return SpirVCompileWrapper::eNullResultObject;

        case shaderc_compilation_status_invalid_assembly:
            return SpirVCompileWrapper::eInvalidAssembly;

        default:
            return SpirVCompileWrapper::eInvalidCompiler;
        }
    };

    if (status != shaderc_compilation_status_success)
    {
        if (result.GetNumErrors() > 0 || result.GetNumWarnings() > 0)
        {
            m_errors = result.GetErrorMessage();
        }

        return getCompileError(status);
    }

    u64 byteCodeSize = (result.cend() - result.cbegin());

    bytecode.clear();
    bytecode.resize(byteCodeSize);
    std::copy(result.cbegin(), result.cend(), bytecode.begin());

    return SpirVCompileWrapper::eNoErrors;
}

renderer::ShaderProgram::ShaderParameters SpirVCompileWrapper::reflection(const std::vector<u32>& bytecode)
{
    spirv_cross::CompilerGLSL glsl(bytecode);
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    auto getInnerDataType = [](const spirv_cross::SPIRType& type) -> renderer::ShaderDataType::EDataType
    {
        switch (type.basetype)
        {
        case spirv_cross::SPIRType::Boolean:
        case spirv_cross::SPIRType::Int:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return renderer::ShaderDataType::EDataType::eInt;
                }
                else if (type.vecsize == 2)
                {
                    return renderer::ShaderDataType::EDataType::eVector2i;
                }
                else if (type.vecsize == 3)
                {
                    return renderer::ShaderDataType::EDataType::eVector3i;
                }
                else if (type.vecsize == 4)
                {
                    return renderer::ShaderDataType::EDataType::eVector4i;
                }
            }
            return renderer::ShaderDataType::EDataType::eUnknown;

        case spirv_cross::SPIRType::UInt:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return renderer::ShaderDataType::EDataType::eUint;
                }
                else if (type.vecsize == 2)
                {
                    return renderer::ShaderDataType::EDataType::eVector2u;
                }
                else if (type.vecsize == 3)
                {
                    return renderer::ShaderDataType::EDataType::eVector3u;
                }
                else if (type.vecsize == 4)
                {
                    return renderer::ShaderDataType::EDataType::eVector4u;
                }
            }
            return renderer::ShaderDataType::EDataType::eUnknown;

        case spirv_cross::SPIRType::Int64:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return renderer::ShaderDataType::EDataType::eInt64;
                }
                else if (type.vecsize == 2)
                {
                    return renderer::ShaderDataType::EDataType::eVector2u64;
                }
                else if (type.vecsize == 3)
                {
                    return renderer::ShaderDataType::EDataType::eVector3u64;
                }
                else if (type.vecsize == 4)
                {
                    return renderer::ShaderDataType::EDataType::eVector4u64;
                }
            }
            return renderer::ShaderDataType::EDataType::eUnknown;

        case spirv_cross::SPIRType::UInt64:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return renderer::ShaderDataType::EDataType::eUint64;
                }
                else if (type.vecsize == 2)
                {
                    return renderer::ShaderDataType::EDataType::eVector2u64;
                }
                else if (type.vecsize == 3)
                {
                    return renderer::ShaderDataType::EDataType::eVector3u64;
                }
                else if (type.vecsize == 4)
                {
                    return renderer::ShaderDataType::EDataType::eVector4u64;
                }
            }
            return renderer::ShaderDataType::EDataType::eUnknown;

        case spirv_cross::SPIRType::Float:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return renderer::ShaderDataType::EDataType::eFloat;
                }
                else if (type.vecsize == 2)
                {
                    return renderer::ShaderDataType::EDataType::eVector2f;
                }
                else if (type.vecsize == 3)
                {
                    return renderer::ShaderDataType::EDataType::eVector3f;
                }
                else if (type.vecsize == 4)
                {
                    return renderer::ShaderDataType::EDataType::eVector4f;
                }
            }
            else if (type.columns == 3 && type.vecsize == 3)
            {
                return renderer::ShaderDataType::EDataType::eMatrix3f;
            }
            else if (type.columns == 4 && type.vecsize == 4)
            {
                return renderer::ShaderDataType::EDataType::eMatrix4f;
            }
            return renderer::ShaderDataType::EDataType::eUnknown;

        case spirv_cross::SPIRType::Double:
            if (type.columns == 1)
            {
                if (type.vecsize == 1)
                {
                    return renderer::ShaderDataType::EDataType::eDouble;
                }
                else if (type.vecsize == 2)
                {
                    return renderer::ShaderDataType::EDataType::eVector2d;
                }
                else if (type.vecsize == 3)
                {
                    return renderer::ShaderDataType::EDataType::eVector3d;
                }
                else if (type.vecsize == 4)
                {
                    return renderer::ShaderDataType::EDataType::eVector4d;
                }
            }
            else if (type.columns == 3 && type.vecsize == 3)
            {
                return renderer::ShaderDataType::EDataType::eMatrix3d;
            }
            else if (type.columns == 4 && type.vecsize == 4)
            {
                return renderer::ShaderDataType::EDataType::eMatrix4d;
            }
            return renderer::ShaderDataType::EDataType::eUnknown;

        case spirv_cross::SPIRType::Char:
        case spirv_cross::SPIRType::Unknown:
        case spirv_cross::SPIRType::Void:
        case spirv_cross::SPIRType::AtomicCounter:
        case spirv_cross::SPIRType::Struct:
        case spirv_cross::SPIRType::Image:
        case spirv_cross::SPIRType::SampledImage:
        case spirv_cross::SPIRType::Sampler:
        default:
            return renderer::ShaderDataType::EDataType::eUnknown;
        };

        return renderer::ShaderDataType::EDataType::eUnknown;
    };

    auto getInnerTextureTarget = [](const spirv_cross::SPIRType& type) -> renderer::ETextureTarget
    {
        switch (type.image.dim)
        {
        case spv::Dim1D:
            if (type.image.arrayed)
            {
                return renderer::ETextureTarget::eTexture1DArray;
            }
            else
            {
                return renderer::ETextureTarget::eTexture1D;
            }

        case spv::Dim2D:
            if (type.image.arrayed)
            {
                return renderer::ETextureTarget::eTexture2DArray;
            }
            else
            {
                if (type.image.ms)
                {
                    return renderer::ETextureTarget::eTexture2DMSAA;
                }
                return renderer::ETextureTarget::eTexture2D;
            }

        case spv::Dim3D:
            if (type.image.ms)
            {
                return renderer::ETextureTarget::eTexture3DMSAA;
            }
            return renderer::ETextureTarget::eTexture3D;

        case spv::DimCube:
            if (type.image.arrayed)
            {
                return renderer::ETextureTarget::eTextureCubeMapArray;
            }
            else
            {
                return renderer::ETextureTarget::eTextureCubeMap;
            }

        case spv::DimRect:
            return renderer::ETextureTarget::eTextureRectangle;

        case spv::DimBuffer:
            return renderer::ETextureTarget::eTextureBuffer;

        default:
            return renderer::ETextureTarget::eTextureNull;
        }

        return renderer::ETextureTarget::eTextureNull;
    };

    renderer::ShaderProgram::ShaderParameters reflection;
    for (auto& inputChannel : resources.stage_inputs)
    {
        const std::string& name = glsl.get_name(inputChannel.id);
        ASSERT(!name.empty(), "empty name");

        u32 location = glsl.get_decoration(inputChannel.id, spv::DecorationLocation);

        const spirv_cross::SPIRType& type = glsl.get_type(inputChannel.type_id);
        renderer::ShaderDataType::EDataType innerType = getInnerDataType(type);
        u32 col = type.columns;
        u32 row = type.vecsize;

        renderer::ShaderProgram::ShaderParameters::Channel channel;
        channel.name = name;
        channel.location = location;
        channel.type = innerType;
        channel.colCount = col;
        channel.rowCount = row;

        reflection.channelsIn.insert(std::make_pair(name, channel));
    }

    for (auto& outputChannel : resources.stage_outputs)
    {
        const std::string& name = glsl.get_name(outputChannel.id);
        ASSERT(!name.empty(), "empty name");

        u32 location = glsl.get_decoration(outputChannel.id, spv::DecorationLocation);

        const spirv_cross::SPIRType& type = glsl.get_type(outputChannel.type_id);
        renderer::ShaderDataType::EDataType innerType = getInnerDataType(type);
        u32 col = type.columns;
        u32 row = type.vecsize;

        renderer::ShaderProgram::ShaderParameters::Channel channel;
        channel.name = name;
        channel.location = location;
        channel.type = innerType;
        channel.colCount = col;
        channel.rowCount = row;

        reflection.channelsOut.insert(std::make_pair(name, channel));
    }

    s32 buffID = 0;
    for (auto& buffer : resources.uniform_buffers)
    {
        const std::string& name = glsl.get_name(buffer.id);
        ASSERT(!name.empty(), "empty name");

        u32 binding = glsl.get_decoration(buffer.id, spv::DecorationBinding);
        u32 set = glsl.get_decoration(buffer.id, spv::DecorationDescriptorSet);
        const spirv_cross::SPIRType& block_type = glsl.get_type(buffer.type_id);

        u32 index = 0;
        u32 block_size = 0;
        u32 offset = 0;
        while (true)
        {
            const std::string& member_name = glsl.get_member_name(buffer.base_type_id, index);
            if (member_name.empty())
            {
                break;
            }
            const spirv_cross::SPIRType& type = glsl.get_type(block_type.member_types[index]);
            renderer::ShaderDataType::EDataType innerType = getInnerDataType(type);
            u32 col = type.columns;
            u32 row = type.vecsize;
            u32 array = std::accumulate(type.array.cbegin(), type.array.cend(), 1);
            u32 size = (type.width / 8) * array * col * row;

            renderer::ShaderProgram::ShaderParameters::UniformParameter uniform;
            uniform.block = buffID;
            uniform.name = name;
            uniform.index = index;
            uniform.type = innerType;
            uniform.colCount = col;
            uniform.rowCount = row;
            uniform.array = array;
            uniform.size = size;
            uniform.offset = offset;

            reflection.uniforms.insert(std::make_pair(name, uniform));

            block_size += size;
            offset += size;

            ++index;
        }

        renderer::ShaderProgram::ShaderParameters::UniformBlockParameter block;
        block.id = buffID;
        block.name = name;
        block.set = set;
        block.binding = binding;
        block.size = block_size;

        reflection.constantBuffers.push_back(block);

        ++buffID;
    }

    for (auto& image : resources.sampled_images)
    {
        const std::string& name = glsl.get_name(image.id);
        ASSERT(!name.empty(), "empty name");

        u32 binding = glsl.get_decoration(image.id, spv::DecorationBinding);
        u32 set = glsl.get_decoration(image.id, spv::DecorationDescriptorSet);

        const spirv_cross::SPIRType& type = glsl.get_type(image.type_id);
        renderer::ETextureTarget innerType = getInnerTextureTarget(type);
        bool depth = type.image.depth;

        renderer::ShaderProgram::ShaderParameters::TextureParameter sampler;
        sampler.name = name;
        sampler.set = set;
        sampler.binding = binding;
        sampler.target = innerType;
        sampler.depth = depth;

        reflection.samplers.insert(std::make_pair(name, sampler));
    }

    return reflection;
}

const std::string& SpirVCompileWrapper::errorMessages()
{
    return m_errors;
}

} //namespace utils
} //namespace v3d

#endif //USE_SPIRV

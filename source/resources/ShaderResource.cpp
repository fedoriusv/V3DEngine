#include "ShaderResource.h"
#include "utils/Logger.h"

namespace v3d
{
namespace resources
{

using namespace renderer;

/////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string ShaderResource::s_shaderTypeName[EShaderType::eShaderCount] =
{
    "vertex",
    "fragment",
    "geometry",
    "control",
    "evaluation",
    "compute"
};

const std::string& ShaderResource::getShaderTypeNameByType(EShaderType type)
{
    return s_shaderTypeName[type];
}

EShaderType ShaderResource::getShaderTypeByName(const std::string& name)
{
    for (u32 i = 0; i < EShaderType::eShaderCount; ++i)
    {
        if (s_shaderTypeName[i].compare(name) == 0)
        {
            return (EShaderType)i;
        }
    }

    return EShaderType::eShaderUnknown;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

ShaderResource::ShaderResource()
    : m_name("")
    , m_source(nullptr)
    , m_bytecode(nullptr)
    , m_hash(0)
    , m_dataRepresent(EShaderDataRepresent::eBytecode)
    , m_shaderType(EShaderType::eShaderUnknown)
{
}

ShaderResource::~ShaderResource()
{
}

void ShaderResource::init(const stream::IStreamPtr& stream)
{
    IResource::setStream(stream);
}

bool ShaderResource::load()
{
    const stream::IStreamPtr& stream = IResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CShaderSourceData::load: Empty Stream with name [%s]", IResource::getResourseName().c_str());
        return false;
    }

    stream->seekBeg(0);

    stream->read<EShaderType>(m_shaderType);
    stream->read<EShaderDataRepresent>(m_dataRepresent);

    if (m_dataRepresent == EShaderDataRepresent::eBytecode || m_dataRepresent == EShaderDataRepresent::eBytecodeSpirV)
    {
        u64 size = 0;
        stream->read<u64>(size);

        m_bytecode = new std::vector<u32>(size);
        stream->read(m_bytecode->data(), size, 1);
    }
    else
    {
        std::string source;
        stream->read(source);

        m_source = new std::string(std::move(source));
    }

    bool reflection = false;
    stream->read<bool>(reflection);

    if (reflection)
    {
        //channels in
        u32 countChannelsIn = 0;
        stream->read<u32>(countChannelsIn);

        if (countChannelsIn > 0)
        {
            m_reflaction.channelsIn.resize(countChannelsIn);
            for (u32 channelIndex = 0; channelIndex < countChannelsIn; ++channelIndex)
            {
                Reflection::Channel& channel = m_reflaction.channelsIn[channelIndex];
                stream->read(channel.name);
                stream->read<u32>(channel.location);
                stream->read<ShaderDataType::EShaderDataType>(channel.type);
                stream->read<u32>(channel.colCount);
                stream->read<u32>(channel.rowCount);
            }
        }

        //channels out
        u32 countChannelsOut = 0;
        stream->read<u32>(countChannelsOut);

        if (countChannelsOut > 0)
        {
            m_reflaction.channelsOut.resize(countChannelsOut);
            for (u32 channelIndex = 0; channelIndex < countChannelsOut; ++channelIndex)
            {
                Reflection::Channel& channel = m_reflaction.channelsOut[channelIndex];
                stream->read(channel.name);
                stream->read<u32>(channel.location);
                stream->read<ShaderDataType::EShaderDataType>(channel.type);
                stream->read<u32>(channel.colCount);
                stream->read<u32>(channel.rowCount);
            }
        }

        //blocks
        u32 countBlocks = 0;
        stream->read<u32>(countBlocks);

        if (countBlocks > 0)
        {
            m_reflaction.constantBuffers.resize(countBlocks);
            for (u32 blockIndex = 0; blockIndex < countBlocks; ++blockIndex)
            {
                Reflection::UniformBlockParameter& block = m_reflaction.constantBuffers[blockIndex];
                stream->read<s32>(block.id);
                stream->read(block.name);
                stream->read<u32>(block.set);
                stream->read<u32>(block.binding);

                u32 membersCount;
                stream->read<u32>(membersCount);

                for (u32 memberIndex = 0; memberIndex < membersCount; ++memberIndex)
                {
                    Reflection::UniformParameter uniform;

                    stream->read<s32>(uniform.block);
                    stream->read(uniform.name);
                    stream->read<ShaderDataType::EShaderDataType>(uniform.type);
                    stream->read<u32>(uniform.colCount);
                    stream->read<u32>(uniform.rowCount);

                    m_reflaction.uniforms.push_back(uniform);
                }
            }
        }

        //samplers
        u32 countSamplers = 0;
        stream->read<u32>(countSamplers);

        if (countSamplers > 0)
        {
            m_reflaction.samplers.resize(countSamplers);
            for (u32 samplerIndex = 0; samplerIndex < countSamplers; ++samplerIndex)
            {
                Reflection::TextureParameter& sampler = m_reflaction.samplers[samplerIndex];
                stream->read(sampler.name);
                stream->read<u32>(sampler.set);
                stream->read<u32>(sampler.binding);
                stream->read<ETextureTarget>(sampler.target);
                stream->read<bool>(sampler.depth);
            }
        }
    }

    return true;
}


const std::string& ShaderResource::getName() const
{
    return m_name;
}

const std::string& ShaderResource::getBody() const
{
    return "";
}

} //namespace resources
} //namespace v3d

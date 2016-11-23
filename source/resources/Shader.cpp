#include "Shader.h"
#include "utils/Logger.h"

namespace v3d
{
namespace resources
{

using namespace renderer;

/////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string Shader::s_shaderTypeName[EShaderType::eShaderCount] =
{
    "vertex",
    "fragment",
    "geometry",
    "control",
    "evaluation",
    "compute"
};

const std::string& Shader::getShaderTypeNameByType(EShaderType type)
{
    return s_shaderTypeName[type];
}

EShaderType Shader::getShaderTypeByName(const std::string& name)
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

Shader::Shader()
    : m_name("")
    , m_source(nullptr)
    , m_bytecode(nullptr)

    , m_dataRepresent(EShaderDataRepresent::eBytecode)
    , m_shaderType(EShaderType::eShaderUnknown)
{
}

Shader::~Shader()
{
    if (m_dataRepresent == EShaderDataRepresent::eBytecode || m_dataRepresent == EShaderDataRepresent::eBytecodeSpirV)
    {
        delete m_bytecode;
        m_bytecode = nullptr;
    }
    else
    {
        delete m_source;
        m_source = nullptr;
    }
}

void Shader::init(const stream::IStreamPtr stream)
{
    IResource::setStream(stream);
}

bool Shader::load()
{
    const stream::IStreamPtr stream = IResource::getStream();
    if (!stream)
    {
        LOG_ERROR("Shader::load: Empty Stream with name [%s]", IResource::getResourseName().c_str());
        return false;
    }

    m_name = IResource::getResourseName();

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

    IResource::setStream(nullptr);
    IResource::setLoaded(true);

    return true;
}


const std::string& Shader::getName() const
{
    return m_name;
}

const std::string& Shader::getSource() const
{
    ASSERT(m_source, "invalid data");
    return *m_source;
}

const std::vector<u32>& Shader::getBytecode() const
{
    ASSERT(m_bytecode, "invalid data");
    return *m_bytecode;
}

Shader::EShaderDataRepresent Shader::getShaderKind() const
{
    return m_dataRepresent;
}

} //namespace resources
} //namespace v3d

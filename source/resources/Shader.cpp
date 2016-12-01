#include "Shader.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

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

Shader::Shader(EShaderType type, const std::string& source)
    : m_name("")
    , m_source(nullptr)
    , m_bytecode(nullptr)

    , m_dataRepresent(EShaderDataRepresent::eSource)
    , m_shaderType(type)
{
    m_source = new std::string(source);
    IResource::setLoaded(true);
}

Shader::Shader(EShaderType type, const Bytecode& bytecode)
    : m_name("")
    , m_source(nullptr)
    , m_bytecode(nullptr)

    , m_dataRepresent(EShaderDataRepresent::eBytecode)
    , m_shaderType(type)
{
    m_bytecode = new std::vector<u32>(bytecode);
    IResource::setLoaded(true);
}

void Shader::setType(EShaderType type)
{
    m_shaderType = type;
}

Shader::~Shader()
{
    if (m_dataRepresent == EShaderDataRepresent::eBytecode)
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

    stream->read<EShaderDataRepresent>(m_dataRepresent);
    stream->read<EShaderType>(m_shaderType);

    if (m_dataRepresent == EShaderDataRepresent::eBytecode)
    {
        u64 size = 0;
        stream->read<u64>(size);

        m_bytecode = new Bytecode(size);
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

stream::IStreamPtr Shader::serialize()
{
    if (!IResource::isLoaded())
    {
        ASSERT(false, "not loaded");
        return false;
    }

    stream::IStreamPtr stream = stream::StreamManager::createMemoryStream();

    stream->seekBeg(0);
    stream->write<Shader::EShaderDataRepresent>(m_dataRepresent);
    stream->write<EShaderType>(m_shaderType);

    if (m_dataRepresent == EShaderDataRepresent::eBytecode)
    {
        stream->write(m_bytecode->data(), m_bytecode->size() * sizeof(u32), 1);
    }
    else
    {
        stream->write(*m_source);
    }

    return stream;
}


const std::string& Shader::getName() const
{
    return m_name;
}

const std::string* Shader::getSource() const
{
    ASSERT(m_source, "invalid data");
    return m_source;
}

const Bytecode* Shader::getBytecode() const
{
    ASSERT(m_bytecode, "invalid data");
    return m_bytecode;
}

Shader::EShaderDataRepresent Shader::getShaderKind() const
{
    return m_dataRepresent;
}

EShaderType Shader::getType() const
{
    return m_shaderType;
}

} //namespace resources
} //namespace v3d

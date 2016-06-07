#include "ShaderSouce.h"
#include "utils/Logger.h"

struct SHash
{
    std::string _header;
    std::string _body;
};

bool operator == (const SHash& lhs, const SHash& rhs)
{
    return lhs._header == rhs._header && lhs._body == rhs._body;
};

namespace std
{
    template<> struct hash<SHash>
    {
        typedef SHash argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& s) const
        {
            result_type const h1(std::hash<std::string>()(s._header));
            result_type const h2(std::hash<std::string>()(s._body));

            return h1 ^ (h2 << 1);
        }
    };
} //namespace std

namespace v3d
{
namespace renderer
{

/////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderSource::s_shaderTypeName[eShaderCount] = 
{
    "vertex",
    "fragment",
    "geometry",
    "control",
    "evaluation",
    "compute"
};

const std::string& CShaderSource::getShaderTypeNameByType(EShaderType type)
{
    return s_shaderTypeName[type];
}

EShaderType CShaderSource::getShaderTypeByName(const std::string& name)
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

CShaderSource::CShaderSource()
    : m_type(EShaderType::eShaderUnknown)
    , m_header("")

    , m_hash(0)
    , m_name("")
{
    LOG_DEBUG("CShaderSource: CShaderSource constructor %x", this);
}

CShaderSource::~CShaderSource()
{
    LOG_DEBUG("CShaderSource: CShaderSource destructor %x", this);
}

std::string CShaderSource::buildHeader(const ShaderDefinesList& list, const std::string& body)
{
    std::string header("");
    for (auto& define : list)
    {
        if (define.first.empty())
        {
            continue;
        }

        if (body.find(define.first + "\n") != std::string::npos ||
            body.find(define.first + " ") != std::string::npos)
        {
            header.append("#define ");
            header.append(define.first);
            if (!define.second.empty())
            {
                header.append(" ");
                header.append(define.second);
            }

            header.append("\n");
        }
    }

    return header;
}

std::size_t CShaderSource::calculateHash(const std::string& header, const std::string& body)
{
    SHash obj = { header, body };

    std::hash<SHash> hashFunc{};
    std::size_t hash = hashFunc(obj);

    return hash;
}

void CShaderSource::setType(EShaderType type)
{
    m_type = type;
}

void CShaderSource::setHeader(const std::string& header)
{
    if (m_header != header)
    {
        m_header = header;
        m_hash = CShaderSource::calculateHash(m_header, m_body);
    }
}

void CShaderSource::setBody(const std::string& body)
{
    if (m_body != body)
    {
        m_body = body;
        m_hash = CShaderSource::calculateHash(m_header, m_body);
    }
}

void CShaderSource::setName(const std::string& name)
{
    m_name = name;
}

const std::string& CShaderSource::getHeader() const
{
    return m_header;
}

const std::string& CShaderSource::getName() const
{
    return m_name;
}

EShaderType CShaderSource::getType() const
{
    return m_type;
}

std::size_t CShaderSource::getHash() const
{
    return m_hash;
}

void CShaderSource::setDefines(const ShaderDefinesList& defines)
{
    std::string header = CShaderSource::buildHeader(defines, m_body);
    CShaderSource::setHeader(header);
}

const std::string& CShaderSource::getBody() const
{
    return m_body;
}

} //namespace renderer
} //namespace v3d

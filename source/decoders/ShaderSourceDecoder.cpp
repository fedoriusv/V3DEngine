#include "ShaderSourceDecoder.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

namespace v3d
{
namespace decoders
{
    using namespace resources;

ShaderSourceDecoder::ShaderSourceDecoder(Shader::EShaderDataRepresent kind)
    : m_kind(kind)
    , m_type(EShaderType::eShaderUnknown)
{
}

ShaderSourceDecoder::ShaderSourceDecoder(std::initializer_list<std::string> supportedExtensions, Shader::EShaderDataRepresent kind)
    : ResourceDecoder(supportedExtensions)
    , m_kind(kind)
    , m_type(EShaderType::eShaderUnknown)
{
}

ShaderSourceDecoder::~ShaderSourceDecoder()
{
}

void ShaderSourceDecoder::setShaderType(resources::EShaderType type)
{
    m_type = type;
}

stream::IResource* ShaderSourceDecoder::decode(const stream::IStreamPtr stream)
{
    if (!stream)
    {
        LOG_ERROR("ShaderSourceDecoder::decode: Empty stream");
        return nullptr;
    }

    if (stream->size() > 0)
    {
        stream->seekBeg(0);

        std::string source;
        stream->read(source);

        const  std::string& file = std::static_pointer_cast<stream::FileStream>(stream)->getName();
        auto getShaderType = [](const std::string& name) -> EShaderType
        {
            std::string fileExtension = "";

            const size_t pos = name.find_last_of('.');
            if (pos != std::string::npos)
            {
                fileExtension = std::string(name.begin() + pos + 1, name.end());
            }

            if (fileExtension == "vert")
            {
                return EShaderType::eVertex;
            }
            else if (fileExtension == "frag")
            {
                return EShaderType::eFragment;
            }
            else if (fileExtension == "geom")
            {
                return EShaderType::eGeometry;
            }
            else if (fileExtension == "comp")
            {
                return EShaderType::eCompute;
            }
            else if (fileExtension == "tesc")
            {
                return EShaderType::eTessellationControl;
            }
            else if (fileExtension == "tese")
            {
                return EShaderType::eTessellationEvaluation;
            }

            return EShaderType::eShaderUnknown;
        };

        if (m_type == EShaderType::eShaderUnknown) //Try define shader type
        {
            m_type = getShaderType(file);
        }


        stream::IStreamPtr data = stream::StreamManager::createMemoryStream();

        data->seekBeg(0);
        data->write<Shader::EShaderDataRepresent>(m_kind);
        data->write<EShaderType>(m_type);
        data->write(source);

        Shader* shader = new Shader();
        shader->init(data);

        return shader;
    }

    return nullptr;
}

} //namespace decoders
} //namespace v3d

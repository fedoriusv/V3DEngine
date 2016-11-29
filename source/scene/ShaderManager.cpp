#include "ShaderManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"
#include "decoders/ShaderSpirVDecoder.h"
#include "decoders/ShaderSourceDecoder.h"

namespace v3d
{
namespace scene
{
    struct CompiledParams
    {
        std::string defines;
        std::string body;
        //u32 vendor;
        //u32 driver;
    };

    bool operator == (const CompiledParams& lhs, const CompiledParams& rhs)
    {
        return lhs.defines == rhs.defines && lhs.body == rhs.body;
    };
} //namespace scene
} //namespace v3d

namespace std
{
    template<>
    struct hash<v3d::scene::CompiledParams>
    {
        typedef v3d::scene::CompiledParams argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& s) const
        {
            result_type const h1(std::hash<std::string>()(s.defines));
            result_type const h2(std::hash<std::string>()(s.body));

            return h1 ^ (h2 << 1);
        }
    };
} //namespace std

namespace v3d
{
namespace scene
{

using namespace stream;
using namespace resources;
using namespace renderer;
using namespace decoders;

ShaderManager::ShaderManager()
{
    TResourceLoader::registerPath("../../../../data/");
    TResourceLoader::registerPath("../../../../../data/");
    TResourceLoader::registerPath("data/");

    std::initializer_list<std::string> extSrc = { ".vert", ".frag", ".tesc", ".tese", ".geom", ".comp" };
    TResourceLoader::registerDecoder(std::make_shared<ShaderSourceDecoder>(extSrc, Shader::EShaderDataRepresent::eSource));

#ifdef USE_SPIRV
    std::initializer_list<std::string> extBin = { ".spv" };
    TResourceLoader::registerDecoder(std::make_shared<ShaderSourceDecoder>(extBin, Shader::EShaderDataRepresent::eBytecode));

    /*std::initializer_list<std::string> extSrc = { ".vert", ".frag", ".tesc", ".tese", ".geom", ".comp" };
    TResourceLoader::registerDecoder(std::make_shared<ShaderSpirVDecoder>(extSrc, ShaderSpirVDecoder::ESpirVResource::eSpirVSource, true));

    std::initializer_list<std::string> extBin = { ".spv" };
    TResourceLoader::registerDecoder(std::make_shared<ShaderSpirVDecoder>(extBin, ShaderSpirVDecoder::ESpirVResource::eSpirVBytecode, true));*/
#endif //USE_SPIRV
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::add(const ShaderPtr shader)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    std::string name = shader->getName();
    TResourceLoader::insert(shader, name);
}

const ShaderPtr ShaderManager::load(const std::string& name, const std::string& alias)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    const ShaderPtr findShader = TResourceLoader::get(alias.empty() ? nameStr : alias);
    if (findShader)
    {
        return findShader;
    }
    else
    {
        for (std::string& path : m_pathes)
        {
            const std::string fullName = path + nameStr;
            const bool isFileExist = stream::FileStream::isFileExist(fullName);
            if (!isFileExist)
            {
                LOG_WARNING("ShaderManager::load: File [%s] not found", name.c_str());
                return nullptr;
            }

            const stream::FileStreamPtr stream = stream::StreamManager::createFileStream(fullName, stream::FileStream::e_in);
            if (stream->isOpen())
            {
                std::string fileExtension = ShaderManager::getFileExtension(nameStr);
                const decoders::DecoderPtr decoder = TResourceLoader::findDecoder(fileExtension);
                if (!decoder)
                {
                    LOG_ERROR("ShaderManager: Extension not supported. File [%s]", nameStr.c_str());
                    return nullptr;
                }

                stream::IResource* resource = decoder->decode(stream);
                stream->close();

                if (!resource)
                {
                    LOG_ERROR("ShaderManager: Streaming error read file [%s]", nameStr.c_str());
                    return nullptr;
                }

                ShaderPtr shader = static_cast<Shader*>(resource);
                shader->setResourseName(fullName);
                const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                shader->setResourseFolder(fullPath);

                if (!shader->load())
                {
                    LOG_ERROR("ShaderManager: Streaming error read file [%s]", nameStr.c_str());
                    return nullptr;
                }

                TResourceLoader::insert(shader, alias.empty() ? nameStr : alias);
                LOG_INFO("ShaderManager: File [%s] success loaded", fullName.c_str());
                return shader;
            }
        }
    }

    LOG_WARNING("ShaderManager::load: File [%s] not found", name.c_str());
    return nullptr;
}

u64 ShaderManager::generateHash(const std::string& body, const std::string& defines)
{
    CompiledParams params = { defines, body };

    std::hash<CompiledParams> hashFunc;
    u64 hash = hashFunc(params);

    return hash;
}

void ShaderManager::addCompiledShader(u64 hash, const Bytecode& bytecode)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (!alreadyCompiled(hash))
    {
        m_compiledShaders.insert(std::make_pair(hash, bytecode));
    }
}

const Bytecode* ShaderManager::getCompiledShader(u64 hash) const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    auto iter = m_compiledShaders.find(hash);
    if (iter != m_compiledShaders.cend())
    {
        return &iter->second;
    }

    return nullptr;
}

bool ShaderManager::alreadyCompiled(u64 hash) const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    auto iter = m_compiledShaders.find(hash);
    if (iter != m_compiledShaders.cend())
    {
        return true;
    }

    return false;
}

const ShaderPtr ShaderManager::createShaderFromSource(const std::string& source, EShaderType type, const std::string& alias)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    ShaderPtr shader = new Shader(type, source);
    shader->setResourseName(alias);

    TResourceLoader::insert(shader, alias);

    return shader;
}

const ShaderPtr ShaderManager::createShaderFromBytecode(const resources::Bytecode& bytecode, EShaderType type, const std::string& alias)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    ShaderPtr shader = new Shader(type, bytecode);
    shader->setResourseName(alias);

    TResourceLoader::insert(shader, alias);

    return shader;
}

std::string ShaderManager::getFileExtension(const std::string& fullFileName)
{
    std::string fileExtension = "";

    const size_t pos = fullFileName.find_last_of('.');
    if (pos != std::string::npos)
    {
        fileExtension = std::string(fullFileName.begin() + pos, fullFileName.end());
    }

    return fileExtension;
}

} //namespace scene
} //namespace v3d

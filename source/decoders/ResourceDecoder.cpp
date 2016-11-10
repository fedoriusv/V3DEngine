#include "ResourceDecoder.h"

namespace v3d
{
namespace decoders
{

ResourceDecoder::ResourceDecoder()
{
}

ResourceDecoder::ResourceDecoder(std::initializer_list<std::string> supportedExtensions)
{
    m_supportedExtensions.assign(supportedExtensions);
}

ResourceDecoder::~ResourceDecoder()
{
    m_supportedExtensions.clear();
}

bool ResourceDecoder::isExtensionSupported(const std::string& extension)  const
{
    auto it = std::find(m_supportedExtensions.begin(), m_supportedExtensions.end(), extension);
    if (it != m_supportedExtensions.end())
    {
        return true;
    }

    return false;
}

void ResourceDecoder::setSupportedExtensions(const std::string supportedExtensions[])
{
    m_supportedExtensions.assign(supportedExtensions, supportedExtensions + sizeof(supportedExtensions) / sizeof(supportedExtensions[0]));
}

const std::vector<std::string>& ResourceDecoder::getSupportedExtensions() const
{
    return m_supportedExtensions;
}

} //namespace decoders
} //namespace v3d

#pragma once

#include "stream/IStream.h"

namespace v3d
{
namespace stream
{
    class IResource;
}
namespace decoders
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class ResourceDecoder
    {
    public:
    
        ResourceDecoder();
        explicit ResourceDecoder(std::initializer_list<std::string> supportedExtensions);

        virtual                         ~ResourceDecoder();

        virtual stream::IResource*      decode(const stream::IStreamPtr stream) = 0;

        bool                            isExtensionSupported(const std::string& extension) const;
        void                            setSupportedExtensions(const std::string supportedExtensions[]);
        const std::vector<std::string>& getSupportedExtensions() const;
    
    protected:
    
        std::vector<std::string>        m_supportedExtensions;
    };
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    using DecoderPtr = std::shared_ptr<ResourceDecoder>;
    using DecoderList = std::vector<DecoderPtr>;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace decoders
} //namespace v3d

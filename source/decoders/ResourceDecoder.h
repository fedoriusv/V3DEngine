#ifndef _V3D_RESOURCE_DECODER_H_
#define _V3D_RESOURCE_DECODER_H_

#include "stream/IStream.h"
#include "stream/Resource.h"

namespace v3d
{
namespace decoders
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CResourceDecoder
    {
    public:
    
        CResourceDecoder();
        explicit CResourceDecoder(std::initializer_list<std::string> supportedExtensions);

        virtual                         ~CResourceDecoder();

        virtual stream::ResourcePtr     decode(const stream::IStreamPtr& stream) = 0;

        bool                            isExtensionSupported(const std::string& extension) const;
        void                            setSupportedExtensions(const std::string supportedExtensions[]);
        const std::vector<std::string>& getSupportedExtensions() const;
    
    protected:
    
        std::vector<std::string>        m_supportedExtensions;
    };
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CResourceDecoder>   DecoderPtr;
    typedef std::vector<DecoderPtr>             DecoderList;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RESOURCE_DECODER_H_
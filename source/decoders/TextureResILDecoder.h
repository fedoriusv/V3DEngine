#ifndef _V3D_TEXTURE_RESIL_DECODER_H_
#define _V3D_TEXTURE_RESIL_DECODER_H_

#include "ResourceDecoder.h"
#include "renderer/Texture.h"

namespace v3d
{
namespace decoders
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTextureResILDecoder : public CResourceDecoder
    {
    public:

        CTextureResILDecoder();
        explicit CTextureResILDecoder(std::initializer_list<std::string> supportedExtensions);

        virtual                 ~CTextureResILDecoder();

        stream::IResource*      decode(const stream::IStreamPtr& stream) override;

    private:

        renderer::EImageFormat  convertILFormat(u32 format);
        renderer::EImageType    convertILType(u32 format);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace decoders
} //namespace v3d

#endif //_V3D_TEXTURE_RESIL_DECODER_H_

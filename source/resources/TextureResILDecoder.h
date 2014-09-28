#ifndef _V3D_TEXTURE_RESIL_DECODER_H_
#define _V3D_TEXTURE_RESIL_DECODER_H_

#include "ResourceDecoder.h"
#include "renderer/Texture.h"

namespace v3d
{
    namespace resources
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class CTextureResILDecoder : public CResourceDecoder
        {
        public:

            CTextureResILDecoder();
            explicit CTextureResILDecoder(std::initializer_list<std::string> supportedExtensions);

            virtual                 ~CTextureResILDecoder();

            stream::ResourcePtr     decode(const stream::IStreamPtr& stream) override;

        private:

            renderer::EImageFormat  convertILFormat(u32 format);
            renderer::EImageType    convertILType(u32 format);
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}

#endif //_V3D_TEXTURE_RESIL_DECODER_H_

#pragma once

#include "ResourceDecoder.h"
#include "renderer/Texture.h"

namespace v3d
{
namespace decoders
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TextureResILDecoder : public ResourceDecoder
    {
    public:

        TextureResILDecoder();
        explicit TextureResILDecoder(std::initializer_list<std::string> supportedExtensions);

        virtual                 ~TextureResILDecoder();

        stream::IResource*      decode(const stream::IStreamPtr stream) override;

    private:

        renderer::EImageFormat  convertILFormat(u32 format);
        renderer::EImageType    convertILType(u32 format);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace decoders
} //namespace v3d

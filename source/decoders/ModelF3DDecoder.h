#pragma once

#include "ResourceDecoder.h"

namespace v3d
{
namespace decoders
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class ModelF3DDecoder : public ResourceDecoder
    {
    public:

        ModelF3DDecoder();
        explicit ModelF3DDecoder(std::initializer_list<std::string> supportedExtensions);

        virtual                 ~ModelF3DDecoder();

        stream::IResource*      decode(const stream::IStreamPtr stream) override;

    private:

        stream::IResource*     decode100(const stream::IStreamPtr stream);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
} //namespace decoders
} //namespace v3d

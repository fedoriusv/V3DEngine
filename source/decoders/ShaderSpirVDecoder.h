#pragma once

#include "ResourceDecoder.h"

namespace v3d
{
namespace decoders
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class ShaderSpirVDecoder : public ResourceDecoder
    {
    public:

        enum ESpirVResource
        {
            eSpirVSource,
            eSpirVBitecode
        };

        ShaderSpirVDecoder(ESpirVResource resource, bool reflections);
        explicit ShaderSpirVDecoder(std::initializer_list<std::string> supportedExtensions, ESpirVResource resource, bool reflections);

        virtual                 ~ShaderSpirVDecoder();

        stream::IResource*      decode(const stream::IStreamPtr stream) override;

    private:

        void                    parseReflactions(const std::vector<u32>& spirv, stream::IStreamPtr stream);

        ESpirVResource          m_resourceType;
        bool                    m_reflections;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace decoders
} //namespace v3d

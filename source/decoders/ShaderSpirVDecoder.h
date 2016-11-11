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

        enum ESourceLanguage
        {
            eGLSLLang,
            eHLSLLang
        };

        ShaderSpirVDecoder(ESourceLanguage lang, bool reflections);
        explicit ShaderSpirVDecoder(std::initializer_list<std::string> supportedExtensions, ESourceLanguage lang, bool reflections);

        virtual                 ~ShaderSpirVDecoder();

        stream::IResource*      decode(const stream::IStreamPtr stream) override;

    private:

        void                    parseReflactions(const std::vector<u32>& spirv, stream::IStreamPtr stream);

        ESourceLanguage         m_lang;
        bool                    m_reflections;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace decoders
} //namespace v3d

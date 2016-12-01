#pragma once

#include "ResourceDecoder.h"
#include "resources/Shader.h"

namespace v3d
{
namespace decoders
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class ShaderSourceDecoder final : public ResourceDecoder
    {
    public:

        explicit ShaderSourceDecoder(resources::Shader::EShaderDataRepresent kind);
        ShaderSourceDecoder(std::initializer_list<std::string> supportedExtensions, resources::Shader::EShaderDataRepresent kind);

        ~ShaderSourceDecoder();

        void                    setShaderType(resources::EShaderType type);

        stream::IResource*      decode(const stream::IStreamPtr stream) override;

    private:

        resources::Shader::EShaderDataRepresent m_kind;
        resources::EShaderType                  m_type;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace decoders
} //namespace v3d

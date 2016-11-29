#pragma once

#include "common.h"
#include "renderer/ShaderDataTypes.h"
#include "renderer/Renderer.h"
#include "resources/Shader.h"
#include "renderer/Texture.h"
#include "renderer/ShaderProgram.h"

#ifdef USE_SPIRV

namespace v3d
{
namespace utils
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class SpirVCompileWrapper
    {
    public:

        enum ECompileError
        {
            eNoErrors = 0,
            eInvalidShaderType,
            eInvalidCompiler,
            eInvalidStage,
            eCompilationError,
            eInternalError,
            eNullResultObject,
            eInvalidAssembly
        };

        SpirVCompileWrapper(renderer::ERenderType vendor, const resources::ShaderDefinesList& defines);
        ~SpirVCompileWrapper();

        ECompileError                               compile(const std::string& source, resources::EShaderType type, std::vector<u32>& bytecode);
        renderer::ShaderProgram::ShaderParameters   reflection(const std::vector<u32>& bytecode);

        const std::string&                          errorMessages();

    private:

        std::string                                 m_errors;
        resources::ShaderDefinesList                m_defines;
        renderer::ERenderType                       m_vendor;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d

#endif //USE_SPIRV

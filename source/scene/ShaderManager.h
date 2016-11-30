#pragma once

#include "utils/Singleton.h"
#include "resources/Shader.h"
#include "stream/ResourceLoader.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * ShaderManager
    * Both side, thread safe
    */
    class ShaderManager : public utils::TSingleton<ShaderManager>, public stream::TResourceLoader<const resources::ShaderPtr>
    {
    private:

        friend utils::TSingleton<ShaderManager>;

        ShaderManager();
        ~ShaderManager();

    public:

        void                                        add(const resources::ShaderPtr source);
        const resources::ShaderPtr                  load(const std::string& name, const std::string& alias = "") override;

        static u64                                  generateHash(const std::string& body, const resources::ShaderDefinesList& defines);

        void                                        addCompiledShader(u64 hash, const resources::Bytecode& bytecode);
        const resources::Bytecode*                  getCompiledShader(u64 hash) const;
        bool                                        alreadyCompiled(u64 hash) const;

        const resources::ShaderPtr                  createShaderFromSource(const std::string& source, resources::EShaderType type, const std::string& alias);
        const resources::ShaderPtr                  createShaderFromBytecode(const resources::Bytecode& bytecode, resources::EShaderType type, const std::string& alias);

    private:

        static std::string                          getFileExtension(const std::string& fullFileName);

        mutable std::recursive_mutex                m_mutex;
        std::map<u64, resources::Bytecode>          m_compiledShaders;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace scene
} //namespace v3d

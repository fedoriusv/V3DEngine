#pragma once

#include "utils/Singleton.h"
#include "resources/ShaderResource.h"
#include "renderer/Shader.h"
#include "stream/ResourceLoader.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class ShaderManager : public utils::TSingleton<ShaderManager>, public stream::TResourceLoader<const resources::ShaderPtr>
    {
    private:

        friend utils::TSingleton<ShaderManager>;

        ShaderManager();
        ~ShaderManager();

    public:

        void                                        add(const resources::ShaderPtr source);
        const resources::ShaderPtr                  load(const std::string& name, const std::string& alias = "") override;

        void                                        add(renderer::ShaderPtr shader);
        const renderer::ShaderWPtr                  get(renderer::ShaderPtr shader) const;
        renderer::ShaderWPtr                        get(std::size_t hash) const;

    private:

        std::string                                 getFileExtension(const std::string& fullFileName);

        renderer::ShaderHashMap                     m_shaderList;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace scene
} //namespace v3d

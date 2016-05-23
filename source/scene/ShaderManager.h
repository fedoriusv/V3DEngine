#ifndef _V3D_SHADER_MANAGER_H_
#define _V3D_SHADER_MANAGER_H_

#include "utils/Singleton.h"
#include "resources/ShaderSouceData.h"
#include "renderer/Shader.h"
#include "stream/ResourceLoader.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderManager : public utils::TSingleton<CShaderManager>, public stream::TResourceLoader<const resources::CShaderSourceData*>
    {
    private:

        friend utils::TSingleton<CShaderManager>;

        CShaderManager();
        ~CShaderManager();

    public:

        void                                        add(const resources::CShaderSourceData* source);
        const resources::CShaderSourceData*         load(const std::string& name, const std::string& alias = "") override;

        void                                        add(renderer::ShaderPtr shader);
        const renderer::ShaderWPtr                  get(renderer::ShaderPtr shader) const;
        renderer::ShaderWPtr                        get(std::size_t hash) const;

    private:

        renderer::ShaderHashMap                     m_shaderList;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace scene
} //namespace v3d

#endif //_V3D_SHADER_MANAGER_H_

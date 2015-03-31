#ifndef _V3D_SHADER_MANAGER_H_
#define _V3D_SHADER_MANAGER_H_

#include "resources/ShaderSouceData.h"
#include "Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderManager : public Singleton<CShaderManager>
    {
    public:

        CShaderManager();
        virtual                                 ~CShaderManager();

        void                                    add(const resources::ShaderSourceDataPtr& shader);
        const resources::ShaderSourceDataPtr    load(const std::string& name);

        void                                    unload(const std::string& name);
        void                                    unload(const resources::ShaderSourceDataPtr& shader);

        void                                    unloadAll();

        void                                    registerPath(const std::string& path);
        void                                    unregisterPath(const std::string& path);

    private:

        resources::ShaderSourceDataMap          m_shaders;
        std::vector<std::string>                m_pathes;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_MANAGER_H_
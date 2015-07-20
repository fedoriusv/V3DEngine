#ifndef _V3D_SHADER_MANAGER_H_
#define _V3D_SHADER_MANAGER_H_

#include "resources/ShaderSouceData.h"
#include "stream/ResourceLoader.h"
#include "Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderManager : public Singleton<CShaderManager>, public stream::TResourceLoader<resources::ShaderSourceDataPtr>
    {
    public:

        CShaderManager();
        virtual                                 ~CShaderManager();

        void                                    add(const resources::ShaderSourceDataPtr& shader);
        const resources::ShaderSourceDataPtr    load(const std::string& name, const std::string& alias = "") override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_MANAGER_H_
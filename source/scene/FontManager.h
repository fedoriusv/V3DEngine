#ifndef _V3D_FONT_MANAGER_H_
#define _V3D_FONT_MANAGER_H_

#include "resources/FontData.h"
#include "Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFontManager : public Singleton<CFontManager>
    {
    public:

        CFontManager();
        virtual                             ~CFontManager();

        void                                add(const renderer::FontDataPtr& font);
        const renderer::FontDataPtr&        get(const std::string& name);
        const renderer::FontDataPtr         load(const std::string& name);

        void                                unload(const std::string& name);
        void                                unload(const renderer::FontDataPtr& font);

        void                                unloadAll();

        void                                registerPath(const std::string& path);
        void                                unregisterPath(const std::string& path);

    private:

        renderer::FontDataList              m_fontsData;
        std::vector<std::string>            m_pathes;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FONT_MANAGER_H_

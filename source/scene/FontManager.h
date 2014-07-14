#ifndef _V3D_FONT_MANAGER_H_
#define _V3D_FONT_MANAGER_H_

#include "renderer/FreeTypeData.h"
#include "Singleton.h"
#include "ResourceDecoder.h"

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

        void                                add(const renderer::FreeTypeDataPtr& font);
        const renderer::FreeTypeDataPtr&    get(const std::string& name);
        const renderer::FreeTypeDataPtr&    load(const std::string& name);

        void                                unload(const std::string& name);
        void                                unload(const renderer::FreeTypeDataPtr& font);

        void                                unloadAll();

        void                                registerPath(const std::string& path);
        void                                unregisterPath(const std::string& path);

    private:

        renderer::FreeTypeDataList          m_fontsData;
        std::vector<std::string>            m_pathes;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FONT_MANAGER_H_
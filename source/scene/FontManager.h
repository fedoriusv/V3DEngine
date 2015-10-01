#ifndef _V3D_FONT_MANAGER_H_
#define _V3D_FONT_MANAGER_H_

#include "resources/FontData.h"
#include "stream/ResourceLoader.h"
#include "utils/Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFontManager : public utils::TSingleton<CFontManager>, public stream::TResourceLoader<const resources::CFontData>
    {
    public:

        CFontManager();
        virtual                             ~CFontManager();

        void                        add(const resources::CFontData* font);
        const resources::CFontData* load(const std::string& name, const std::string& alias = "") override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FONT_MANAGER_H_

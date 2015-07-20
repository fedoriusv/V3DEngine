#ifndef _V3D_FONT_MANAGER_H_
#define _V3D_FONT_MANAGER_H_

#include "resources/FontData.h"
#include "stream/ResourceLoader.h"
#include "Singleton.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFontManager : public Singleton<CFontManager>, public stream::TResourceLoader<resources::FontDataPtr>
    {
    public:

        CFontManager();
        virtual                             ~CFontManager();

        void                                add(const resources::FontDataPtr& font);
        const resources::FontDataPtr        load(const std::string& name, const std::string& alias = "") override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FONT_MANAGER_H_

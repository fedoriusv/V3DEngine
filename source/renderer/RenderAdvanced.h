#ifndef _V3D_RENDER_ADVANCED_H_
#define _V3D_RENDER_ADVANCED_H_

#include "common.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderAdvanced
    {
    public:

        CRenderAdvanced();
        ~CRenderAdvanced();

        void    setCountInstance(u32 lod);
        u32     getCountInstance() const;

        bool    parse(const tinyxml2::XMLElement* root);

    private:

        u32     m_instanced;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderAdvanced> RenderAdvancedPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_ADVANCED_H_
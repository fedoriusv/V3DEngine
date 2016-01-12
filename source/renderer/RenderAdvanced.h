#ifndef _V3D_RENDER_ADVANCED_H_
#define _V3D_RENDER_ADVANCED_H_

#include "Geometry.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;

    class CRenderAdvanced
    {
    public:

        CRenderAdvanced();
        ~CRenderAdvanced();

        u32                         getCountInstance() const;
        EPrimitivesMode             getPrimitiveMode() const;
        const CGeometry::SInterval& getDrawInterval()  const;

    private:

        friend                      CRenderPass;

        void                        setCountInstance(u32 lod);
        void                        setPrimitiveMode(EPrimitivesMode mode);
        void                        setDrawInterval(u32 offset, u32 amount);

        bool                        parse(const tinyxml2::XMLElement* root);

        EPrimitivesMode             m_mode;
        u32                         m_instanced;
        CGeometry::SInterval        m_interval;


    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderAdvanced> RenderAdvancedPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDER_ADVANCED_H_

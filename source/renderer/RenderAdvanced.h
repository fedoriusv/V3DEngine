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

    /**
    * Advanced properties for render.
    */
    class CRenderAdvanced final
    {
    public:

        CRenderAdvanced();
        CRenderAdvanced(const CRenderAdvanced& advanced);
        CRenderAdvanced& operator=(const CRenderAdvanced& advanced);

        ~CRenderAdvanced();

        u32                         getCountInstance() const;
        EPrimitivesMode             getPrimitiveMode() const;
        const CGeometry::SInterval& getDrawInterval()  const;

        u32                         getCountPatches()  const;
        f32                         getPatchInnerLevel() const;
        f32                         getPatchOuterLevel() const;

    private:

        friend                      CRenderPass;

        void                        setCountInstance(u32 lod);

        void                        setPrimitiveMode(EPrimitivesMode mode);
        void                        setDrawInterval(u32 offset, u32 amount);

        void                        setCountPatches(u32 patches);
        void                        setPatchLevel(f32 inner, f32 outer);

        bool                        parse(const tinyxml2::XMLElement* root);

        EPrimitivesMode             m_mode;
        u32                         m_instanced;
        CGeometry::SInterval        m_interval;

        //patches
        u32                         m_patches;
        f32                         m_patchInnerLevel;
        f32                         m_patchOuterLevel;


    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderAdvanced> RenderAdvancedPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDER_ADVANCED_H_

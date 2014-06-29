#ifndef _V3D_RENDER_STATE_H_
#define _V3D_RENDER_STATE_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderState
    {
    public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////

        enum EWinding
        {
            eWindingCW,
            eWindingCCW
        };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

        enum EPolygonMode
        {
            ePolyModeFill,
            ePolyModeLine,
            ePolyModePoint,

            eModeCount
        };

        //TODO:add blend

    /////////////////////////////////////////////////////////////////////////////////////////////////////

        CRenderState();
        virtual                   ~CRenderState();

        bool                      getCullFace()    const;
        EWinding                  getWinding()     const;
        EPolygonMode              getPolygonMode() const;
                                  
        void                      setCullFace(bool type);
        void                      setWinding(EWinding type);
        void                      setPolygonMode(EPolygonMode type);

        virtual void              bind() = 0;

        static EPolygonMode       getPolygonModeByName(const std::string& name);

    protected:

        bool                      m_cullFace;
        EWinding                  m_winding;
        EPolygonMode              m_polygonMode;

    private:

        static const std::string  s_renderPolygonMode[EPolygonMode::eModeCount];

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderState> RenderStatePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_STATE_H_

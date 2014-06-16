#ifndef _V3D_RENDER_STATE_H_
#define _V3D_RENDER_STATE_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ERenderWinding
    {
        eWindingCW,
        eWindingCCW
    };

    enum ERenderPolygonMode
    {
        ePolyModeFill,
        ePolyModeLine,
        ePolyModePoint,

        ePolygonModeCount
    };

    //TODO:add blend

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderState
    {
    public:

        CRenderState();
        virtual                   ~CRenderState();
                                  
        bool                      getCullFace()    const;
        ERenderWinding            getWinding()     const;
        ERenderPolygonMode        getPolygonMode() const;
                                  
        void                      setCullFace(bool type);
        void                      setWinding(ERenderWinding type);
        void                      setPolygonMode(ERenderPolygonMode type);

        virtual void              bind() = 0;

        static ERenderPolygonMode getPolygonModeByName(const std::string& name);

    protected:

        bool                      m_cullFace;
        ERenderWinding            m_winding;
        ERenderPolygonMode        m_polygonMode;

    private:

        static const std::string s_renderPolygonMode[ERenderPolygonMode::ePolygonModeCount];

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderState> RenderStatePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_STATE_H_

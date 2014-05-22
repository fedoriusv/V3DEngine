#ifndef _F3D_RENDER_STATE_H_
#define _F3D_RENDER_STATE_H_

#include "common.h"

namespace f3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ERenderCullFace
    {
        eCullBack,
        ECullFront
    };

    enum ERenderWinding
    {
        eWindingCW,
        eWindingCCW
    };

    enum ERenderPolygonMode
    {
        ePolyModeFill,
        ePolyModeLine,
        ePolyModePoint
    };

    //TODO:add blend

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderState
    {
    public:

        CRenderState();
        virtual             ~CRenderState();

        ERenderCullFace     getCullFace()    const;
        ERenderWinding      getWinding()     const;
        ERenderPolygonMode  getPolygonMode() const;

        void                setCullFace(ERenderCullFace type);
        void                getWinding(ERenderWinding type);
        void                getPolygonMode(ERenderPolygonMode type);

    protected:

        ERenderCullFace     m_cullFace;
        ERenderWinding      m_winding;
        ERenderPolygonMode  m_polygonMode;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderState> RenderStatePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}


#endif //_F3D_RENDER_STATE_H_
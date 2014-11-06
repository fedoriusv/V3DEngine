#ifndef _V3D_RENDER_STATE_GL_H_
#define _V3D_RENDER_STATE_GL_H_

#include "renderer/RenderState.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderStateGL : public CRenderState
    {
    public:

        CRenderStateGL();
        ~CRenderStateGL();

        void    bind() override;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_STATE_GL_H_
#ifndef _V3D_BILLBOARD_H_
#define _V3D_BILLBOARD_H_

#include "Node.h"
#include "renderer/Renderable.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CBillboard : public CNode, public renderer::CRenderable
    {
    public:

        explicit CBillboard(const std::string& texture);
        ~CBillboard();

        void                            init()          override;
        void                            render()        override;
        void                            update(s32 dt)  override;

    private:

        void                            build();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_BILLBOARD_H_

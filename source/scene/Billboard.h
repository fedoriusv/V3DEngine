#ifndef _V3D_BILLBOARD_H_
#define _V3D_BILLBOARD_H_

#include "Node.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CBillboard : public CNode
    {
    public:

        CBillboard(const std::string& texture);
        ~CBillboard();

        void                            render()            override;
        void                            update(s32 time)    override;

        void                            init()              override;

    private:

        void                            build();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_BILLBOARD_H_
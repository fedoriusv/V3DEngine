#ifndef _V3D_GEOMETRY_D3D_H_
#define _V3D_GEOMETRY_D3D_H_

#include "renderer/Geometry.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CGeometryD3D : public CGeometry
    {
    public:

        CGeometryD3D(const RenderTechniquePtr& technique);
        ~CGeometryD3D();

        void                init()    override;
        void                draw()    override;
        void                free()    override;
        void                refresh() override;

    private:

        static ID3D11Device*        s_device;
        static ID3D11DeviceContext* s_deviceContext;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_GEOMETRY_D3D_H_

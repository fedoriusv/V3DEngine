#include "GeometryD3D.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _DIRECT3D_DRIVER_
#include <d3d11.h>

using namespace v3d;
using namespace renderer;

D3D_PRIMITIVE_TOPOLOGY EDrawModeD3D[CGeometry::eDrawCount] =
{
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
    D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
    D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
};

ID3D11Device* CGeometryD3D::s_device = nullptr;
ID3D11DeviceContext* CGeometryD3D::s_deviceContext = nullptr;

CGeometryD3D::CGeometryD3D(const RenderTechniquePtr& technique)
: CGeometry(technique)
{
}

CGeometryD3D::~CGeometryD3D()
{
}

void CGeometryD3D::init()
{
    if (m_data.empty())
    {
        LOG_WARNING("CGeometryD3D: Data empty");
        return;
    }

    m_interval._begin = 0;
    m_interval._count = m_data._countVertices;

    for (u32 idx = 0; idx < m_technique->getRenderPassCount(); ++idx)
    {
        const RenderPassPtr& pass = m_technique->getRenderPass(idx);
        const AttributeList& attributes = pass->getDefaultShaderData()->getAttributeList();

       
    }

    //s_device->CreateBuffer()

    //Indices
    if (m_data._countIndices > 0)
    {
       //TODO:
    }

    RENDERER->checkForErrors("GeometryGL: Init Error");

   //
}

void CGeometryD3D::draw()
{

    s_deviceContext->IASetPrimitiveTopology(EDrawModeD3D[m_drawMode]);
}

void CGeometryD3D::free()
{
}

void CGeometryD3D::refresh()
{
}

#endif //_DIRECT3D_DRIVER_

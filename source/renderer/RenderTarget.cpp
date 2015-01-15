#include "RenderTarget.h"

using namespace v3d;
using namespace renderer;

CRenderTarget::CRenderTarget()
: m_colorTexture(nullptr)
, m_depthTexture(nullptr)
, m_color(core::Vector4D(0.0f))
{
}

CRenderTarget::~CRenderTarget()
{
}

const TexturePtr& CRenderTarget::getColorTexture() const
{
    return m_colorTexture;
}

TexturePtr& CRenderTarget::getColorTexture()
{
    return m_colorTexture;
}

const TexturePtr& CRenderTarget::getDepthTexture() const
{
    return m_depthTexture;
}

TexturePtr& CRenderTarget::getDepthTexture()
{
    return m_depthTexture;
}

void CRenderTarget::setClearColor(const core::Vector4D& color)
{
    m_color = color;
}

const core::Vector4D& CRenderTarget::getCearColor() const
{
    return m_color;
}

void CRenderTarget::setViewportSize(const core::Dimension2D& size)
{
    m_viewportSize = size;
}

const core::Dimension2D& CRenderTarget::getViewportSize() const
{
    return m_viewportSize;
}

bool CRenderTarget::parse(tinyxml2::XMLElement* root)
{
    //TODO:
    return false;
}
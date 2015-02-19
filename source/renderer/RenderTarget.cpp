#include "RenderTarget.h"

using namespace v3d;
using namespace renderer;

CRenderTarget::CRenderTarget()
: m_colorTexture(nullptr)
, m_depthTexture(nullptr)

, m_color(core::Vector4D(0.0f))

, m_clearColorBuffer(true)
, m_clearDepthBuffer(true)

, m_hasClearColor(false)
, m_hasClearDepth(false)

, m_imageFormat(EImageFormat::eRGBA)
, m_imageType(EImageType::eUnsignedByte)

, m_viewportSize(Dimension2D(0U, 0U))
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

void CRenderTarget::setColorTexture(const TexturePtr& texture)
{
    m_colorTexture = texture;
}

void CRenderTarget::setDepthTexture(const TexturePtr& texture)
{
    m_depthTexture = texture;
}

void CRenderTarget::setClearColorBuffer(bool clear)
{
    m_clearColorBuffer = clear;
}

bool CRenderTarget::getClearColorBuffer() const
{
    return m_clearColorBuffer;
}

bool CRenderTarget::hasClearColorTarget() const
{
    return m_hasClearColor;
}

void CRenderTarget::setClearDepthBuffer(bool clear)
{
    m_clearDepthBuffer = clear;
}

bool CRenderTarget::getClearDepthBuffer() const
{
    return m_clearDepthBuffer;
}

bool CRenderTarget::hasClearDepthTarget() const
{
    return m_hasClearDepth;
}

EImageFormat CRenderTarget::getImageFormat() const
{
    return m_imageFormat;
}

EImageType CRenderTarget::getImageType() const
{
    return m_imageType;
}

bool CRenderTarget::parse(tinyxml2::XMLElement* root)
{
    //TODO:
    return false;
}
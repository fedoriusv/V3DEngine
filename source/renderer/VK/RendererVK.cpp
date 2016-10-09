#include "RendererVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

RendererVK::RendererVK(const ContextPtr context)
    : IRenderer(context, true)
{
}

RendererVK::~RendererVK()
{
}

platform::ERenderType RendererVK::getRenderType() const
{
    return platform::ERenderType::eRenderVulkan;
}

void RendererVK::immediateInit()
{
}

void RendererVK::immediaterBeginFrame()
{
}

void RendererVK::immediateEndFrame()
{
}

void RendererVK::immediatePresentFrame()
{
}

void RendererVK::immediateDraw()
{
}

void RendererVK::preRender(bool clear)
{
}

void RendererVK::postRender()
{
}

ShaderPtr RendererVK::makeSharedShader()
{
    return ShaderPtr();
}

ShaderProgramPtr RendererVK::makeSharedProgram()
{
    return ShaderProgramPtr();
}

GeometryPtr RendererVK::makeSharedGeometry(const CRenderTechnique * technique)
{
    return GeometryPtr();
}

RenderStatePtr RendererVK::makeSharedRenderState()
{
    return RenderStatePtr();
}

RenderTargetPtr RendererVK::makeSharedRenderTarget()
{
    return RenderTargetPtr();
}

GeometryTargetPtr RendererVK::makeSharedGeometryTarget()
{
    return GeometryTargetPtr();
}

void RendererVK::resetTextures()
{
}

} //namespace vk
} //namespace renderer
} //namespace v3d

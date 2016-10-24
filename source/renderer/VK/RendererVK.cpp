#include "RendererVK.h"
#include "MemoryManagerVK.h"
#include "CommandPoolVK.h"
#include "CommandBufferVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

RendererVK::RendererVK(const ContextPtr context)
    : IRenderer(context, true)
    , m_memoryMamager(nullptr)
    , m_commandPool(nullptr)
{
    m_device = RendererVK::getVulkanContext()->getVulkanDevice();
    m_queueFamilyIndex = RendererVK::getVulkanContext()->getVulkanQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
}

RendererVK::~RendererVK()
{
    ASSERT(!m_memoryMamager, "m_memoryMamager already exist");
    ASSERT(!m_commandPool, "m_commandPool already exist");
}

platform::ERenderType RendererVK::getRenderType() const
{
    return platform::ERenderType::eRenderVulkan;
}

void RendererVK::immediateInit()
{
    if (!m_memoryMamager)
    {
        m_memoryMamager = new MemoryManagerVK(RendererVK::getVulkanContext());
    }

    if (!m_commandPool)
    {
        m_commandPool = new CommandPoolVK(RendererVK::getVulkanContext(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    }

    //TODO:
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

const ContextVKPtr RendererVK::getVulkanContext() const
{
    return std::static_pointer_cast<DeviceContextVK>(m_context.lock());
}

MemoryManagerVK* RendererVK::getMemoryManager()
{
    return m_memoryMamager;
}

CommandBufferVK* RendererVK::getCurrentCommandBuffer() const
{
    return m_commandPool->commandBuffer();
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

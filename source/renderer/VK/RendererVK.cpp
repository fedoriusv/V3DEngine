#include "RendererVK.h"
#include "MemoryManagerVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

RendererVK::RendererVK(const ContextPtr context)
    : IRenderer(context, false)
    , m_memoryMamager(nullptr)
{
    m_device = RendererVK::getVulkanContext()->getVulkanDevice();
    m_queueFamilyIndex = RendererVK::getVulkanContext()->getVulkanQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
}

RendererVK::~RendererVK()
{
    ASSERT(!m_memoryMamager, "m_memoryMamager exist");
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

    //TEST 
    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = nullptr;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = VK_FORMAT_R8_UNORM;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.pQueueFamilyIndices = &m_queueFamilyIndex;
    imageCreateInfo.queueFamilyIndexCount = 1;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.extent = { 10, 10, 1};
    imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    VkImage image;
    VkResult result = vkCreateImage(m_device, &imageCreateInfo, nullptr, &image);
    AllocatorVK* alloc = m_memoryMamager->getSimpleAllocator();
    m_memoryMamager->allocateImage(*alloc, image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
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

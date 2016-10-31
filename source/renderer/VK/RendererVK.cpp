#include "RendererVK.h"
#include "utils/Logger.h"

#ifdef _VULKAN_RENDER_
#include "MemoryManagerVK.h"
#include "CommandPoolVK.h"
#include "CommandBufferVK.h"
#include "context/DebugVK.h"
#include "RenderStateVK.h"
#include "FramebufferVK.h"

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

void RendererVK::createGraphicPipeline(const RenderStateVK* renderState, const FramebufferVK* framebuffer)
{
    VkPipeline pipeline;
    VkPipelineCache pipelineCache = VK_NULL_HANDLE;

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pNext = nullptr;
    pipelineCreateInfo.flags = 0; //VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT;

    VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo = {};
    pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipelineDynamicStateCreateInfo.pNext = nullptr;
    pipelineDynamicStateCreateInfo.flags = 0;
    pipelineDynamicStateCreateInfo.dynamicStateCount = k_dynamicStateVK.size();
    pipelineDynamicStateCreateInfo.pDynamicStates = k_dynamicStateVK.data();

    pipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;

    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {};
    pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineViewportStateCreateInfo.pNext = nullptr;
    pipelineViewportStateCreateInfo.flags = 0;

    if (RendererVK::checkPresentState(k_dynamicStateVK, VK_DYNAMIC_STATE_VIEWPORT))
    {
        pipelineViewportStateCreateInfo.viewportCount = 1;
        pipelineViewportStateCreateInfo.pViewports = nullptr;
    }
    else
    {
        VkViewport viewport = {};
        viewport.x = framebuffer->getViewport().getLeftX();
        viewport.y = framebuffer->getViewport().getTopY();
        viewport.width = framebuffer->getViewport().getWidth();
        viewport.height = framebuffer->getViewport().getHeight();
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        pipelineViewportStateCreateInfo.viewportCount = 1;
        pipelineViewportStateCreateInfo.pViewports = &viewport;
    }

    if (checkPresentState(k_dynamicStateVK, VK_DYNAMIC_STATE_SCISSOR))
    {
        pipelineViewportStateCreateInfo.scissorCount = 1;
        pipelineViewportStateCreateInfo.pScissors = nullptr;
    }
    else
    {
        VkRect2D scissor = {};
        scissor.extent = { 0, 0 };
        scissor.offset = { 0, 0 };

        pipelineViewportStateCreateInfo.scissorCount = 1;
        pipelineViewportStateCreateInfo.pScissors = &scissor;
    }

    pipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
    pipelineCreateInfo.pRasterizationState = &renderState->getPipelineRasterizationStateCreateInfo();
    pipelineCreateInfo.pColorBlendState = &renderState->getPipelineColorBlendStateCreateInfo(framebuffer);
    pipelineCreateInfo.pDepthStencilState = &renderState->getPipelineDepthStencilStateCreateInfo();
    pipelineCreateInfo.pMultisampleState = &renderState->getPipelineMultisampleStateCreateInfo();

    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {};
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
    pipelineInputAssemblyStateCreateInfo.flags = 0;
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
    pipelineInputAssemblyStateCreateInfo.topology = ;

    pipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
    pipelineCreateInfo.pTessellationState = ;

    VkResult result = vkCreateGraphicsPipelines(m_device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &pipeline);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("RendererVK::createGraphicPipeline: vkCreateGraphicsPipelines. Error: %s", DebugVK::errorString(result).c_str());
    }
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


#endif //_VULKAN_RENDER_

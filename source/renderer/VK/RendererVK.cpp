#include "RendererVK.h"
#include "utils/Logger.h"

#ifdef _VULKAN_RENDER_
#include "MemoryManagerVK.h"
#include "CommandPoolVK.h"
#include "CommandBufferVK.h"
#include "context/DebugVK.h"
#include "RenderStateVK.h"
#include "FramebufferVK.h"
#include "GeometryVK.h"
#include "ShaderProgramVK.h"

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
    ASSERT(m_pipelineList.empty(), "m_pipelineList is not empty");
}

ERenderType RendererVK::getRenderType() const
{
    return ERenderType::eRenderVulkan;
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

VkPipeline RendererVK::createGraphicPipeline(const RenderStateVK* renderState, const FramebufferVK* framebuffer, const GeometryVK* geometry, const ShaderProgramVK* program)
{
    VkPipeline pipeline;
    VkPipelineCache pipelineCache = VK_NULL_HANDLE;

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pNext = nullptr;
    pipelineCreateInfo.flags = 0; //VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

    VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo = {};
    pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipelineDynamicStateCreateInfo.pNext = nullptr;
    pipelineDynamicStateCreateInfo.flags = 0;
    pipelineDynamicStateCreateInfo.dynamicStateCount = static_cast<u32>(k_dynamicStateVK.size());
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
        viewport.x = static_cast<f32>(framebuffer->getViewport().getLeftX());
        viewport.y = static_cast<f32>(framebuffer->getViewport().getTopY());
        viewport.width = static_cast<f32>(framebuffer->getViewport().getWidth());
        viewport.height = static_cast<f32>(framebuffer->getViewport().getHeight());
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
    pipelineCreateInfo.pInputAssemblyState = &renderState->getPipelineInputAssemblyStateCreateInfo();
    pipelineCreateInfo.pTessellationState = &renderState->getPipelineTessellationStateCreateInfo();
    pipelineCreateInfo.pVertexInputState = &geometry->getPipelineVertexInputStateCreateInfo();

    std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos;
    u32 stageCount = program->getShaderStagesCount();
    for (u32 stage = 0; stage < stageCount; ++stage)
    {
        pipelineShaderStageCreateInfos.push_back(program->createShaderStage(stage));
    }
    pipelineCreateInfo.stageCount = static_cast<u32>(pipelineShaderStageCreateInfos.size());
    pipelineCreateInfo.pStages = pipelineShaderStageCreateInfos.data();

    //pipelineCreateInfo.layout = ;

    //pipelineCreateInfo.renderPass = ;
    //pipelineCreateInfo.subpass = ;

    VkResult result = vkCreateGraphicsPipelines(m_device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &pipeline);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("RendererVK::createGraphicPipeline: vkCreateGraphicsPipelines. Error: %s", DebugVK::errorString(result).c_str());
        return VK_NULL_HANDLE;
    }

    return pipeline;
}

VkPipeline RendererVK::getGraphicPipeline(const RenderStateVK* renderState, const FramebufferVK* framebuffer, const ShaderProgramVK* program)
{
    u64 hashPipeline = 0; //TODO: get hash
    auto pipeline = m_pipelineList.find(hashPipeline);
    if (pipeline == m_pipelineList.cend())
    {
        VkPipeline newPipeline = RendererVK::createGraphicPipeline(renderState, framebuffer, nullptr, program);
        m_pipelineList.insert(std::make_pair(hashPipeline, newPipeline));

        return newPipeline;
    }

    return pipeline->second;
}

void RendererVK::destroyGraphicPipelines()
{
    for (auto& pipline : m_pipelineList)
    {
        if (pipline.second != VK_NULL_HANDLE)
        {
            vkDestroyPipeline(m_device, pipline.second, nullptr);
            pipline.second = VK_NULL_HANDLE;
        }
    }

    m_pipelineList.clear();
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

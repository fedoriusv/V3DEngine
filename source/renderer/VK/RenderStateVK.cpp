#include "RenderStateVK.h"

#ifdef _VULKAN_RENDER_

namespace v3d
{
namespace renderer
{
namespace vk
{

VkCullModeFlagBits ECullfaceVK[ECullMode::eCullModeCount] =
{
    VK_CULL_MODE_NONE,
    VK_CULL_MODE_FRONT_BIT,
    VK_CULL_MODE_BACK_BIT,
    VK_CULL_MODE_FRONT_AND_BACK
};

VkPolygonMode EPolygonModeVK[EPolygonMode::ePolygonModeCount] =
{
    VK_POLYGON_MODE_FILL,
    VK_POLYGON_MODE_LINE,
    VK_POLYGON_MODE_POINT
};

VkFrontFace EWindingGL[] =
{
    VK_FRONT_FACE_CLOCKWISE,
    VK_FRONT_FACE_COUNTER_CLOCKWISE
};

RenderStateVK::RenderStateVK()
{
}

RenderStateVK::~RenderStateVK()
{
}

VkPipelineRasterizationStateCreateInfo RenderStateVK::getPipelineRasterizationStateCreateInfo()
{
    if (m_isChanged)
    {
        VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {};
        pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pipelineRasterizationStateCreateInfo.pNext = nullptr;
        pipelineRasterizationStateCreateInfo.flags = 0;
        pipelineRasterizationStateCreateInfo.cullMode = ECullfaceVK[m_culling];
        pipelineRasterizationStateCreateInfo.polygonMode = EPolygonModeVK[m_polygonMode];
        pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = m_isRasterizerEnable ? VK_TRUE : VK_FALSE;
        pipelineRasterizationStateCreateInfo.frontFace = EWindingGL[m_winding];

        pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

        pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_TRUE;
        pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
        pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
        pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
        pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
    }

    return m_pipelineRasterizationStateCreateInfo;
}

void RenderStateVK::bind()
{
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

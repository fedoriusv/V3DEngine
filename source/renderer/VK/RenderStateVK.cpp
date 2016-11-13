#include "RenderStateVK.h"
#include "Engine.h"

#ifdef _VULKAN_RENDER_
#include "RendererVK.h"
#include "FramebufferVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

extern VkSampleCountFlagBits getSampleCountVK(u32 size, ETextureTarget target);

const VkCullModeFlagBits ECullfaceVK[ECullMode::eCullModeCount] =
{
    VK_CULL_MODE_NONE,
    VK_CULL_MODE_FRONT_BIT,
    VK_CULL_MODE_BACK_BIT,
    VK_CULL_MODE_FRONT_AND_BACK
};

const VkPolygonMode EPolygonModeVK[EPolygonMode::ePolygonModeCount] =
{
    VK_POLYGON_MODE_FILL,
    VK_POLYGON_MODE_LINE,
    VK_POLYGON_MODE_POINT
};

const VkFrontFace EWindingGL[] =
{
    VK_FRONT_FACE_CLOCKWISE,
    VK_FRONT_FACE_COUNTER_CLOCKWISE
};

const VkBlendFactor EBlendFactorVK[EBlendFactor::eBlendFactorCount] =
{
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_FACTOR_ONE,
    VK_BLEND_FACTOR_SRC_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    VK_BLEND_FACTOR_DST_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_FACTOR_DST_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    VK_BLEND_FACTOR_CONSTANT_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    VK_BLEND_FACTOR_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_SRC_ALPHA_SATURATE
};

const VkBlendOp EBlendEquationVK[EBlendEquation::eFuncBlendEquationCount] =
{
    VK_BLEND_OP_ADD,
    VK_BLEND_OP_SUBTRACT,
    VK_BLEND_OP_REVERSE_SUBTRACT,
    VK_BLEND_OP_MIN,
    VK_BLEND_OP_MAX
};

const VkCompareOp ECompareFuncVK[ECompareFunc::eCompareCount] =
{
    VK_COMPARE_OP_NEVER,
    VK_COMPARE_OP_LESS,
    VK_COMPARE_OP_LESS_OR_EQUAL,
    VK_COMPARE_OP_EQUAL,
    VK_COMPARE_OP_GREATER_OR_EQUAL,
    VK_COMPARE_OP_GREATER,
    VK_COMPARE_OP_NOT_EQUAL,
    VK_COMPARE_OP_ALWAYS
};

const VkPrimitiveTopology EPrimitivesModeVK[EPrimitivesTopology::ePrimitivesTopologyCount] =
{
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
};

RenderStateVK::RenderStateVK()
{
}

RenderStateVK::~RenderStateVK()
{
}

const VkPipelineRasterizationStateCreateInfo& RenderStateVK::getPipelineRasterizationStateCreateInfo() const
{
    if (m_isChanged)
    {
        RenderStateVK::updatePipelineRasterizationStateCreateInfo();
    }

    return m_pipelineRasterizationStateCreateInfo;
}

const VkPipelineColorBlendStateCreateInfo& RenderStateVK::getPipelineColorBlendStateCreateInfo(const FramebufferVK* framebuffer) const
{
    if (m_isChanged)
    {
        RenderStateVK::updaePipelineColorBlendStateCreateInfo(framebuffer);
    }

    return m_pipelineColorBlendStateCreateInfo;
}

const VkPipelineDepthStencilStateCreateInfo& RenderStateVK::getPipelineDepthStencilStateCreateInfo() const
{
    if (m_isChanged)
    {
        RenderStateVK::updatePipelineDepthStencilStateCreateInfo();
    }

    return m_pipelineDepthStencilStateCreateInfo;
}

const VkPipelineMultisampleStateCreateInfo& RenderStateVK::getPipelineMultisampleStateCreateInfo() const
{
    if (m_isChanged)
    {
        RenderStateVK::updatePipelineMultisampleStateCreateInfo();
    }

    return m_pipelineMultisampleStateCreateInfo;
}

const VkPipelineInputAssemblyStateCreateInfo& RenderStateVK::getPipelineInputAssemblyStateCreateInfo() const
{
    if (m_isChanged)
    {
        RenderStateVK::updatePipelineInputAssemblyStateCreateInfo();
    }

    return m_pipelineInputAssemblyStateCreateInfo;
}

const VkPipelineTessellationStateCreateInfo& RenderStateVK::getPipelineTessellationStateCreateInfo() const
{
    if (m_isChanged)
    {
        RenderStateVK::updateVkPipelineTessellationStateCreateInfo();
    }

    return m_pipelineTessellationStateCreateInfo;
}

void RenderStateVK::bind()
{
    //TODO:
}

void RenderStateVK::updatePipelineRasterizationStateCreateInfo() const
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

    m_pipelineRasterizationStateCreateInfo = pipelineRasterizationStateCreateInfo;
}

void RenderStateVK::updaePipelineColorBlendStateCreateInfo(const FramebufferVK* framebuffer) const
{
    VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {};
    pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineColorBlendStateCreateInfo.pNext = nullptr;
    pipelineColorBlendStateCreateInfo.flags = 0;

    //pipelineColorBlendStateCreateInfo.logicOpEnable = ;
    //pipelineColorBlendStateCreateInfo.logicOp = ;

    if (RendererVK::checkPresentState(k_dynamicStateVK, VK_DYNAMIC_STATE_BLEND_CONSTANTS))
    {
        pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
        pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
        pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
        pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;
    }
    else
    {
        pipelineColorBlendStateCreateInfo.blendConstants[0] = 1.0f;
        pipelineColorBlendStateCreateInfo.blendConstants[1] = 1.0f;
        pipelineColorBlendStateCreateInfo.blendConstants[2] = 1.0f;
        pipelineColorBlendStateCreateInfo.blendConstants[3] = 1.0f;
    }


    u32 attachmentCount = framebuffer->countAttachments();
    std::vector<VkPipelineColorBlendAttachmentState> pipelineColorBlendAttachmentStates;
    for (u32 i = 0; i < attachmentCount; ++i)
    {
        VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {};

        const RenderTarget::SAttachments& attachment = framebuffer->getAttachment(i);
        if (ImageFormat::componentCount(attachment._format) == 1)
        {
            pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT;
        }
        else if (ImageFormat::componentCount(attachment._format) == 2)
        {
            pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT;
        }
        else if (ImageFormat::componentCount(attachment._format) == 3)
        {
            pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT;
        }
        else if (ImageFormat::componentCount(attachment._format) == 4)
        {
            pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        }
        else
        {
            pipelineColorBlendAttachmentState.colorWriteMask = 0;
        }

        pipelineColorBlendAttachmentState.blendEnable = m_blend ? VK_TRUE : VK_FALSE;

        pipelineColorBlendAttachmentState.colorBlendOp = EBlendEquationVK[m_blendColorEquation];
        pipelineColorBlendAttachmentState.srcColorBlendFactor = EBlendFactorVK[m_blendColorSrc];
        pipelineColorBlendAttachmentState.dstColorBlendFactor = EBlendFactorVK[m_blendColorDst];

        pipelineColorBlendAttachmentState.alphaBlendOp = EBlendEquationVK[m_blendAlphaEquation];
        pipelineColorBlendAttachmentState.srcAlphaBlendFactor = EBlendFactorVK[m_blendAlphaSrc];
        pipelineColorBlendAttachmentState.dstAlphaBlendFactor = EBlendFactorVK[m_blendAlphaDst];

        pipelineColorBlendAttachmentStates.push_back(pipelineColorBlendAttachmentState);
    }

    pipelineColorBlendStateCreateInfo.attachmentCount = static_cast<u32>(pipelineColorBlendAttachmentStates.size());
    pipelineColorBlendStateCreateInfo.pAttachments = pipelineColorBlendAttachmentStates.data();

    m_pipelineColorBlendStateCreateInfo = pipelineColorBlendStateCreateInfo;
}

void RenderStateVK::updatePipelineDepthStencilStateCreateInfo() const
{
    VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo = {};
    pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineDepthStencilStateCreateInfo.pNext = nullptr;
    pipelineDepthStencilStateCreateInfo.flags = 0;
    pipelineDepthStencilStateCreateInfo.depthTestEnable = m_depthTest ? VK_TRUE : VK_FALSE;
    pipelineDepthStencilStateCreateInfo.depthWriteEnable = m_depthWrite ? VK_TRUE : VK_FALSE;
    pipelineDepthStencilStateCreateInfo.depthCompareOp = ECompareFuncVK[m_depthFunc];

    if (RendererVK::checkPresentState(k_dynamicStateVK, VK_DYNAMIC_STATE_DEPTH_BOUNDS))
    {
        pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_TRUE;
        pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
        pipelineDepthStencilStateCreateInfo.maxDepthBounds = 1.0f;
    }
    else
    {
        pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
        pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
        pipelineDepthStencilStateCreateInfo.maxDepthBounds = 1.0f;
    }

    pipelineDepthStencilStateCreateInfo.stencilTestEnable = m_stencilTest ? VK_TRUE : VK_FALSE;
   /* pipelineDepthStencilStateCreateInfo.front.compareMask = ;
    pipelineDepthStencilStateCreateInfo.front.compareOp = ;
    pipelineDepthStencilStateCreateInfo.front.depthFailOp = ;
    pipelineDepthStencilStateCreateInfo.front.failOp = ;
    pipelineDepthStencilStateCreateInfo.front.passOp = ;
    pipelineDepthStencilStateCreateInfo.front.reference = ;
    pipelineDepthStencilStateCreateInfo.front.writeMask = ;
    pipelineDepthStencilStateCreateInfo.back.compareMask = ;
    pipelineDepthStencilStateCreateInfo.back.compareOp = ;
    pipelineDepthStencilStateCreateInfo.back.depthFailOp = ;
    pipelineDepthStencilStateCreateInfo.back.failOp = ;
    pipelineDepthStencilStateCreateInfo.back.passOp = ;
    pipelineDepthStencilStateCreateInfo.back.reference = ;
    pipelineDepthStencilStateCreateInfo.back.writeMask = ;*/

    m_pipelineDepthStencilStateCreateInfo = pipelineDepthStencilStateCreateInfo;
}

void RenderStateVK::updatePipelineMultisampleStateCreateInfo() const
{
    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {};
    pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineMultisampleStateCreateInfo.pNext = nullptr;
    pipelineMultisampleStateCreateInfo.flags = 0;

    if (ENGINE_CONTEXT->getSamplesCount() >= m_rasterizationSamples)
    {
        pipelineMultisampleStateCreateInfo.rasterizationSamples = getSampleCountVK(m_rasterizationSamples, ETextureTarget::eTexture2DMSAA);
        pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
        pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
        pipelineMultisampleStateCreateInfo.minSampleShading = 0.25f;
        pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
        pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    }
    else
    {
        pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
        pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
        pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
        pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
        pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    }

    m_pipelineMultisampleStateCreateInfo = pipelineMultisampleStateCreateInfo;
}

void RenderStateVK::updatePipelineInputAssemblyStateCreateInfo() const
{
    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {};
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
    pipelineInputAssemblyStateCreateInfo.flags = 0;
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
    pipelineInputAssemblyStateCreateInfo.topology = EPrimitivesModeVK[m_topology];

    m_pipelineInputAssemblyStateCreateInfo = pipelineInputAssemblyStateCreateInfo;
}

void RenderStateVK::updateVkPipelineTessellationStateCreateInfo() const
{
    VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo = {};
    pipelineTessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    pipelineTessellationStateCreateInfo.pNext = nullptr;
    pipelineTessellationStateCreateInfo.flags = 0;

    if (EPrimitivesModeVK[m_topology] == EPrimitivesTopology::ePatches)
    {
        pipelineTessellationStateCreateInfo.patchControlPoints = m_patches;
    }
    else
    {
        pipelineTessellationStateCreateInfo.patchControlPoints = 0U;
    }

    m_pipelineTessellationStateCreateInfo = pipelineTessellationStateCreateInfo;
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

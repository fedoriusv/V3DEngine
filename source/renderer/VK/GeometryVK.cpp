#include "GeometryVK.h"

#ifdef _VULKAN_RENDER_

namespace v3d
{
namespace renderer
{
namespace vk
{
GeometryVK::GeometryVK(const CRenderTechnique* technique)
    : Geometry(technique)
{
}

GeometryVK::~GeometryVK()
{
}

void GeometryVK::init()
{
    if (m_initialized)
    {
        return;
    }

    const RenderPassPtr pass = m_technique->getRenderPass(m_technique->getCurrentPass());
    u32 mask = pass->getDefaultShaderData()->getVertexFormatMask() | pass->getUserShaderData()->getVertexFormatMask();
    Geometry::setVertexMask(mask);

    ShaderDataList shaderData;
    shaderData.push_back(pass->getDefaultShaderData());
    shaderData.push_back(pass->getUserShaderData());

    GeometryVK::updatePipelineVertexInputStateCreateInfo(shaderData);

    m_initialized = true;
}

void GeometryVK::free()
{
}

void GeometryVK::draw()
{
    //TODO: will need remove from here
}

void GeometryVK::refresh()
{
    GeometryVK::updatePipelineVertexInputStateCreateInfo();
}

const VkPipelineVertexInputStateCreateInfo& GeometryVK::getPipelineVertexInputStateCreateInfo() const
{
    return m_pipelineVertexInputStateCreateInfo;
}
void GeometryVK::updatePipelineVertexInputStateCreateInfo(const ShaderDataList& shaderDataList)
{
    VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {};
    pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pipelineVertexInputStateCreateInfo.pNext = nullptr;
    pipelineVertexInputStateCreateInfo.flags = 0;

    std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
    for (auto& shaderData : shaderDataList)
    {
        for (const AttributePair& attr : shaderData.lock()->getAttributeList())
        {
            CShaderAttribute::EShaderAttribute attribute = attr.second->getData();
            if (attr.second->getID() < 0)
            {
                continue;
            }

            //TODO:

            VkVertexInputAttributeDescription vertexInputAttributeDescription = {};
            vertexInputAttributeDescription.location = ;
            vertexInputAttributeDescription.binding = ;
            vertexInputAttributeDescription.offset = ;
            vertexInputAttributeDescription.format = ;

            vertexInputAttributeDescriptions.push_back(vertexInputAttributeDescription);
        }
    }

    pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<u32>(vertexInputAttributeDescriptions.size());
    pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data();

    std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;

    //TODO:

    pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<u32>(vertexInputBindingDescriptions.size());
    pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vertexInputBindingDescriptions.data();

    m_pipelineVertexInputStateCreateInfo = pipelineVertexInputStateCreateInfo;
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

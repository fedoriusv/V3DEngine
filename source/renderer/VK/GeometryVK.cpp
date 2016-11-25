#include "GeometryVK.h"
#include "utils/Logger.h"
#include "renderer/ShaderAttribute.h"

#ifdef _VULKAN_RENDER_
#include "BufferVK.h"


namespace v3d
{
namespace renderer
{
namespace vk
{

extern VkFormat getImageFormatVK(EImageFormat format, EImageType type);

GeometryVK::GeometryVK(const RenderTechnique* technique)
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

    if (m_data.empty())
    {
        LOG_WARNING("GeometryVK::init: Data empty");
        return;
    }

    const RenderPassPtr pass = m_technique->getRenderPass(m_technique->getCurrentPass());
    u32 mask = pass->getDefaultShaderData()->getVertexFormatMask() | pass->getUserShaderData()->getVertexFormatMask();
    Geometry::setVertexMask(mask);

    ShaderDataList shaderData;
    shaderData.push_back(pass->getDefaultShaderData());
    shaderData.push_back(pass->getUserShaderData());

    m_vertexBuffer = new BufferVK(eVertexBuffer, eWriteStatic, false);
    static_cast<BufferVK*>(m_vertexBuffer)->create();

    if (m_data.indicesSize() > 0)
    {
        m_indexBuffer = new BufferVK(eIndexBuffer, eWriteStatic, false);
        static_cast<BufferVK*>(m_indexBuffer)->create();
    }
    
    GeometryVK::updatePipelineVertexInputStateCreateInfo(shaderData);

    m_initialized = true;
}

void GeometryVK::free()
{
    if (m_vertexBuffer)
    {
        static_cast<BufferVK*>(m_vertexBuffer)->destroy();

        delete m_vertexBuffer;
        m_vertexBuffer = nullptr;
    }

    if (m_indexBuffer)
    {
        static_cast<BufferVK*>(m_indexBuffer)->destroy();

        delete m_indexBuffer;
        m_indexBuffer = nullptr;
    }
}

void GeometryVK::draw()
{
    //TODO: will need remove from here
}

void GeometryVK::refresh()
{
    if (!m_initialized)
    {
        return;
    }

    if (m_data.empty())
    {
        LOG_WARNING("GeometryVK::refresh: Data empty");
        return;
    }

    u32 passIdx = m_technique->getCurrentPass();
    const RenderPassPtr pass = m_technique->getRenderPass(passIdx);

    ShaderDataList shaderData;
    shaderData.push_back(pass->getDefaultShaderData());
    shaderData.push_back(pass->getUserShaderData());

    GeometryVK::updatePipelineVertexInputStateCreateInfo(shaderData);
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

    s32 offset = 0;
    s32 size = 0;
    u32 layer = 0;
    u32 bufferSize = Geometry::computeBufferSize(shaderDataList);
    ASSERT(bufferSize > 0, "invalid size");
    u8* tempData = reinterpret_cast<u8*>(std::malloc(bufferSize));

    u32 binding = 0;
    std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
    std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;
    for (auto& shaderData : shaderDataList)
    {
        for (const AttributePair& attr : shaderData.lock()->getAttributeList())
        {
            ShaderAttribute::EShaderAttribute attribute = attr.second->getChannel();
            if (attr.second->getLocation() < 0)
            {
                LOG_WARNING("GeometryVK: invalid location or binding. Attribute [%s]. Skip", attr.second->getName());
                continue;
            }

            VkVertexInputAttributeDescription vertexInputAttributeDescription = {};
            vertexInputAttributeDescription.location = attr.second->getLocation();
            vertexInputAttributeDescription.binding = binding;

            VkVertexInputBindingDescription vertexInputBindingDescription = {};
            vertexInputBindingDescription.binding = binding;

            binding++;

            switch (attribute)
            {
            case ShaderAttribute::eAttribVertexPosition:
            {
                size = sizeof(f32)* m_data.verticesSize() * 3;
                memcpy(tempData + offset, m_data._vertices.data(), size);

                vertexInputAttributeDescription.offset = offset;
                vertexInputAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
                offset += size;

                vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                vertexInputBindingDescription.stride = size;
            }
            break;

            case ShaderAttribute::eAttribVertexNormal:
            {
                size = sizeof(f32)* m_data.verticesSize() * 3;
                memcpy(tempData + offset, m_data._normals.data(), size);

                vertexInputAttributeDescription.offset = offset;
                vertexInputAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
                offset += size;

                vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                vertexInputBindingDescription.stride = size;
            }
            break;

            case ShaderAttribute::eAttribVertexColor:
            {
                size = sizeof(f32)* m_data.verticesSize() * 3;
                memcpy(tempData + offset, m_data._colors.data(), size);

                vertexInputAttributeDescription.offset = offset;
                vertexInputAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
                offset += size;

                vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                vertexInputBindingDescription.stride = size;
            }
            break;

            case ShaderAttribute::eAttribVertexBinormal:
            {
                size = sizeof(f32)* m_data.verticesSize() * 3;
                memcpy(tempData + offset, m_data._binormals.data(), size);

                vertexInputAttributeDescription.offset = offset;
                vertexInputAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
                offset += size;

                vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                vertexInputBindingDescription.stride = size;
            }
            break;

            case ShaderAttribute::eAttribVertexTangent:
            {
                size = sizeof(f32)* m_data.verticesSize() * 3;
                memcpy(tempData + offset, m_data._tangents.data(), size);

                vertexInputAttributeDescription.offset = offset;
                vertexInputAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
                offset += size;

                vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                vertexInputBindingDescription.stride = size;
            }
            break;

            case ShaderAttribute::eAttribVertexTexture0:
            case ShaderAttribute::eAttribVertexTexture1:
            case ShaderAttribute::eAttribVertexTexture2:
            case ShaderAttribute::eAttribVertexTexture3:
            {
                size = sizeof(f32)* m_data.verticesSize() * 2;
                memcpy(tempData + offset, m_data._texCoords[layer].data(), size);

                vertexInputAttributeDescription.offset = offset;
                vertexInputAttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
                offset += size;
                ++layer;

                vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                vertexInputBindingDescription.stride = size;
            }
            break;

            case ShaderAttribute::eAttribParticalPosition:
            case ShaderAttribute::eAttribParticalColor:
            case ShaderAttribute::eAttribParticalVelocity:
            case ShaderAttribute::eAttribParticalLifeTime:
            case ShaderAttribute::eAttribParticalSize:
            case ShaderAttribute::eAttribParticalType:
            case ShaderAttribute::eAttribUser:
            {
                if (attr.second->getUserDataCount() <= 0)
                {
                    LOG_WARNING("GeometryVK: User data have diferent size [%d]", attr.second->getUserDataCount());
                }

                size = attr.second->getUserDataSize() * attr.second->getUserDataCount();
                if (size > 0)
                {
                    memcpy(tempData + offset, attr.second->getUserData(), size);
                    auto formatFunc = [](ShaderDataType::EShaderDataType type) -> VkFormat
                    {
                        switch (type)
                        {
                        case ShaderDataType::eInt:
                            return VK_FORMAT_R32_SINT;

                        case ShaderDataType::eFloat:
                            return VK_FORMAT_R32_SFLOAT;

                        case ShaderDataType::eVector2f:
                            return VK_FORMAT_R32G32_SFLOAT;

                        case ShaderDataType::eVector3f:
                            return VK_FORMAT_R32G32B32_SFLOAT;

                        case ShaderDataType::eVector4f:
                            return VK_FORMAT_R32G32B32A32_SFLOAT;

                        case ShaderDataType::eDouble:
                        case ShaderDataType::eMatrix3f:
                        case ShaderDataType::eMatrix4f:
                        default:
                            ASSERT(false, "unsupported format");
                            return VK_FORMAT_UNDEFINED;
                        }
                    };

                    vertexInputAttributeDescription.offset = offset;
                    vertexInputAttributeDescription.format = formatFunc(attr.second->getDataType());
                    offset += size;

                    vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
                    vertexInputBindingDescription.stride = size;
                }
                else
                {
                    ASSERT(false, "invalid data");
                }
            }
            break;

            default:
                break;
            }

            vertexInputAttributeDescriptions.push_back(vertexInputAttributeDescription);
        }
    }

    m_vertexBuffer->update(0, bufferSize, tempData);

    std::free(tempData);
    tempData = nullptr;

    pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<u32>(vertexInputAttributeDescriptions.size());
    pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data();

    pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<u32>(vertexInputBindingDescriptions.size());
    pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vertexInputBindingDescriptions.data();

    m_pipelineVertexInputStateCreateInfo = pipelineVertexInputStateCreateInfo;
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

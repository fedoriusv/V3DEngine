#include "BufferVK.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _VULKAN_RENDER_
#include "context/DebugVK.h"
#include "RendererVK.h"
#include "CommandBufferVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

VkBufferUsageFlags EBufferTargetVK[EBufferTarget::eBufferTargetCount] =
{
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
};

BufferVK::BufferVK(EBufferTarget target, EDataUsageType type, bool mappable)
    : m_target(target)
    , m_type(type)

    , m_mappableMemory(mappable)
    , m_coherentMemory(false)
    , m_mapped(false)

    , m_device(VK_NULL_HANDLE)
    , m_queueFamilyIndex(0)

    , m_memory(k_invalidMemory)
    , m_buffer(VK_NULL_HANDLE)
    , m_flags(0)
    , m_usage(0)

    , m_initialized(false)
{
    LOG_DEBUG("BufferVK::BufferVK constructor %x", this);

    if (m_target == EBufferTarget::eStagingBuffer)
    {
        m_usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        m_mappableMemory = true;
        m_coherentMemory = false;

        return;
    }

    if (m_mappableMemory)
    {
        m_usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | EBufferTargetVK[m_target];
        if (m_type == EDataUsageType::eWriteStatic || m_type == EDataUsageType::eReadStatic || m_type == EDataUsageType::eCopyStatic)
        {
            m_coherentMemory = false;
        }
        else
        {
            m_coherentMemory = true;
        }
    }
    else //use staging transfer
    {
        m_usage = EBufferTargetVK[m_target];
        m_coherentMemory = false;
    }
}

BufferVK::~BufferVK()
{
    LOG_DEBUG("BufferVK::~BufferVK destructor %x", this);

    ASSERT(m_buffer == VK_NULL_HANDLE, "m_buffer already exist");
    ASSERT(m_memory._memory == VK_NULL_HANDLE, "m_memory already exist");
}

void BufferVK::bind() const
{
    //TODO:
}

void BufferVK::bindToTarget(EBufferTarget target, u32 offset, u32 size) const
{
    //TODO:
}

void BufferVK::unbind() const
{
    //TODO:
}

void BufferVK::update(u32 offset, u32 size, const void* data)
{
    if (!m_initialized)
    {
        return;
    }

    if (m_mappableMemory || m_coherentMemory)
    {
        void* mappedData = BufferVK::map(offset, size);
        ASSERT(mappedData, "m_memory._mapped not mapped");

        u8* memPos = reinterpret_cast<u8*>(mappedData);
        if ((memPos + offset) + size > memPos + m_memory._size)
        {
            ASSERT(false, "data range out");
        }
        memcpy(memPos + offset, data, size);

        BufferVK::unmap(offset, size);
    }
    else
    {
        BufferVK* stagingBuffer = new BufferVK(eStagingBuffer, eWriteStatic);
        if (stagingBuffer->create(size, data))
        {
            CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();
            commandBuffer->copyBufferToBuffer(stagingBuffer->m_buffer, m_buffer, size);
        }

        stagingBuffer->destroy();
        delete stagingBuffer;
    }
}

void BufferVK::read(u32 offset, u32 size, void* const data)
{
    if (!m_initialized)
    {
        return;
    }

    if (m_mappableMemory || m_coherentMemory)
    {
        void* mappedData = BufferVK::map(offset, size);
        ASSERT(mappedData, "m_memory._mapped not mapped");

        u8* memPos = reinterpret_cast<u8*>(mappedData);
        if ((memPos + offset) + size > memPos + m_memory._size)
        {
            ASSERT(false, "data range out");
        }
        memcpy(data, memPos + offset, size);

        BufferVK::unmap(offset, size);
    }
    else
    {
        BufferVK* stagingBuffer = new BufferVK(eStagingBuffer, eReadStatic);
        if (stagingBuffer->create())
        {
            CommandBufferVK* commandBuffer = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getCurrentCommandBuffer();
            commandBuffer->copyBufferToBuffer(m_buffer, stagingBuffer->m_buffer, size);

            stagingBuffer->read(offset, size, data);
        }

        stagingBuffer->destroy();
        delete stagingBuffer;
    }
}

void* const BufferVK::map(u32 offset, u32 size)
{
    if (!m_initialized)
    {
        return nullptr;
    }

    if (!m_mappableMemory)
    {
        ASSERT(false, "Try mapped not mappable memory");
        return nullptr;
    }

    if (!m_mapped && m_memory._mapped)
    {
        m_mapped = true;

        if (m_target == EBufferTarget::eStagingBuffer)
        {
            return m_memory._mapped;
        }

        MemoryManagerVK* memoryManager = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getMemoryManager();
        return memoryManager->beginAccessToHostMemory(m_memory, offset, size);
    }

    return nullptr;
}

bool BufferVK::unmap(u32 offset, u32 size)
{
    if (!m_initialized)
    {
        return false;
    }

    if (!m_mappableMemory)
    {
        ASSERT(false, "Try unmapped not mappable memory");
        return false;
    }

    if (m_mapped && m_memory._mapped)
    {
        m_mapped = false;

        if (m_target == EBufferTarget::eStagingBuffer)
        {
            return true;
        }

        MemoryManagerVK* memoryManager = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getMemoryManager();
        return memoryManager->endAccessToHostMemory(m_memory, offset, size);
    }

    return false;
}

bool BufferVK::unmap()
{
    if (!m_initialized)
    {
        return false;
    }

    if (!m_mappableMemory)
    {
        ASSERT(false, "Try unmapped not mappable memory");
        return false;
    }

    if (m_mapped && m_memory._mapped)
    {
        m_mapped = false;

        if (m_target == EBufferTarget::eStagingBuffer)
        {
            return true;
        }

         MemoryManagerVK* memoryManager = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getMemoryManager();
         return memoryManager->endAccessToHostMemory(m_memory, 0, VK_WHOLE_SIZE);
    }

    return false;
}

bool BufferVK::create(u32 size, const void* data)
{
    if (m_initialized)
    {
        return true;
    }

    m_device = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanDevice();
    m_queueFamilyIndex = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);

    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.flags = m_flags;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.pQueueFamilyIndices = &m_queueFamilyIndex;
    bufferCreateInfo.queueFamilyIndexCount = 1;
    bufferCreateInfo.size = static_cast<VkDeviceSize>(size);
    bufferCreateInfo.usage = m_usage;

    VkResult result = vkCreateBuffer(m_device, &bufferCreateInfo, nullptr, &m_buffer);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("BufferVK::create. Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    MemoryManagerVK* memoryManager = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getMemoryManager();
    VkMemoryPropertyFlags memFlags = 0;
    if (m_mappableMemory)
    {
        memFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        if (m_coherentMemory)
        {
            memFlags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        }
        else
        {
            memFlags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
        }
    }
    else
    {
        memFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }

    m_memory = memoryManager->allocateBuffer(*memoryManager->getSimpleAllocator(), m_buffer, memFlags);
    if (m_memory._memory == VK_NULL_HANDLE)
    {
        LOG_ERROR("BufferVK::create: return invalid memory");
        return false;
    }

    m_initialized = true;
    BufferVK::update(0, size, data);

    return true;
}

void BufferVK::destroy()
{
    if (m_memory._memory != VK_NULL_HANDLE)
    {
        MemoryManagerVK* memoryManager = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getMemoryManager();
        memoryManager->free(*memoryManager->getSimpleAllocator(), m_memory);
    }

    if (m_buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_device, m_buffer, nullptr);
    }
}

VkBuffer BufferVK::getBuffer() const
{
    return m_buffer;
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

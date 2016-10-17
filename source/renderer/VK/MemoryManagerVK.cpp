#include "MemoryManagerVK.h"
#include "utils/Logger.h"

#ifdef _VULKAN_RENDER_
#include "context/DebugVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

const SMemoryVK k_InvalidMemory =
{
    VK_NULL_HANDLE,
    0,
    0,
    nullptr,
    -1,
    0
};

using namespace utils;


MemoryManagerVK::MemoryManagerVK(const ContextVKPtr context)
    : m_imagePoolAllocator(1 * 1024 * 1024, 256, 512) //image allocator
    , m_bufferPoolAllocator(1 * 1024, 128, 256) //buffer allocator
    , m_device(context->getVulkanDevice())
    , m_physicalDeviceMemoryProperties(context->getVulkanPhysicalDeviceMemoryProperties())

{
}

MemoryManagerVK::~MemoryManagerVK()
{
}

SimpleAllocatorVK* MemoryManagerVK::getSimpleAllocator()
{
    return &m_simpleAllocator;
}

PoolsAllocatorVK* MemoryManagerVK::getImagePoolAllocator()
{
    return &m_imagePoolAllocator;
}

PoolsAllocatorVK* MemoryManagerVK::getBufferPoolAllocator()
{
    return &m_bufferPoolAllocator;
}

SMemoryVK MemoryManagerVK::allocateImage(AllocatorVK& allocator, VkImage image, VkMemoryPropertyFlags flags)
{
    if (image == VK_NULL_HANDLE)
    {
        LOG_ERROR("MemoryManagerVK::allocateImage: invalid image");
        ASSERT(false, "invalid image");

        return k_InvalidMemory;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    VkMemoryRequirements memoryRequirements = {};
    vkGetImageMemoryRequirements(m_device, image, &memoryRequirements);

    s32 memoryTypeIndex = MemoryManagerVK::findMemoryTypeIndex(memoryRequirements, flags);
    if (memoryTypeIndex < 0)
    {
        LOG_ERROR("MemoryManagerVK::allocateImage: invalid memoryTypeIndex");
        ASSERT(false, "invalid memoryTypeIndex");

        return k_InvalidMemory;
    }

    SMemoryVK memory = allocator.allocate(m_device, m_physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags, memoryRequirements.size, memoryTypeIndex);
    if (memory._memory == VK_NULL_HANDLE)
    {
        return k_InvalidMemory;
    }

    VkResult result = vkBindImageMemory(m_device, image, memory._memory, memory._offset);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("MemoryManagerVK::allocateImage: vkBindImageMemory. Error %s", DebugVK::errorString(result).c_str());
        return k_InvalidMemory;
    }

    return memory;
}

SMemoryVK MemoryManagerVK::allocateBuffer(AllocatorVK& allocator, VkBuffer buffer, VkMemoryPropertyFlags flags)
{
    if (buffer == VK_NULL_HANDLE)
    {
        LOG_ERROR("MemoryManagerVK::allocateBuffer: invalid buffer");
        ASSERT(false, "invalid buffer");

        return k_InvalidMemory;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    VkMemoryRequirements memoryRequirements = {};
    vkGetBufferMemoryRequirements(m_device, buffer, &memoryRequirements);

    s32 memoryTypeIndex = MemoryManagerVK::findMemoryTypeIndex(memoryRequirements, flags);
    if (memoryTypeIndex < 0)
    {
        LOG_ERROR("MemoryManagerVK::allocateBuffer: invalid memoryTypeIndex");
        ASSERT(false, "invalid memoryTypeIndex");

        return k_InvalidMemory;
    }

    SMemoryVK memory = allocator.allocate(m_device, m_physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags, memoryRequirements.size, memoryTypeIndex);
    if (memory._memory == VK_NULL_HANDLE)
    {
        return k_InvalidMemory;
    }

    VkResult result = vkBindBufferMemory(m_device, buffer, memory._memory, memory._offset);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("MemoryManagerVK::allocateBuffer: vkBindBufferMemory. Error %s", DebugVK::errorString(result).c_str());
        return k_InvalidMemory;
    }

    return memory;
}

void MemoryManagerVK::free(AllocatorVK& allocator, SMemoryVK& memory)
{
    allocator.free(m_device, memory);
}

s32 MemoryManagerVK::findMemoryTypeIndex(const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags memoryPropertyFlags)
{
    u32 memoryTypeBits = memoryRequirements.memoryTypeBits;
    for (u32 memoryTypeIndex = 0; memoryTypeIndex < VK_MAX_MEMORY_TYPES; ++memoryTypeIndex)
    {
        if ((memoryTypeBits & 1) == 1)
        {
            if ((m_physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
            {
                return memoryTypeIndex;
            }
        }

        memoryTypeBits >>= 1;
    }

    return -1;
}

SimpleAllocatorVK::SimpleAllocatorVK()
{
}

SimpleAllocatorVK::~SimpleAllocatorVK()
{
}

SMemoryVK SimpleAllocatorVK::allocate(VkDevice device, VkMemoryPropertyFlags flags, u64 memorySize, s32 memoryTypeIndex)
{
    SMemoryVK memory = {};

    VkMemoryAllocateInfo memAllocInfo = {};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.pNext = nullptr;
    memAllocInfo.memoryTypeIndex = memoryTypeIndex;
    memAllocInfo.allocationSize = memorySize;

    VkResult result = vkAllocateMemory(device, &memAllocInfo, nullptr, &memory._memory);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("SimpleAllocator::allocate: vkAllocateMemory. Error %s", DebugVK::errorString(result).c_str());
        return k_InvalidMemory;
    }

    memory._size = memorySize;
    memory._offset = 0;
    memory._memoryTypeIndex = memoryTypeIndex;
    memory._flags = flags;
    memory._mapped = nullptr;
    if (memory._flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
        VkResult result = vkMapMemory(device, memory._memory, 0, VK_WHOLE_SIZE, 0, &memory._mapped);
        {
            LOG_ERROR("SimpleAllocator::allocate: vkMapMemory. Error %s", DebugVK::errorString(result).c_str());
            return k_InvalidMemory;
        }
    }

    return memory;
}

void SimpleAllocatorVK::free(VkDevice device, SMemoryVK& memory)
{
    if (memory._mapped)
    {
        vkUnmapMemory(device, memory._memory);
    }

    if (memory._memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, memory._memory, nullptr);
        memory._memory = VK_NULL_HANDLE;
    }
}

PoolsAllocatorVK::PoolsAllocatorVK(u64 initializeMemSize, u64 blockSize, u64 minAllocSize)
    : m_initializeMemSize(initializeMemSize)
    , m_blockSize(blockSize)
    , m_minAllocSize(minAllocSize)
{
}

PoolsAllocatorVK::~PoolsAllocatorVK()
{
    for (auto& pool : m_pools)
    {
        delete pool.second;
        pool.second = nullptr;
    }
}

SMemoryVK PoolsAllocatorVK::allocate(VkDevice device, VkMemoryPropertyFlags flags, u64 memorySize, s32 memoryTypeIndex)
{
    VkDeviceMemory deviceMemory = VK_NULL_HANDLE;

    utils::MemoryPool* pool = nullptr;
    std::map<s32, utils::MemoryPool*>::const_iterator memoryPool = m_pools.find(memoryTypeIndex);
    if (memoryPool == m_pools.cend())
    {
        m_customAllocator.setAllocateFunctor([device, flags, memoryTypeIndex, &deviceMemory](u64 memorySize) -> void*
        {
            VkMemoryAllocateInfo memAllocInfo = {};
            memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            memAllocInfo.pNext = nullptr;
            memAllocInfo.memoryTypeIndex = memoryTypeIndex;
            memAllocInfo.allocationSize = memorySize;

            VkResult result = vkAllocateMemory(device, &memAllocInfo, nullptr, &deviceMemory);
            if (result != VK_SUCCESS)
            {
                LOG_ERROR("allocateFunctor: vkAllocateMemory. Error %s", DebugVK::errorString(result).c_str());
                return VK_NULL_HANDLE;
            }

            void* mapped = nullptr;
            if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            {
                VkResult result = vkMapMemory(device, deviceMemory, 0, VK_WHOLE_SIZE, 0, &mapped);
                {
                    LOG_ERROR("SimpleAllocator::allocate: vkMapMemory. Error %s", DebugVK::errorString(result).c_str());
                    return VK_NULL_HANDLE;
                }
            }

            return mapped;
        });

        utils::MemoryPool* newPool = new utils::MemoryPool(m_initializeMemSize, m_blockSize, m_minAllocSize, &m_customAllocator);
        m_pools.insert(std::make_pair(memoryTypeIndex, newPool));

        pool = newPool;
    }
    else
    {
        pool = (*memoryPool).second;
    }
    ASSERT(pool, "Invalid pool");

    ////TEST
    //char* a = (char*)pool->getMemory(253);
    //memset(a, 'a', 253);
    //u64 ofsa = pool->getOffsetInBlock(a);
    //char* b = (char*)pool->getMemory(253);
    //memset(b, 'b', 253);
    //u64 ofsb = pool->getOffsetInBlock(b);
    //char* c = (char*)pool->getMemory(253);
    //memset(c, 'c', 253);
    //u64 ofsc = pool->getOffsetInBlock(c);
    //char* d = (char*)pool->getMemory(253);
    //memset(d, 'd', 253);
    //u64 ofsd = pool->getOffsetInBlock(d);
    //char* e = (char*)pool->getMemory(253);
    //memset(e, 'e', 253);
    //u64 ofse = pool->getOffsetInBlock(e);
    //char* f = (char*)pool->getMemory(253);
    //memset(f, 'f', 10);
    //u64 ofsf = pool->getOffsetInBlock(f);
    ////

    SMemoryVK memory = {};

    memory._mapped = pool->getMemory(memorySize);
    memory._memory = deviceMemory;
    memory._size = memorySize;
    memory._offset = pool->getOffsetInBlock(memory._mapped);
    memory._memoryTypeIndex = memoryTypeIndex;
    memory._flags = flags;

    return k_InvalidMemory;
}

void PoolsAllocatorVK::free(VkDevice device, SMemoryVK& memory)
{
    s32 memoryTypeIndex = memory._memoryTypeIndex;
    std::map<s32, utils::MemoryPool*>::const_iterator memoryPool = m_pools.find(memoryTypeIndex);
    if (memoryPool != m_pools.cend())
    {
        if ((*memoryPool).second->isValidPointer(memory._mapped))
        {
            VkDeviceMemory deviceMemory = memory._memory;
            bool mapped = (memory._mapped) ? true : false;
            m_customAllocator.setDeallocateFunctor([device, mapped, &deviceMemory](void* pointer)
            {
                if (mapped)
                {
                    vkUnmapMemory(device, deviceMemory);
                }

                if (deviceMemory != VK_NULL_HANDLE)
                {
                    vkFreeMemory(device, deviceMemory, nullptr);
                }
            });

            (*memoryPool).second->freeMemory(memory._mapped, memory._size);
        }
        else
        {
            ASSERT(false, "Try delete invalid pointer");
        }
    }
}


PoolsAllocatorVK::CustomMemoryPoolAllocator::CustomMemoryPoolAllocator()
{

}

PoolsAllocatorVK::CustomMemoryPoolAllocator::~CustomMemoryPoolAllocator()
{
}

void* PoolsAllocatorVK::CustomMemoryPoolAllocator::allocate(u64 size, s32 aligment)
{
    if (m_allocateFunctor)
    {
        return m_allocateFunctor(size);
    }

    return 0;
}

void PoolsAllocatorVK::CustomMemoryPoolAllocator::deallocate(void* block, u64 size)
{
    if (m_deallocateFunctor)
    {
        m_deallocateFunctor(block);
    }
}

void PoolsAllocatorVK::CustomMemoryPoolAllocator::setAllocateFunctor(std::function<void*(u64)> functor)
{
    m_allocateFunctor = functor;
}

void PoolsAllocatorVK::CustomMemoryPoolAllocator::setDeallocateFunctor(std::function<void(void*)> functor)
{
    m_deallocateFunctor = functor;
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

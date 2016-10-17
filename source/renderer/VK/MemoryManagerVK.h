#pragma once

#include "common.h"
#include "utils/MemoryPool.h"

#ifdef _VULKAN_RENDER_
#   include "vulkan/vulkan.h"
#   include "context/DeviceContextVK.h"
#endif //_VULKAN_RENDER_

namespace v3d
{
namespace renderer
{
namespace vk
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SMemoryVK
    {
        VkDeviceMemory          _memory;
        VkDeviceSize            _offset;
        VkDeviceSize            _size;
        void*                   _mapped;
        s32                     _memoryTypeIndex;
        VkMemoryPropertyFlags   _flags;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class MemoryManagerVK;

    class AllocatorVK
    {
    public:

        AllocatorVK() {};
        virtual             ~AllocatorVK() {};

    private:

        friend              MemoryManagerVK;

        virtual SMemoryVK   allocate(VkDevice device, VkMemoryPropertyFlags flags, u64 memorySize, s32 memoryTypeIndex) = 0;
        virtual void        free(VkDevice device, SMemoryVK& memory) = 0;
    };

    class SimpleAllocatorVK : public AllocatorVK
    {
    public:

        SimpleAllocatorVK();
        ~SimpleAllocatorVK();

    private:

        SMemoryVK   allocate(VkDevice device, VkMemoryPropertyFlags flags, u64 memorySize, s32 memoryTypeIndex) override;
        void        free(VkDevice device, SMemoryVK& memory) override;
    };

    class PoolsAllocatorVK : public AllocatorVK
    {
    public:

        PoolsAllocatorVK(u64 initializeMemSize, u64 blockSize, u64 minAllocSize);
        ~PoolsAllocatorVK();

    private:

        SMemoryVK                           allocate(VkDevice device, VkMemoryPropertyFlags flags, u64 memorySize, s32 memoryTypeIndex) override;
        void                                free(VkDevice device, SMemoryVK& memory) override;

        const u32                           m_initializeMemSize;
        const u32                           m_blockSize;
        const u32                           m_minAllocSize;

        std::map<s32, utils::MemoryPool*>   m_pools;

        class CustomMemoryPoolAllocator : public utils::MemoryPool::MemoryPoolAllocator
        {
        public:

            CustomMemoryPoolAllocator();
            ~CustomMemoryPoolAllocator();

            void*   allocate(u64 size, s32 aligment = 0) override;
            void    deallocate(void* block, u64 size = 0) override;

            void    setAllocateFunctor(std::function<void*(u64)> functor);
            void    setDeallocateFunctor(std::function<void(void*)> functor);

        private:

            std::function<void*(u64)>           m_allocateFunctor;
            std::function<void(void*)>          m_deallocateFunctor;
        };

        CustomMemoryPoolAllocator           m_customAllocator;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Vulkan memory management. VK render only.
    */
    class MemoryManagerVK
    {
    public:

        explicit MemoryManagerVK(const ContextVKPtr context);
        ~MemoryManagerVK();


        SimpleAllocatorVK*  getSimpleAllocator();

        PoolsAllocatorVK*   getImagePoolAllocator();
        PoolsAllocatorVK*   getBufferPoolAllocator();

        SMemoryVK           allocateImage(AllocatorVK& allocator, VkImage image, VkMemoryPropertyFlags flags);
        SMemoryVK           allocateBuffer(AllocatorVK& allocator, VkBuffer buffer, VkMemoryPropertyFlags flags);

        void                free(AllocatorVK& allocator, SMemoryVK& memory);

    private:

        s32                 findMemoryTypeIndex(const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags memoryPropertyFlags);

        SimpleAllocatorVK   m_simpleAllocator;

        PoolsAllocatorVK    m_imagePoolAllocator;
        PoolsAllocatorVK    m_bufferPoolAllocator;

        std::mutex          m_mutex;

        VkDevice            m_device;
        const VkPhysicalDeviceMemoryProperties& m_physicalDeviceMemoryProperties;
   };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

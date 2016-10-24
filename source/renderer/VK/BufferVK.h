#pragma once

#include "common.h"
#include "renderer/Buffer.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"
#include "MemoryManagerVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Command buffer management. VK render only.
    */
    class BufferVK : public Buffer
    {
    public:

        BufferVK(EBufferTarget target, EDataUsageType type, bool mappable = false);

        ~BufferVK();

        void                        bind() const override;
        void                        bindToTarget(EBufferTarget target, u32 offset, u32 size) const override;
        void                        unbind() const override;

        void                        update(u32 offset, u32 size, const void* data) override;
        void                        read(u32 offset, u32 size, void* const data) override;

        void* const                 map(u32 offset, u32 size) override;
        bool                        unmap(u32 offset, u32 size) override;
        bool                        unmap() override;

        bool                        create(u32 size = 0, const void* data = nullptr) override;
        void                        destroy() override;

        VkBuffer                    getBuffer() const;

    private:

        const EBufferTarget         m_target;
        const EDataUsageType        m_type;

        bool                        m_mappableMemory;
        bool                        m_coherentMemory;
        bool                        m_mapped;

        VkDevice                    m_device;
        u32                         m_queueFamilyIndex;

        SMemoryVK                   m_memory;
        VkBuffer                    m_buffer;
        VkBufferCreateFlags         m_flags;
        VkBufferUsageFlags          m_usage;

        std::atomic<bool>           m_initialized;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

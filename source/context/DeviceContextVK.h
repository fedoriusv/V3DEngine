#pragma once

#include "DeviceContext.h"

#ifdef _VULKAN_RENDER_
#include <vulkan/vulkan.h>

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class SwapChainVK;

    class DeviceContextVK final : public DeviceContext, public std::enable_shared_from_this<DeviceContextVK>
    {
    public:

        explicit                                DeviceContextVK(const platform::WindowPtr window);
        ~DeviceContextVK();

        void                                    checkForErrors(const std::string& location = "") override;
        bool                                    create() override;
        void                                    destroy() override;
        bool                                    present() override;
        void                                    flush();

        bool                                    setVSync(bool use) override;

        VkInstance                              getVulkanInstance() const;
        VkDevice                                getVulkanDevice() const;
        VkPhysicalDevice                        getVulkanPhysicalDevice() const;
        VkQueue                                 getVuklanQueue(u32 queueFamily, u32 index) const;
        s32                                     getVulkanQueueFamilyIndex(VkQueueFlagBits queueFlag) const;
        const VkPhysicalDeviceMemoryProperties& getVulkanPhysicalDeviceMemoryProperties() const;

    protected:

        Texture*                                createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level) override;
        Texture*                                createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level) override;
        Buffer*                                 createBuffer(EBufferTarget target, EDataUsageType type) override;

        void                                    fillGrapthicCaps() override;

        void                                    printExtensionList() const;

        bool                                    createContext();

        bool                                    createInstance(bool enableValidation);
        bool                                    createPhysicalDevice();
        bool                                    createLogicalDevice(bool useSwapChain, const std::list<std::tuple<s32, VkQueueFlags, std::vector<f32>>>& queueLists);

        u32                                     getQueueFamiliyIndex(VkQueueFlagBits queueFlags);

        VkBool32                                checkGlobalExtensionPresent(const c8* extensionName);
        VkBool32                                checkDeviceExtensionPresent(const c8* extensionName);

        VkInstance                              m_instance;
        VkPhysicalDevice                        m_physicalDevice;
        VkDevice                                m_device;

        struct SQueueFamily
        {
            s32                                 _queueFamilyIndex;
            VkQueueFlags                        _queueFlag;
            std::vector<VkQueue>                _queue;
        };

        std::map<s32, SQueueFamily>             m_queueFamily;

        struct SVulkanProperties
        {

            VkPhysicalDeviceProperties           _properties;
            VkPhysicalDeviceFeatures             _features;
            VkPhysicalDeviceMemoryProperties     _memoryProperties;
            std::vector<VkQueueFamilyProperties> _queueFamilyProperties;
        };
        SVulkanProperties                       m_vulkanPropsDevice;

        SwapChainVK*                            m_swapchain;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    using ContextVKPtr = std::shared_ptr<DeviceContextVK>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

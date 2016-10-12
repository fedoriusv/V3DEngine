#pragma once

#include "DeviceContext.h"

#if defined(_VULKAN_RENDER_) 
#   include <vulkan/vulkan.h>

#   include "SwapChainVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class DeviceContextVK final : public DeviceContext
    {
    public:

        explicit            DeviceContextVK(const platform::WindowPtr window);
        ~DeviceContextVK();

        void                checkForErrors(const std::string& location = "") override;
        bool                create() override;
        void                destroy() override;
        bool                present() override;
        void                flush();

        bool                setVSync(bool use) override;

        VkInstance          getVulkanInstance() const;
        VkDevice            getVulkanDevice() const;
        VkPhysicalDevice    getVulkanPhysicalDevice() const;
        VkQueue             getVuklanQueue() const;
        s32                 getVulkanQueueFamilyGraphicsIndex() const;

    protected:

        TexturePtr          createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level) override;
        TexturePtr          createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level) override;

        void                fillGrapthicCaps() override;

        void                printExtensionList() const;

#if defined(_PLATFORM_WIN_)
        bool                createWinApiContext();
#endif //_PLATFORM_WIN_

        bool                createInstance(bool enableValidation);
        bool                createPhysicalDevice();
        bool                createLogicalDevice(bool useSwapChain, VkQueueFlags requestedQueueTypes);

        u32                 getQueueFamiliyIndex(VkQueueFlagBits queueFlags);

        VkBool32            checkGlobalExtensionPresent(const c8* extensionName);
        VkBool32            checkDeviceExtensionPresent(const c8* extensionName);

        VkInstance          m_instance;
        VkPhysicalDevice    m_physicalDevice;
        VkDevice            m_device;
        VkQueue             m_queueRender;

        struct SVulkanProperties
        {

            VkPhysicalDeviceProperties           _properties;
            VkPhysicalDeviceFeatures             _features;
            VkPhysicalDeviceMemoryProperties     _memoryProperties;
            std::vector<VkQueueFamilyProperties> _queueFamilyProperties;

            struct
            {
                s32 _graphics;
                s32 _compute;
                s32 _transfer;
            }                                    _queueFamilyIndices;
        };

        SVulkanProperties   m_vulkanDevice;

        SwapChainVK*        m_swapchain;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

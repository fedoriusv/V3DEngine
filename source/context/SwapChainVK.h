#pragma once

#include "common.h"
#include "DeviceContext.h"

#if defined(_VULKAN_RENDER_)
#   include <vulkan/vulkan.h>

#   if defined(_PLATFORM_WIN_)
#   include <windows.h>
#   endif //_PLATFORM_WIN_

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class SwapChainVK
    {
    public:

        SwapChainVK(const ContextPtr context);
        ~SwapChainVK();

        bool                        create();
        void                        destroy();

        bool                        update(const core::Dimension2D& size, bool vsync);

        void                        presentFrame();
        s32                         prepareFrame();

    private:

        bool                        createSurface();
        bool                        createSwapChain();
        bool                        createSwapchainImages();

        u32                         m_currentBuffer;

        VkSwapchainKHR              m_swapChain;
        VkSurfaceKHR                m_surface;

        s32                         m_queueFamilyIndex;
        VkQueue                     m_queuePresent;

        VkSemaphore                 m_semaphorePresent;

        VkInstance                  m_instance;
        VkDevice                    m_device;
        VkPhysicalDevice            m_physicalDevice;

        VkSurfaceCapabilitiesKHR    m_surfaceCaps;

        std::mutex                  m_mutex;

        VkSurfaceFormatKHR          m_surfaceFormat;
        VkExtent2D                  m_surfaceSize;
        VkBool32                    m_isVSync;

#if defined(_PLATFORM_WIN_)
        bool                        createSurfaceWinApi();

        HINSTANCE                   m_appInstance;
        HWND                        m_appWindow;
#endif //_PLATFORM_WIN_
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

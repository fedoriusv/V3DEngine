#include "SwapChainVK.h"
#include "utils/Logger.h"

#ifdef _VULKAN_RENDER_
#include "DebugVK.h"
#include "DeviceContextVK.h"

#if defined(_PLATFORM_WIN_)
#   include "platform/WindowWinApi.h"
#endif //_PLATFORM_WIN_

namespace v3d
{
namespace renderer
{
namespace vk
{

SwapChainVK::SwapChainVK(const ContextVKPtr context)
    : m_currentBuffer(0)
    , m_swapChain(VK_NULL_HANDLE)
    , m_surface(VK_NULL_HANDLE)

    , m_queueFamilyIndex(0)
    , m_queuePresent(VK_NULL_HANDLE)

    , m_semaphorePresent(VK_NULL_HANDLE)
    , m_semaphoreRenderComplete(VK_NULL_HANDLE)

    , m_instance(VK_NULL_HANDLE)
    , m_device(VK_NULL_HANDLE)
    , m_physicalDevice(VK_NULL_HANDLE)
#if defined(_PLATFORM_WIN_)
    , m_appInstance(NULL)
    , m_appWindow(NULL)
#endif //_PLATFORM_WIN_
{
#if defined(_PLATFORM_WIN_)
    m_appInstance = GetModuleHandle(NULL);
    m_appWindow = std::static_pointer_cast<const platform::WindowWinApi>(context->getWindow())->getHandleWindow();
#endif //_PLATFORM_WIN_

    m_instance = context->getVulkanInstance();
    m_physicalDevice = context->getVulkanPhysicalDevice();
    m_device = context->getVulkanDevice();
    m_queueFamilyIndex = context->getVulkanQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
    ASSERT(m_queueFamilyIndex >= 0, "m_queueFamilyIndex < 0");
    m_queuePresent = context->getVuklanQueue(m_queueFamilyIndex, 0);

    m_surfaceSize.width = context->getWindowSize().width;
    m_surfaceSize.height = context->getWindowSize().height;
    m_isVSync = context->isVSync();

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    VkResult result = vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_semaphorePresent);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(" SwapChainVK::SwapChainVK: vkCreateSemaphore error %s", DebugVK::errorString(result).c_str());
    }

    result = vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_semaphoreRenderComplete);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(" SwapChainVK::SwapChainVK: vkCreateSemaphore error %s", DebugVK::errorString(result).c_str());
    }
}

SwapChainVK::~SwapChainVK()
{
    ASSERT(!m_swapChain, "m_swapChain is exist");
    ASSERT(!m_surface, "m_surface is exsit");

    if (m_semaphorePresent != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(m_device, m_semaphorePresent, nullptr);
        m_semaphorePresent = VK_NULL_HANDLE;
    }

    if (m_semaphoreRenderComplete != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(m_device, m_semaphoreRenderComplete, nullptr);
        m_semaphoreRenderComplete = VK_NULL_HANDLE;
    }
}

bool SwapChainVK::create()
{
    if (!SwapChainVK::createSurface())
    {
        LOG_ERROR(" SwapChainVK::create: cannot create surface");
        return false;
    }

    if (!SwapChainVK::createSwapChain())
    {
        LOG_ERROR(" SwapChainVK::create: cannot create swapchain");
        return false;
    }

    if (!SwapChainVK::createSwapchainImages())
    {
        LOG_ERROR(" SwapChainVK::create: cannot create swapchain images");
        return false;
    }

    return true;
}

void SwapChainVK::destroy()
{
    if (m_swapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
    }

    if (m_surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        m_surface = VK_NULL_HANDLE;
    }
}

bool SwapChainVK::update(const core::Dimension2D& size, bool vsync)
{
    if (m_swapChain != VK_NULL_HANDLE)
    {
        SwapChainVK::destroy();
    }

    m_surfaceSize.width = size.width;
    m_surfaceSize.height = size.height;
    m_isVSync = vsync;

    return SwapChainVK::create();
}

void SwapChainVK::presentFrame()
{
    ASSERT(m_semaphoreRenderComplete, "Invalid semaphore");

    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapChain;
    presentInfo.pImageIndices = &m_currentBuffer;
    presentInfo.pWaitSemaphores = &m_semaphoreRenderComplete;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pResults = nullptr;

    std::lock_guard<std::mutex> lock(m_mutex);

    VkResult result = vkQueuePresentKHR(m_queuePresent, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        LOG_WARNING("SwapChainVK::presentFrame: Error VK_ERROR_OUT_OF_DATE_KHR. Swapchain is out of date. Needs to be recreated for defined results");
    }
    else if (result != VK_SUCCESS)
    {
        LOG_ERROR("SwapChainVK::presentFrame: vkQueuePresentKHR. Error %s", DebugVK::errorString(result).c_str());
    }
}

void SwapChainVK::submitFrame(VkCommandBuffer buffer, VkFence fence)
{
    VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = &waitStageMask;
    submitInfo.pWaitSemaphores = &m_semaphorePresent;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_semaphoreRenderComplete;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pCommandBuffers = &buffer;
    submitInfo.commandBufferCount = 1;

    VkResult result = vkQueueSubmit(m_queuePresent, 1, &submitInfo, fence);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("RendererVK::immediatePresentFrame: vkQueueSubmit error %s", vk::DebugVK::errorString(result).c_str());
    }
}

s32 SwapChainVK::prepareFrame()
{
    ASSERT(m_semaphorePresent, "Invalid semaphore");

    std::lock_guard<std::mutex> lock(m_mutex);

    u32 outImageIndex = 0;
    VkResult result = vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, m_semaphorePresent, VK_NULL_HANDLE, &outImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        LOG_WARNING("SwapChainVK::prepareFrame: Error VK_ERROR_OUT_OF_DATE_KHR. Swapchain is out of date. Needs to be recreated for defined results");

        vkDeviceWaitIdle(m_device);
        result = vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, m_semaphorePresent, VK_NULL_HANDLE, &outImageIndex);
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("SwapChainVK::prepareFrame: vkAcquireNextImageKHR. Error %s", DebugVK::errorString(result).c_str());
            return m_currentBuffer;
        }
    }
    else if (result != VK_SUCCESS)
    {
        LOG_ERROR("SwapChainVK::prepareFrame: vkAcquireNextImageKHR. Error %s", DebugVK::errorString(result).c_str());
        return m_currentBuffer;
    }
    m_currentBuffer = outImageIndex;

    return m_currentBuffer;
}

u32 SwapChainVK::swapBuffersCount() const
{
    return static_cast<u32>(m_swapBuffers.size());
}

bool SwapChainVK::createSurface()
{
    LOG_DEBUG("SwapChainVK::createSurface");
#if defined(_PLATFORM_WIN_)
    if (!SwapChainVK::createSurfaceWinApi())
    {
        LOG_ERROR("SwapChainVK::createSurface: Create win surface is falied");
        return false;
    }
#else //_PLATFORM_WIN_
    LOG_ERROR("SwapChainVK::createSurface: paltform not supported");
    return false;
#endif //_PLATFORM_WIN_

    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCaps);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("SwapChainVK::createSurface: vkGetPhysicalDeviceSurfaceCapabilitiesKHR. Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    if (m_surfaceCaps.maxImageCount < 2)
    {
        LOG_ERROR("SwapChainVK::createSurface: Not enough images supported in vulkan swapchain");
        return false;
    }

    VkBool32 supportsPresentation = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, m_queueFamilyIndex, m_surface, &supportsPresentation);
    if (!supportsPresentation)
    {
        LOG_ERROR("SwapChainVK::createSurface: not support presentation");
        return false;
    }

    //Get Surface format
    u32 surfaceFormatCount;
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatCount, nullptr);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("SwapChainVK::createSurface: vkGetPhysicalDeviceSurfaceFormatsKHR. Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatCount, surfaceFormats.data());
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("SwapChainVK::createSurface: vkGetPhysicalDeviceSurfaceFormatsKHR. Error %s", DebugVK::errorString(result).c_str());
        return false;
    }
    std::for_each(surfaceFormats.begin(), surfaceFormats.end(), [](const VkSurfaceFormatKHR& surfaceFormat)
    {
        LOG_INFO("SurfaceFormat supported format: %d, colorspace: %d", surfaceFormat.format, surfaceFormat.colorSpace);
    });

    u32 surfaceFormatIndex = 0;
    // If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
    // there is no preferered format, so we assume VK_FORMAT_B8G8R8A8_UNORM
    if ((surfaceFormatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)) //???
    {
        surfaceFormatCount = VK_FORMAT_B8G8R8A8_UNORM;
    }
    else
    {
        // Always select the first available color format
        // If you need a specific format (e.g. SRGB) you'd need to
        // iterate over the list of available surface format and
        // check for it's presence
        m_surfaceFormat.format = surfaceFormats[0].format;
    }
    m_surfaceFormat.colorSpace = surfaceFormats[0].colorSpace;

    return true;
}


bool SwapChainVK::createSwapChain()
{
    LOG_DEBUG("SwapChainVK::createSwapChain");

    ASSERT(m_surface, "surface is nullptr");

    // Select a present mode for the swapchain
    u32 presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes.data());

    // The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
    // This mode waits for the vertical blank ("v-sync")
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    // If v-sync is not requested, try to find a mailbox mode
    // It's the lowest latency non-tearing present mode available
    if (!m_isVSync)
    {
        for (u32 i = 0; i < presentModeCount; i++)
        {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }

            if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
            {
                swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }
        }
    }

    // Determine the number of images
    uint32_t desiredNumberOfSwapchainImages = m_surfaceCaps.minImageCount + 1;
    if ((m_surfaceCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > m_surfaceCaps.maxImageCount))
    {
        desiredNumberOfSwapchainImages = m_surfaceCaps.maxImageCount;
    }

    // Find the transformation of the surface
    VkSurfaceTransformFlagsKHR preTransform;
    if (m_surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        // We prefer a non-rotated transform
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {
        preTransform = m_surfaceCaps.currentTransform;
    }

    VkSwapchainCreateInfoKHR swapChainInfo = {};
    swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainInfo.pNext = nullptr;
    swapChainInfo.surface = m_surface;
    swapChainInfo.minImageCount = desiredNumberOfSwapchainImages;
    swapChainInfo.imageFormat = m_surfaceFormat.format;
    swapChainInfo.imageColorSpace = m_surfaceFormat.colorSpace;
    swapChainInfo.imageExtent = { m_surfaceSize.width, m_surfaceSize.height };
    swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
    swapChainInfo.imageArrayLayers = 1;
    swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainInfo.queueFamilyIndexCount = 0;
    swapChainInfo.pQueueFamilyIndices = nullptr;
    swapChainInfo.presentMode = swapchainPresentMode;
    swapChainInfo.oldSwapchain = nullptr;
    swapChainInfo.clipped = VK_TRUE; // Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
    swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    VkResult result = vkCreateSwapchainKHR(m_device, &swapChainInfo, nullptr, &m_swapChain);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("SwapChainVK::createSwapChain: vkCreateSwapchainKHR. Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    LOG_DEBUG("SwapChainVK::createSwapChain created");

    return true;
}

bool SwapChainVK::createSwapchainImages()
{
    u32 swapChainImageCount;
    VkResult result = vkGetSwapchainImagesKHR(m_device, m_swapChain, &swapChainImageCount, nullptr);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("SwapChainVK::createSwapchainImages: vkGetSwapchainImagesKHR count. Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    if (swapChainImageCount < 2)
    {
        LOG_ERROR("SwapChainVK::createSurface: Not enough images supported in vulkan swapchain");
        return false;
    }

    LOG_DEBUG("SwapChainVK::createSwapchainImages: Count images %d", swapChainImageCount);

    std::vector<VkImage> images(swapChainImageCount);
    result = vkGetSwapchainImagesKHR(m_device, m_swapChain, &swapChainImageCount, images.data());
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("SwapChainVK::createSwapchainImages: vkGetSwapchainImagesKHR array. Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

   //TODO:
    //buffers.resize(imageCount);
    //for (uint32_t i = 0; i < imageCount; i++)
    //{
    //    VkImageViewCreateInfo colorAttachmentView = {};
    //    colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    //    colorAttachmentView.pNext = NULL;
    //    colorAttachmentView.format = colorFormat;
    //    colorAttachmentView.components = {
    //        VK_COMPONENT_SWIZZLE_R,
    //        VK_COMPONENT_SWIZZLE_G,
    //        VK_COMPONENT_SWIZZLE_B,
    //        VK_COMPONENT_SWIZZLE_A
    //    };
    //    colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    //    colorAttachmentView.subresourceRange.baseMipLevel = 0;
    //    colorAttachmentView.subresourceRange.levelCount = 1;
    //    colorAttachmentView.subresourceRange.baseArrayLayer = 0;
    //    colorAttachmentView.subresourceRange.layerCount = 1;
    //    colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    //    colorAttachmentView.flags = 0;

    //    buffers[i].image = images[i];

    //    colorAttachmentView.image = buffers[i].image;

    //    err = vkCreateImageView(device, &colorAttachmentView, nullptr, &buffers[i].view);
    //    assert(!err);

    m_swapBuffers.resize(swapChainImageCount);

    return true;
}

#if defined(_PLATFORM_WIN_)
bool SwapChainVK::createSurfaceWinApi()
{
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = nullptr;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = m_appInstance;
    surfaceCreateInfo.hwnd = m_appWindow;
    VkResult result = vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &m_surface);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("SwapChainVK::createSurfaceWinApi: vkCreateWin32SurfaceKHR. Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    LOG_DEBUG("SwapChainVK::createSurfaceWinApi created");

    return true;
}
#endif //_PLATFORM_WIN_

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

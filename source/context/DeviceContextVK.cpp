#include "DeviceContextVK.h"
#include "DebugVK.h"
#include "utils/Logger.h"

#ifdef _VULKAN_RENDER_
#   include <vulkan/vulkan.h>
#   include <vulkan/vk_sdk_platform.h>

#   if defined(_PLATFORM_WIN_)
#   include <windows.h>
#   pragma comment(lib, "vulkan-1.lib")

#   include "platform/WindowWinApi.h"
#   endif //_PLATFORM_WIN_

namespace v3d
{
namespace renderer
{
namespace vk
{

const bool k_useValidationLayers = false;
const std::string k_vulkanApplicationName = "V3DWinApiVulkan";

DeviceContextVK::DeviceContextVK(const platform::WindowPtr window)
    : DeviceContext(window)
    , m_instance(VK_NULL_HANDLE)
    , m_physicalDevice(VK_NULL_HANDLE)
    , m_swapchain(nullptr)
{
    m_vulkanDevice._device = VK_NULL_HANDLE;
}

DeviceContextVK::~DeviceContextVK()
{
    if (m_swapchain)
    {
        delete m_swapchain;
        m_swapchain = nullptr;
    }

    ASSERT(!m_swapchain, "m_device already exits");

    ASSERT(!m_physicalDevice, "m_physicalDevice already exits");
    ASSERT(!m_instance, "m_instance already exits");
}

void DeviceContextVK::checkForErrors(const std::string& location)
{
}

bool DeviceContextVK::create()
{
#if defined(_PLATFORM_WIN_)
    return createWinApiContext();
#else
    return false;
#endif //_PLATFORM_WIN_
    return false;
}

#if defined(_PLATFORM_WIN_)
bool DeviceContextVK::createWinApiContext()
{
    LOG_INFO("Create WinApi Vulkan Context");

    if (!DeviceContextVK::createInstance(DebugVK::s_enableValidationLayers))
    {
        LOG_ERROR("DeviceContextVK::createWinApiContext: Could not create Vulkan instance");
        return false;
    }

#if VULKAN_DEBUG
    // The report flags determine what type of messages for the layers will be displayed
    // For validating (debugging) an appplication the error and warning bits should suffice
    VkDebugReportFlagsEXT debugReportFlags = VK_DEBUG_REPORT_ERROR_BIT_EXT; // | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
                                                                            // Additional flags include performance info, loader and layer debug messages, etc.
    DebugVK::setupDebugging(m_instance, debugReportFlags, VK_NULL_HANDLE, nullptr);
#endif //VULKAN_DEBUG

    if (!createPhysicalDevice())
    {
        LOG_ERROR("DeviceContextVK::createWinApiContext: Could not create Vulkan PhysicalDevice");
        DeviceContextVK::destroy();

        return false;
    }

    if (!createLogicalDevice(true, VK_QUEUE_GRAPHICS_BIT /*| VK_QUEUE_COMPUTE_BIT*/))
    {
        LOG_ERROR("DeviceContextVK::createWinApiContext: Could not create Vulkan Device");
        DeviceContextVK::destroy();

        return false;
    }

    m_swapchain = new SwapChainVK(shared_from_this());
    if (!m_swapchain->create())
    {
        LOG_ERROR("DeviceContextVK::createWinApiContext: Could not create SwapChain");
        DeviceContextVK::destroy();

        return false;
    }

    return true;
}
#endif //_PLATFORM_WIN_

bool DeviceContextVK::createInstance(bool enableValidation)
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = k_vulkanApplicationName.c_str();
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = k_vulkanApplicationName.c_str();
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_MAKE_VERSION(
        VULKAN_VERSION_MAJOR, 
        VULKAN_VERSION_MINOR, 
        VULKAN_VERSION_PATCH
    );

    std::vector<const c8*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

    // Enable surface extensions depending on os
#if defined(_PLATFORM_WIN_)
    enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(_PLATFORM_ANDROID_)
    enabledExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_PLATFORM_LINUX_)
    enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif //_PLATFORM_LINUX_

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    if (enabledExtensions.size() > 0)
    {
#if VULKAN_DEBUG
         enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif //VULKAN_DEBUG
        instanceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    }

    if (enableValidation)
    {
        instanceCreateInfo.enabledLayerCount = static_cast<u32>(DebugVK::s_validationLayerNames.size());
        instanceCreateInfo.ppEnabledLayerNames = DebugVK::s_validationLayerNames.data();
    }

    VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("DeviceContextVK::createInstance: vkCreateInstance error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    return true;
}

bool DeviceContextVK::createPhysicalDevice()
{
    // Physical device
    u32 gpuCount = 0;
    // Get number of available physical devices
    VkResult result = vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("DeviceContextVK::createDevice: vkEnumeratePhysicalDevices Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    result = vkEnumeratePhysicalDevices(m_instance, &gpuCount, physicalDevices.data());
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("DeviceContextVK::createDevice: ould not enumerate phyiscal devices. vkEnumeratePhysicalDevices Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    // Note :
    // This example will always use the first physical device reported,
    // change the vector index if you have multiple Vulkan devices installed
    // and want to use another one
    m_physicalDevice = physicalDevices[0];

    // Store Properties features, limits and properties of the physical device for later use
    // Device properties also contain limits and sparse properties
    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_vulkanDevice._properties);
    // Features should be checked by the examples before using them
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_vulkanDevice._features);
    // Memory properties are used regularly for creating all kinds of buffer
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_vulkanDevice._memoryProperties);
    // Queue family properties, used for setting up requested queues upon device creation

    u32 queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
    ASSERT(queueFamilyCount > 0, "Must be greater than 0");
    m_vulkanDevice._queueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, m_vulkanDevice._queueFamilyProperties.data());

    return true;
}

bool DeviceContextVK::createLogicalDevice(bool useSwapChain, VkQueueFlags requestedQueueTypes)
{
    if (!m_physicalDevice)
    {
        LOG_ERROR("DeviceContextVK::createLogicalDevice: m_physicalDevice is nullptr");
        return false;
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

    // Get queue family indices for the requested queue family types
    // Note that the indices may overlap depending on the implementation

    const f32 defaultQueuePriority(0.0f);

    // Graphics queue
    if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
    {
        m_vulkanDevice._queueFamilyIndices._graphics = getQueueFamiliyIndex(VK_QUEUE_GRAPHICS_BIT);

        VkDeviceQueueCreateInfo queueInfo = {};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.pNext = nullptr;
        queueInfo.flags = 0;
        queueInfo.queueFamilyIndex = m_vulkanDevice._queueFamilyIndices._graphics;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &defaultQueuePriority;

        queueCreateInfos.push_back(queueInfo);
    }
    else
    {
        m_vulkanDevice._queueFamilyIndices._graphics = VK_NULL_HANDLE;
    }

    // Dedicated compute queue
    if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
    {
        m_vulkanDevice._queueFamilyIndices._compute = getQueueFamiliyIndex(VK_QUEUE_COMPUTE_BIT);
        if (m_vulkanDevice._queueFamilyIndices._compute != m_vulkanDevice._queueFamilyIndices._graphics)
        {
            // If compute family index differs, we need an additional queue create info for the compute queue
            VkDeviceQueueCreateInfo queueInfo = {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.pNext = nullptr;
            queueInfo.flags = 0;
            queueInfo.queueFamilyIndex = m_vulkanDevice._queueFamilyIndices._compute;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = &defaultQueuePriority;

            queueCreateInfos.push_back(queueInfo);
        }
    }
    else
    {
        // Else we use the same queue
        m_vulkanDevice._queueFamilyIndices._compute = m_vulkanDevice._queueFamilyIndices._graphics;
    }

    // Dedicated transfer queue
    if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
    {
        m_vulkanDevice._queueFamilyIndices._transfer = getQueueFamiliyIndex(VK_QUEUE_TRANSFER_BIT);
        if ((m_vulkanDevice._queueFamilyIndices._transfer != m_vulkanDevice._queueFamilyIndices._graphics) &&
            (m_vulkanDevice._queueFamilyIndices._transfer != m_vulkanDevice._queueFamilyIndices._compute))
        {
            // If compute family index differs, we need an additional queue create info for the compute queue
            VkDeviceQueueCreateInfo queueInfo = {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.pNext = nullptr;
            queueInfo.flags = 0;
            queueInfo.queueFamilyIndex = m_vulkanDevice._queueFamilyIndices._transfer;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = &defaultQueuePriority;

            queueCreateInfos.push_back(queueInfo);
        }
    }
    else
    {
        // Else we use the same queue
        m_vulkanDevice._queueFamilyIndices._transfer = m_vulkanDevice._queueFamilyIndices._graphics;
    }

    // Create the logical device representation
    std::vector<const c8*> deviceExtensions;
    if (useSwapChain)
    {
        // If the device will be used for presenting to a display via a swapchain we need to request the swapchain extension
        deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &m_vulkanDevice._features;

    // Enable the debug marker extension if it is present (likely meaning a debugging tool is present)
    if (DebugVK::s_enableDebugMarkers)
    {
        if (DeviceContextVK::checkDeviceExtensionPresent(m_physicalDevice, VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
        {
            deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
        }
    }

    if (!deviceExtensions.empty())
    {
        deviceCreateInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }

    if (DebugVK::s_enableValidationLayers)
    {
        deviceCreateInfo.enabledLayerCount = static_cast<u32>(DebugVK::s_validationLayerNames.size());
        deviceCreateInfo.ppEnabledLayerNames = DebugVK::s_validationLayerNames.data();
    }

    VkResult result = vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_vulkanDevice._device);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("DeviceContextVK::createLogicalDevice: vkCreateDevice Error %s", DebugVK::errorString(result).c_str());
        return false;
    }

    return true;
}

u32 DeviceContextVK::getQueueFamiliyIndex(VkQueueFlagBits queueFlags)
{
    // Dedicated queue for compute
    // Try to find a queue family index that supports compute but not graphics
    if (queueFlags & VK_QUEUE_COMPUTE_BIT)
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(m_vulkanDevice._queueFamilyProperties.size()); i++)
        {
            if ((m_vulkanDevice._queueFamilyProperties[i].queueFlags & queueFlags) && ((m_vulkanDevice._queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
            {
                return i;
                break;
            }
        }
    }

    // Dedicated queue for transfer
    // Try to find a queue family index that supports transfer but not graphics and compute
    if (queueFlags & VK_QUEUE_TRANSFER_BIT)
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(m_vulkanDevice._queueFamilyProperties.size()); i++)
        {
            if ((m_vulkanDevice._queueFamilyProperties[i].queueFlags & queueFlags) &&
                ((m_vulkanDevice._queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) &&
                ((m_vulkanDevice._queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
            {
                return i;
                break;
            }
        }
    }

    // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
    for (uint32_t i = 0; i < static_cast<uint32_t>(m_vulkanDevice._queueFamilyProperties.size()); i++)
    {
        if (m_vulkanDevice._queueFamilyProperties[i].queueFlags & queueFlags)
        {
            return i;
            break;
        }
    }

    LOG_WARNING("DeviceContextVK::getQueueFamiliyIndex: Could not find a matching queue family index %d" , queueFlags);
    return 0;
}

VkBool32 DeviceContextVK::checkGlobalExtensionPresent(const c8* extensionName)
{
    u32 extensionCount = 0;
    std::vector<VkExtensionProperties> extensions;
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
    extensions.resize(extensionCount);
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions.data());
    for (auto& ext : extensions)
    {
        if (!strcmp(extensionName, ext.extensionName))
        {
            return true;
        }
    }
    return false;
}

VkBool32 DeviceContextVK::checkDeviceExtensionPresent(VkPhysicalDevice physicalDevice, const c8* extensionName)
{
    u32 extensionCount = 0;
    std::vector<VkExtensionProperties> extensions;
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, NULL);
    extensions.resize(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, extensions.data());
    for (auto& ext : extensions)
    {
        if (!strcmp(extensionName, ext.extensionName))
        {
            return true;
        }
    }
    return false;
}

void DeviceContextVK::destroy()
{
    if (m_vulkanDevice._device != VK_NULL_HANDLE)
    {
        vkDestroyDevice(m_vulkanDevice._device, nullptr);
        m_vulkanDevice._device = VK_NULL_HANDLE;
    }

    m_physicalDevice = VK_NULL_HANDLE;

#if VULKAN_DEBUG
    DebugVK::freeDebugCallback(m_instance);
#endif //VULKAN_DEBUG

    if (m_instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }
}

bool DeviceContextVK::present()
{
    m_swapchain->presentFrame();
    return true;
}

void DeviceContextVK::flush()
{
}

bool DeviceContextVK::setVSync(bool use)
{
    m_isVSync = use;

    return true;
}

VkInstance DeviceContextVK::getVulkanInstance() const
{
    return m_instance;
}

VkDevice DeviceContextVK::getVulkanDevice() const
{
    return m_vulkanDevice._device;
}

VkPhysicalDevice DeviceContextVK::getVulkanPhysicalDevice() const
{
    return m_physicalDevice;
}

TexturePtr DeviceContextVK::createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D & size, const void * data, u32 level)
{
    return TexturePtr();
}

TexturePtr DeviceContextVK::createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D & size, const void * data[6], u32 level)
{
    return TexturePtr();
}

void DeviceContextVK::fillGrapthicCaps()
{
}

void DeviceContextVK::printExtensionList() const
{
}

} //namespace vk
} //namespace renderer
} //namespace v3d


#endif //_VULKAN_RENDER_

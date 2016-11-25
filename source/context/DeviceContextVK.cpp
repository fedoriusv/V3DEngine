#include "DeviceContextVK.h"
#include "DebugVK.h"
#include "utils/Logger.h"

#ifdef _VULKAN_RENDER_
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>

#include "SwapChainVK.h"
#include "renderer/VK/TextureVK.h"
#include "renderer/VK/BufferVK.h"
#include "renderer/VK/ShaderProgramVK.h"

#if defined(_PLATFORM_WIN_)
#   include <windows.h>
#   pragma comment(lib, "vulkan-1.lib")

#   include "platform/WindowWinApi.h"
#endif //_PLATFORM_WIN_

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
    , m_device(VK_NULL_HANDLE)
    , m_swapchain(nullptr)
{
}

DeviceContextVK::~DeviceContextVK()
{
    ASSERT(!m_swapchain, "m_device already exits");

    ASSERT(m_queueFamily.empty(), "m_queueFamily.empty");

    ASSERT(!m_device, "m_physicalDevice already exits");
    ASSERT(!m_instance, "m_instance already exits");
}

void DeviceContextVK::checkForErrors(const std::string& location)
{
}

bool DeviceContextVK::create()
{
    return createContext();
}

bool DeviceContextVK::createContext()
{
    LOG_INFO("Create Vulkan Context");

    if (!DeviceContextVK::createInstance(DebugVK::s_enableValidationLayers))
    {
        LOG_ERROR("DeviceContextVK::createContext: Could not create Vulkan instance");
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
        LOG_ERROR("DeviceContextVK::createContext: Could not create Vulkan PhysicalDevice");
        return false;
    }

    s32 familyIndices[1];
    familyIndices[0] = getQueueFamiliyIndex(VK_QUEUE_GRAPHICS_BIT);

    //index family, queue bits, priority list 
    const std::list<std::tuple<s32, VkQueueFlags, std::vector<f32>>> queueLists =
    { 
        { familyIndices[0], VK_QUEUE_GRAPHICS_BIT, { 0.0f } },
    };

    if (!createLogicalDevice(true, queueLists))
    {
        LOG_ERROR("DeviceContextVK::createContext: Could not create Vulkan Device");
        return false;
    }

    for (auto& queueFamily : m_queueFamily)
    {
        ASSERT(queueFamily.second._queueFamilyIndex >= 0, "invalid queue family index");
        for (u32 queueIndex = 0; queueIndex < queueFamily.second._queue.size(); ++queueIndex)
        {
            auto& queue = queueFamily.second._queue;
            vkGetDeviceQueue(m_device, static_cast<u32>(queueFamily.second._queueFamilyIndex), queueIndex, &queue[queueIndex]);
        }
    }

    m_swapchain = new SwapChainVK(shared_from_this());
    if (!m_swapchain->create())
    {
        m_swapchain->destroy();

        LOG_ERROR("DeviceContextVK::createContext: Could not create SwapChain");
        return false;
    }

    return true;
}

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

    std::vector<const c8*> enabledExtensions;
    if (DeviceContextVK::checkGlobalExtensionPresent(VK_KHR_SURFACE_EXTENSION_NAME))
    {
        enabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    }
    else
    {
        LOG_ERROR("DeviceContextVK::createInstance: vkCreateInstance does not support surface extensions");
        ASSERT(false, "VK_KHR_SURFACE_EXTENSION_NAME");
        return false;
    }
    // Enable surface extensions depending on os
#if defined(_PLATFORM_WIN_)
    if (DeviceContextVK::checkGlobalExtensionPresent(VK_KHR_WIN32_SURFACE_EXTENSION_NAME))
    {
        enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    }
    else
    {
        ASSERT(false, "VK_KHR_WIN32_SURFACE_EXTENSION_NAME");
    }
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

    if (!enabledExtensions.empty())
    {
#if VULKAN_DEBUG
        if (DeviceContextVK::checkGlobalExtensionPresent(VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
        {
            enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
        else
        {
            ASSERT(false, "VK_EXT_DEBUG_REPORT_EXTENSION_NAME");
        }
#endif //VULKAN_DEBUG
        instanceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    }
    else
    {
        ASSERT(false, "enabledExtensions.empty()");
    }

    if (enableValidation)
    {
        std::vector<const c8*> validationLayerNames;
        for (auto layerName = DebugVK::s_validationLayerNames.cbegin(); layerName < DebugVK::s_validationLayerNames.cend(); ++layerName)
        {
            if (DebugVK::checkValidationLayerSupported(*layerName))
            {
                validationLayerNames.push_back(*layerName);
            }
        }

        instanceCreateInfo.enabledLayerCount = static_cast<u32>(validationLayerNames.size());
        instanceCreateInfo.ppEnabledLayerNames = validationLayerNames.data();
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
    m_physicalDevice = physicalDevices.front();

    // Store Properties features, limits and properties of the physical device for later use
    // Device properties also contain limits and sparse properties
    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_vulkanPropsDevice._properties);
    // Features should be checked by the examples before using them
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_vulkanPropsDevice._features);
    // Memory properties are used regularly for creating all kinds of buffer
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_vulkanPropsDevice._memoryProperties);
    // Queue family properties, used for setting up requested queues upon device creation

    u32 queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
    ASSERT(queueFamilyCount > 0, "Must be greater than 0");
    m_vulkanPropsDevice._queueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, m_vulkanPropsDevice._queueFamilyProperties.data());

    return true;
}

bool DeviceContextVK::createLogicalDevice(bool useSwapChain, const std::list<std::tuple<s32, VkQueueFlags, std::vector<f32>>>& queueLists)
{
    if (!m_physicalDevice)
    {
        LOG_ERROR("DeviceContextVK::createLogicalDevice: m_physicalDevice is nullptr");
        return false;
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    // Get queue family indices for the requested queue family types
    // Note that the indices may overlap depending on the implementation
    for (auto& requestedQueue : queueLists)
    {
        s32 requestedQueueFamalyIndex = std::get<0>(requestedQueue);
        VkQueueFlags requestedQueueTypes = std::get<1>(requestedQueue);
        const std::vector<f32>& queuePriority = std::get<2>(requestedQueue);
        s32 requestedQueueCount = static_cast<s32>(std::get<2>(requestedQueue).size());

        SQueueFamily queue;
        queue._queueFamilyIndex = requestedQueueFamalyIndex;
        queue._queue.resize(requestedQueueCount);
        if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT) // Graphics queue
        {
            queue._queueFlag = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
        }
        else if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT) // Dedicated compute queue
        {
            queue._queueFlag = VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
        }
        else if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT) // Dedicated transfer queue
        {
            queue._queueFlag = VK_QUEUE_TRANSFER_BIT;
        }
        else
        {
            ASSERT(false, "invalid requestedQueueTypes");
        }

        m_queueFamily.insert(std::make_pair(requestedQueueFamalyIndex, queue));


        VkDeviceQueueCreateInfo queueInfo = {};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.pNext = nullptr;
        queueInfo.flags = 0;
        queueInfo.queueFamilyIndex = requestedQueueFamalyIndex;
        queueInfo.queueCount = requestedQueueCount;
        queueInfo.pQueuePriorities = queuePriority.data();

        queueCreateInfos.push_back(queueInfo);
    }

    // Create the logical device representation
    std::vector<const c8*> deviceExtensions;
    if (useSwapChain && DeviceContextVK::checkDeviceExtensionPresent(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
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
    deviceCreateInfo.pEnabledFeatures = &m_vulkanPropsDevice._features;

    // Enable the debug marker extension if it is present (likely meaning a debugging tool is present)
    if (DebugVK::s_enableDebugMarkers)
    {
        if (DeviceContextVK::checkDeviceExtensionPresent(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
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

    VkResult result = vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);
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
        for (uint32_t i = 0; i < static_cast<uint32_t>(m_vulkanPropsDevice._queueFamilyProperties.size()); i++)
        {
            if ((m_vulkanPropsDevice._queueFamilyProperties[i].queueFlags & queueFlags) && ((m_vulkanPropsDevice._queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
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
        for (uint32_t i = 0; i < static_cast<uint32_t>(m_vulkanPropsDevice._queueFamilyProperties.size()); i++)
        {
            if ((m_vulkanPropsDevice._queueFamilyProperties[i].queueFlags & queueFlags) &&
                ((m_vulkanPropsDevice._queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) &&
                ((m_vulkanPropsDevice._queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
            {
                return i;
                break;
            }
        }
    }

    // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
    for (uint32_t i = 0; i < static_cast<uint32_t>(m_vulkanPropsDevice._queueFamilyProperties.size()); i++)
    {
        if (m_vulkanPropsDevice._queueFamilyProperties[i].queueFlags & queueFlags)
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
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    extensions.resize(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    for (auto& ext : extensions)
    {
        if (!strcmp(extensionName, ext.extensionName))
        {
            return true;
        }
    }
    return false;
}

VkBool32 DeviceContextVK::checkDeviceExtensionPresent(const c8* extensionName)
{
    u32 extensionCount = 0;
    std::vector<VkExtensionProperties> extensions;
    vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, nullptr);
    extensions.resize(extensionCount);
    vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, extensions.data());
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
    if (m_swapchain)
    {
        m_swapchain->destroy();

        delete m_swapchain;
        m_swapchain = nullptr;
    }

    for (auto& queueFamily : m_queueFamily)
    {
        queueFamily.second._queue.clear();
    }
    m_queueFamily.clear();

    if (m_device != VK_NULL_HANDLE)
    {
        vkDestroyDevice(m_device, nullptr);
        m_device = VK_NULL_HANDLE;
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
    return m_device;
}

VkPhysicalDevice DeviceContextVK::getVulkanPhysicalDevice() const
{
    return m_physicalDevice;
}

VkQueue DeviceContextVK::getVuklanQueue(u32 queueFamily, u32 index) const
{
    auto result = m_queueFamily.find(queueFamily);
    if (result != m_queueFamily.cend())
    {
        ASSERT((*result).second._queue.size() > index, "Out of range");
        return (*result).second._queue[index];
    }

    return VK_NULL_HANDLE;
}

s32 DeviceContextVK::getVulkanQueueFamilyIndex(VkQueueFlagBits queueFlag) const
{
    auto result = std::find_if(m_queueFamily.cbegin(), m_queueFamily.cend(), [&queueFlag](std::pair<const s32, SQueueFamily> item) -> bool
    {
        if (item.second._queueFlag & queueFlag)
        {
            return true;
        }

        return false;
    });

    if (result != m_queueFamily.cend())
    {
        return (*result).second._queueFamilyIndex;
    }

    return -1;
}

const VkPhysicalDeviceMemoryProperties& DeviceContextVK::getVulkanPhysicalDeviceMemoryProperties() const
{
    return m_vulkanPropsDevice._memoryProperties;
}

Texture* DeviceContextVK::createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level)
{
    return new TextureVK(target, format, type, size, data, level);
}

Texture* DeviceContextVK::createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level)
{
    return new TextureVK(format, type, size, data, level);
}

Buffer* DeviceContextVK::createBuffer(EBufferTarget target, EDataUsageType type)
{
    bool mappable = false;
    return new BufferVK(target, type, mappable);
}

ShaderProgram* DeviceContextVK::createShaderProgram(const resources::ShaderList& shaders, const resources::ShaderDefinesList& defines)
{
    return new ShaderProgramVK(shaders, defines);
}

void DeviceContextVK::fillGrapthicCaps()
{
    if (m_vulkanPropsDevice._features.sampleRateShading)
    {
        u32 countSamples = 1;
        VkSampleCountFlags flag = m_vulkanPropsDevice._properties.limits.sampledImageColorSampleCounts;
        if (VK_SAMPLE_COUNT_64_BIT & flag)
        {
            countSamples = 64;
        }
        else if (VK_SAMPLE_COUNT_32_BIT & flag)
        {
            countSamples = 32;
        }
        else if (VK_SAMPLE_COUNT_16_BIT & flag)
        {
            countSamples = 16;
        }
        else if (VK_SAMPLE_COUNT_8_BIT & flag)
        {
            countSamples = 8;
        }
        else if (VK_SAMPLE_COUNT_4_BIT & flag)
        {
            countSamples = 4;
        }
        else if (VK_SAMPLE_COUNT_2_BIT & flag)
        {
            countSamples = 2;
        }

        m_graphicsCaps.setSamplesCount(countSamples);
    }

    VkDeviceSize memoryBlockSize = m_vulkanPropsDevice._properties.limits.nonCoherentAtomSize;
    m_graphicsCaps.setMemoryBlockSize(memoryBlockSize);
    
    m_graphicsCaps.setMaxColorAttachments(m_vulkanPropsDevice._properties.limits.maxColorAttachments);

    LOG_INFO("Vuklan driver info:");
    /*LOG("Render: %s", renderer);
    LOG("Vendor: %s", vendor);
    LOG("GLSL: %s", GLSL);
    LOG("GL Version: %s", version);
    LOG("Max Texure Units: %d", maxTextureUnits);
    LOG("Max Texure Samplers: %d", maxTextureSamplers);
    LOG("Max Anisotropy: %f", maxAnisotropy);
    LOG("Max Draw Buffers: %d", maxDrawBuffers);
    LOG("MSAA x%d", samplesCount);
    LOG("Max supported patch vertices: %d", maxPatchVertices);*/
}

void DeviceContextVK::printExtensionList() const
{
}

} //namespace vk
} //namespace renderer
} //namespace v3d


#endif //_VULKAN_RENDER_

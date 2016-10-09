#pragma once

#include "common.h"

#if defined(_VULKAN_RENDER_) 
#   include <vulkan/vulkan.h>
#endif //_VULKAN_RENDER_

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class DebugVK
    {
    public:

        static std::string              errorString(VkResult errorCode);

        static VkBool32                 messageCallback(
                                                 VkDebugReportFlagsEXT flags,
                                                 VkDebugReportObjectTypeEXT objType,
                                                 u64 srcObject,
                                                 size_t location,
                                                 s32 msgCode,
                                                 c8* layerPrefix,
                                                 c8* msg,
                                                 void* userData);

        static void                     setupDebugging(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportCallbackEXT callBack, void* userData);
        static void                     freeDebugCallback(VkInstance instance);

        static VkDebugReportCallbackEXT s_msgCallback;

        static bool                     s_enableValidationLayers;
        static bool                     s_enableDebugMarkers;

        static std::vector<const c8*>   s_validationLayerNames;

        //TODO: DebugMarker

    private:

        static PFN_vkCreateDebugReportCallbackEXT   s_vkCreateDebugReportCallbackEXT;
        static PFN_vkDestroyDebugReportCallbackEXT  s_vkDestroyDebugReportCallbackEXT;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#pragma once

#include "renderer/Geometry.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"
#include "renderer/ShaderDataTypes.h"
#include "renderer/RenderTechnique.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for geometry management. Vulkan render only.
    */
    class GeometryVK final : public Geometry
    {
    public:

        GeometryVK(const RenderTechnique* technique);
        ~GeometryVK();
        
        void                init()      override;
        void                free()      override;
        void                draw()      override;
        void                refresh()   override;


        const VkPipelineVertexInputStateCreateInfo&     getPipelineVertexInputStateCreateInfo() const;
        void                                            updatePipelineVertexInputStateCreateInfo(const ShaderDataList& shaderDataList);

    private:

        VkPipelineVertexInputStateCreateInfo            m_pipelineVertexInputStateCreateInfo;

        bool                                            m_initialized;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

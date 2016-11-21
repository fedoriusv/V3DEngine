#pragma once

#include "renderer/ShaderProgram.h"
#include "renderer/ConstantBuffer.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"

namespace v3d
{
namespace renderer
{
namespace vk
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for render states management. Vulkan render only.
    */
    class ShaderProgramVK final : public ShaderProgram
    {
    public:

        ShaderProgramVK();
        ShaderProgramVK(const ShaderList& shaders, const ShaderDefinesList& defines = {});

        ~ShaderProgramVK();

        bool                                setDefine(const std::string& name, const std::string& value = "") override;
        bool                                setUndefine(const std::string& name) override;

        void                                attachShader(const ShaderPtr shader) override;
        void                                detachShader(const ShaderPtr shader) override;

        u16                                 getFlags() const override;
        bool                                isFlagPresent(EProgramFlags flag) override;

        const std::vector<VkShaderModule>&  getShaderModules() const;

    private:

        void                                applyUniform(const std::string& name, const void* value, u32 size) override;
        void                                applyAttribute(const std::string& name, const void* value, u32 size) override;
        void                                applyTexture(const std::string& name, const TexturePtr texure) override;

        const ShaderDefinesList&            getMacroDefinitions() const override;
        const ShaderList&                   getShaders() const override;

        void                                setMacroDefinitions(const ShaderDefinesList& list) override;
        void                                setShaderParams(ShaderParameters& params) override;

        bool                                compile(const ShaderDefinesList& defines, const ShaderList& shaders, ShaderParameters& outParameters) override;
        void                                destroy();

        void                                destoryAllModules();

        ShaderList                          m_shaderList;
        ShaderDefinesList                   m_defines;

        ShaderParameters                    m_parameters;
        ConstantBuffers                     m_constantBuffers;

        VkDevice                            m_device;
        std::vector<VkShaderModule>         m_shadersModules;

        std::atomic<u16>                    m_flags;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

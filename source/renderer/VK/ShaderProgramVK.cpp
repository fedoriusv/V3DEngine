#include "ShaderProgramVK.h"
#include "utils/Logger.h"

#ifdef _VULKAN_RENDER_
#include "context/DebugVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

ShaderProgramVK::ShaderProgramVK()
    : ShaderProgram()
    , m_flags(ShaderProgram::eCreated)

    , m_device(VK_NULL_HANDLE)
{
    LOG_DEBUG("ShaderProgramVK::ShaderProgramVK constructor %x", this);
}

ShaderProgramVK::ShaderProgramVK(const ShaderList& shaders, const ShaderDefinesList& defines)
    : ShaderProgram()
    , m_shaderList(shaders)
    , m_defines(defines)
    , m_flags(ShaderProgram::eCreated)

    , m_device(VK_NULL_HANDLE)
{
    LOG_DEBUG("ShaderProgramVK::ShaderProgramVK constructor %x", this);
}

ShaderProgramVK::~ShaderProgramVK()
{
    LOG_DEBUG("ShaderProgramVK::ShaderProgramVK destructor %x", this);
    ASSERT(m_shadersModules.empty(), "shaders modules already exist");
}

bool ShaderProgramVK::setDefine(const std::string& name, const std::string& value)
{
    auto iter = m_defines.find(name);
    if (iter != m_defines.cend())
    {
        if ((*iter).second == value)
        {
            return false;
        }

        (*iter).second = value;
        m_flags &= ~ShaderProgram::eCompiled;

        return true;
    }

    m_defines.insert(ShaderDefinesList::value_type(name, value));
    m_flags &= ~ShaderProgram::eCompiled;

    return true;
}

bool ShaderProgramVK::setUndefine(const std::string& name)
{
    auto iter = m_defines.find(name);
    if (iter != m_defines.cend())
    {
        m_defines.erase(name);
        m_flags &= ~ShaderProgram::eCompiled;

        return true;
    }

    return false;
}

void ShaderProgramVK::attachShader(const ShaderPtr shader)
{
    if (!shader)
    {
        m_shaderList.push_back(shader);
        m_flags &= ~ShaderProgram::eCompiled;
    }
}

void ShaderProgramVK::detachShader(const ShaderPtr shader)
{
    if (!shader)
    {
        auto current = std::find_if(m_shaderList.cbegin(), m_shaderList.cend(), [&shader](const ShaderPtr& item) -> bool
        {
            if (!item)
            {
                return item == shader;
            }

            return false;
        });

        if (current == m_shaderList.cend())
        {
            LOG_WARNING("ShaderProgramVK: Shader Program not found");
            return;
        }

        m_shaderList.erase(current);
        m_flags &= ~ShaderProgram::eCompiled;
    }
}

u16 ShaderProgramVK::getFlags() const
{
    return m_flags;
}

bool ShaderProgramVK::isFlagPresent(EProgramFlags flag)
{
    return (m_flags & flag) != 0;
}

const std::vector<VkShaderModule>& ShaderProgramVK::getShaderModules() const
{
    return m_shadersModules;
}

void ShaderProgramVK::applyUniform(const std::string& name, const void* value, u32 size)
{
    //TODO
}

void ShaderProgramVK::applyAttribute(const std::string& name, const void* value, u32 size)
{
    //TODO
}

void ShaderProgramVK::applyTexture(const std::string& name, const TexturePtr texure)
{
    //TODO
}

const ShaderDefinesList& ShaderProgramVK::getMacroDefinitions() const
{
    return m_defines;
}

const ShaderList& ShaderProgramVK::getShaders() const
{
    return m_shaderList;
}

void ShaderProgramVK::setMacroDefinitions(const ShaderDefinesList& list)
{
    m_defines = list;
    m_flags &= ~ShaderProgram::eCompiled;
}

void ShaderProgramVK::setShaderParams(ShaderParameters& params)
{
    m_parameters = params;
}

bool ShaderProgramVK::compile(const ShaderDefinesList& defines, const ShaderList& shaders, ShaderParameters& outParameters)
{
    m_flags = ShaderProgram::eCreated;
    ShaderProgramVK::destoryAllModules();

    for (auto& shader : m_shaderList)
    {
        std::vector<u32> bytecode;
        //TODO: compile, reflection

        VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.pNext = nullptr;
        shaderModuleCreateInfo.flags = 0;
        shaderModuleCreateInfo.pCode = bytecode.data();
        shaderModuleCreateInfo.codeSize = bytecode.size();

        VkShaderModule module = VK_NULL_HANDLE;
        VkResult result = vkCreateShaderModule(m_device, &shaderModuleCreateInfo, nullptr, &module);
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("ShaderProgramVK::compile: vkCreateShaderModule error %s", DebugVK::errorString(result));
            ShaderProgramVK::destoryAllModules();

            m_flags |= ShaderProgram::eInvalid;
            return false;
        }

        m_shadersModules.push_back(module);
    }

    m_flags |= ShaderProgram::eCompiled;
    return true;
}

void ShaderProgramVK::destroy()
{
    ShaderProgramVK::destoryAllModules();
    m_flags |= ShaderProgram::eDeleted;
}

void ShaderProgramVK::destoryAllModules()
{
    for (auto& module : m_shadersModules)
    {
        vkDestroyShaderModule(m_device, module, nullptr);
        module = VK_NULL_HANDLE;
    }

    m_shadersModules.clear();
}

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_

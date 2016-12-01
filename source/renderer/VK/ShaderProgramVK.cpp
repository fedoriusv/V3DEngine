#include "ShaderProgramVK.h"
#include "utils/Logger.h"
#include "Engine.h"
#include "scene/ShaderManager.h"

#ifdef _VULKAN_RENDER_
#include "context/DebugVK.h"
#include "RendererVK.h"
#ifdef USE_SPIRV
#   include "utils/SpirVCompiler.h"
#endif //USE_SPIRV

namespace v3d
{
namespace renderer
{
namespace vk
{

using namespace resources;

ShaderProgramVK::ShaderProgramVK()
    : ShaderProgram()
    , m_flags(ShaderProgram::eCreated)

    , m_device(VK_NULL_HANDLE)
{
    LOG_DEBUG("ShaderProgramVK::ShaderProgramVK constructor %x", this);
    m_device = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanDevice();
}

ShaderProgramVK::ShaderProgramVK(const ShaderList& shaders, const ShaderDefinesList& defines)
    : ShaderProgram()
    , m_shaderList(shaders)
    , m_defines(defines)
    , m_flags(ShaderProgram::eCreated)

    , m_device(VK_NULL_HANDLE)
{
    LOG_DEBUG("ShaderProgramVK::ShaderProgramVK constructor %x", this);
    m_device = std::static_pointer_cast<RendererVK>(ENGINE_RENDERER)->getVulkanContext()->getVulkanDevice();
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
    if (shader)
    {
        m_shaderList.push_back(shader);
        m_flags &= ~ShaderProgram::eCompiled;
    }
}

void ShaderProgramVK::detachShader(const ShaderPtr shader)
{
    if (shader)
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
    if (ShaderProgramVK::isFlagPresent(EProgramFlags::eCompiled))
    {

        auto iter = m_parameters.uniforms.find(name);
        if (iter == m_parameters.uniforms.cend())
        {
            return;
        }

        ShaderProgram::ShaderParameters::UniformParameter& param = (*iter).second;
        if (param.block > -1)
        {
            ConstantBuffer* buffer = m_constantBuffers[param.block];
            ENGINE_RENDERER->updateConstantBuffers(buffer, param.size, param.offset, value);
        }
        else
        {
            ASSERT(false, "uniform without block");
        }
    }
}

void ShaderProgramVK::applyAttribute(const std::string& name, const void* value, u32 size)
{
    if (ShaderProgramVK::isFlagPresent(EProgramFlags::eCompiled))
    {
        auto iter = m_parameters.channelsIn.find(name);
        if (iter == m_parameters.channelsIn.cend())
        {
            return;
        }

        ShaderProgram::ShaderParameters::Channel& param = (*iter).second;
        
        //TODO:
    }
}

void ShaderProgramVK::applyTexture(const std::string& name, const TexturePtr texure)
{
    if (ShaderProgramVK::isFlagPresent(EProgramFlags::eCompiled))
    {
        auto iter = m_parameters.samplers.find(name);
        if (iter == m_parameters.samplers.cend())
        {
            return;
        }

        ShaderProgram::ShaderParameters::TextureParameter& param = (*iter).second;

        //TODO:
    }
}

void ShaderProgramVK::addUniform(ShaderUniform* uniform)
{
    ShaderProgram::addUniformToShaderData(m_shaderData, uniform);
}

void ShaderProgramVK::addAttribute(ShaderAttribute* attribute)
{
    ShaderProgram::addAttributeToShaderData(m_shaderData, attribute);
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
    m_parameters = std::move(params);
    ShaderProgram::updateShaderData(m_parameters, m_shaderData);
}

bool ShaderProgramVK::compile(const resources::ShaderDefinesList& defines, const resources::ShaderList& shaders, ShaderProgram::ShaderParameters& outParameters)
{
    m_flags = ShaderProgram::eCreated;
    ShaderProgramVK::destoryAllModules();

#ifdef USE_SPIRV
    utils::SpirVCompileWrapper compiler(ENGINE_RENDERER->getRenderType(), defines);
    for (auto& shader : m_shaderList)
    {
        EShaderType type = shader->getType();
        std::vector<u32> bytecode;
        if (shader->getShaderKind() == Shader::EShaderDataRepresent::eBytecode)
        {
            ASSERT(shader->getBytecode(), "invalid bytecode");
            bytecode = *shader->getBytecode();
            //defines ignored
        }
        else //source
        {
            const std::string* source = shader->getSource();

            u64 hash = scene::ShaderManager::generateHash(*source, defines);
            const std::vector<u32>* compiledBytecode = scene::ShaderManager::getInstance()->getCompiledShader(hash);
            if (compiledBytecode)
            {
                bytecode = *compiledBytecode;
            }
            else
            {
                utils::SpirVCompileWrapper::ECompileError error = compiler.compile(*source, type, bytecode);
                if (error != utils::SpirVCompileWrapper::eNoErrors)
                {
                    LOG_ERROR("ShaderProgramVK::compile: %s[%s] has compile error %d.", shader->getName().c_str(), Shader::getShaderTypeNameByType(type).c_str(), error);
                    if (!compiler.errorMessages().empty())
                    {
                        LOG("\n%s\n", compiler.errorMessages().c_str());
                    }

                    ShaderProgramVK::destoryAllModules();

                    m_flags |= ShaderProgram::eInvalid;
                    return false;
                }

                scene::ShaderManager::getInstance()->addCompiledShader(hash, bytecode);
            }
        }

        ShaderProgram::ShaderParameters reflection = compiler.reflection(bytecode);
        if (type == EShaderType::eVertex)
        {
            outParameters.channelsIn = std::move(reflection.channelsIn);
        }
        else if (type == EShaderType::eFragment)
        {
            outParameters.channelsOut = std::move(reflection.channelsOut);
        }
        std::copy(reflection.samplers.cbegin(), reflection.samplers.cend(), std::inserter(outParameters.samplers, reflection.samplers.end()));
        std::copy(reflection.uniforms.cbegin(), reflection.uniforms.cend(), std::inserter(outParameters.uniforms, reflection.uniforms.end()));
        std::copy(reflection.constantBuffers.cbegin(), reflection.constantBuffers.cend(), std::back_inserter(outParameters.constantBuffers));

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
#else //USE_SPIRV
    ASSERT(false, "shader compiler not found");
    return false;
#endif //USE_SPIRV
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

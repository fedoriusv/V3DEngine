#include "ShaderProgram.h"
#include "ShaderUniform.h"
#include "utils/Logger.h"
#include "scene/ShaderManager.h"
#include "RenderThread.h"
#include "Engine.h"

namespace v3d
{
namespace renderer
{

using namespace scene;
using namespace resources;

ShaderProgram::ShaderProgram()
    : m_impl(nullptr)
{
}

ShaderProgram::ShaderProgram(const ShaderList& shaders, const ShaderDefinesList& defines)
    : m_impl(ENGINE_CONTEXT->createShaderProgram(shaders, defines))
{
}

ShaderProgram::~ShaderProgram()
{
    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandDestroyProgram);
        command.writeValue<ShaderProgram* const>(m_impl);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, false);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        m_impl->destroy();
        delete m_impl;
    }
}

bool ShaderProgram::setDefine(const std::string& name, const std::string& value)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->setDefine(name, value);
}

bool ShaderProgram::setUndefine(const std::string& name)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->setUndefine(name);
}

void ShaderProgram::applyTexture(const std::string& name, const TexturePtr texure)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->applyTexture(name, texure);
}

void ShaderProgram::attachShader(const ShaderPtr shader)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->attachShader(shader);
}

void ShaderProgram::detachShader(const ShaderPtr shader)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->detachShader(shader);
}

bool ShaderProgram::compile()
{
    if (ShaderProgram::isFlagPresent(ShaderProgram::eCompiled))
    {
        return true;
    }

    ASSERT(m_impl, "m_impl is nullptr");
    bool result = false;
    ShaderProgram::ShaderParameters params;

    if (ENGINE_RENDERER->isThreaded())
    {
        RenderStreamCommand command(ERenderCommand::eCommandCompileProgram);
        command.writeValue<ShaderProgram* const>(m_impl);

        command.writeValue<u64>(m_impl->getMacroDefinitions().size());
        for (auto& def : m_impl->getMacroDefinitions())
        {
            command.writeValue(def.first);
            command.writeValue(def.second);
        }

        command.writeValue<u64>(m_impl->getShaders().size());
        for (auto& shader : m_impl->getShaders())
        {
            command.writeString(shader->getName());
            stream::MemoryStreamPtr stream = std::static_pointer_cast<stream::MemoryStream>(shader->serialize());
            command.writeValue<u32>(stream->size());
            command.writeValue(stream->getData(), stream->size(), 1);
        }

        command.writeValue<void* const>(&params);
        command.writeValue<bool*>(&result);
        command.endCommand();

        ENGINE_RENDERER->pushCommand(command, true);
    }
    else
    {
        ASSERT(m_impl, "m_impl is nullptr");
        result = m_impl->compile(m_impl->getMacroDefinitions(), m_impl->getShaders(), params);
    }

    m_impl->setShaderParams(params);
    return result;
}

u16 ShaderProgram::getFlags() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getFlags();
}

bool ShaderProgram::isFlagPresent(EProgramFlags flag)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->isFlagPresent(flag);
}

ShaderProgramPtr ShaderProgram::clone() const
{
    //TODO:
    return ShaderProgramPtr();
}

void ShaderProgram::applyUniform(const std::string& name, const void* value, u32 size)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->applyUniform(name, value, size);
}

void ShaderProgram::applyAttribute(const std::string& name, const void* value, u32 size)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->applyAttribute(name, value, size);
}

void ShaderProgram::addUniform(ShaderUniform* uniform)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->addUniform(uniform);
}

void ShaderProgram::addAttribute(ShaderAttribute* attribute)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->addAttribute(attribute);
}

const ShaderDefinesList& ShaderProgram::getMacroDefinitions() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getMacroDefinitions();
}

const ShaderList& ShaderProgram::getShaders() const
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->getShaders();
}

void ShaderProgram::setShaderParams(ShaderParameters& params)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->setShaderParams(params);
}

void ShaderProgram::setMacroDefinitions(const ShaderDefinesList& list)
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->setMacroDefinitions(list);
}

bool ShaderProgram::compile(const resources::ShaderDefinesList& defines, const resources::ShaderList& shaders, ShaderProgram::ShaderParameters& outParameters)
{
    ASSERT(m_impl, "m_impl is nullptr");
    return m_impl->compile(defines, shaders, outParameters);
}

void ShaderProgram::destroy()
{
    ASSERT(m_impl, "m_impl is nullptr");
    m_impl->destroy();
}


} //namespace renderer
} //namespace v3d

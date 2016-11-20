#include "ShaderProgram.h"
#include "ShaderUniform.h"
#include "utils/Logger.h"
#include "scene/ShaderManager.h"
#include "Engine.h"

namespace v3d
{
namespace renderer
{

using namespace scene;

ShaderProgram::ShaderProgram()
    : m_enable(true)
    , m_flags(ShaderProgram::eInvalid)
{
}

ShaderProgram::ShaderProgram(const ShaderProgram& program)
    : m_enable(program.m_enable)
    , m_flags(ShaderProgram::eInvalid)
{
    m_shaderList = program.m_shaderList;
    m_defines = program.m_defines;

    //TODO: maybe memory leak
    std::copy(program.m_varyingsList.cbegin(), program.m_varyingsList.cend(), m_varyingsList.begin());
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& program)
{
    if (&program == this)
    {
        return *this;
    }

    m_enable = program.m_enable;
    m_flags = ShaderProgram::eInvalid;

    m_shaderDataList.clear(); //filled form render pass
    m_shaderList = program.m_shaderList;
    m_defines = program.m_defines;

    //TODO: maybe memory leak
    m_varyingsList.clear();
    std::copy(program.m_varyingsList.cbegin(), program.m_varyingsList.cend(), m_varyingsList.begin());

    return *this;
}

ShaderProgram::~ShaderProgram()
{
    ShaderProgram::clear();

    if (!ShaderProgram::isFlagPresent(ShaderProgram::eDeleted))
    {
        ASSERT(false, "Program incorrect deleted");
    }
}

bool ShaderProgram::isEnable() const
{
    return m_enable;
}

void ShaderProgram::setEnable(bool enable)
{
    m_enable = enable;
}

bool ShaderProgram::setDefine(const std::string& name, const std::string& value)
{
    auto iter = m_defines.find(name);
    if (iter != m_defines.cend())
    {
        if ((*iter).second == value)
        {
            return false;
        }

        (*iter).second = value;
        m_flags &= ~ShaderProgram::eLinked;

        return true;
    }

    m_defines.insert(ShaderDefinesList::value_type(name, value));
    m_flags &= ~ShaderProgram::eLinked;

    return true;
}

bool ShaderProgram::setUndefine(const std::string& name)
{
    auto iter = m_defines.find(name);
    if (iter != m_defines.cend())
    {
        m_defines.erase(name);
        m_flags &= ~ShaderProgram::eLinked;

        return true;
    }

    return false;
}

bool ShaderProgram::applyUniform(ShaderUniform * uniform)
{
    return false;
}

void ShaderProgram::applyUniformInt(s32 location, s32 value)
{
}

void ShaderProgram::applyUniformFloat(s32 location, f32 value)
{
}

void ShaderProgram::applyUniformVector2(s32 location, const core::Vector2D & vector)
{
}

void ShaderProgram::applyUniformVector3(s32 location, const core::Vector3D & vector)
{
}

void ShaderProgram::applyUniformVector4(s32 location, const core::Vector4D & vector)
{
}

void ShaderProgram::applyUniformMatrix3(s32 location, const core::Matrix3D & matrix)
{
}

void ShaderProgram::applyUniformMatrix4(s32 location, const core::Matrix4D & matrix)
{
}

void ShaderProgram::attachShader(const ShaderWPtr& shader)
{
    if (!shader.expired())
    {
        m_shaderList.push_back(shader);
        m_flags &= ~ShaderProgram::eLinked;
    }
}

void ShaderProgram::detachShader(const ShaderWPtr& shader)
{
    if (!shader.expired())
    {
        auto current = std::find_if(m_shaderList.cbegin(), m_shaderList.cend(), [&shader](const ShaderWPtr& item) -> bool
        {
            if (!item.expired())
            {
                return item.lock() == shader.lock();
            }

            return false;
        });

        if (current == m_shaderList.cend())
        {
            LOG_WARNING("ShaderProgram: Shader Program not found");
            return;
        }

        m_shaderList.erase(current);
        m_flags &= ~ShaderProgram::eLinked;
    }
}

void ShaderProgram::addShaderData(const ShaderDataPtr& data)
{
    if (data)
    {
        m_shaderDataList.push_back(data);
        m_flags &= ~ShaderProgram::eLinked;
    }
}

void ShaderProgram::addVaryingsAttibutes(const std::vector<const c8*>& list)
{
    if (!list.empty())
    {
        m_varyingsList = list;
        m_flags &= ~ShaderProgram::eLinked;
    }
}

void ShaderProgram::setMacroDefinition(const ShaderDefinesList& list)
{
    m_defines = list;
    m_flags &= ~ShaderProgram::eLinked;
}

bool ShaderProgram::updateShaderList()
{
    bool result = false;
    for (ShaderList::iterator iter = m_shaderList.begin(), end = m_shaderList.end(); iter != end;)
    {
        if (!(*iter).expired())
        {
            const CShaderSource& data = (*iter).lock()->getShaderSource();
            std::string header = CShaderSource::buildHeader(m_defines, data.getBody());
            std::size_t hash =  CShaderSource::calculateHash(header, data.getBody());

            if (hash == data.getHash())
            {
                iter++;
                continue;
            }

            ShaderWPtr shader = (*iter);
            //m_shaderList.erase(iter++);
            LOG_DEBUG("CShaderProgram::updateShaderList: Shader Program must be relinked");

            ShaderWPtr hashedShader = ShaderManager::getInstance()->get(hash);
            if (!hashedShader.expired())
            {
                //ShaderProgram::attachShader(hashedShader);
                (*iter) = hashedShader;
            }
            else
            {
                ShaderPtr newShader = shader.lock()->clone();
                if (!newShader->create(data.getType(), data.getBody(), m_defines))
                {
                    LOG_ERROR("ShaderProgram::updateShaderList: Error Create Shader Program");
                    return false;
                }

                ShaderManager::getInstance()->add(newShader);
                //ShaderProgram::attachShader(newShader);
                (*iter) = newShader;

                result = true;
            }
        }
    }

    return result;
}

bool ShaderProgram::create(const std::string& vertex, const std::string& fragment, u32 arg, ...)
{
    if (vertex.empty() || fragment.empty())
    {
        ASSERT(false, "Empty Shader body");
        return false;
    }

    ShaderProgram::clear();

    ShaderPtr vshader = ENGINE_RENDERER->makeSharedShader();
    ShaderProgram::attachShader(vshader);
    vshader->create(EShaderType::eVertex, vertex);

    ShaderPtr fshader = ENGINE_RENDERER->makeSharedShader();
    ShaderProgram::attachShader(fshader);
    fshader->create(EShaderType::eFragment, fragment);

    va_list argList;
    va_start(argList, arg);
    for (u32 i = 0; i < arg; i += 2)
    {
        c8* strName = va_arg(argList, c8*);
        s32 type = va_arg(argList, s32);

        ShaderPtr shader = ENGINE_RENDERER->makeSharedShader();
        ShaderProgram::attachShader(shader);
        shader->create((EShaderType)type, strName);
    }
    va_end(argList);

    bool status = create();

    return status;
}

bool ShaderProgram::create(const std::string& compute)
{
    if (compute.empty())
    {
        ASSERT(false, "Empty Shader body");
        return false;
    }

    ShaderProgram::clear();

    ShaderPtr cshader = ENGINE_RENDERER->makeSharedShader();
    ShaderProgram::attachShader(cshader);
    cshader->create(EShaderType::eCompute, compute);

    bool status = create();

    return status;
}

u16 ShaderProgram::getFlags() const
{
    return m_flags;
}

bool ShaderProgram::isFlagPresent(EProgramFlags flag)
{
    return (m_flags & flag) != 0;
}

void ShaderProgram::setFlag(EProgramFlags flag)
{
    m_flags = flag;
}

void ShaderProgram::addFlag(EProgramFlags flag)
{
    m_flags |= flag;
}

void ShaderProgram::clear()
{
    m_shaderList.clear();
    m_shaderDataList.clear();
    m_defines.clear();
    m_varyingsList.clear();
}

} //namespace renderer
} //namespace v3d

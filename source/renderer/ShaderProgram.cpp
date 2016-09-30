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

IShaderProgram::IShaderProgram()
    : m_enable(true)
    , m_flags(IShaderProgram::eInvalid)
{
}

IShaderProgram::IShaderProgram(const IShaderProgram& program)
    : m_enable(program.m_enable)
    , m_flags(IShaderProgram::eInvalid)
{
    m_shaderList = program.m_shaderList;
    m_defines = program.m_defines;

    //TODO: maybe memory leak
    std::copy(program.m_varyingsList.cbegin(), program.m_varyingsList.cend(), m_varyingsList.begin());
}

IShaderProgram& IShaderProgram::operator=(const IShaderProgram& program)
{
    if (&program == this)
    {
        return *this;
    }

    m_enable = program.m_enable;
    m_flags = IShaderProgram::eInvalid;

    m_shaderDataList.clear(); //filled form render pass
    m_shaderList = program.m_shaderList;
    m_defines = program.m_defines;

    //TODO: maybe memory leak
    m_varyingsList.clear();
    std::copy(program.m_varyingsList.cbegin(), program.m_varyingsList.cend(), m_varyingsList.begin());

    return *this;
}

IShaderProgram::~IShaderProgram()
{
    IShaderProgram::clear();

    if (!IShaderProgram::isFlagPresent(IShaderProgram::eDeleted))
    {
        ASSERT(false, "Program incorrect deleted");
    }
}

bool IShaderProgram::isEnable() const
{
    return m_enable;
}

void IShaderProgram::setEnable(bool enable)
{
    m_enable = enable;
}

bool IShaderProgram::setDefine(const std::string& name, const std::string& value)
{
    auto iter = m_defines.find(name);
    if (iter != m_defines.cend())
    {
        if ((*iter).second == value)
        {
            return false;
        }

        (*iter).second = value;
        m_flags &= ~IShaderProgram::eLinked;

        return true;
    }

    m_defines.insert(ShaderDefinesList::value_type(name, value));
    m_flags &= ~IShaderProgram::eLinked;

    return true;
}

bool IShaderProgram::setUndefine(const std::string& name)
{
    auto iter = m_defines.find(name);
    if (iter != m_defines.cend())
    {
        m_defines.erase(name);
        m_flags &= ~IShaderProgram::eLinked;

        return true;
    }

    return false;
}

void IShaderProgram::attachShader(const ShaderWPtr& shader)
{
    if (!shader.expired())
    {
        m_shaderList.push_back(shader);
        m_flags &= ~IShaderProgram::eLinked;
    }
}

void IShaderProgram::detachShader(const ShaderWPtr& shader)
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
            LOG_WARNING("IShaderProgram: Shader Program not found");
            return;
        }

        m_shaderList.erase(current);
        m_flags &= ~IShaderProgram::eLinked;
    }
}

void IShaderProgram::addShaderData(const ShaderDataPtr& data)
{
    if (data)
    {
        m_shaderDataList.push_back(data);
        m_flags &= ~IShaderProgram::eLinked;
    }
}

void IShaderProgram::addVaryingsAttibutes(const std::vector<const c8*>& list)
{
    if (!list.empty())
    {
        m_varyingsList = list;
        m_flags &= ~IShaderProgram::eLinked;
    }
}

void IShaderProgram::addDefines(const ShaderDefinesList& list)
{
    m_defines = list;
    m_flags &= ~IShaderProgram::eLinked;
}

bool IShaderProgram::updateShaderList()
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
            LOG_DEBUG("CIShaderProgram::updateShaderList: Shader Program must be relinked");

            ShaderWPtr hashedShader = CShaderManager::getInstance()->get(hash);
            if (!hashedShader.expired())
            {
                //IShaderProgram::attachShader(hashedShader);
                (*iter) = hashedShader;
            }
            else
            {
                ShaderPtr newShader = shader.lock()->clone();
                if (!newShader->create(data.getType(), data.getBody(), m_defines))
                {
                    LOG_ERROR("IShaderProgram::updateShaderList: Error Create Shader Program");
                    return false;
                }

                CShaderManager::getInstance()->add(newShader);
                //IShaderProgram::attachShader(newShader);
                (*iter) = newShader;

                result = true;
            }
        }
    }

    return result;
}

bool IShaderProgram::create(const std::string& vertex, const std::string& fragment, u32 arg, ...)
{
    if (vertex.empty() || fragment.empty())
    {
        ASSERT(false, "Empty Shader body");
        return false;
    }

    IShaderProgram::clear();

    ShaderPtr vshader = ENGINE_RENDERER->makeSharedShader();
    IShaderProgram::attachShader(vshader);
    vshader->create(EShaderType::eVertex, vertex);

    ShaderPtr fshader = ENGINE_RENDERER->makeSharedShader();
    IShaderProgram::attachShader(fshader);
    fshader->create(EShaderType::eFragment, fragment);

    va_list argList;
    va_start(argList, arg);
    for (u32 i = 0; i < arg; i += 2)
    {
        c8* strName = va_arg(argList, c8*);
        s32 type = va_arg(argList, s32);

        ShaderPtr shader = ENGINE_RENDERER->makeSharedShader();
        IShaderProgram::attachShader(shader);
        shader->create((EShaderType)type, strName);
    }
    va_end(argList);

    bool status = create();

    return status;
}

bool IShaderProgram::create(const std::string& compute)
{
    if (compute.empty())
    {
        ASSERT(false, "Empty Shader body");
        return false;
    }

    IShaderProgram::clear();

    ShaderPtr cshader = ENGINE_RENDERER->makeSharedShader();
    IShaderProgram::attachShader(cshader);
    cshader->create(EShaderType::eCompute, compute);

    bool status = create();

    return status;
}

u16 IShaderProgram::getFlags() const
{
    return m_flags;
}

bool IShaderProgram::isFlagPresent(EProgramFlags flag)
{
    return (m_flags & flag) != 0;
}

void IShaderProgram::setFlag(EProgramFlags flag)
{
    m_flags = flag;
}

void IShaderProgram::addFlag(EProgramFlags flag)
{
    m_flags |= flag;
}

void IShaderProgram::clear()
{
    m_shaderList.clear();
    m_shaderDataList.clear();
    m_defines.clear();
    m_varyingsList.clear();
}

} //namespace renderer
} //namespace v3d

#include "ShaderProgramGL.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _OPENGL_DRIVER_
#include "renderer/GL/ShaderGL.h"
#include "renderer/GL/TransformFeedbackGL.h"
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{

using namespace core;


u32 CShaderProgramGL::s_currentShader = 0;

CShaderProgramGL::CShaderProgramGL()
    : IShaderProgram()
    , m_id(0)
{
    LOG_DEBUG("CShaderProgramGL: CShaderProgramGL constructor %x", this);
}

CShaderProgramGL::CShaderProgramGL(const CShaderProgramGL& program)
    : IShaderProgram()
    , m_id(0)
{
    LOG_DEBUG("CShaderProgramGL: CShaderProgramGL constructor %x", this);
}

CShaderProgramGL& CShaderProgramGL::operator=(const CShaderProgramGL& program)
{
    if (&program == this)
    {
        return *this;
    }

    IShaderProgram::operator=(program);

    return *this;
}

CShaderProgramGL::~CShaderProgramGL()
{
    CShaderProgramGL::destroy();
    ASSERT(m_id == 0, "Shader doesn't deleted");

    LOG_DEBUG("CShaderProgramGL: CShaderProgramGL destructor %x", this);
}

bool CShaderProgramGL::create()
{
    updateShaderList();

    std::vector<u32> shadersId;
    CShaderProgramGL::getShaderIDArray(shadersId);
    ASSERT(!shadersId.empty(), "Empty shader list");

    bool status = CShaderProgramGL::createProgram(shadersId);
    shadersId.clear();

    return status;
}

void CShaderProgramGL::getShaderIDArray(std::vector<u32>& shaders)
{
    for (u32 i = 0; i < m_shaderList.size(); ++i)
    {
        if (!m_shaderList[i].expired() && m_shaderList[i].lock()->isFlagPresent(IShader::eCompiled))
        {
            shaders.push_back(std::static_pointer_cast<CShaderGL>(m_shaderList[i].lock())->getShaderID());
        }
    }
}

void CShaderProgramGL::destroy()
{
    if (m_id > 0)
    {
#ifdef _DEBUG_GL
        ASSERT(glIsProgram(m_id), "Invalid Index delete Shader program");
#endif //_DEBUG_GL
        glDeleteProgram(m_id);
        m_id = 0;

        CShaderProgramGL::addFlag(CShaderProgramGL::eDeleted);
    }
}

void CShaderProgramGL::bind()
{
    if (m_enable)
    {
        CShaderProgramGL::useProgram(m_id);
#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CShaderProgramGL: Bind ShaderProgram Error");
#endif //_DEBUG_GL
    }
}

void CShaderProgramGL::unbind()
{
    CShaderProgramGL::useProgram(0);
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: Unbind ShaderProgram Error");
#endif //_DEBUG_GL
}

u32 CShaderProgramGL::getShaderProgramID() const
{
    return m_id;
}

ShaderProgramPtr CShaderProgramGL::clone() const
{
    ShaderProgramPtr program = RENDERER->makeSharedProgram();
    std::static_pointer_cast<CShaderProgramGL>(program)->operator=(*this);
    
    if (program->create())
    {
        return program;
    }

    ASSERT(false, "CShaderProgramGL::clone fail");
    return nullptr;
}

bool CShaderProgramGL::createProgram(const std::vector<u32>& shaders)
{
    if (!IShaderProgram::isFlagPresent(IShaderProgram::eCreated))
    {
        ASSERT(!m_id, "Shader id exist");

        m_id = glCreateProgram();
#ifdef _DEBUG_GL
        ASSERT(glIsProgram(m_id), "Invalid Index Created Shader program");
#endif //_DEBUG_GL
        if (!m_id)
        {
            IShaderProgram::setFlag(IShaderProgram::eInvalid);
            return false;
        }

        IShaderProgram::addFlag(IShaderProgram::eCreated);
    }
   
    for (u32 i = 0; i < shaders.size(); ++i)
    {
        ASSERT(glIsShader(shaders[i]), "Invalid Index Attached Shader program");
        glAttachShader(m_id, shaders[i]);
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: Attach Program error");
#endif //_DEBUG_GL

    if (!m_varyingsList.empty())
    {
        gl::TransformFeedbackGL::transformFeedbackVaryings(m_id, m_varyingsList);
#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CShaderProgramGL: Add Varyings to program error");
#endif //_DEBUG_GL
    }

    if (!IShaderProgram::isFlagPresent(IShaderProgram::eLinked))
    {
        if (!CShaderProgramGL::linkProgram())
        {
            LOG_ERROR("CShaderProgramGL: Program didn't linked: %d", m_id);
            CShaderProgramGL::destroy();

            IShaderProgram::setFlag(IShaderProgram::eInvalid);
            return false;
        }

        LOG_DEBUG("CShaderProgramGL::create: Shader Program [%d] linked successful", m_id);
        IShaderProgram::addFlag(IShaderProgram::eLinked);

        if (!CShaderProgramGL::validateProgram())
        {
            LOG_ERROR("CShaderProgramGL: Program didn't validate: %d", m_id);
            CShaderProgramGL::destroy();

            IShaderProgram::setFlag(IShaderProgram::eInvalid);
            return false;
        }
        IShaderProgram::addFlag(IShaderProgram::eValidated);
    }


    GLint originalProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &originalProgram);

    CShaderProgramGL::useProgram(m_id);

    //Shader data
    s32 samplerCount = 0;
    for (auto& shaderData : m_shaderDataList)
    {
        AttributeList& attributeList = shaderData.lock()->getAttributeList();
        for (AttributeList::iterator attribute = attributeList.begin(), end = attributeList.end(); attribute != end;)
        {
            const std::string& name = (*attribute).second->getName();
            s32 id = glGetAttribLocation(m_id, name.c_str());
            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Attribute not found: %s in program %d", name.c_str(), m_id);
                ++attribute;
                // Not need delete more, maybe it will used after clone render technique
                //attributeList.erase(attribute++);
            }
            else
            {
                (*attribute).second->setID(id);
                ++attribute;
            }
        }
#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CShaderProgramGL: Bind attribute error");
#endif //_DEBUG_GL
        AttributeList& fragDataList = shaderData.lock()->getFragDataList();
        for (AttributeList::iterator fragData = fragDataList.begin(), end = fragDataList.end(); fragData != end;)
        {
            const std::string& name = (*fragData).second->getName();
            s32 id = glGetFragDataLocation(m_id, name.c_str());
            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Frag data not found: %s in program %d", name.c_str(), m_id);
                ++fragData;
                // Not need delete more, maybe it will used after clone render technique
                //fragDataList.erase(fragData++);
            }
            else
            {
                (*fragData).second->setID(id);
                ++fragData;
            }
        }
#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CShaderProgramGL: Bind frag data error");
#endif //_DEBUG_GL
        UniformList& uniformList = shaderData.lock()->getUniformList();
        for (UniformList::iterator uniform = uniformList.begin(), end = uniformList.end(); uniform != end;)
        {
            const std::string& name = (*uniform).second->getName();
            s32 id = glGetUniformLocation(m_id, name.c_str());

            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Uniform not found: %s in program %d", name.c_str(), m_id);
                ++uniform;
                //Not need delete more, maybe it will used after clone render technique
                //uniformList.erase(uniform++);
            }
            else
            {
                (*uniform).second->setID(id);
                ++uniform;
            }
        }
#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CShaderProgramGL: Bind uniform error");
#endif //_DEBUG_GL
        SamplerList& samplerList = shaderData.lock()->getSamplerList();
        for (auto& sampler : samplerList)
        {
            const std::string& name = sampler->getName();
            s32 id = glGetUniformLocation(m_id, name.c_str());
            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Sampler not found: %s in program %d", name.c_str(), m_id);
            }
            else
            {
                sampler->setID(samplerCount);

                CShaderProgramGL::applyUniformInt(id, samplerCount);
                ++samplerCount;
            }
        }
#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CShaderProgramGL: Bind sampler error");
#endif //_DEBUG_GL
        /*SamplerList::iterator samplerIter = std::remove_if(samplerList.begin(), samplerList.end(), [](const CShaderSampler* item) -> bool
        {
            return (item->getID() == -1);
        });

        if (samplerIter != samplerList.end())
        {
            samplerList.erase(samplerIter, samplerList.end());
        }*/
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: Init ShaderProgram Error");
#endif //_DEBUG_GL
    if (originalProgram >= 0)
    {
        CShaderProgramGL::useProgram(originalProgram);
    }

    for (u32 i = 0; i < shaders.size(); ++i)
    {
        ASSERT(glIsShader(shaders[i]), "Invalid Index Detached Shader program");
        glDetachShader(m_id, shaders[i]);
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: Detach ShaderProgram Error");
#endif //_DEBUG_GL

    return true;
}

bool CShaderProgramGL::linkProgram()
{
#ifdef _DEBUG_GL
    ASSERT(glIsProgram(m_id), "Invalid Index Link Shader program");
#endif //_DEBUG_GL
    glLinkProgram(m_id);

    RENDERER->checkForErrors("CShaderProgramGL: link");

    GLint linkStatus;
    glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        LOG_ERROR("CShaderProgramGL: Shader program not linked id: %d", m_id);
    }
#ifdef _DEBUG_GL
    GLint length;
    GLint charsWritten;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);

    if (length > 0)
    {
        GLchar* buffer = new GLchar[length];

        glGetProgramInfoLog(m_id, length, &charsWritten, buffer);
        if (strlen(buffer) > 0)
        {
            LOG_ERROR("CShaderProgramGL: Shader Program [%d] Link Logs:\n %s", m_id, buffer);
        }

        delete[] buffer;
    }
#endif //_DEBUG_GL

    return (linkStatus == GL_TRUE) ? true : false;
}

bool CShaderProgramGL::validateProgram()
{
#ifdef _DEBUG_GL
    ASSERT(glIsProgram(m_id), "Invalid Index Validate Shader program");
#endif //_DEBUG_GL
    glValidateProgram(m_id);

    GLint validateStatus;
    glGetProgramiv(m_id, GL_VALIDATE_STATUS, &validateStatus);
    if (!validateStatus)
    {
        LOG_ERROR("CShaderProgramGL: Shader program not validate id: %d", m_id);
    }
#ifdef _DEBUG_GL
   GLint length;
   GLint charsWritten;
   glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);

   if (length > 0)
   {
       GLchar* buffer = new GLchar[length];

       glGetProgramInfoLog(m_id, length, &charsWritten, buffer);
       if (strlen(buffer) > 0)
       {
           LOG_ERROR("CShaderProgramGL: Shader Program [%d] Validate Logs:\n %s", m_id, buffer);
       }

       delete[] buffer;
   }
#endif //_DEBUG_GL

    return (validateStatus == GL_TRUE) ? true : false;
}

bool CShaderProgramGL::useProgram(u32 shaderProgram)
{
    if (s_currentShader != shaderProgram)
    {
#ifdef _DEBUG_GL
        ASSERT((glIsProgram(shaderProgram) || shaderProgram == 0), "Invalid Index bind Shader program");
#endif //_DEBUG_GL
        glUseProgram(shaderProgram);
        s_currentShader = shaderProgram;

        return true;
    }

    return false;
}

s32 CShaderProgramGL::getCurrentProgram()
{
    s32 program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    return program;
}

bool CShaderProgramGL::applyUniform(CShaderUniform* uniform)
{
    if (m_id <= 0)
    {
        return false;
    }

    GLint location = uniform->getID();
    if (location < 0)
    {
        location = glGetUniformLocation(m_id, uniform->getName().c_str());
        if (location > -1)
        {
            uniform->setID(location);
        }
    }

    if (location > -1)
    {
        void* value = uniform->getValue();
        switch (uniform->getType())
        {
            case EDataType::eTypeNone:
            {
                location = -1;
            }
                break;

            case EDataType::eTypeInt:
            {
                GLint& val = *(GLint*)value;
                CShaderProgramGL::applyUniformInt(location, val);
            }
                break;

            case EDataType::eTypeFloat:
            {
                GLfloat& val = *(GLfloat*)value;
                CShaderProgramGL::applyUniformFloat(location, val);
            }
                break;

            case EDataType::eTypeVector2:
            {
                Vector2D& val = *(Vector2D*)value;
                CShaderProgramGL::applyUniformVector2(location, val);
            }
                break;

            case EDataType::eTypeVector3:
            {
                Vector3D& val = *(Vector3D*)value;
                CShaderProgramGL::applyUniformVector3(location, val);
            }
                break;

            case EDataType::eTypeVector4:
            {
                Vector4D& val = *(Vector4D*)value;
                CShaderProgramGL::applyUniformVector4(location, val);
            }
                break;

            case EDataType::eTypeMatrix3:
            {
                Matrix3D& val = *(Matrix3D*)value;
                CShaderProgramGL::applyUniformMatrix3(location, val);
            }
                break;

            case EDataType::eTypeMatrix4:
            {
                Matrix4D& val = *(Matrix4D*)value;
                CShaderProgramGL::applyUniformMatrix4(location, val);
            }
                break;

            default:
            {
                location = -1;
            }
        }
    }

#ifdef _DEBUG_GL
    if (location == -1)
    {
        LOG_ERROR("CShaderProgramGL: Error Uniform Location: %s . Shader ID : %d", uniform->getName().c_str(), m_id);
    }

    RENDERER->checkForErrors("CShaderProgramGL Set Uniform Error: " + uniform->getName());
#endif //_DEBUG_GL
    return (location != -1);
}

void CShaderProgramGL::applyUniformInt(s32 location, s32 value)
{
    if (location > -1)
    {
        glUniform1i(location, value);
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: applyUniformInt Error");
#endif //_DEBUG_GL
}

void CShaderProgramGL::applyUniformFloat(s32 location, f32 value)
{
    if (location > -1)
    {
        glUniform1f(location, value);
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: applyUniformFloat Error");
#endif //_DEBUG_GL
}

void CShaderProgramGL::applyUniformVector2(s32 location, const core::Vector2D& vector)
{
    if (location > -1)
    {
        glUniform2fv(location, 1, &vector.x);
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: applyUniformVector2 Error");
#endif //_DEBUG_GL
}
void CShaderProgramGL::applyUniformVector3(s32 location, const core::Vector3D& vector)
{
    if (location > -1)
    {
        glUniform3fv(location, 1, &vector.x);
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: applyUniformVector3 Error");
#endif //_DEBUG_GL
}

void CShaderProgramGL::applyUniformVector4(s32 location, const core::Vector4D& vector)
{
    if (location > -1)
    {
        glUniform4fv(location, 1, &vector.x);
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: applyUniformVector4 Error");
#endif //_DEBUG_GL
}

void CShaderProgramGL::applyUniformMatrix3(s32 location, const core::Matrix3D& matrix)
{
    if (location > -1)
    {
        glUniformMatrix3fv(location, 1, GL_TRUE, matrix.getPtr());
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: applyUniformMatrix3 Error");
#endif //_DEBUG_GL
}

void CShaderProgramGL::applyUniformMatrix4(s32 location, const core::Matrix4D& matrix)
{
    if (location > -1)
    {
        glUniformMatrix4fv(location, 1, GL_TRUE, matrix.getPtr());
    }
#ifdef _DEBUG_GL
    RENDERER->checkForErrors("CShaderProgramGL: applyUniformMatrix4 Error");
#endif //_DEBUG_GL
}

} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_

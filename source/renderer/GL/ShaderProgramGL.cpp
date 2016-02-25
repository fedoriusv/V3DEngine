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
    : m_shaderProgramID(0U)
{
    LOG_DEBUG("CShaderProgramGL: CShaderProgramGL constructor %x", this);
}

CShaderProgramGL::~CShaderProgramGL()
{
    CShaderProgramGL::destroy();
    ASSERT(m_shaderProgramID == 0, "Shader doesn't deleted");

    LOG_DEBUG("CShaderProgramGL: CShaderProgramGL destructor %x", this);
}

bool CShaderProgramGL::create()
{
    if (CShaderProgram::getCompileStatus())
    {
        return true;
    }

    std::vector<u32> shadersId;
    CShaderProgramGL::getShaderIDArray(shadersId);

    m_compileStatus = CShaderProgramGL::init(shadersId);
    shadersId.clear();

    return m_compileStatus;
}

void CShaderProgramGL::getShaderIDArray(std::vector<u32>& shaders)
{
    for (u32 i = 0; i < m_shaderList.size(); ++i)
    {
        if (m_shaderList[i]->getCompileStatus())
        {
            shaders.push_back(std::static_pointer_cast<CShaderGL>(m_shaderList[i])->getShaderID());
        }
    }
}

void CShaderProgramGL::destroy()
{
    if (m_shaderProgramID > 0)
    {
        ASSERT(glIsProgram(m_shaderProgramID), "Invalid Index delete Shader program");
        glDeleteProgram(m_shaderProgramID);

        m_shaderProgramID = 0;
    }
}

void CShaderProgramGL::bind()
{
    if (m_enable)
    {
        CShaderProgramGL::useProgram(m_shaderProgramID);
        RENDERER->checkForErrors("CShaderProgramGL: Bind ShaderProgram Error");
    }
}

void CShaderProgramGL::unbind()
{
    CShaderProgramGL::useProgram(0);
    RENDERER->checkForErrors("CShaderProgramGL: Unbind ShaderProgram Error");
}

u32 CShaderProgramGL::getShaderProgramID() const
{
    return m_shaderProgramID;
}

bool CShaderProgramGL::init(const std::vector<u32>& shaders)
{
    GLint originalProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &originalProgram);

    CShaderProgramGL::destroy();

    m_shaderProgramID = glCreateProgram();
    ASSERT(glIsProgram(m_shaderProgramID), "Invalid Index Created Shader program");

    for (u32 i = 0; i < shaders.size(); ++i)
    {
        ASSERT(glIsShader(shaders[i]), "Invalid Index Attached Shader program");
        glAttachShader(m_shaderProgramID, shaders[i]);
    }
    RENDERER->checkForErrors("CShaderProgramGL: Create Program error");

    //Varyings list
    if (!m_varyingsList.empty())
    {
        gl::TransformFeedbackGL::transformFeedbackVaryings(m_shaderProgramID, m_varyingsList);
        RENDERER->checkForErrors("CShaderProgramGL: Add Varyings to program error");
    }

    if (!CShaderProgramGL::link())
    {
        LOG_ERROR("CShaderProgramGL: Program didn't linked: %d", m_shaderProgramID);
        CShaderProgramGL::destroy();
        return false;
    }

    if (!CShaderProgramGL::validate())
    {
        LOG_ERROR("CShaderProgramGL: Program didn't validate: %d", m_shaderProgramID);
        CShaderProgramGL::destroy();
        return false;
    }

    CShaderProgramGL::useProgram(m_shaderProgramID);

    //Shader data
    s32 samplerCount = 0;
    for (auto& shaderData : m_shaderDataList)
    {
        AttributeList& attributeList = shaderData.lock()->getAttributeList();
        for (AttributeList::iterator attribute = attributeList.begin(), end = attributeList.end(); attribute != end;)
        {
            const std::string& name = (*attribute).second->getName();
            s32 id = glGetAttribLocation(m_shaderProgramID, name.c_str());
            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Attribute not found: %s", name.c_str());
                attributeList.erase(attribute++);
            }
            else
            {
                (*attribute).second->setID(id);
                ++attribute;
            }
        }
        RENDERER->checkForErrors("CShaderProgramGL: Bind attribute error");

        AttributeList& fragDataList = shaderData.lock()->getFragDataList();
        for (AttributeList::iterator fragData = fragDataList.begin(), end = fragDataList.end(); fragData != end;)
        {
            const std::string& name = (*fragData).second->getName();
            s32 id = glGetFragDataLocation(m_shaderProgramID, name.c_str());
            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Frag data not found: %s", name.c_str());
                fragDataList.erase(fragData++);
            }
            else
            {
                (*fragData).second->setID(id);
                ++fragData;
            }
        }
        RENDERER->checkForErrors("CShaderProgramGL: Bind frag data error");

        UniformList& uniformList = shaderData.lock()->getUniformList();
        for (UniformList::iterator uniform = uniformList.begin(), end = uniformList.end(); uniform != end;)
        {
            const std::string& name = (*uniform).second->getName();
            s32 id = glGetUniformLocation(m_shaderProgramID, name.c_str());

            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Uniform not found: %s", name.c_str());
                uniformList.erase(uniform++);
            }
            else
            {
                (*uniform).second->setID(id);
                ++uniform;
            }
        }
        RENDERER->checkForErrors("CShaderProgramGL: Bind uniform error");

        SamplerList& samplerList = shaderData.lock()->getSamplerList();
        for (auto& sampler : samplerList)
        {
            const std::string& name = sampler->getAttribute();
            s32 id = glGetUniformLocation(m_shaderProgramID, name.c_str());
            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Sampler not found: %s", name.c_str());
            }
            else
            {
                sampler->setID(samplerCount);

                CShaderProgramGL::applyUniformInt(id, samplerCount);
                ++samplerCount;
            }
        }
        RENDERER->checkForErrors("CShaderProgramGL: Bind sampler error");

        std::remove_if(samplerList.begin(), samplerList.end(), [](const SamplerPtr item) -> bool
        {
            return (item->getID() == -1);
        });
    }
    RENDERER->checkForErrors("CShaderProgramGL: Init ShaderProgram Error");

    if (originalProgram >= 0)
    {
        CShaderProgramGL::useProgram(originalProgram);
    }

    for (u32 i = 0; i < shaders.size(); ++i)
    {
        ASSERT(glIsShader(shaders[i]), "Invalid Index Detached Shader program");
        glDetachShader(m_shaderProgramID, shaders[i]);
    }

    return true;
}

bool CShaderProgramGL::link()
{
    ASSERT(glIsProgram(m_shaderProgramID), "Invalid Index Link Shader program");
    glLinkProgram(m_shaderProgramID);

    GLint linkStatus;
    glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        LOG_ERROR("CShaderProgramGL: Shader program not linked id: %d", m_shaderProgramID);
    }
#ifdef _DEBUG_GL
    GLint length;
    GLint charsWritten;
    glGetProgramiv(m_shaderProgramID, GL_INFO_LOG_LENGTH, &length);

    GLchar* buffer = new GLchar[length];
    glGetProgramInfoLog(m_shaderProgramID, length, &charsWritten, buffer);
    if (strlen(buffer) > 0)
    {
        LOG_ERROR("CShaderProgramGL: Shader Program [%d] Link Logs:\n %s", m_shaderProgramID, buffer);
    }
    delete[] buffer;
#endif //_DEBUG_GL

    return (linkStatus == GL_TRUE) ? true : false;
}

bool CShaderProgramGL::validate()
{
    ASSERT(glIsProgram(m_shaderProgramID), "Invalid Index Validate Shader program");
    glValidateProgram(m_shaderProgramID);

    GLint validateStatus;
    glGetProgramiv(m_shaderProgramID, GL_VALIDATE_STATUS, &validateStatus);
    if (!validateStatus)
    {
        LOG_ERROR("CShaderProgramGL: Shader program not validate id: %d", m_shaderProgramID);
    }
#ifdef _DEBUG_GL

   GLint length;
   GLint charsWritten;
   glGetProgramiv(m_shaderProgramID, GL_INFO_LOG_LENGTH, &length);

   GLchar* buffer = new GLchar[length];
   glGetProgramInfoLog(m_shaderProgramID, length, &charsWritten, buffer);
   if (strlen(buffer) > 0)
   {
       LOG_ERROR("CShaderProgramGL: Shader Program [%d] Validate Logs:\n %s", m_shaderProgramID, buffer);
   }
   delete[] buffer;
#endif //_DEBUG_GL

    return (validateStatus == GL_TRUE) ? true : false;
}

bool CShaderProgramGL::useProgram(u32 shaderProgram)
{
    if (s_currentShader != shaderProgram)
    {
        ASSERT((glIsProgram(shaderProgram) || shaderProgram == 0), "Invalid Index bind Shader program");
        glUseProgram(shaderProgram);
        s_currentShader = shaderProgram;

        return true;
    }

    return false;
}

bool CShaderProgramGL::applyUniform(CShaderUniform* uniform)
{
    if (m_shaderProgramID <= 0)
    {
        return false;
    }

    GLint location = uniform->getID();
    if (location < 0)
    {
        location = glGetUniformLocation(m_shaderProgramID, uniform->getName().c_str());
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

    if (location == -1)
    {
        LOG_ERROR("CShaderProgramGL: Error Uniform Location: %s . Shader ID : %d", uniform->getName().c_str(), m_shaderProgramID);
    }

    RENDERER->checkForErrors("CShaderProgramGL Set Uniform Error: " + uniform->getName());

    return (location != -1);
}

void CShaderProgramGL::applyUniformInt(s32 location, s32 value)
{
    if (location > -1)
    {
        glUniform1i(location, value);
    }
}

void CShaderProgramGL::applyUniformFloat(s32 location, f32 value)
{
    if (location > -1)
    {
        glUniform1f(location, value);
    }
}

void CShaderProgramGL::applyUniformVector2(s32 location, const core::Vector2D& vector)
{
    if (location > -1)
    {
        glUniform2fv(location, 1, &vector.x);
    }
}
void CShaderProgramGL::applyUniformVector3(s32 location, const core::Vector3D& vector)
{
    if (location > -1)
    {
        glUniform3fv(location, 1, &vector.x);
    }
}

void CShaderProgramGL::applyUniformVector4(s32 location, const core::Vector4D& vector)
{
    if (location > -1)
    {
        glUniform4fv(location, 1, &vector.x);
    }
}

void CShaderProgramGL::applyUniformMatrix3(s32 location, const core::Matrix3D& matrix)
{
    if (location > -1)
    {
        glUniformMatrix3fv(location, 1, GL_TRUE, matrix.getPtr());
    }
}

void CShaderProgramGL::applyUniformMatrix4(s32 location, const core::Matrix4D& matrix)
{
    if (location > -1)
    {
        glUniformMatrix4fv(location, 1, GL_TRUE, matrix.getPtr());
    }
}

} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_

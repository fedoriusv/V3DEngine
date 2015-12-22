#include "ShaderProgramGL.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _OPENGL_DRIVER_
#include "renderer/GL/ShaderGL.h"
#include "GL/glew.h"

using namespace v3d;
using namespace core;
using namespace renderer;

u32 CShaderProgramGL::s_currentShader = 0;

CShaderProgramGL::CShaderProgramGL()
{
}

CShaderProgramGL::~CShaderProgramGL()
{
    CShaderProgramGL::destroy();
}

bool CShaderProgramGL::create()
{
    std::vector<u32> shadersId;
    CShaderProgramGL::getShaderIDArray(shadersId);

    bool status = CShaderProgramGL::init(shadersId);
    shadersId.clear();

    return status;
}

void CShaderProgramGL::destroy()
{
    CShaderProgramGL::deleteProgram(m_shaderProgID);
    m_shaderProgID = 0;
}

void CShaderProgramGL::bind()
{
    if (m_enable)
    {
        CShaderProgramGL::useProgram(m_shaderProgID);
        RENDERER->checkForErrors("CShaderProgramGL: Bind ShaderProgram Error");
    }
}

void CShaderProgramGL::unbind()
{
    CShaderProgramGL::useProgram(0);
    RENDERER->checkForErrors("CShaderProgramGL: Unbind ShaderProgram Error");
}

bool CShaderProgramGL::init(const std::vector<u32>& shaders)
{
    GLint originalProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &originalProgram);

    CShaderProgramGL::createProgram(m_shaderProgID);
    RENDERER->checkForErrors("CShaderProgramGL: Create Program error");

    for (u32 i = 0; i < shaders.size(); ++i)
    {
        CShaderProgramGL::attachShader(m_shaderProgID, shaders[i]);
    }
    
    u32 currentUserLayout = 0;
    for (auto& shaderData : m_shaderDataList)
    {
        const AttributeList& attributeList = shaderData.lock()->getAttributeList();
        for (const AttributePair& attribute : attributeList)
        {
            const std::string& name = attribute.second->getName();
            CShaderAttribute::EShaderAttribute type = attribute.second->getType();

            if (type == CShaderAttribute::eAttributeUser)
            {
                u32 index = CShaderAttribute::eAttributeCount + currentUserLayout;
                ++currentUserLayout;
                CShaderProgramGL::bindAttrib(m_shaderProgID, index, name);
            }
            else
            {
                CShaderProgramGL::bindAttrib(m_shaderProgID, type, name);
            }
        }
    }

    if (!CShaderProgramGL::linkProgram(m_shaderProgID))
    {
        CShaderProgramGL::destroy();
        return false;
    }

    if (!CShaderProgramGL::validateProgram(m_shaderProgID))
    {
        CShaderProgramGL::destroy();
        return false;
    }

    RENDERER->checkForErrors("CShaderProgramGL: Link Program error");
    CShaderProgramGL::useProgram(m_shaderProgID);

    s32 samplerCount = 0;
    for (auto& shaderData : m_shaderDataList)
    {
        AttributeList& attributeList = shaderData.lock()->getAttributeList();
        for (AttributeList::iterator attribute = attributeList.begin(), end = attributeList.end(); attribute != end;)
        {
            const std::string& name = (*attribute).second->getName();
            CShaderAttribute::EShaderAttribute type = (*attribute).second->getType();

            s32 id = CShaderProgramGL::getAttrib(m_shaderProgID, name);
            if ((CShaderAttribute::EShaderAttribute)id != type && type != CShaderAttribute::eAttributeUser)
            {
                LOG_ERROR("CShaderProgramGL: Invalid attribute Index for: %s", name.c_str());
            }

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

        UniformList& uniformList = shaderData.lock()->getUniformList();
        for (UniformList::iterator uniform = uniformList.begin(), end = uniformList.end(); uniform != end;)
        {
            const std::string& name = (*uniform).second->getName();
            s32 id = CShaderProgramGL::getUniformLocation(m_shaderProgID, name);

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

        SamplerList& samplerList = shaderData.lock()->getSamplerList();
        for (auto& sampler : samplerList)
        {
            const std::string& name = sampler->getAttribute();
            s32 id = CShaderProgramGL::getUniformLocation(m_shaderProgID, name.c_str());
            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Sampler not found: %s", name.c_str());
            }
            else
            {
                sampler->setID(samplerCount);

                CShaderProgramGL::setUniformInt(id, samplerCount);
                ++samplerCount;
            }
        }

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

    return true;
}

bool CShaderProgramGL::linkProgram(u32 shaderProgram)
{
    ASSERT(glIsProgram(shaderProgram) && "Invalid Index Link Shader program");
    glLinkProgram(shaderProgram);

    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        LOG_ERROR("CShaderProgramGL: Shader program not linked id: %d", shaderProgram);
    }
#ifdef _DEBUG
    GLint length;
    GLint charsWritten;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

    GLchar* buffer = new GLchar[length];
    glGetProgramInfoLog(shaderProgram, length, &charsWritten, buffer);
    if (strlen(buffer) > 0)
    {
        LOG_ERROR("CShaderProgramGL: Shader Program [%d] Link Logs:\n %s", shaderProgram, buffer);
    }
    delete[] buffer;
#endif

    return (linkStatus == GL_TRUE) ? true : false;
}

bool CShaderProgramGL::validateProgram(u32 shaderProgram)
{
    ASSERT(glIsProgram(shaderProgram) && "Invalid Index Validate Shader program");
    glValidateProgram(shaderProgram);

    GLint validateStatus;
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &validateStatus);
    if (!validateStatus)
    {
        LOG_ERROR("CShaderProgramGL: Shader program not validate id: %d", shaderProgram);
    }
#ifdef _DEBUG

   GLint length;
   GLint charsWritten;
   glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

   GLchar* buffer = new GLchar[length];
   glGetProgramInfoLog(shaderProgram, length, &charsWritten, buffer);
   if (strlen(buffer) > 0)
   {
       LOG_ERROR("CShaderProgramGL: Shader Program [%d] Validate Logs:\n %s", shaderProgram, buffer);
   }
   delete[] buffer;
#endif

    return (validateStatus == GL_TRUE) ? true : false;
}

void CShaderProgramGL::createProgram(u32& shaderProgram)
{
    shaderProgram = glCreateProgram();
    ASSERT(glIsProgram(shaderProgram) && "Invalid Index Created Shader program");
}

void CShaderProgramGL::attachShader(u32 shaderProgram, u32 shader)
{
    ASSERT(glIsProgram(shaderProgram) && "Invalid Index Attachment Shader program");
    ASSERT(glIsShader(shader) && "Invalid Index Attached Shader program");
    glAttachShader(shaderProgram, shader);
}

void CShaderProgramGL::detachShader(u32 shaderProgram, u32 shader)
{
    ASSERT(glIsProgram(shaderProgram) && "Invalid Index Detachment Shader program");
    ASSERT(glIsShader(shader) && "Invalid Index detach Shader program");
    glDetachShader(shaderProgram, shader);
}

void CShaderProgramGL::bindAttrib(u32 shaderProgram, u32 type, const std::string& name)
{
    ASSERT(glIsProgram(shaderProgram) && "Invalid Index bindAttrib Shader program");
    glBindAttribLocation(shaderProgram, type, name.c_str());
}

int CShaderProgramGL::getAttrib(u32 shaderProgram, const std::string& name)
{
    ASSERT(glIsProgram(shaderProgram) && "Invalid Index getAttrib Shader program");
    return (s32)glGetAttribLocation(shaderProgram, name.c_str());
}

void CShaderProgramGL::deleteProgram(u32 shaderProgram)
{
    if (shaderProgram > 0)
    {
        ASSERT(glIsProgram(shaderProgram) && "Invalid Index delete Shader program");
        glDeleteProgram(shaderProgram);
    }
}

bool CShaderProgramGL::useProgram(u32 shaderProgram)
{
    if (s_currentShader != shaderProgram)
    {
        ASSERT((glIsProgram(shaderProgram) || shaderProgram == 0) && "Invalid Index bind Shader program");
        glUseProgram(shaderProgram);
        s_currentShader = shaderProgram;

        return true;
    }

    return false;
}

bool CShaderProgramGL::setUniform(const UniformPtr& uniform)
{
    if (m_shaderProgID <= 0)
    {
        return false;
    }

    GLint location = uniform->getID();
    if (location < 0)
    {
        location = CShaderProgramGL::getUniformLocation(m_shaderProgID, uniform->getName());
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
                CShaderProgramGL::setUniformInt(location, val);
            }
                break;

            case EDataType::eTypeFloat:
            {
                GLfloat& val = *(GLfloat*)value;
                CShaderProgramGL::setUniformFloat(location, val);
            }
                break;

            case EDataType::eTypeVector2:
            {
                Vector2D& val = *(Vector2D*)value;
                CShaderProgramGL::setUniformVector2(location, val);
            }
                break;

            case EDataType::eTypeVector3:
            {
                Vector3D& val = *(Vector3D*)value;
                CShaderProgramGL::setUniformVector3(location, val);
            }
                break;

            case EDataType::eTypeVector4:
            {
                Vector4D& val = *(Vector4D*)value;
                CShaderProgramGL::setUniformVector4(location, val);
            }
                break;

            case EDataType::eTypeMatrix3:
            {
                Matrix3D& val = *(Matrix3D*)value;
                CShaderProgramGL::setUniformMatrix3(location, val);
            }
                break;

            case EDataType::eTypeMatrix4:
            {
                Matrix4D& val = *(Matrix4D*)value;
                CShaderProgramGL::setUniformMatrix4(location, val);
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
        LOG_ERROR("CShaderProgramGL: Error Uniform Location: %s . Shader ID : %d", uniform->getName().c_str(), m_shaderProgID);
    }

    RENDERER->checkForErrors("CShaderProgramGL Set Uniform Error: " + uniform->getName());

    return (location != -1);
}

void CShaderProgramGL::setUniformInt(s32 location, s32 value)
{
    if (location > -1)
    {
        glUniform1i(location, value);
    }
}

void CShaderProgramGL::setUniformFloat(s32 location, f32 value)
{
    if (location > -1)
    {
        glUniform1f(location, value);
    }
}

void CShaderProgramGL::setUniformVector2(s32 location, const core::Vector2D& vector)
{
    if (location > -1)
    {
        glUniform2fv(location, 1, &vector.x);
    }
}
void CShaderProgramGL::setUniformVector3(s32 location, const core::Vector3D& vector)
{
    if (location > -1)
    {
        glUniform3fv(location, 1, &vector.x);
    }
}

void CShaderProgramGL::setUniformVector4(s32 location, const core::Vector4D& vector)
{
    if (location > -1)
    {
        glUniform4fv(location, 1, &vector.x);
    }
}

void CShaderProgramGL::setUniformMatrix3(s32 location, const core::Matrix3D& matrix)
{
    if (location > -1)
    {
        glUniformMatrix3fv(location, 1, GL_TRUE, matrix.getPtr());
    }
}

void CShaderProgramGL::setUniformMatrix4(s32 location, const core::Matrix4D& matrix)
{
    if (location > -1)
    {
        glUniformMatrix4fv(location, 1, GL_TRUE, matrix.getPtr());
    }
}

s32 CShaderProgramGL::getUniformLocation(u32 shaderProgram, const std::string& name)
{
    if (shaderProgram > 0)
    {
        GLint location = glGetUniformLocation(shaderProgram, name.data());
        return location;
    }

    return -1;
}

#endif //_OPENGL_DRIVER_

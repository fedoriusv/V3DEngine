#include "ShaderProgramGL.h"

#include "utils/Logger.h"
#include "renderer/GL/ShaderGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
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
    CShaderProgramGL::createProgram(m_shaderProgID);

    for (u32 i = 0; i < shaders.size(); ++i)
    {
        CShaderProgramGL::attachShader(m_shaderProgID, shaders[i]);
    }

    for (auto& shaderData : m_shaderDataList)
    {
        const AttributeList& attributeList = shaderData.lock()->getAttributeList();
        for (auto& attribute : attributeList)
        {
            const std::string& name = attribute->getAttribute();
            CShaderAttribute::EShaderAttribute type = attribute->getType();

            CShaderProgramGL::bindAttrib(m_shaderProgID, type, name);
        }
    }

    glLinkProgram(m_shaderProgID);
    glValidateProgram(m_shaderProgID);

    for (auto& shaderData : m_shaderDataList)
    {
        const AttributeList& attributeList = shaderData.lock()->getAttributeList();
        for (auto& attribute : attributeList)
        {
            const std::string& name = attribute->getAttribute();
            CShaderAttribute::EShaderAttribute type = attribute->getType();

            s32 id = CShaderProgramGL::getAttrib(m_shaderProgID, name);
            if ((CShaderAttribute::EShaderAttribute)id != type)
            {
                LOG_ERROR("CShaderProgramGL: Invalid attribute Index for: %s", name.c_str());
            }
        }

        UniformList& uniformList = shaderData.lock()->getUniformList();
        for (UniformList::iterator uniform = uniformList.begin(), end = uniformList.end(); uniform != end;)
        {
            const std::string& name = (*uniform).second->getAttribute();
            s32 id = CShaderProgramGL::getUniformID(m_shaderProgID, name);

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
            s32 id = CShaderProgramGL::getUniformID(m_shaderProgID, name.c_str());
            if (id < 0)
            {
                LOG_WARNING("CShaderProgramGL: Sampler not found: %s", name.c_str());
            }
            sampler->setID(id);
        }

        std::remove_if(samplerList.begin(), samplerList.end(), [](const SamplerPtr item) -> bool
        {
            return (item->getID() == -1);
        });

    }

    GLint linkStatus;
    glGetProgramiv(m_shaderProgID, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        LOG_ERROR("CShaderProgramGL: Shader program not compiled id: %d", m_shaderProgID);
#ifdef _DEBUG
        GLint length;
        GLint charsWritten;
        glGetProgramiv(m_shaderProgID, GL_INFO_LOG_LENGTH, &length);

        GLchar* buffer = new GLchar[length];
        glGetProgramInfoLog(m_shaderProgID, length, &charsWritten, buffer);
        if (strlen(buffer) > 0)
        {
            LOG_ERROR("CShaderProgramGL: Shader Program Error:\n %s", buffer);
        }
#endif
    }

    RENDERER->checkForErrors("CShaderProgramGL: Init ShaderProgram Error");

    return (linkStatus == GL_TRUE) ? true : false;
}

void CShaderProgramGL::createProgram(u32& shaderProgram)
{
    shaderProgram = glCreateProgram();
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index Created Shader program");
}

void CShaderProgramGL::attachShader(u32 shaderProgram, u32 shader)
{
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index Attachment Shader program");
    ASSERT(glIsShader(shader) || "Invalid Index Attached Shader program");
    glAttachShader(shaderProgram, shader);
}

void CShaderProgramGL::detachShader(u32 shaderProgram, u32 shader)
{
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index Detachment Shader program");
    ASSERT(glIsShader(shader) || "Invalid Index detach Shader program");
    glDetachShader(shaderProgram, shader);
}

void CShaderProgramGL::bindAttrib(u32 shaderProgram, CShaderAttribute::EShaderAttribute type, const std::string& name)
{
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index bindAttrib Shader program");
    glBindAttribLocation(shaderProgram, type, name.c_str());
}

int CShaderProgramGL::getAttrib(u32 shaderProgram, const std::string& name)
{
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index getAttrib Shader program");
    return (s32)glGetAttribLocation(shaderProgram, name.c_str());
}

void CShaderProgramGL::deleteProgram(u32 shaderProgram)
{
    if (shaderProgram > 0)
    {
        ASSERT(glIsProgram(shaderProgram) || "Invalid Index delete Shader program");
        glDeleteProgram(shaderProgram);
    }
}

bool CShaderProgramGL::useProgram(u32 shaderProgram)
{
    if (shaderProgram != 0)
    {
        ASSERT(glIsProgram(shaderProgram) || "Invalid Index bind Shader program");
    }

    if (s_currentShader != shaderProgram)
    {
        glUseProgram(shaderProgram);
        s_currentShader = shaderProgram;

        return true;
    }

    return false;
}

bool CShaderProgramGL::setUniform(EDataType type, const u32 shader, const std::string& attribute, void* value)
{
    GLint location = CShaderProgramGL::getUniformID(shader, attribute);

    if (location > -1)
    {
        switch (type)
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
                core::Vector2D& val = *(core::Vector2D*)value;
                CShaderProgramGL::setUniformVector2(location, val);
            }
            break;

            case EDataType::eTypeVector3:
            {
                core::Vector3D& val = *(core::Vector3D*)value;
                CShaderProgramGL::setUniformVector3(location, val);
            }
            break;

            case EDataType::eTypeVector4:
            {
                core::Vector4D& val = *(core::Vector4D*)value;
                CShaderProgramGL::setUniformVector4(location, val);
            }
            break;

            case EDataType::eTypeMatrix3:
            {
                core::Matrix3D& val = *(core::Matrix3D*)value;
                CShaderProgramGL::setUniformMatrix3(location, val);
            }
            break;

            case EDataType::eTypeMatrix4:
            {
                core::Matrix4D& val = *(core::Matrix4D*)value;
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
        LOG_ERROR("CShaderProgramGL: Error Uniform Location: %s . Shader ID : %d", attribute.data(), shader);
    }

    RENDERER->checkForErrors("CShaderProgramGL Set Uniform Error: " + attribute);

    return (location != -1);
}

void CShaderProgramGL::setUniformInt(const s32 location, const s32 value)
{
    if (location > -1)
    {
        glUniform1i(location, value);
    }
}

void CShaderProgramGL::setUniformFloat(const s32 location, const f32 value)
{
    if (location > -1)
    {
        glUniform1f(location, value);
    }
}

void CShaderProgramGL::setUniformVector2(const s32 location, const core::Vector2D& vector)
{
    if (location > -1)
    {
        glUniform2fv(location, 1, &vector.x);
    }
}
void CShaderProgramGL::setUniformVector3(const s32 location, const core::Vector3D& vector)
{
    if (location > -1)
    {
        glUniform3fv(location, 1, &vector.x);
    }
}

void CShaderProgramGL::setUniformVector4(const s32 location, const core::Vector4D& vector)
{
    if (location > -1)
    {
        glUniform4fv(location, 1, &vector.x);
    }
}

void CShaderProgramGL::setUniformMatrix3(const s32 location, const core::Matrix3D& matrix)
{
    if (location > -1)
    {
        glUniformMatrix3fv(location, 1, GL_TRUE, matrix.getPtr());
    }
}

void CShaderProgramGL::setUniformMatrix4(const s32 location, const core::Matrix4D& matrix)
{
    if (location > -1)
    {
        glUniformMatrix4fv(location, 1, GL_TRUE, matrix.getPtr());
    }
}

s32 CShaderProgramGL::getUniformID(const u32 shader, const std::string& name)
{
    GLint location = glGetUniformLocation(shader, name.data());

    return location;
}

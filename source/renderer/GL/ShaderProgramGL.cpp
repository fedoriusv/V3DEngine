#include "ShaderProgramGL.h"

#include "utils/Logger.h"
#include "renderer/GL/ShaderGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
using namespace v3d::renderer;

CShaderProgramGL::CShaderProgramGL(const ShaderDataPtr& data)
    : CShaderProgram(data)
{
}

CShaderProgramGL::~CShaderProgramGL()
{
}

bool CShaderProgramGL::create()
{
    std::vector<u32> shadersId;
    CShaderProgramGL::getShaderIDArray(shadersId);

    bool status = CShaderProgramGL::initProgram(m_shaderProgID, shadersId);
    shadersId.clear();

    return status;

}

bool CShaderProgramGL::create(const std::string& vertex, const std::string& fragment, u32 arg, ...)
{
    if (vertex.empty() || fragment.empty())
    {
        ASSERT(false && "Empty Shader FileName");
        return false;
    }

    ShaderPtr vshader = std::make_shared<CShaderGL>();
    vshader->create(vertex, CShader::eVertex);
    CShaderProgram::addShader(vshader);

    ShaderPtr fshader = std::make_shared<CShaderGL>();
    fshader->create(fragment, CShader::eFragment);
    CShaderProgram::addShader(fshader);


    va_list argList;
    va_start(argList, arg);
    for (u32 i = 0; i < arg; i += 2)
    {
        char* strName = va_arg(argList, char*);
        int type = va_arg(argList, int);

        ShaderPtr shader = std::make_shared<CShaderGL>();
        shader->create(strName, (CShader::EShaderType)type);

        CShaderProgram::addShader(shader);
    }
    va_end(argList);


    std::vector<u32> shadersId;
    CShaderProgram::getShaderIDArray(shadersId);

    bool status = CShaderProgramGL::initProgram(m_shaderProgID, shadersId);
    shadersId.clear();

    return status;
}

void CShaderProgramGL::destroy()
{
    for (auto shader : m_shaderList)
    {
        CShaderProgramGL::destroyShader(shader);
    }

    CShaderProgramGL::deleteProgram(m_shaderProgID);
}

void CShaderProgramGL::bind()
{
    CShaderProgramGL::useProgram(m_shaderProgID);
}

void CShaderProgramGL::unbind()
{
    CShaderProgramGL::useProgram(0);
}

bool CShaderProgramGL::initProgram(u32& shaderProgram, const std::vector<u32>& shaders)
{
    CShaderProgramGL::createProgram(shaderProgram);

    for (u32 i = 0; i < shaders.size(); ++i)
    {
        CShaderProgramGL::attachShader(shaderProgram, shaders[i]);
    }

    const AttributeList& data = m_shaderData->getAttributeList();
    for (auto attribute : data)
    {
        const std::string& name = attribute.second->getAttributeName();
        CShaderAttribute::EShaderAttribute type = attribute.second->getAttributeType();

        CShaderProgramGL::bindAttrib(shaderProgram, type, name);
    }

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    for (auto attribute : data)
    {
        const std::string& name = attribute.second->getAttributeName();
        CShaderAttribute::EShaderAttribute type = attribute.second->getAttributeType();
        
        s32 id = CShaderProgramGL::getAttrib(shaderProgram, name);
        if ((CShaderAttribute::EShaderAttribute)id != type)
        {
            LOG_ERROR("InitShaderProgram: Invalid attribute Index for: %s", name.c_str());
        }
    }

    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        LOG_ERROR("InitShaderProgram: shader program not compiled id: %d", shaderProgram);
#ifdef _DEBUG
        GLint length;
        GLint charsWritten;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

        GLchar* buffer = new GLchar[length];
        glGetProgramInfoLog(shaderProgram, length, &charsWritten, buffer);
        if (strlen(buffer) > 0)
        {
            LOG_ERROR("Shader Program Error: %s", buffer);
        }
#endif
    }

    RENDERER->checkForErrors("Init ShaderProgram Error");

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

void CShaderProgramGL::useProgram(u32 shaderProgram)
{
    if (shaderProgram != 0)
    {
        ASSERT(glIsProgram(shaderProgram) || "Invalid Index bind Shader program");
    }
    glUseProgram(shaderProgram);

    RENDERER->checkForErrors("Bind ShaderProgram Error");
}

bool CShaderProgramGL::setUniform(CShaderUniform::EDataType type, const u32 shader, const std::string& attribute, void* value)
{
    GLint location = -1;

    switch (type)
    {
        case CShaderUniform::eTypeNone:
            {
                location = -1;
            }
            break;

        case CShaderUniform::eTypeInt:
            {
                GLint val = *(GLint*)value;
                location = setUniformInt(shader, attribute, val);
            }
            break;

        case CShaderUniform::eTypeFloat:
            {
                GLfloat val = *(GLfloat*)value;
                location = setUniformFloat(shader, attribute, val);
            }
            break;

        case CShaderUniform::eTypeVector2:
            {
                core::Vector2D val = *(core::Vector2D*)value;
                location = setUniformVector2(shader, attribute, val);
            }
            break;

        case CShaderUniform::eTypeVector3:
            {
                core::Vector3D val = *(core::Vector3D*)value;
                location = setUniformVector3(shader, attribute, val);
            }
            break;

        case CShaderUniform::eTypeVector4:
            {
                core::Vector4D val = *(core::Vector4D*)value;
                location = setUniformVector4(shader, attribute, val);
            }
            break;

        case CShaderUniform::eTypeMatrix3:
            {
                core::Matrix3D val = *(core::Matrix3D*)value;
                location = setUniformMatrix3(shader, attribute, val);
            }
            break;
        
        case CShaderUniform::eTypeMatrix4:
            {
                core::Matrix4D val = *(core::Matrix4D*)value;
                location = setUniformMatrix4(shader, attribute, val);
            }
            break;
    
        default:
            break;
    }

    if (location == -1)
    {
        LOG_ERROR(" Error Uniform Location: %s . Shader ID : %d", attribute.data(), shader);
    }

    RENDERER->checkForErrors("CShaderProgramGL Set Uniform Error: " + attribute);

    return (location != -1);
}

s32 CShaderProgramGL::setUniformInt(const u32 shader, const std::string& name, const u32 value)
{
    GLint location = glGetUniformLocation(shader, name.data());
    if (location > -1)
    {
        glUniform1i(location, value);
    }

    return location;
}

s32 CShaderProgramGL::setUniformFloat(const u32 shader, const std::string& name, const f32 value)
{
    GLint location = glGetUniformLocation(shader, name.data());
    if (location > -1)
    {
        glUniform1f(location, value);
    }

    return location;
}

s32 CShaderProgramGL::setUniformVector2(const u32 shader, const std::string& name, const core::Vector2D& vector)
{
    GLint location = glGetUniformLocation(shader, name.data());
    if (location > -1)
    {
        glUniform2fv(location, 1, &vector.x);
    }

    return location;
}
s32 CShaderProgramGL::setUniformVector3(const u32 shader, const std::string& name, const core::Vector3D& vector)
{
    GLint location = glGetUniformLocation(shader, name.data());
    if (location > -1)
    {
        glUniform3fv(location, 1, &vector.x);
    }

    return location;
}

s32 CShaderProgramGL::setUniformVector4(const u32 shader, const std::string& name, const core::Vector4D& vector)
{
    GLint location = glGetUniformLocation(shader, name.data());
    if (location > -1)
    {
        glUniform4fv(location, 1, &vector.x);
    }

    return location;
}

s32 CShaderProgramGL::setUniformMatrix3(const u32 shader, const std::string& name, const core::Matrix3D& matrix)
{
    GLint location = glGetUniformLocation(shader, name.data());
    if (location > -1)
    {
        glUniformMatrix3fv(location, 1, GL_TRUE, matrix.getPtr());
    }

    return location;
}

s32 CShaderProgramGL::setUniformMatrix4(const u32 shader, const std::string& name, const core::Matrix4D& matrix)
{
    GLint location = glGetUniformLocation(shader, name.data());
    if (location > -1)
    {
        glUniformMatrix4fv(location, 1, GL_TRUE, matrix.getPtr());
    }

    return location;
}

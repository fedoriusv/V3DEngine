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

bool CShaderProgramGL::create(const std::string& vShader, const std::string& fShader, u32 arg, ...)
{
    if (vShader.empty() || fShader.empty())
    {
        ASSERT(false && "Empty Shader FileName");
        return false;
    }

    ShaderPtr vshader = std::make_shared<CShaderGL>();
    vshader->create(vShader, EShaderType::eVertex);
    CShaderProgram::addShader(vshader);

    ShaderPtr fshader = std::make_shared<CShaderGL>();
    fshader->create(fShader, EShaderType::eFragment);
    CShaderProgram::addShader(fshader);


    va_list argList;
    va_start(argList, arg);
    for (u32 i = 0; i < arg; i += 2)
    {
        char* strName = va_arg(argList, char*);
        int type = va_arg(argList, int);

        ShaderPtr shader = std::make_shared<CShaderGL>();
        shader->create(strName, (EShaderType)type);

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
        EShaderAttribute type = attribute.second->getAttributeType();

        CShaderProgramGL::bindAttrib(shaderProgram, type, name);
    }

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    for (auto attribute : data)
    {
        const std::string& name = attribute.second->getAttributeName();
        EShaderAttribute type = attribute.second->getAttributeType();
        
        s32 id = CShaderProgramGL::getAttrib(shaderProgram, name);
        if ((EShaderAttribute)id != type)
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

void CShaderProgramGL::bindAttrib(u32 shaderProgram, EShaderAttribute type, const std::string& name)
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

bool CShaderProgramGL::setUniform(EShaderDataType type, const u32 shader, const std::string& attribute, void* value)
{
    int location = -1;

    switch (type)
    {
    case EShaderDataType::eTypeNone:
        {
            location = -1;
        }
        break;

        case EShaderDataType::eTypeInt:
        {
            GLint val = *(GLint*)value;
            location = glGetUniformLocation(shader, attribute.data());
            glUniform1i(location, val);
        }
        break;

        case EShaderDataType::eTypeFloat:
        {
            GLfloat val = *(GLfloat*)value;
            location = glGetUniformLocation(shader, attribute.data());
            glUniform1f(location, val);
        }
        break;

        case EShaderDataType::eTypeVector2:
        {
            core::Vector2D val = *(core::Vector2D*)value;
            location = glGetUniformLocation(shader, attribute.data());
            glUniform2fv(location, 1, &val[0]);
        }
        break;

        case EShaderDataType::eTypeVector3:
        {
            core::Vector3D val = *(core::Vector3D*)value;
            location = glGetUniformLocation(shader, attribute.data());
            glUniform3fv(location, 1, &val[0]);
        }
        break;

        case EShaderDataType::eTypeVector4:
        {
            core::Vector4D val = *(core::Vector4D*)value;
            location = glGetUniformLocation(shader, attribute.data());
            glUniform4fv(location, 1, &val[0]);
        }
        break;

        case EShaderDataType::eTypeMatrix3:
        {
            core::Matrix3D val = *(core::Matrix3D*)value;
            location = glGetUniformLocation(shader, attribute.data());
            glUniformMatrix3fv(location, 1, GL_TRUE, val.getPtr());
        }
        break;
    
        case EShaderDataType::eTypeMatrix4:
        {
            core::Matrix4D val = *(core::Matrix4D*)value;
            location = glGetUniformLocation(shader, attribute.data());
            glUniformMatrix4fv(location, 1, GL_TRUE, val.getPtr());
        }
        break;
    
        default:
        break;
    }

    if (location == -1)
    {
        LOG_ERROR(" Error Uniform Location: %s . Shader ID : %d", attribute.data(), shader);
    }

    return (location != -1);
}
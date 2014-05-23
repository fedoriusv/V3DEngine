#include "ShaderProgramGL.h"
#include "utils/Logger.h"
#include "GL/glew.h"

using namespace f3d;
using namespace f3d::renderer;

CShaderProgramGL::CShaderProgramGL()
{
}

CShaderProgramGL::~CShaderProgramGL()
{
}

void CShaderProgramGL::create()
{
}

void CShaderProgramGL::destroy()
{
    CShaderProgramGL::deleteProgram(m_shaderProgID);
}

void CShaderProgramGL::bind()
{

}

bool CShaderProgramGL::initProgram(u32& shaderProgram, std::vector<u32>& shaders)
{
    shaderProgram = glCreateProgram();
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index Created Shader program");

    for (u32 i = 0; i < shaders.size(); ++i)
    {
        CShaderProgramGL::attachShader(shaderProgram, shaders[i]);
    }

    //
    //CShaderProgramGL::bindAttrib(shaderProgram, 0, "");
    //

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    //
    //CShaderProgramGL::getAttrib();
    //

    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        LOG_ERROR("IniiShaderProgram: shader program not compiled id: %s", shaderProgram);
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

    return (linkStatus == GL_TRUE) ? true : false;
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
    return (int)glGetAttribLocation(shaderProgram, name.c_str());
}

void CShaderProgramGL::deleteProgram(u32 shaderProgram)
{
    if (shaderProgram > 0)
    {
        ASSERT(glIsProgram(shaderProgram) || "Invalid Index delete Shader program");
        glDeleteProgram(shaderProgram);
    }
}
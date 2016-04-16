#include "ShaderGL.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _OPENGL_DRIVER_
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{

CShaderGL::CShaderGL()
    : m_shaderID(0U)
{
    LOG_DEBUG("CShaderGL: CShaderGL constructor %x", this);
}

CShaderGL::~CShaderGL()
{
    CShaderGL::destroy();
    ASSERT(m_shaderID == 0, "Shader doesn't deleted");

    LOG_DEBUG("CShaderGL: CShaderGL destructor %x", this);
}

u32 CShaderGL::getShaderID() const
{
    return m_shaderID;
}

bool CShaderGL::create()
{
    if (m_data.empty())
    {
        LOG_ERROR("Shader: Empty Shader Body");
        return false;
    }

    if (m_type == eShaderUnknown)
    {
        LOG_ERROR("Shader: Invalid Shader Type");
        return false;
    }

    m_compileStatus = CShaderGL::initShader(m_shaderID, m_type, m_name, m_data);
#ifndef _DEBUG
    m_data.clear();
#endif //_DEBUG

    if (!m_compileStatus)
    {
        CShaderGL::destroy();
    }

    return m_compileStatus;
}

void CShaderGL::destroy()
{
    if (m_shaderID > 0)
    {
        ASSERT(glIsShader(m_shaderID), "Shader Index Invalid");
        glDeleteShader(m_shaderID);

        m_shaderID = 0;
    }

    m_data.clear();
}

bool CShaderGL::initShader(u32& shader, const EShaderType type, const std::string& name, const std::string& body)
{
    std::function<u32(EShaderType)> createShader = [](EShaderType type) -> u32
    {
        u32 shader = 0;

        switch (type)
        {
        case EShaderType::eVertex:
        {
            shader = glCreateShader(GL_VERTEX_SHADER);
        }
        break;

        case EShaderType::eFragment:
        {
            shader = glCreateShader(GL_FRAGMENT_SHADER);
        }
        break;

        case EShaderType::eGeometry:
        {
            shader = glCreateShader(GL_GEOMETRY_SHADER);
        }
        break;

        case EShaderType::eCompute:
        {
            shader = glCreateShader(GL_COMPUTE_SHADER);
        }
        break;

        default:
            ASSERT(false, "Invalid Type");
            break;
        }

        ASSERT(glIsShader(shader), "Shader Index Invalid");
        return shader;
    };

    shader = createShader(type);
    ASSERT(glIsShader(shader), "Shader Index Invalid");

    GLchar* stringPtr[1];
    stringPtr[0] = (GLchar*)body.c_str();
    glShaderSource(shader, 1, (const GLchar**)stringPtr, NULL);
    RENDERER->checkForErrors("Set Source Shader Error");

    glCompileShader(shader);
    RENDERER->checkForErrors("Compile Shader Error");


    GLint isCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        LOG_ERROR("CShaderGL: shader not compiled id: %d", shader);
    }
#ifdef _DEBUG_GL
    GLint length;
    GLint charsWritten;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length > 0)
    {
        GLchar* buffer = new GLchar[length];

        glGetShaderInfoLog(shader, length, &charsWritten, buffer);
        if (strlen(buffer) > 0)
        {
            std::function<const char*(EShaderType)> strFunc = [](EShaderType type)
            {
                switch (type)
                {
                case EShaderType::eVertex:
                    return "Vertex";
                case EShaderType::eFragment:
                    return "Fragment";
                case EShaderType::eGeometry:
                    return "Geometry";
                case EShaderType::eCompute:
                    return "Compute";
                default:
                    return "Unknown";
                }
                return "Unknown";
            };
            LOG_INFO("CShaderGL: Shader [%s] Name [%s] Compile Logs:\n%s", strFunc(type), name.c_str(), buffer);
        }

        delete[] buffer;
    }
#endif //_DEBUG_GL

    RENDERER->checkForErrors("Init Shader Error");

    return (isCompiled == GL_TRUE) ? true : false;
}

bool CShaderGL::create(const std::string& shader, EShaderType type)
{
    m_data = shader;
    m_type = type;

    return CShaderGL::create();
}

} //namespace v3d
} //namespace renderer

#endif //_OPENGL_DRIVER_

#include "ShaderGL.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _OPENGL_DRIVER_
#include "GL/glew.h"

using namespace v3d;
using namespace renderer;

CShaderGL::CShaderGL()
{
}

CShaderGL::~CShaderGL()
{
    CShaderGL::destroy();
}

bool CShaderGL::create()
{
    if (!m_data)
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
    CShaderGL::clear();

    if (!m_compileStatus)
    {
        CShaderGL::destroy();
    }

    return m_compileStatus;
}

void CShaderGL::destroy()
{
    CShaderGL::deleteShader(m_shaderID);
    m_shaderID = 0;
}

bool CShaderGL::initShader(u32& shader, const EShaderType type, const std::string& name, void* body)
{

    shader = CShaderGL::createShader(type);

    CShaderGL::sourceShader(shader, body);
    CShaderGL::compileShader(shader);

    GLint isCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        LOG_ERROR("CShaderGL: shader not compiled id: %d", shader);
    }
#ifdef _DEBUG
    GLint length;
    GLint charsWritten;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

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
#endif

    RENDERER->checkForErrors("Init Shader Error");

    return (isCompiled == GL_TRUE) ? true : false;
}

u32 CShaderGL::createShader(EShaderType type)
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
        break;
    }

    ASSERT(glIsShader(shader) && "Shader Index Invalid");
    return shader;
}

void CShaderGL::deleteShader(u32 shader)
{
    if (shader > 0)
    {
        ASSERT(glIsShader(shader) && "Shader Index Invalid");
        glDeleteShader(shader);
    }
}

void CShaderGL::sourceShader(u32 shader, void* body)
{
    ASSERT(glIsShader(shader) && "Shader Index Invalid");

    GLchar* stringPtr[1];
    stringPtr[0] = (GLchar*)body;
    glShaderSource(shader, 1, (const GLchar**)stringPtr, NULL);
}

void CShaderGL::compileShader(u32 shader)
{
    ASSERT(glIsShader(shader) && "Shader Index Invalid");
    glCompileShader(shader);
}

bool CShaderGL::create(const std::string& shader, EShaderType type)
{
    c8* data = (c8*)malloc(shader.size() + 1);
    memcpy(data, shader.data(), shader.size());
    data[shader.size()] = '\0';
    m_data = reinterpret_cast<void*>(data);
    m_type = type;

    return CShaderGL::create();
}

#endif //_OPENGL_DRIVER_

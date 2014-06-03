#include "ShaderGL.h"

#include "utils/Logger.h"
#include "GL/glew.h"

using namespace v3d;
using namespace v3d::renderer;

CShaderGL::CShaderGL()
{
}

CShaderGL::~CShaderGL()
{
    CShader::clearShader();
    CShaderGL::deleteShader(m_shaderID);
    m_shaderID = 0;
}

bool CShaderGL::create(const std::string& body, EShaderType type)
{
    if (body.empty())
    {
        LOG_ERROR("Shader: Empty Shader Body");
        return false;
    }

    m_shaderType = type;

    c8* data = (c8*)malloc(body.size() + 1);
    memcpy(data, body.data(), body.size());
    data[body.size()] = '\0';

    m_data = (void*)data;

    m_compileStatus = CShaderGL::initShader(m_shaderID, m_shaderType, m_data);
    CShaderGL::clearShader();

    return m_compileStatus;
}

bool CShaderGL::load(const std::string& file, EShaderType type)
{
    m_data = reinterpret_cast<void*>(readShader(file));
    m_shaderType = type;

    if (!m_data)
    {
        LOG_ERROR("Shader: Empty Shader Body");
        return false;
    }

    m_compileStatus = CShaderGL::initShader(m_shaderID, m_shaderType, m_data);
    CShaderGL::clearShader();

    return m_compileStatus;
}

void CShaderGL::destroy()
{
    CShaderGL::deleteShader(m_shaderID);
    m_shaderID = 0;
}

bool CShaderGL::initShader(u32& shader, const EShaderType type, void* body)
{
    shader = CShaderGL::createShader(type);

    GLchar* stringPtr[1];
    stringPtr[0] = (GLchar*)body;
    glShaderSource(shader, 1, (const GLchar**)stringPtr, NULL);

    glCompileShader(shader);

    GLint isCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        LOG_ERROR("InitShader: shader not compiled id: %d", shader);
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
                    case eTypeVertex:
                        return "Vertex";
                    case eTypeFragment:
                        return "Fragment";
                    case eTypeGeometry:
                        return "Geometry";
                    case eTypeCompute:
                        return "Compute";
                    default:
                        return "Unknown";
                }
                return "Unknown";
            };
            LOG_ERROR("Shader [%s] Error: %s", strFunc(type), buffer);
        }
#endif
    }

    return (isCompiled == GL_TRUE) ? true : false;
}

u32 CShaderGL::createShader(const EShaderType type)
{
    u32 shader = 0;

    switch (type)
    {
        case eTypeVertex:
        {
            shader = glCreateShader(GL_VERTEX_SHADER);
        }
        break;

        case eTypeFragment:
        {
            shader = glCreateShader(GL_FRAGMENT_SHADER);
        }
        break;

        case eTypeGeometry:
        {
            shader = glCreateShader(GL_GEOMETRY_SHADER);
        }
        break;

        case eTypeCompute:
        {
            shader = glCreateShader(GL_COMPUTE_SHADER);
        }
        break;

        default:
        break;

    }

    ASSERT(glIsShader(shader) || "Can not create Shader");
    return shader;
}

void CShaderGL::deleteShader(u32 shader)
{
    if (shader > 0)
    {
        ASSERT(glIsShader(shader) || "Invalid Index Deleted Shader");
        glDeleteShader(shader);
    }
}


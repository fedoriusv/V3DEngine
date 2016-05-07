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
    : m_id(0U)
{
    LOG_DEBUG("CShaderGL: CShaderGL constructor %x", this);
}

CShaderGL::~CShaderGL()
{
    CShaderGL::destroy();
    ASSERT(m_id == 0, "Shader doesn't deleted");

    LOG_DEBUG("CShaderGL: CShaderGL destructor %x", this);
}

u32 CShaderGL::getShaderID() const
{
    return m_id;
}

bool CShaderGL::create(CShaderSource* data)
{
    ASSERT(data, "Shader data is nullptr");
    if (m_data)
    {
        delete m_data;
        m_data = nullptr;
    }

    m_data = data;
    if (m_data->getBody().empty())
    {
        LOG_ERROR("CShaderGL::create: Empty Shader Body");
        return false;
    }

    if (m_data->getType() == EShaderType::eShaderUnknown)
    {
        LOG_ERROR("CShaderGL::create: Invalid Shader Type");
        return false;
    }

    if (!IShader::isFlagPresent(IShader::eCreated))
    {
        ASSERT(!m_id, "Shader id exist");

        m_id = CShaderGL::createShader(m_data->getType());
#ifdef _DEBUG_GL
        ASSERT(glIsShader(m_id), "Shader Index Invalid");
#endif //_DEBUG_GL
        if (!m_id)
        {
            IShader::setFlag(IShader::eInvalid);
            return false;
        }

        IShader::addFlag(IShader::eCreated);
    };

    if (!IShader::isFlagPresent(IShader::eCompiled))
    {
        ASSERT(m_id, "Shader id doesn't exist");

        const std::string& version = DRIVER_CONTEXT->getShaderVersion();

        std::string shaderSource = "#version ";
        shaderSource.append(version);
        shaderSource.append("\n");

        shaderSource.append(m_data->getHeader());
        shaderSource.append("\n");

        shaderSource.append(m_data->getBody());
        shaderSource.append("\n");

        if (!CShaderGL::compileShader(m_id, shaderSource, m_data->getType()))
        {
            IShader::setFlag(IShader::eInvalid);
            return false;
        }

        IShader::addFlag(IShader::eCompiled);
    }

    return true;
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

    case EShaderType::eTessellationControl:
    {
        shader = glCreateShader(GL_TESS_CONTROL_SHADER);
    }
    break;

    case EShaderType::eTessellationEvaluation:
    {
        shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
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

    return shader;
}

bool CShaderGL::compileShader(u32 shader, const std::string& body, EShaderType type)
{
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
                case EShaderType::eTessellationControl:
                    return "TessellationControl";
                case EShaderType::eTessellationEvaluation:
                    return "TessellationEvaluation";
                case EShaderType::eCompute:
                    return "Compute";
                default:
                    return "Unknown";
                }
                return "Unknown";
            };
            LOG_ERROR("CShaderGL: Shader[%s] Id: %d, Name: %s. Compile Logs:\n%s", strFunc(type), m_id, m_data->getName().c_str(), buffer);
        }

        delete[] buffer;
    }
#endif //_DEBUG_GL

    RENDERER->checkForErrors("Init Shader Error");

    return (isCompiled == GL_TRUE) ? true : false;
}

void CShaderGL::destroy()
{
    if (m_id > 0)
    {
#ifdef _DEBUG_GL
        ASSERT(glIsShader(m_id), "Shader Index Invalid");
#endif //_DEBUG_GL
        glDeleteShader(m_id);
        m_id = 0;

        IShader::addFlag(IShader::eDeleted);
    }
}

} //namespace v3d
} //namespace renderer

#endif //_OPENGL_DRIVER_

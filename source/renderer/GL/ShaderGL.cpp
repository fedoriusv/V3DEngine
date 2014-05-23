#include "ShaderGL.h"
#include "utils/Logger.h"
#include "GL/glew.h"

using namespace f3d;
using namespace f3d::renderer;

CShaderGL::CShaderGL()
{
}

CShaderGL::~CShaderGL()
{
}

bool CShaderGL::create(const std::string& body, EShaderType type)
{
    std::memcpy(m_data, body.c_str(), body.size());
    if (!m_data)
    {
        LOG_ERROR("Shader: Empty Shader Body");
        return false;
    }

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
            //shader = glCreateShader(GL_VERTEX_SHADER);
        }
        break;
    }

    GLchar* stringPtr[1];
    stringPtr[0] = (GLchar*)body;
    glShaderSource(shader, 1, (const GLchar**)stringPtr, NULL);

    glCompileShader(shader);

    GLint isCompiled;;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        LOG_ERROR("IniiShader: shader not compiled id: %s", shader);
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
                return type == eTypeVertex ? "Vertex" : "Fragment";
            };
            LOG_ERROR("Shader [%s] Error: %s", strFunc(type), buffer);
        }
#endif
    }

    return (isCompiled == GL_TRUE) ? true : false;
}

void CShaderGL::deleteShader(u32 shader)
{
    if (shader > 0)
    {
        ASSERT(glIsShader(shader) || "Invalid Index Deleted Shader");
        glDeleteShader(shader);
    }
}
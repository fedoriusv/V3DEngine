#include "ShaderGL.h"
#include "GL/glew.h"

using namespace f3d;
using namespace f3d::renderer;

CShaderGL::CShaderGL()
{
}

CShaderGL::~CShaderGL()
{
}

void CShaderGL::create(const std::string& body, EShaderType type)
{
}

void CShaderGL::load(const std::string& file, EShaderType type)
{
}

void CShaderGL::destroy()
{
}

void CShaderGL::initShader(u32& shader, const EShaderType type, void* body)
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

    GLchar* fsStringPtr[1];
    fsStringPtr[0] = (GLchar*)body;
    glShaderSource(shader, 1, (const GLchar**)fsStringPtr, NULL);

    glCompileShader(shader);

    GLint testVal;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &testVal);
    ASSERT(testVal == GL_TRUE || "Invalid Shader Program");
#ifdef _DEBUG
    GLint length;
    GLchar buffer[1024];
    glGetShaderInfoLog(shader, 1024, &length, buffer);
    if (strlen(buffer) > 0)
    {
        std::function<const char*(int)> strFunc = [](int _type)
        {
            return _type == 1 ? "Vertex" : "Fragment";
        };

        //LOG_CONSOLE("Shader Program " << strFunc(_typeShader)
         //   << " id " << _shaderID << " :\n" << (const char*)buffer);
    }
#endif
   // return (testVal == GL_TRUE) ? true : false;
}
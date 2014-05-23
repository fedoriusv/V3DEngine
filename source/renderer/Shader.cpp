#include "Shader.h"

using namespace f3d;
using namespace f3d::renderer;

/////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string k_shaderTypeName[EShaderType::eShaderTypeCount] = {

    "vertex",
    "fragment",
    "geometry"
};

const std::string& getShaderTypeNameByType(EShaderType type)
{
    return k_shaderTypeName[type];
}

EShaderType getShaderTypeByName(const std::string& name)
{
    for (int i = 0; i < eShaderTypeCount; ++i)
    {
        if (k_shaderTypeName[i].compare(name) == 0)
        {
            return (EShaderType)i;
        }
    }

    return EShaderType::eTypeUnknown;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CShader::CShader()
	: CObject()
	, m_shaderID(0)
	, m_shaderType(EShaderType::eTypeUnknown)
	, m_compileStatus(false)
	, m_data(nullptr)
{
	m_type = EObjectType::eTypeShader;
}

CShader::~CShader()
{
    CShader::clearShader();
    CShader::destroy();
}

u32 CShader::getShaderID() const
{
	return m_shaderID;
}

EShaderType CShader::getShaderType() const
{
	return m_shaderType;
}

bool CShader::getCompileStatus() const
{
	return m_compileStatus;
}

char* CShader::readShader(const std::string& file)
{
    FILE* pFile;
    char* content = NULL;

    int count = 0;

    if (!file.empty())
    {
        fopen_s(&pFile, file.c_str(), "rt");

        if (pFile != NULL)
        {
            fseek(pFile, 0, SEEK_END);
            count = ftell(pFile);
            rewind(pFile);

            if (count > 0)
            {
                content = (char *)malloc(sizeof(char)* (count + 1));
                count = fread(content, sizeof(char), count, pFile);
                content[count] = '\0';
            }

            fclose(pFile);
        }
    }

    return content;
}


void CShader::clearShader()
{
    if (m_data != NULL)
    {
        free(m_data);
        m_data = NULL;
    }
}
#include "Shader.h"

using namespace v3d;
using namespace v3d::renderer;

/////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShader::s_shaderTypeName[EShaderType::eCount] = {

    "vertex",
    "fragment",
    "geometry",
    "compute"
};

const std::string& CShader::getShaderTypeNameByType(EShaderType type)
{
    return s_shaderTypeName[type];
}

EShaderType CShader::getShaderTypeByName(const std::string& name)
{
    for (int i = 0; i < eCount; ++i)
    {
        if (s_shaderTypeName[i].compare(name) == 0)
        {
            return (EShaderType)i;
        }
    }

    return EShaderType::eUnknown;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CShader::CShader()
	: CObject()
	, m_shaderID(0)
	, m_shaderType(EShaderType::eUnknown)
	, m_compileStatus(false)
	, m_data(nullptr)
{
	m_type = EObjectType::eTypeShader;
}

CShader::~CShader()
{
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

char* CShader::read(const std::string& file)
{
    FILE* pFile;
    char* content = nullptr;

    int count = 0;

    if (!file.empty())
    {
        fopen_s(&pFile, file.c_str(), "rt");

        if (pFile != nullptr)
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


void CShader::clear()
{
    if (m_data != nullptr)
    {
        free(m_data);
        m_data = nullptr;
    }
}
#include "ShaderProgram.h"
#include "ShaderUniform.h"
#include "utils/Logger.h"

using namespace f3d;
using namespace f3d::renderer;

CShaderProgram::CShaderProgram()
	: CObject()
	, m_shaderProgID(0)
	, m_enable(true)
	, m_isActive(false)
{
	m_type = EObjectType::eTypeShaderProgram;
}

CShaderProgram::~CShaderProgram()
{
    m_shaderList.clear();
}
	
u32 CShaderProgram::getShaderID() const
{
	return m_shaderProgID;
}

bool CShaderProgram::isEnable() const
{
	return m_enable;
}

void CShaderProgram::setEnable( bool enable )
{
	m_enable = enable;
}

void CShaderProgram::addShader(ShaderPtr shaderProgram)
{
	if (shaderProgram)
	{
		m_shaderList.push_back(shaderProgram);
	}
}

void CShaderProgram::destroyShader(ShaderPtr shaderProgram)
{
	if (shaderProgram)
	{
		auto shader = std::find(m_shaderList.begin(), m_shaderList.end(), shaderProgram);
		if (shader == m_shaderList.end())
		{
			//assert "Shader Program not found";
			return;
		}
		//TODO : not final
		m_shaderList.erase(shader);
	}
}

bool CShaderProgram::parse(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse.  Cannot read render shader element");
        return false;
    }

    //uniforms
    tinyxml2::XMLElement*  uniformsElement = root->FirstChildElement("uniforms");
    if (uniformsElement)
    {
        if (!parseUniforms(uniformsElement))
        {
            return false;
        }
    }

    //attributes
    tinyxml2::XMLElement*  attributesElement = root->FirstChildElement("attributes");
    if (attributesElement)
    {
        if (!parseAttributes(attributesElement))
        {
            return false;
        }
    }

    //samplers
    tinyxml2::XMLElement*  samplersElement = root->FirstChildElement("samplers");
    if (samplersElement)
    {
        if (!parseSamplers(samplersElement))
        {
            return false;
        }
    }

    //shaders
    tinyxml2::XMLElement*  shadersElement = root->FirstChildElement("shaders");
    if (shadersElement)
    {
        if (!parseShaders(shadersElement))
        {
            return false;
        }
    }


    return true;
}

bool CShaderProgram::parseUniforms(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml uniforms element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root;
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find uniform name from pass '%s'", m_name);
            return false;
        }

        const std::string varType = varElement->Attribute("type");
        if (varType.empty())
        {
            LOG_ERROR("Cannot find uniform type from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        const std::string varVal = varElement->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("Cannot find uniform val from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        EDefaultUniformData uniformName = getShaderUniformTypeByName(varName);
        EShaderDataType  uniformType = (uniformName == EDefaultUniformData::eUserUniform)
            ? EShaderDataType::eUniformNone : getShaderDataTypeByName(varType);

        m_shaderData->addDefaultUniform(varName, uniformType, uniformName);

        varElement = varElement->NextSiblingElement("uniforms");
    }

    return true;
}

bool CShaderProgram::parseAttributes(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml attributes element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root;
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find uniform name from pass '%s'", m_name);
            return false;
        }

        const std::string varVal = varElement->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("Cannot find uniform val from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        EShaderAttribute attribureName = getShaderAttributeTypeByName(varName);
        m_shaderData->addAttribute(varName, attribureName);

        varElement = varElement->NextSiblingElement("uniforms");
    }

    return true;
}

bool CShaderProgram::parseSamplers(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml samplers element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root;
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find sampler name from pass '%s'", m_name);
            return false;
        }

        m_shaderData->addSampler(varName);

        varElement = varElement->NextSiblingElement("uniforms");
    }

    return true;
}

bool CShaderProgram::parseShaders(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}
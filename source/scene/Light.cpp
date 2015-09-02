#include "Light.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CLight::CLight()
{
    m_nodeType = ENodeType::eLight;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());
}

CLight::~CLight()
{
}

void CLight::setAmbient(const Vector4D& color)
{
    m_data._ambient = color;
}

void CLight::setDiffuse(const Vector4D& color)
{
    m_data._diffuse = color;
}

void CLight::setSpecular(const Vector4D& color)
{
    m_data._specular = color;
}

void CLight::setRadius(f32 radius)
{
    m_data._radius = radius;
}

void CLight::setAttenuation(const Vector3D& attenuation)
{
    m_data._attenuation = attenuation;
}

void CLight::setDirection(const Vector3D& direction)
{
    m_data._direction = direction;
}

const Vector4D& CLight::getAmbient() const
{
    return m_data._ambient;
}

const Vector4D& CLight::getDiffuse() const
{
    return m_data._diffuse;
}

const Vector4D& CLight::getSpecular() const
{
    return m_data._specular;
}

f32 CLight::getRadius() const
{
    return m_data._radius;
}

const Vector3D& CLight::getAttenuation() const
{
    return m_data._attenuation;
}

const Vector3D& CLight::getDirection() const
{
    return m_data._direction;
}

void CLight::update(s32 time)
{
}

void CLight::init()
{
    if (m_initialiazed)
    {
        return;
    }

    m_data._ambient = Vector4D(1.0f);
    m_data._diffuse = Vector4D(1.0f);
    m_data._specular = Vector4D(1.0f);
    m_data._radius = 100.0f;
    m_data._direction = Vector3D(0.0f, 0.0f, -1.0f);
    m_data._attenuation = Vector3D(0.5f, 0.0f, 0.02f);
}

#include "Light.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CLight::CLight()
    : m_needUpdate(true)
#ifdef _DEBUG
    , m_debug(nullptr)
#endif
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
    m_needUpdate = true;
}

void CLight::setDiffuse(const Vector4D& color)
{
    m_data._diffuse = color;
    m_needUpdate = true;
}

void CLight::setSpecular(const Vector4D& color)
{
    m_data._specular = color;
    m_needUpdate = true;
}

void CLight::setRadius(const f32 radius)
{
    m_data._radius = radius;
    m_needUpdate = true;
}

void CLight::setAttenuation(const Vector3D& attenuation)
{
    m_data._attenuation = attenuation;
    m_needUpdate = true;
}

void CLight::setDirection(const Vector3D& direction)
{
    m_data._direction = direction;
    m_needUpdate = true;
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

void CLight::render()
{
#ifdef _DEBUG
    if (RENDERER->isDebugMode())
    {
        m_debug->draw();
    }
#endif
}

void CLight::update(s32 time)
{
}

void CLight::init()
{
    m_data._ambient = Vector4D(1.0f);
    m_data._diffuse = Vector4D(1.0f);
    m_data._specular = Vector4D(1.0f);
    m_data._radius = 100.0f;
    m_data._direction = Vector3D(0.0f, 0.0f, -1.0f);
    m_data._attenuation = Vector3D(0.5f, 0.0f, 0.02f);

#ifdef _DEBUG
    m_debug = RENDERER->makeDebugLight(m_position, m_data);
    m_debug->setDebugFlag(EDebugLightFlag::eLightFlagPosition);
    m_debug->init();
#endif
}

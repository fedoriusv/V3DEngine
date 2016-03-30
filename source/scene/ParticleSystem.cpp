#include "ParticleSystem.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "renderer/Material.h"

using namespace v3d;
using namespace scene;
using namespace core;
using namespace renderer;

CParticleSystem::CParticleSystem(const std::string& texture)
    : m_velocityMin(Vector3D(0.0f))
    , m_velocityMax(Vector3D(0.0f))

    , m_lifeMin(0.0f)
    , m_lifeMax(0.0f)

    , m_amount(0U)
{
    m_nodeType = ENodeType::eParticleSystem;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    CRenderable::setMaterial(new CMaterial());
    CRenderable::getMaterial()->setTexture(0, texture);
}

CParticleSystem::~CParticleSystem()
{
    CRenderable::getGeometry()->free();
}

void CParticleSystem::init()
{
    if (m_initialiazed)
    {
        return;
    }

    const CRenderTechnique* technique = CRenderable::getMaterial()->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CParticalSystem: RenderTechique doesn't exist");
        ASSERT(false, "CParticalSystem: RenderTechique doesn't exist");
        return;
    }

    CRenderable::setGeometry(RENDERER->makeSharedGeometry(technique));
    CRenderable::setRenderJob(std::make_shared<CRenderJob>(CRenderable::getMaterial(), CRenderable::getGeometry(), CNode::getAbsTransform()));


    CParticleSystem::build();
    CRenderable::getGeometry()->setDrawMode(ePoints);

    CRenderable::getGeometry()->init();

   /* technique->getRenderPass(0)->getUserShaderData()->setUniform("fTimePassed", fTimePassed);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenPosition", vGenPosition);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenVelocityMin", vGenVelocityMin);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenVelocityRange", vGenVelocityRange);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenColor", vGenColor);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenGravityVector", vGenGravityVector);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("fGenLifeMin", fGenLifeMin);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("fGenLifeRange", fGenLifeRange);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("fGenSize", fGenSize);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("iNumToGenerate", 0);*/

    m_initialiazed = true;
}

void CParticleSystem::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
    CRenderable::getRenderJob()->setTransform(CNode::getAbsTransform());

    /*fElapsedTime += dt;

    if (fElapsedTime > fNextGenerationTime)
    {
        const CRenderTechnique* technique = CRenderable::getMaterial()->getRenderTechique();
        ASSERT(technique && "RenderTechnique don't exist");

        technique->getRenderPass(0)->getUserShaderData()->setUniform("iNumToGenerate", iNumToGenerate);
        fElapsedTime -= fNextGenerationTime;

        glm::vec3 vRandomSeed = glm::vec3(grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f));
        technique->getRenderPass(0)->getUserShaderData()->setUniform("vRandomSeed", &vRandomSeed);
    }*/
}

void CParticleSystem::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CRenderable::render();
}

void CParticleSystem::setVelocityRange(const core::Vector3D & min, const core::Vector3D & max)
{
    ASSERT(min < max, "CParticalSystem::setVelocityRange: incorrect range");
    m_velocityMin = min;
    m_velocityMax = max;
}

void CParticleSystem::setLifeRange(f32 min, const f32 max)
{
    ASSERT(min < max, "CParticalSystem::setLifeRange: incorrect range");
    m_lifeMin = min;
    m_lifeMax = max;
}

void CParticleSystem::setAmountOfParticles(u32 amount)
{
    m_amount = amount;
}

void CParticleSystem::build()
{
    SVertexData& data = CRenderable::getGeometry()->getData();
    data._vertices =
    {
        { 0.0f, 0.0f, 0.0f }
    };
}

void CParticleSystem::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    const CRenderTechnique* technique = CRenderable::getMaterial()->getRenderTechique();
    ASSERT(technique, "RenderTechique doesn't exist");

    /*technique->getRenderPass(0)->getUserShaderData()->setUniform("fTimePassed", fTimePassed);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenPosition", vGenPosition);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenVelocityMin", vGenVelocityMin);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenVelocityRange", vGenVelocityRange);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenColor", vGenColor);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("vGenGravityVector", vGenGravityVector);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("fGenLifeMin", fGenLifeMin);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("fGenLifeRange", fGenLifeRange);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("fGenSize", fGenSize);
    technique->getRenderPass(0)->getUserShaderData()->setUniform("iNumToGenerate", 0);
*/
    //CParticleSystem::build();
    //CRenderable::getGeometry()->refresh();
}

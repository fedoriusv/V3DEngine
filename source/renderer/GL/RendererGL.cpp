#include "RendererGL.h"
#include "ShaderGL.h"
#include "ShaderProgramGL.h"
#include "GeomertyGL.h"
#include "TextureGL.h"
#include "RenderStateGL.h"
#include "DebugGeometryGL.h"
#include "DebugLightGL.h"

#include "GL/glew.h"

#include "utils/Logger.h"
#include "context/DriverContext.h"

using namespace v3d;
using namespace v3d::renderer;

CRendererGL::CRendererGL(const DriverContextPtr& context)
	: CRenderer(context)
{
	m_viewportSize = m_context->getViewport();
}

CRendererGL::~CRendererGL()
{
}

void CRendererGL::init()
{

    LOG_INFO("OpenGL Render Init");

	reshape(m_viewportSize.width, m_viewportSize.height);
	glClearColor(m_backColor[0], m_backColor[1], m_backColor[2], 0.0f);

    m_context->setVSync(false);

    glEnable(GL_MULTISAMPLE);

    //glDisable(GL_MULTISAMPLE);

    //glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    //glDepthFunc(GL_LEQUAL);
    //glShadeModel(GL_SMOOTH);


    //glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ////


#ifdef _DEBUG
	m_context->checkForErrors("Render Init");
#endif
}

void CRendererGL::preRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backColor[0], m_backColor[1], m_backColor[2], 0.0f);
}

void CRendererGL::postRender()
{
#ifdef _DEBUG
	m_context->checkForErrors();
#endif
	glFlush();
}

void CRendererGL::reshape(u32 width, u32 height)
{
    CRenderer::reshape(width, height);

    glViewport(0, 0, width, height);
}

ShaderPtr CRendererGL::makeSharedShader()
{
    return std::make_shared<CShaderGL>();
}

ShaderProgramPtr CRendererGL::makeSharedProgram(const ShaderDataPtr& data)
{
    return std::make_shared<CShaderProgramGL>(data);
}

GeometryPtr CRendererGL::makeSharedGeometry(const RenderTechniquePtr& technique)
{
    return std::make_shared<CGeometryGL>(technique);
}

TexturePtr CRendererGL::makeSharedTexture()
{
    return std::make_shared<CTextureGL>();
}

RenderStatePtr CRendererGL::makeSharedRenderState()
{
    return std::make_shared<CRenderStateGL>();
}

#ifdef _DEBUG
DebugGeometryPtr CRendererGL::makeDebugDraw(const GeometryPtr& geometry)
{
    return std::make_shared<CDebugGeometryGL>(geometry);
}

DebugLightPtr CRendererGL::makeDebugLight(const Vector3D& position, const scene::SLightData& data)
{
    return std::make_shared<CDebugLightGL>(position, data);
}
#endif

void CRendererGL::updateMaterial(const MaterialPtr& material, const ShaderDataPtr& data)
{
   /* if (!material->m_needUpdate)
    {
        return;
    }

    if (data->isExistUniform(eMaterialAmbient))
    {
        data->setUniformVector4(eMaterialAmbient, material->getAmbientColor());
    }

    if (data->isExistUniform(eMaterialDiffuse))
    {
        data->setUniformVector4(eMaterialDiffuse, material->getDiffuseColor());
    }

    if (data->isExistUniform(eMaterialSpecular))
    {
        data->setUniformVector4(eMaterialSpecular, material->getSpecularColor());
    }

    if (data->isExistUniform(eMaterialEmission))
    {
        data->setUniformVector4(eMaterialEmission, material->getEmissionColor());
    }

    if (data->isExistUniform(eMaterialShininess))
    {
        data->setUniformFloat(eMaterialShininess, material->getShininess());
    }

    material->m_needUpdate = false;*/
}

void CRendererGL::updateLight(const core::Matrix4D& transform, const ShaderDataPtr& data)
{
    if (m_lightList.empty())
    {
        return;
    }

    //std::vector<scene::CLight*> lights = m_lightList;
    //const Vector3D& pos = transform.getTranslation();

    //std::remove_if(lights.begin(), lights.end(), [pos](scene::CLight* light) -> bool
    //{
    //    if (!light->isVisible())
    //    {
    //        return true;
    //    }

    //    f32 distance = (light->getPosition() - pos).length();
    //    if (light->getRadius() < distance)
    //    {
    //        return true;
    //    }

    //    return true;
    //});

    //if (data->isExistUniform(eLightsCount))
    //{
    //    data->setUniformInt(eLightsCount, lights.size());
    //}

    //if (lights.empty())
    //{
    //    return;
    //}

    //s32 index = 0;
    //for (std::vector<scene::CLight*>::iterator light = lights.begin(); light < lights.end(); ++light)
    //{
    //    if (!(*light)->m_needUpdate)
    //    {
    //        //continue;
    //    }

    //    if (data->isExistUniform(eLightPosition))
    //    {
    //        data->setUniformVector4(eLightPosition, Vector4D((*light)->getPosition(), 0.0f), index);
    //    }

    //    if (data->isExistUniform(eLightAmbient))
    //    {
    //        data->setUniformVector4(eLightAmbient, (*light)->getAmbient(), index);
    //    }

    //    if (data->isExistUniform(eLightDiffuse))
    //    {
    //        data->setUniformVector4(eLightDiffuse, (*light)->getDiffuse(), index);
    //    }

    //    if (data->isExistUniform(eLightSpecular))
    //    {
    //        data->setUniformVector4(eLightSpecular, (*light)->getSpecular(), index);
    //    }

    //    if (data->isExistUniform(eLightDirection))
    //    {
    //        data->setUniformVector3(eLightDirection, (*light)->getDirection(), index);
    //    }

    //    if (data->isExistUniform(eLightAttenuation))
    //    {
    //        data->setUniformVector3(eLightAttenuation, (*light)->getAttenuation(), index);
    //    }

    //    if (data->isExistUniform(eLightRadius))
    //    {
    //        data->setUniformFloat(eLightRadius, (*light)->getRadius(), index);
    //    }

    //    (*light)->m_needUpdate = false;
    //}

}

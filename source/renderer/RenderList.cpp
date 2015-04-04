#include "RenderList.h"
#include "scene/Node.h"
#include "scene/Shape.h"
#include "scene/Model.h"
#include "scene/Skybox.h"
#include "scene/Text.h"
#include "scene/Billboard.h"
#include "scene/Camera.h"

using namespace v3d;
using namespace renderer;
using namespace scene;

CRenderList::CRenderList(const RenderTargetPtr& target)
: m_enable(false)
, m_target(target)
, m_camera(nullptr)
{
}

CRenderList::~CRenderList()
{
    m_list.clear();
    m_draw.clear();
}

void CRenderList::setCamera(scene::CCamera* camera)
{
    m_camera = camera;
}

scene::CCamera* CRenderList::getCamera() const
{
    return m_camera;
}

bool CRenderList::isEnable() const
{
    return m_enable;
}

void CRenderList::setEnable(bool enable)
{
    m_enable = enable;
}

void CRenderList::add(scene::CNode* node, u32 target)
{
    if (node)
    {
        m_list.push_back(SNodeList(node, target));
    }
}

void CRenderList::clear()
{
    m_list.clear();
}

const RenderTargetPtr& CRenderList::getRenderTarget() const
{
    return m_target;
}

const std::string& CRenderList::getTargetName() const
{
    return m_target->getName();
}

void CRenderList::update(u32 delta)
{
    if (m_camera && m_camera->isActive())
    {
        m_camera->update(delta);
    }

    for (std::vector<SNodeList>::const_iterator iter = m_draw.begin(); iter < m_draw.end(); ++iter)
    {
        CNode* item = (*iter)._node;
        item->update(delta);
    }
}

void CRenderList::render()
{

    if (m_camera && m_camera->isActive())
    {
        m_camera->render();
    }

    for (std::vector<SNodeList>::const_iterator iter = m_draw.begin(); iter < m_draw.end(); ++iter)
    {
        CNode* item = (*iter)._node;
        item->render();
    }
}


void CRenderList::refresh()
{
    m_draw.clear();

    for (std::vector<SNodeList>::iterator iter = m_list.begin(); iter < m_list.end(); ++iter)
    {
        CNode* node = (*iter)._node;
        switch (node->getNodeType())
        {
        case ENodeType::eShape:
        //case ENodeType::eModel:
        {
            f32 priority = 0.0f;
            CShape* shape = static_cast<CShape*>(node);
            if (shape->getMaterial()->getTransparency() > 0.0f)
            {
                if (m_camera)
                {
                    priority = (node->getPosition() - m_camera->getPosition()).length();
                }
                else
                {
                    priority = node->getPosition().z;
                }
                node->m_priority = priority;

                if (checkDistance(node, priority))
                {
                    const RenderJobPtr& job = shape->getRenderJob();
                    job->setRenderTarget((*iter)._targetIndex);

                    m_draw.push_back((*iter));
                }
            }
        }
            break;

        case ENodeType::eBillboard:
        {
            f32 priority = 0.0f;
            CBillboard* billboard = static_cast<CBillboard*>(node);
            if (billboard->getMaterial()->getTransparency() > 0.0f)
            {
                if (m_camera)
                {
                    priority = (node->getPosition() - m_camera->getPosition()).length();
                }
                else
                {
                    priority = node->getPosition().z;
                }
                node->m_priority = priority;

                if (checkDistance(node, priority))
                {
                    const RenderJobPtr& job = billboard->getRenderJob();
                    job->setRenderTarget((*iter)._targetIndex);

                    m_draw.push_back((*iter));
                }
            }
        }
            break;

        case ENodeType::eCamera:
        {
            /*node->m_priority = k_maxPriority;
            if (static_cast<CCamera*>(node)->isActive())
            {
                m_draw.push_back(node);
            }*/
        }
            break;

        case ENodeType::eSkyBox:
        {
            node->m_priority = k_maxPriority;

            CSkybox* skybox = static_cast<CSkybox*>(node);
            const RenderJobPtr& job = skybox->getRenderJob();
            job->setRenderTarget((*iter)._targetIndex);

            m_draw.push_back((*iter));
        }
            break;

        case ENodeType::eLight:
        case ENodeType::eFog:
        {
            node->m_priority = -k_maxPriority;
        }
            break;

        case ENodeType::eText:
        {
            node->m_priority = 0.0f;

            CText* text = static_cast<CText*>(node);
            const RenderJobPtr& job = text->getRenderJob();
            job->setRenderTarget((*iter)._targetIndex);

            m_draw.push_back((*iter));
        }
            break;

        default:
            break;
        }
    }

    std::sort(m_draw.begin(), m_draw.end(), [](const SNodeList& node0, const SNodeList& node1) -> bool
    {
        return  (node0._node->getPriority() > node1._node->getPriority());
    });
}

bool CRenderList::checkDistance(const CNode* node, const f32 distance)
{
    if (node->getNodeType() == ENodeType::eShape || node->getNodeType() == ENodeType::eModel)
    {
        const RenderTechniquePtr& technique = static_cast<const CShape*>(node)->getMaterial()->getRenderTechique();
        for (u32 pass = 0; pass < technique->getRenderPassCount(); ++pass)
        {
            const RenderLODPtr& lod = technique->getRenderPass(pass)->getRenderLOD();
            if (lod->getGeometryDistance() < distance && lod->getGeometryDistance() > 0)
            {
                return false;
            }
        }
    }

    return true;
}
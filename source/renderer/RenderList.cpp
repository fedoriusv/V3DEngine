#include "RenderList.h"
#include "scene/Node.h"
#include "scene/Shape.h"
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

void CRenderList::add(scene::CNode* node)
{
    if (node)
    {
        m_list.push_back(node);
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

    for (std::vector<CNode*>::const_iterator iter = m_draw.begin(); iter < m_draw.end(); ++iter)
    {
        CNode* item = (*iter);
        item->update(delta);
    }
}

void CRenderList::render()
{

    if (m_camera && m_camera->isActive())
    {
        m_camera->render();
    }

    for (std::vector<CNode*>::const_iterator iter = m_draw.begin(); iter < m_draw.end(); ++iter)
    {
        CNode* item = (*iter);
        item->render();
    }
}


void CRenderList::refresh()
{
    m_draw.clear();

    for (std::vector<CNode*>::iterator iter = m_list.begin(); iter < m_list.end(); ++iter)
    {
        CNode* node = (*iter);
        switch (node->getNodeType())
        {
        case ENodeType::eShape:
        case ENodeType::eModel:
        {
            f32 priority = 0.0f;
            if (static_cast<CShape*>(node)->getMaterial()->getTransparency() > 0.0f)
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
                    m_draw.push_back(node);
                }
            }
        }
            break;

        //case ENodeType::eCamera:
        //{
        //    node->m_priority = k_maxPriority;
        //    if (static_cast<CCamera*>(node)->isActive())
        //    {
        //        m_draw.push_back(node);
        //    }
        //}
        //    break;

        case ENodeType::eSkyBox:
        {
            node->m_priority = k_maxPriority;
            m_draw.push_back(node);
        }
            break;

        case ENodeType::eLight:
        case ENodeType::eFog:
        {
            node->m_priority = -k_maxPriority;
        }
            break;

        case ENodeType::eFont:
        {
            node->m_priority = 0.0f;
            m_draw.push_back(node);
        }
            break;

        default:
            break;
        }
    }

    std::sort(m_draw.begin(), m_draw.end(), [](CNode* node0, CNode* node1) -> bool
    {
        return  (node0->getPriority() > node1->getPriority());
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
#include "Scene.h"
#include "utils/Logger.h"
#include "Engine.h"
#include "Node.h"
#include "Shape.h"
#include "Camera.h"
#include "renderer/Renderer.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CScene::CScene()
: m_camera(nullptr)
{
}

CScene::~CScene()
{
    CScene::clear();

    m_drawObjects.clear();
    m_renderList.clear();
}

void CScene::setActiveCamera(CCamera* camera)
{
    if (m_camera)
    {
        m_camera->m_active = false;
    }
    m_camera = camera;
    m_camera->m_active = true;
}

CCamera* CScene::getActiveCamera() const
{
    return m_camera;
}

bool CScene::isActiveCamera(const CCamera* camera)
{
    return m_camera == camera;
}

void CScene::init()
{
    for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        CNode* item = (*iter);
        item->init();
    }

    LOG_INFO("Scene: Init completed");
}

void CScene::update(f64 time)
{
    CScene::updateNodes(time);

    for (std::vector<CNode*>::iterator iter = m_drawObjects.begin(); iter < m_drawObjects.end(); ++iter)
    {
        CNode* item = (*iter);
        item->update(time);
    }
}

void CScene::renderer()
{
    RENDERER->preRender();

    for (std::vector<CNode*>::iterator iter = m_drawObjects.begin(); iter < m_drawObjects.end(); ++iter)
    {
        CNode* item = (*iter);
        item->render();
    }

    RENDERER->postRender();
}

void CScene::add(CNode* node)
{
    if (node)
    {
        m_objects.push_back(node);
    }
}

bool CScene::drop(CNode* node)
{
    if (!node)
    {
        return false;
    }

    std::vector<CNode*>::iterator iter = std::find(m_objects.begin(), m_objects.end(), node);

    if (iter != m_objects.end())
    {
        delete (*iter);
        (*iter) = nullptr;
        m_objects.erase(iter);

        return true;
    }

    return false;
}

void CScene::clear()
{
    for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        delete (*iter);
        (*iter) = nullptr;
    }

    m_objects.clear();
}

void CScene::updateNodes(u32 delta)
{
    m_drawObjects.clear();

    for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        CNode* node = (*iter);
        switch (node->getNodeType())
        {
            case ENodeType::eShape:
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
                        m_drawObjects.push_back(node);
                    }
                }
            }
                break;

            case ENodeType::eModel:
                //TODO:
                break;

            case ENodeType::eCamera:
            {
                node->m_priority = k_maxPriority;

                if (static_cast<CCamera*>(node)->isActive())
                {
                    m_drawObjects.push_back(node);
                }
            }
                break;

            case ENodeType::eSkyBox:
            {
                node->m_priority = k_maxPriority;

                m_drawObjects.push_back(node);
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

                m_drawObjects.push_back(node);
            }
                break;

            default:
                break;
        }
    }

    std::sort(m_drawObjects.begin(), m_drawObjects.end(), [](CNode* node0, CNode* node1)
    {
        return  (node0->getPriority() > node1->getPriority());
    });
}

bool CScene::checkDistance(const CNode* node, const f32 distance)
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

CNode* CScene::getNodeByID(s32 id)
{
    for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        if ((*iter)->getID() == id)
        {
            return (*iter);
        }
    }

    return nullptr;
}

CNode* CScene::getNodeByName(const std::string& name)
{
    for (std::vector<CNode*>::const_iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        if ((*iter)->getName().compare(name) == 0)
        {
            return (*iter);
        }
    }

    return nullptr;
}
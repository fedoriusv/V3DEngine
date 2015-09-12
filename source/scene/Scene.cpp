#include "Scene.h"
#include "utils/Logger.h"
#include "Engine.h"
#include "Node.h"
#include "Shape.h"
#include "Billboard.h"
#include "Mesh.h"
#include "Text.h"
#include "Skybox.h"
#include "Camera.h"
#include "Model.h"
#include "renderer/Renderer.h"
#include "RenderTargetManager.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CScene::CScene()
: m_camera(nullptr)
, m_refresh(false)
{
}

CScene::~CScene()
{
    CScene::clear();
    m_renderList.clear();
}

void CScene::setActiveCamera(CCamera* camera)
{
    if (m_camera)
    {
        m_camera->setActive(false);
    }
    m_camera = camera;
    m_camera->setActive(true);
}

CCamera* CScene::getActiveCamera() const
{
    return m_camera;
}

bool CScene::isActiveCamera(const CCamera* camera)
{
    if (!m_camera)
    {
        return false;
    }

    return m_camera == camera;
}

void CScene::init()
{
    for (NodeList::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        CNode* item = (*iter);
        item->init();
    }

    LOG_INFO("Scene: Init completed");
}

void CScene::draw(s32 dt)
{
    CScene::updateRenderLists(dt);

    RENDERER->preRender();

    for (std::vector<CRenderList>::iterator iter = m_renderList.begin(); iter < m_renderList.end(); ++iter)
    {
        if ((*iter).isEnable())
        {
            CRenderList& list = (*iter);
            list.refresh();

            //TODO: need rework render camera
            if (!CScene::isActiveCamera(list.getCamera()))
            {
                list.setCamera(m_camera);
            }

            list.update(dt);
            list.render();
        }
    }

    RENDERER->postRender();
}

void CScene::add(CNode* node)
{
    if (node)
    {
        m_objects.push_back(node);
        CScene::needRefresh();
    }
}

bool CScene::drop(CNode* node)
{
    if (!node)
    {
        return false;
    }

    NodeList::iterator iter = std::find(m_objects.begin(), m_objects.end(), node);

    if (iter != m_objects.end())
    {
        delete (*iter);
        (*iter) = nullptr;

        m_objects.erase(iter);
        CScene::needRefresh();

        return true;
    }

    return false;
}

void CScene::clear()
{
    for (NodeList::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        delete (*iter);
        (*iter) = nullptr;
    }
    m_objects.clear();

    CScene::needRefresh();
}

CNode* CScene::getNodeByID(s32 id)
{
    auto predCheckId = [id](const CNode* node) -> bool
    {
        return id == node->getID();
    };

    auto iter = std::find_if(m_objects.begin(), m_objects.end(), predCheckId);
    if (iter != m_objects.end())
    {
        return (*iter);
    }

    return nullptr;
}

CNode* CScene::getNodeByName(const std::string& name)
{
    auto predCheckName = [&name](const CNode* node) -> bool
    {
        return name == node->getName();
    };

    auto iter = std::find_if(m_objects.begin(), m_objects.end(), predCheckName);
    if (iter != m_objects.end())
    {
        return (*iter);
    }

    return nullptr;
}

void CScene::initRenderLists()
{
    m_renderList.clear();

    for (NodeList::const_iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        CNode* node = (*iter);
        switch (node->getNodeType())
        {
            case ENodeType::eShape:
            case ENodeType::eMesh:
            case ENodeType::eSkyBox:
            case ENodeType::eText:
            case ENodeType::eBillboard:
            {
                CScene::attachToRenderList(node);
            }
                break;

            case ENodeType::eModel:
            {
                const CModel* model = static_cast<CModel*>(node);
                for (CNode* subNode : model->getNodeList())
                {
                    switch (subNode->getNodeType())
                    {
                      case ENodeType::eMesh:
                          CScene::attachToRenderList(subNode);
                          break;

                      case ENodeType::eCamera:
                      case ENodeType::eLight:
                      default:
                          break;
                    }
                }
            }
                break;

            case ENodeType::eCamera:
            case ENodeType::eLight:
            case ENodeType::eFog:
            {
                //TODO:
            }
                break;

            default:
                break;
        }
    }
}

void CScene::attachToRenderList(CNode* node)
{
    const CRenderTechnique* techniqe = nullptr;
    CRenderable* draw = nullptr;

    switch (node->getNodeType())
    {
    case ENodeType::eShape:
        {
            draw = static_cast<CShape*>(node);
            techniqe = draw->getMaterial()->getRenderTechique();
        }
            break;

    case ENodeType::eMesh:
        {
            draw = static_cast<CMesh*>(node);
            techniqe = draw->getMaterial()->getRenderTechique();
        }
            break;

        case ENodeType::eSkyBox:
            {
                draw = static_cast<CSkybox*>(node);
                techniqe = draw->getMaterial()->getRenderTechique();
            }
            break;

        case ENodeType::eText:
            {
                draw = static_cast<CText*>(node);
                techniqe = draw->getMaterial()->getRenderTechique();
            }
            break;

        case ENodeType::eBillboard:
            {
                draw = static_cast<CBillboard*>(node);
                techniqe = draw->getMaterial()->getRenderTechique();
            }
            break;

        case ENodeType::eModel:
        default:
            break;
    };

    if (!techniqe || !draw)
    {
        return;
    }

    for (u32 i = 0; i < techniqe->getRenderPassCount(); ++i)
    {
        const RenderPassPtr& pass = techniqe->getRenderPass(i);

        for (u32 targetIndex = 0; targetIndex < pass->getRenderTargetCount(); ++targetIndex)
        {
            const RenderTargetPtr& target = pass->getRenderTarget(targetIndex);
            auto findPred = [target](const CRenderList& list) -> bool
            {
                if (list.getTargetName() == target->getName())
                {
                    return true;
                }

                return false;
            };

            std::vector<CRenderList>::iterator findTarget = std::find_if(m_renderList.begin(), m_renderList.end(), findPred);
            if (findTarget != m_renderList.end())
            {
                (*findTarget).add(node, draw, targetIndex);
            }
            else
            {
                CRenderList list(target);
                list.setEnable(true);
                list.add(node, draw, targetIndex);

                m_renderList.push_back(list);
            }
        }
    }
}

void CScene::updateRenderLists(s32 dt)
{
    if (m_refresh)
    {
        CScene::initRenderLists();
        m_refresh = false;
    }
}

void CScene::needRefresh()
{
    m_refresh = true;
}
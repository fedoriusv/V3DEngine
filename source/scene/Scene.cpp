#include "Scene.h"
#include "utils/Logger.h"
#include "Engine.h"
#include "Node.h"
#include "Shape.h"
#include "Camera.h"
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
    if (!m_camera)
    {
        return false;
    }

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

void CScene::draw(u32 delta)
{
    CScene::updateRenderLists(delta);

    RENDERER->preRender();

    for (std::vector<CRenderList>::iterator iter = m_renderList.begin(); iter < m_renderList.end(); ++iter)
    {
        if ((*iter).isEnable())
        {
            CRenderList& list = (*iter);
            list.refresh();

            if (!CScene::isActiveCamera(list.getCamera()))
            {
                list.setCamera(m_camera);
            }

            list.update(delta);
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

    std::vector<CNode*>::iterator iter = std::find(m_objects.begin(), m_objects.end(), node);

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
    for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        delete (*iter);
        (*iter) = nullptr;
    }
    m_objects.clear();

    CScene::needRefresh();
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

void CScene::initRenderLists()
{
    m_renderList.clear();

    for (std::vector<CNode*>::const_iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        CNode* node = (*iter);
        switch (node->getNodeType())
        {
            case ENodeType::eShape:
            case ENodeType::eModel:
            case ENodeType::eSkyBox:
            case ENodeType::eText:
            {
                const RenderTechniquePtr& techniqe = static_cast<CShape*>(node)->getMaterial()->getRenderTechique();
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
                            (*findTarget).add(node, targetIndex);
                        }
                        else
                        {
                            CRenderList list(target);
                            list.setEnable(true);
                            list.add(node, targetIndex);

                            m_renderList.push_back(list);
                        }
                    }
                }
            }
                break;

            case ENodeType::eCamera:
            {
                //TODO:
            }
                break;

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

void CScene::updateRenderLists(u32 delta)
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
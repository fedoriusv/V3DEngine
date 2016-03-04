#include "RenderList.h"
#include "Engine.h"
#include "Material.h"
#include "scene/Node.h"
#include "scene/Shape.h"
#include "scene/Mesh.h"
#include "scene/Skybox.h"
#include "scene/Text.h"
#include "scene/Billboard.h"
#include "scene/Camera.h"
#include "scene/Model.h"

namespace v3d
{
namespace renderer
{

using namespace scene;

CRenderList::SNodeList::SNodeList(scene::CNode* node, CRenderable* draw, u32 target, u32 pass)
    : _node(node)
    , _draw(draw)
    , _targetIndex(target)
    , _passIndex(pass)
{
}

CRenderList::CRenderList(const TargetPtr& target)
    : m_enable(false)
    , m_target(target)
    , m_camera(nullptr)
{
}

CRenderList::~CRenderList()
{
    CRenderList::clear();
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

void CRenderList::add(CNode* node, CRenderable* draw, u32 target, u32 pass)
{
    if (node && draw)
    {
        m_list.push_back(SNodeList(node, draw, target, pass));
    }
}

void CRenderList::clear()
{
    m_list.clear();
    m_draw.clear();

    m_drawStatic.clear();
    m_drawAlpha.clear();
    m_drawTransparency.clear();
}

bool CRenderList::contain(scene::CNode* node)
{
    auto findPred = [node](const SNodeList& list) -> bool
    {
        if (list._node == node)
        {
            return true;
        }

        return false;
    };

    std::vector<SNodeList>::const_iterator iter = std::find_if(m_list.cbegin(), m_list.cend(), findPred);
     if (iter != m_list.cend())
     {
         return true;
     }

     return false;
}

const TargetPtr CRenderList::getTarget() const
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
        if (!item)
        {
            continue;
        }

        item->update(delta);
    }
}

void CRenderList::render()
{
    m_target->bind();

    for (std::vector<SNodeList>::const_iterator iter = m_draw.begin(); iter < m_draw.end(); ++iter)
    {
        CRenderable* item = (*iter)._draw;
        if (!item)
        {
            continue;
        }

        ASSERT(item->getRenderJob(), "Invalid Render job");
        item->getRenderJob()->setRenderTarget((*iter)._targetIndex);

        item->render();
    }

    m_target->unbind();
}


void CRenderList::refresh()
{
    m_draw.clear();

    m_drawAlpha.clear();
    m_drawStatic.clear();
    m_drawTransparency.clear();

    u32 size = (u32)m_list.size();
    m_drawAlpha.reserve(size);
    m_drawStatic.reserve(size);
    m_drawTransparency.reserve(size);

    for (std::vector<SNodeList>::iterator iter = m_list.begin(); iter < m_list.end(); ++iter)
    {
        CNode* node = (*iter)._node;
        switch (node->getNodeType())
        {
            case ENodeType::eShape:
            case ENodeType::eMesh:
            case ENodeType::eBillboard:
            case ENodeType::eParticleSystem:
            {
                f32 priority = 0.0f;
                CMesh* mesh = static_cast<CMesh*>(node);
                const MaterialPtr& material = mesh->getMaterial();
                if (material->getTransparency() < 1.0f)
                {
                    if (m_camera)
                    {
                        priority = (node->getAbsPosition() - m_camera->getAbsPosition()).length();
                    }
                    else
                    {
                        priority = node->getAbsPosition().z;
                    }
                    node->setPriority(priority);

                    if (checkDistance(node, priority))
                    {
                        m_drawTransparency.push_back((*iter));
                    }
                }
                else if (material->getRenderTechique()->getRenderPass((*iter)._passIndex)->getRenderState()->isBlend())
                {
                    if (m_camera)
                    {
                        priority = (node->getAbsPosition() - m_camera->getAbsPosition()).length();
                    }
                    else
                    {
                        priority = node->getAbsPosition().z;
                    }
                    node->setPriority(priority);

                    if (checkDistance(node, priority))
                    {
                        m_drawAlpha.push_back((*iter));
                    }
                }
                else
                {
                    if (checkDistance(node, priority))
                    {
                        m_drawStatic.push_back((*iter));
                    }
                }
            }
                break;

            case ENodeType::eCamera:
            {
                /*node->setPriority(k_maxPriority);
                if (static_cast<CCamera*>(node)->isActive())
                {
                    m_update.push_back(node);
                }*/
            }
                break;

            case ENodeType::eSkyBox:
            {
                node->setPriority(k_maxPriority);
                m_drawStatic.push_back((*iter));
            }
                break;

            case ENodeType::eLight:
            case ENodeType::eFog:
            {
                node->setPriority(-k_maxPriority);
            }
                break;

            case ENodeType::eText:
            {
                node->setPriority(0.0f);

                CText* text = static_cast<CText*>(node);
                const MaterialPtr& material = text->getMaterial();
                if (material->getTransparency() < 1.0f)
                {
                    m_drawTransparency.push_back((*iter));
                }
                else if (material->getRenderTechique()->getRenderPass((*iter)._passIndex)->getRenderState()->isBlend())
                {
                    m_drawAlpha.push_back((*iter));
                }
                else
                {
                    m_drawStatic.push_back((*iter));
                }
            }
                break;

            case ENodeType::eModel:
            default:
                break;
        }
    }

    m_drawStatic.shrink_to_fit();
    m_draw.insert(m_draw.begin(), m_drawStatic.begin(), m_drawStatic.end());

    if (!m_drawTransparency.empty())
    {
        m_drawTransparency.shrink_to_fit();
        std::sort(m_drawTransparency.begin(), m_drawTransparency.end(), [](const SNodeList& node0, const SNodeList& node1) -> bool
        {
            return  (node0._node->getPriority() > node1._node->getPriority());
        });
        m_drawAlpha.insert(m_drawAlpha.end(), m_drawTransparency.begin(), m_drawTransparency.end());
    }

    if (!m_drawAlpha.empty())
    {
        m_drawAlpha.shrink_to_fit();
        std::sort(m_drawAlpha.begin(), m_drawAlpha.end(), [](const SNodeList& node0, const SNodeList& node1) -> bool
        {
            return  (node0._node->getPriority() > node1._node->getPriority());
        });
        m_draw.insert(m_draw.end(), m_drawAlpha.begin(), m_drawAlpha.end());
    }
}

bool CRenderList::checkDistance(const CNode* node, const f32 distance)
{
    if (node->getNodeType() == ENodeType::eShape || node->getNodeType() == ENodeType::eMesh)
    {
        const CRenderTechnique* technique = static_cast<const CShape*>(node)->getMaterial()->getRenderTechique();
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

} //namespace renderer
} //namespace v3d

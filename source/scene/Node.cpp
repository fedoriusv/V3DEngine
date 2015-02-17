#include "Node.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::scene;

std::string  CNode::s_nodeTypes[ENodeType::eCount] = {

    "Shape",
    "Model",
    "Camera",
    "Light",
    "Fog",
    "SkyBox",
    "Font",
};

const std::string& CNode::getNodeNameByType(ENodeType type)
{
    return s_nodeTypes[type];
}

CNode::CNode()
    : m_parentNode(nullptr)
    , m_visible(true)
    , m_needUpdate(true)
    , m_nodeType(ENodeType::eUnknown)
    , m_priority(0.0f)
    , m_position(core::Vector3D(0.0f))
    , m_rotation(core::Vector3D(0.0f))
    , m_scale(core::Vector3D(1.0f))
    , m_initialiazed(false)
{
    m_type = EObjectType::eTypeNode;
}

CNode::~CNode()
{
    if (m_parentNode)
    {
        m_parentNode->dettachChild(this);
    }

    for (CNode* node : m_childNodes)
    {
        node->setParent(nullptr);
    }

    m_childNodes.clear();
}

void CNode::setPosition(const core::Vector3D& position)
{
    m_position = position;
    bool hasTranslate = (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f);
    CNode::updateTransform(hasTranslate ? ENodeTransform::eTranslation : ENodeTransform::eNone);

    m_needUpdate = true;
}

void CNode::setRotation(const core::Vector3D& rotation)
{
    m_rotation = rotation;
    bool hasRotation = (rotation.x != 0.0f || rotation.y != 0.0f || rotation.z != 0.0f);
    CNode::updateTransform(hasRotation ? ENodeTransform::eRotation : ENodeTransform::eNone);

    m_needUpdate = true;
}

void CNode::setScale(const core::Vector3D& scale)
{
    m_scale = scale;
    bool hasScale = (scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f);
    CNode::updateTransform(hasScale ? ENodeTransform::eScale : ENodeTransform::eNone);

    m_needUpdate = true;
}

void CNode::setTransform(const core::Matrix4D& transform)
{
    m_transform = transform;
    CNode::updateTransform(ENodeTransform::eTransform);

    m_needUpdate = true;
}

void CNode::setParent(CNode* parent)
{
    if (!parent)
    {
        return;
    }

    if (m_parentNode)
    {
        m_parentNode->dettachChild(this);
    }
    m_parentNode = parent;
}

void CNode::attachChild(CNode* child)
{
    child->setParent(this);
    m_childNodes.push_back(child);
    m_needUpdate = true;
}

void CNode::dettachChild(CNode* child)
{
    if (!m_childNodes.empty())
    {
        m_childNodes.erase(std::remove(m_childNodes.begin(), m_childNodes.end(), child), m_childNodes.end());
        child->setParent(nullptr);
        m_needUpdate = true;
    }
}

const core::Vector3D& CNode::getPosition() const
{
    return m_position;
}

const core::Vector3D& CNode::getRotation() const
{
    return m_rotation;
}

const core::Vector3D& CNode::getScale() const
{
    return m_scale;
}

core::Matrix4D CNode::getTransform() const
{
    return m_transform;
}

core::Matrix4D CNode::getAbsTransform() const
{
    if (m_parentNode)
    {
        const core::Matrix4D absTransform = m_parentNode->getAbsTransform() * m_transform;
        return absTransform;
    }
    return m_transform;
}

CNode* CNode::getParent() const
{
    return m_parentNode;
}

f32 CNode::getPriority() const
{
    return m_priority;
}

CNode* CNode::getChildNodeByID(u32 id) const
{
    return m_childNodes[id];
}

CNode* CNode::getChildNodeByName(const std::string& name) const
{
    for (CNode* node : m_childNodes)
    {
        const std::string& nodeName = node->getName();
        if (nodeName == name)
        {
            return node;
        }
    }
    return nullptr;
}

ENodeType CNode::getNodeType() const
{
    return m_nodeType;
}

void CNode::setVisible(bool visible)
{
    m_visible = visible;
}

bool CNode::isVisible() const
{
    return m_visible;
}

void CNode::updateTransform(ENodeTransform transform)
{
    if (!m_visible)
    {
        return;
    }

    switch (transform)
    {
    case ENodeTransform::eTranslation:
        m_transform.setTranslation(m_position);
        break;

    case ENodeTransform::eRotation:
        m_transform.setRotation(m_rotation);
        break;

    case ENodeTransform::eScale:
        m_transform.setScale(m_scale);
        break;

    case ENodeTransform::eAll:
        m_transform.setTranslation(m_position);
        m_transform.setScale(m_scale);
        m_transform.setRotation(m_rotation);
        break;

    case ENodeTransform::eTransform:
        m_position = m_transform.getTranslation();
        m_rotation = m_transform.getRotation();
        m_scale    = m_transform.getScale();
        break;

    case ENodeTransform::eNone:
        break;
    }

    for (std::vector<CNode*>::iterator iter = m_childNodes.begin(); iter < m_childNodes.end(); ++iter)
    {
        (*iter)->updateTransform(transform);
        (*iter)->m_needUpdate = true;
    }
}

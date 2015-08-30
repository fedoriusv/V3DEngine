#include "Node.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace scene;
using namespace core;

std::string  CNode::s_nodeTypes[ENodeType::eCount] = {

    "Shape",
    "Model",
    "Mesh",
    "Camera",
    "Light",
    "Fog",
    "SkyBox",
    "Text",
    "Billboard"
};

const std::string& CNode::getNodeNameByType(ENodeType type)
{
    return s_nodeTypes[type];
}

CNode::CNode()
    : m_parentNode(nullptr)
    , m_nodeType(ENodeType::eUnknown)
    , m_initialiazed(false)
    , m_position(Vector3D(0.f))
    , m_rotation(Vector3D(0.f))
    , m_scale(Vector3D(1.f))
    , m_priority(0.f)
    , m_visible(true)
    , m_transformFlag(eNodeTransform)
{
    m_transformFlag |= eNodeAll | eNodeTransform;
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

void CNode::setPosition(const Vector3D& position)
{
    m_position = position;
    m_transformFlag |= eNodeTranslation | eNodeTransform;
}

void CNode::setRotation(const Vector3D& rotation)
{
    m_rotation = rotation;
    m_transformFlag |= eNodeRotation | eNodeTransform;
}

void CNode::setScale(const Vector3D& scale)
{
    m_scale = scale;

    bool hasScale = (scale.x != 1.f || scale.y != 1.f || scale.z != 1.f);
    if (hasScale)
    {
        m_transformFlag |= eNodeScale | eNodeTransform;
    }
}

void CNode::setTransform(const Matrix4D& transform)
{
    m_modelMatrix = transform;

    m_position = m_modelMatrix.getTranslation();
    m_rotation = m_modelMatrix.getRotation();
    m_scale = m_modelMatrix.getScale();
    m_transformFlag &= ~eNodeTransform;
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
    if (child)
    {
        child->setParent(this);
        m_childNodes.push_back(child);
    }
}

void CNode::dettachChild(CNode* child)
{
    if (!m_childNodes.empty())
    {
        m_childNodes.erase(std::remove(m_childNodes.begin(), m_childNodes.end(), child), m_childNodes.end());
        child->setParent(nullptr);

        child->m_transformFlag |= eNodeAll | eNodeTransform;
    }
}

const Vector3D& CNode::getPosition() const
{
    return m_position;
}

const Vector3D& CNode::getRotation() const
{
    return m_rotation;
}

const Vector3D& CNode::getScale() const
{
    return m_scale;
}

const Matrix4D& CNode::getTransform() const
{
    CNode::updateTransform();
    return m_modelMatrix;
}

Matrix4D CNode::getAbsTransform() const
{
    if (m_parentNode)
    {
        const Matrix4D absTransform = m_parentNode->getAbsTransform() * m_modelMatrix;
        return absTransform;
    }
    return m_modelMatrix;
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
    ASSERT(id < m_childNodes.size() && "invalid index");
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

void CNode::updateTransform() const
{
    if (!m_visible)
    {
        return;
    }

    if (m_transformFlag & ENodeTransform::eNodeTransform)
    {
        if (m_transformFlag & ENodeTransform::eNodeRotation)
        {
            m_modelMatrix.setRotation(m_rotation);
            m_transformFlag &= ~eNodeRotation;

            m_modelMatrix.postScale(m_scale);
            m_transformFlag &= ~eNodeScale;
        }

        if (m_transformFlag & ENodeTransform::eNodeScale)
        {
            m_modelMatrix.setScale(m_scale);
            m_transformFlag &= ~eNodeScale;
        }

        if (m_transformFlag & ENodeTransform::eNodeTranslation)
        {
            m_modelMatrix.setTranslation(m_position);
            m_transformFlag &= ~eNodeTranslation;
        }
        m_transformFlag &= ~eNodeTransform;
    }

    for (std::vector<CNode*>::const_iterator iter = m_childNodes.begin(); iter < m_childNodes.end(); ++iter)
    {
        (*iter)->m_transformFlag = m_transformFlag;
        (*iter)->updateTransform();
    }
}

const s32 CNode::getID() const
{
    return m_id;
}

const std::string& CNode::getName() const
{
    return m_name;
}

void CNode::setID(s32 id)
{
    m_id = id;
}

void CNode::setName(const std::string& name)
{
    m_name = name;
}

void CNode::update(s32 dt)
{
    CNode::updateTransform();
}

#include "Node.h"
#include "Engine.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::scene;

std::string  CNode::s_nodeTypes[ENodeType::eNodeCount] = {

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
    return  s_nodeTypes[type];
}

CNode::CNode()
    : m_parentNode(nullptr)
    , m_visible(true)
    , m_needUpdate(true)
    , m_nodeType(ENodeType::eNodeUnknown)
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
	m_transform.setTranslation(position);
    m_needUpdate = true;
}

void CNode::setRotation(const core::Vector3D& rotation)
{
	m_transform.setRotation(rotation);
    m_needUpdate = true;
}

void CNode::setScale(const core::Vector3D& scale)
{
	m_transform.setScale(scale);
    m_needUpdate = true;
}

void CNode::setTransform(const core::Matrix4D& transform)
{
	m_transform = transform;
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

core::Vector3D CNode::getPosition() const
{
	return m_transform.getTranslation();
}

core::Vector3D CNode::getRotation() const
{
	return m_transform.getRotation();
}

core::Vector3D CNode::getScale() const
{
	return m_transform.getScale();
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

void CNode::setVisible(bool visible)
{
    m_visible = visible;
}

bool CNode::isVisible() const
{
    return m_visible;
}

void CNode::updateTransform(f64 time)
{
    if (m_needUpdate)
    {
        RENDERER->updateTransform(m_transform);
        m_needUpdate = false;
    }
}

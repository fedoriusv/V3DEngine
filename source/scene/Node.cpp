#include "Node.h"

using namespace f3d;
using namespace f3d::scene;

CNode::CNode()
    : m_parentNode(nullptr)
    , m_visible(true)
    , m_needUpdate(true)
{
	m_type = EObjectType::eTypeNode;
}

CNode::~CNode()
{
	if (m_parentNode)
	{
		m_parentNode->dettachChildNode(this);
	}

	for (CNode* node : m_childNodes)
	{
		node->setParentNode(nullptr);
	}

	m_childNodes.clear();
}

void CNode::setPosition(const core::Vector3D& position)
{
	m_transform.setTranslation(position);
}

void CNode::setRotation(const core::Vector3D& rotation)
{
	m_transform.setRotation(rotation);
}

void CNode::setScale(const core::Vector3D& scale)
{
	m_transform.setScale(scale);
}

void CNode::setTransform(const core::Matrix4D& transform)
{
	m_transform = transform;
}

void CNode::setParentNode(CNode* parent)
{
	if (m_parentNode)
	{
		m_parentNode->dettachChildNode(this);
	}
	m_parentNode = parent;
}

void CNode::attachChildNode(CNode* child)
{
	child->setParentNode(this);
	m_childNodes.push_back(child);
}

void CNode::dettachChildNode(CNode* child)
{
	if (!m_childNodes.empty())
	{
		m_childNodes.erase(std::remove(m_childNodes.begin(), m_childNodes.end(), child), m_childNodes.end());
		child->setParentNode(nullptr);
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

CNode* CNode::getParentNode() const
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

void CNode::updateTransform(f64 time)
{
    //TODO: update childs
}

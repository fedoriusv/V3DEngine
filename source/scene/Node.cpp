#include "Node.h"

namespace f3d
{
namespace scene
{
	CNode::CNode()
		: m_parentNode( nullptr )
	{
		m_type = EObjectType::eTypeNode;
	}

	CNode::~CNode()
	{
		if( m_parentNode )
		{
			m_parentNode->dettachChildNode( this );
		}

		for( CNode* node : m_childNodes )
		{
			node->setParentNode( nullptr );
		}

		m_childNodes.clear(); 
	}

	void CNode::setPosition( const core::Vector3D& position )
	{
		m_transform.setTranslation( position );
	}

	void CNode::setRotation( const core::Vector3D& rotation )
	{
		m_transform.setRotation( rotation );
	}

	void CNode::setScale( const core::Vector3D& scale )
	{
		m_transform.setScale( scale );
	}

	void CNode::move( const core::Vector3D& deltaPosition )
	{
		const core::Vector3D position = m_transform.getTranslation();
		m_transform.setTranslation( position + deltaPosition );
	}

	void CNode::rotate( const core::Vector3D& deltaRotation )
	{
		const core::Vector3D rotation = m_transform.getRotation();
		m_transform.setRotation( rotation + deltaRotation );
	}

	void CNode::setTransform( const core::Matrix4D& transform )
	{
		m_transform = transform;
	}

	void CNode::setParentNode( CNode* parrentNode )
	{
		if( m_parentNode )
		{
			m_parentNode->dettachChildNode( this ); 
		}
		m_parentNode = parrentNode;
	}

	void CNode::attachChildNode( CNode* childNode )
	{
		childNode->setParentNode( this );
		m_childNodes.push_back( childNode );
	}

	void CNode::dettachChildNode( CNode* childNode )
	{
		if( !m_childNodes.empty() )
		{
			m_childNodes.erase( std::remove( m_childNodes.begin(), m_childNodes.end(), childNode ), m_childNodes.end() ); 
			childNode->setParentNode( nullptr );
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
		if( m_parentNode )
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

	CNode* CNode::getChildNodeByID( u32 childNodeID ) const
	{
		return m_childNodes[childNodeID];
	}

	CNode* CNode::getChildNodeByName( const std::string& name ) const
	{
		for( CNode* node : m_childNodes )
		{
			const std::string& nodeName = node->getName(); 
			if( nodeName == name )
			{
				return node;
			}
		}
		return nullptr;
	}

}
}

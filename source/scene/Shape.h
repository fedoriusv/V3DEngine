#ifndef _F3D_SHAPE_H_
#define _F3D_SHAPE_H_

#include "scene/Node.h"
#include "renderer/Material.h"
#include "renderer/Geometry.h"

namespace f3d
{
namespace scene
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	enum class EShapeType
	{
		eUnknownShape,
		eCubeShape,
		eSphereShape,
		eCylinderShape,
		eTorusShape,
		ePlaneShape,
		eDiskShape,
		eHeightMapShape,

		eCountShape
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CShape : public CNode
	{
	public:

								CShape();
		virtual					~CShape();

		EShapeType				getShapeType() const;

		void					setMaterial(renderer::CMaterialPtr material);
		renderer::CMaterialPtr	getMaterial() const;

	protected:

		EShapeType				m_shapeType;
		
		renderer::CMaterialPtr	m_material;
		renderer::CGeometryPtr	m_geometry;

		SVertexData&			getGeometryData();



	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHAPE_H_
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

		void					setMaterial(renderer::MaterialPtr material);
		renderer::MaterialPtr	getMaterial() const;

	protected:

		EShapeType				m_shapeType;
		
		renderer::MaterialPtr	m_material;
		renderer::GeometryPtr	m_geometry;

		SVertexData&			getGeometryData();
		renderer::EDrawMode		getGeometryDrawMode() const;
		void					setGeometryDrawMode(renderer::EDrawMode mode);

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHAPE_H_
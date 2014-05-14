#ifndef _F3D_GEOMETRY_H_
#define _F3D_GEOMETRY_H_

#include "common.h"

namespace f3d
{

namespace scene
{
	class CShape;
}

namespace renderer
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CGeometry
	{
	public:

									CGeometry();
		virtual						~CGeometry();

		virtual void				init()   = 0;
		virtual void				update() = 0;
		virtual void				free()   = 0;

	protected:

		friend						f3d::scene::CShape;

		SVertexData&				getData();
		SVertexData					m_data;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<CGeometry> CGeometryPtr;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_GEOMETRY_H_
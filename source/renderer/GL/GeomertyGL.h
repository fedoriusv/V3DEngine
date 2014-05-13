#ifndef _F3D_GEOMETRY_GL__H_
#define _F3D_GEOMETRY_GL_H_

#include "renderer/Geometry.h"

namespace f3d
{
namespace renderer
{
	class CGeometryGL : public CGeometry
	{
	public:

		CGeometryGL();
		virtual ~CGeometryGL();

		void	init()    override;
		void	update()  override;
		void	free()    override;
	};
}
}

#endif //_F3D_GEOMETRY_GL_H_

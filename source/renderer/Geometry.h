#ifndef _F3D_GEOMETRY_H_
#define _F3D_GEOMETRY_H_

#include "common.h"

namespace f3d
{
namespace renderer
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	struct SVertices
	{
		f3d::u16		id;
		std::vector<T>	vertices;

		SVertices()
			: id(0)
		{
		};
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef  std::vector<SVertices<core::Vector2D>> TextureCoords;

	class CGeometry
	{
	public:

									CGeometry();
		virtual						~CGeometry();

		virtual void				init()   = 0;
		virtual void				update() = 0;
		virtual void				free()   = 0;

	protected:

		SVertices<core::Vector3D>	m_vertex;
		SVertices<core::Vector3D>	m_normal;
		TextureCoords				m_texCoords;
		SVertices<f3d::u32>			m_indices;

		f3d::u32					m_arrayId;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<CGeometry> CGeometryPtr;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_GEOMETRY_H_
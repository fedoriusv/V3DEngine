#ifndef _F3D_VERTEX_H_
#define _F3D_VERTEX_H_

#include "common.h"

namespace f3d
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	struct SVertices
	{
		f3d::u32		id;
		std::vector<T>	vertex;

		SVertices()
			: id(0)
		{
		};
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef  std::vector<SVertices<core::Vector2D>> TextureCoords;

	struct SVertexData
	{
		SVertices<core::Vector3D>	m_vertices;
		SVertices<core::Vector3D>	m_normals;
		TextureCoords				m_texCoords;
		SVertices<f3d::u32>			m_indices;

		f3d::u32					m_countVertices;
		f3d::u32					m_countIndices;;

		f3d::u32					m_arrayId;

		void clear();
		void malloc(f3d::u32 count, f3d::u32 index = 0, f3d::u32 layer = 1);
		bool empty() const;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< class _Kty, class _Pr = _STD less<_Kty>>
	class hash_compare
	{
	private:

		_Pr pred;

	public:

		enum
		{
			bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8		// min_buckets = 2 ^^ N, 0 < N
		};

		hash_compare()
		{
		}

		hash_compare(_Pr _Pred)
			:pred(_Pr)
		{
		}

		size_t operator()(const _Kty& _Keyval) const
		{
			return (get_hash_value(_Keyval));
		}

		bool operator()(const _Kty& _Keyval1, const _Kty& _Keyval2) const
		{
			return (compare(_Keyval1, _Keyval2));
		}
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

}
#endif //_F3D_VERTEX_H_
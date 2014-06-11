#ifndef _V3D_VERTEX_H_
#define _V3D_VERTEX_H_

#include "common.h"

namespace v3d
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	struct SVertices
	{
		v3d::u32        id;
		std::vector<T>  vertex;

		SVertices()
			: id(0)
		{
		};
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef  std::vector<SVertices<core::Vector2D>> TextureCoords;

	struct SVertexData
	{
		SVertices<core::Vector3D>   m_vertices;
		SVertices<core::Vector3D>   m_normals;
		TextureCoords               m_texCoords;
		SVertices<v3d::u32>         m_indices;

		v3d::u32                    m_countVertices;
		v3d::u32                    m_countIndices;;

		void                        clear();
		void                        malloc(v3d::u32 count, v3d::u32 index = 0, v3d::u32 layer = 1);
		bool                        empty() const;
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
#endif //_V3D_VERTEX_H_

#ifndef _V3D_MATRIX_3D_H_
#define _V3D_MATRIX_3D_H_

#include "Vector2D.h"

namespace v3d
{
namespace core
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	class TMatrix3D
	{
	public:

		TMatrix3D();
		TMatrix3D(const TMatrix3D<T>& other);
		TMatrix3D(const T* matrix);
		TMatrix3D(const T& m0, const T& m1, const T& m2,
			const T& m3, const T& m4, const T& m5,
			const T& m6, const T& m7, const T& m8);

		T&				operator () (u32 row, u32 col);
		const T&		operator () (u32 row, u32 col)                  const;
		T&				operator [] (u32 index);
		const T&		operator [] (u32 index)                         const;
		TMatrix3D<T>&	operator =  (const T& scalar);
		bool			operator == (const TMatrix3D<T>& other)         const;
		bool			operator != (const TMatrix3D<T>& other)         const;
		TMatrix3D<T>	operator +  (const TMatrix3D<T>& other)         const;
		TMatrix3D<T>&	operator += (const TMatrix3D<T>& other);
		TMatrix3D<T>	operator -  (const TMatrix3D<T>& other)         const;
		TMatrix3D<T>&	operator -= (const TMatrix3D<T>& other);
		TMatrix3D<T>	operator *  (const TMatrix3D<T>& other)         const;
		TMatrix3D<T>&	operator *= (const TMatrix3D<T>& other);
		TMatrix3D<T>	operator *  (const T& scalar)                   const;
		TMatrix3D<T>&	operator *= (const T& scalar);

		void			set(const T* matrix);

		const T*		getPtr()                                        const;
		T*				getPtr();

		void			makeIdentity();
		bool			isIdentity()                                    const;

		bool			makeInverse();
		bool			getInverse(TMatrix3D<T>& outMatrix)             const;

		void			makeTransposed();
		TMatrix3D<T>	getTransposed()                                 const;

		void			setTranslation(const TVector2D<T>& translation);
		TVector2D<T>	getTranslation()                                const;

		void			setRotation(const T& angle);
		T				getRotation()                                   const;

		void			preScale(const TVector2D<T>& scale);
		void			postScale(const TVector2D<T>& scale);
		void			setScale(const TVector2D<T>& scale);

		TVector2D<T>	getScale()                                      const;

	private:

		T m_matrix[9];
	};

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef TMatrix3D<f32> Matrix3D;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#include "Matrix3D.inl"

#endif //_V3D_MATRIX_3D_H_

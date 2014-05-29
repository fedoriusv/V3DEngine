#ifndef _F3D_MATRIX_4D_H_
#define _F3D_MATRIX_4D_H_

#include "Vector3D.h"
#include "Vector4D.h"

namespace v3d
{
namespace core
{

	template <class T>
	class TMatrix4D
	{
	public:

		TMatrix4D();
		TMatrix4D(const TMatrix4D<T>& other);
		TMatrix4D(const T* matrix);
		TMatrix4D(T m0,	T m1,	T m2,	T m3,
				T m4,	T m5,	T m6,	T m7,
				T m8,	T m9,	T m10,	T m11,
				T m12,	T m13,	T m14,	T m15);

		T&				operator () (u32 row, u32 col);
		const T&		operator () (u32 row, u32 col)              const;
		T&				operator [] (u32 index);
		const T&		operator [] (u32 index)                     const;
		TMatrix4D<T>&	operator =  (const T& scalar);
		bool			operator == (const TMatrix4D<T>& other)     const;
		bool			operator != (const TMatrix4D<T>& other)     const;
		TMatrix4D<T>	operator +  (const TMatrix4D<T>& other)     const;
		TMatrix4D<T>&	operator += (const TMatrix4D<T>& other);
		TMatrix4D<T>	operator -  (const TMatrix4D<T>& other)     const;
		TMatrix4D<T>&	operator -= (const TMatrix4D<T>& other);
		TMatrix4D<T>	operator *  (const TMatrix4D<T>& other)     const;
		TMatrix4D<T>&	operator *= (const TMatrix4D<T>& other);
		TMatrix4D<T>	operator *  (const T& scalar)               const;
		TMatrix4D<T>&	operator *= (const T& scalar);

		void			set(const T* matrix);

		T*				getPtr();
		const T*		getPtr()                                    const;

		void			makeIdentity();
		bool			isIdentity()                                const;

		void			makeTransposed();
		TMatrix4D<T>	getTransposed()                             const;

		void			setTranslation(const TVector3D<T>& translation);
		TVector3D<T>	getTranslation()                            const;

		void			setRotation(const TVector3D<T>& rotation);
		TVector3D<T>	getRotation()                               const;

		void			preScale(const TVector3D<T>& scale);
		void			postScale(const TVector3D<T>& scale);
		void			setScale(const TVector3D<T>& scale);

		TVector3D<T>	getScale()                                  const;

		bool			makeInverse();
		bool			getInverse(TMatrix4D<T>& outMatrix)         const;

	private:

		T m_matrix[16];

	};

	typedef TMatrix4D<f32> Matrix4D;

	inline Matrix4D buildProjectionMatrixPerspective(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
	{
		Matrix4D outMatrix;

		f32* matrix = outMatrix.getPtr();

		const f32 yFac = tanf(fieldOfViewRadians / 2.0f);
		const f32 xFac = yFac*aspectRatio;

		matrix[0] = 1 / xFac;
		matrix[1] = 0;
		matrix[2] = 0;
		matrix[3] = 0;

		matrix[4] = 0;
		matrix[5] = 1 / yFac;
		matrix[6] = 0;
		matrix[7] = 0;

		matrix[8] = 0;
		matrix[9] = 0;
		matrix[10] = -(zFar + zNear) / (zFar - zNear);
		matrix[11] = -1;

		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = -(2 * zFar*zNear) / (zFar - zNear);
		matrix[15] = 0;

		return outMatrix;
	}

	inline Matrix4D buildProjectionMatrixOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
	{
		Matrix4D outMatrix;

		f32* matrix = outMatrix.getPtr();

		matrix[0] = 2.f / (right - left);
		matrix[1] = 0.f;
		matrix[2] = 0.f;
		matrix[3] = 0.f;

		matrix[4] = 0.f;
		matrix[5] = 2.f / (top - bottom);
		matrix[6] = 0.f;
		matrix[7] = 0.f;

		matrix[8] = 0.f;
		matrix[9] = 0.f;
		matrix[10] = -1.f / (zFar - zNear);
		matrix[11] = 0.f;

		matrix[12] = 0.f;
		matrix[13] = 0.f;
		matrix[14] = -(zNear) / (zFar - zNear);
		matrix[15] = 1.f;

		return outMatrix;
	}

	inline Matrix4D buildLookAtMatrix(const Vector3D& position, const Vector3D& target, const Vector3D& upVector)
	{
		Matrix4D outMatrix;

		f32* matrix = outMatrix.getPtr();

		Vector3D axisZ = target - position;
		axisZ.normalize();

		Vector3D axisX = crossProduct(upVector, axisZ);
		axisX.normalize();

		Vector3D axisY = crossProduct(axisZ, axisX);

		matrix[0] = axisX.x;
		matrix[1] = axisY.x;
		matrix[2] = axisZ.x;
		matrix[3] = 0;

		matrix[4] = axisX.y;
		matrix[5] = axisY.y;
		matrix[6] = axisZ.y;
		matrix[7] = 0;

		matrix[8] = axisX.z;
		matrix[9] = axisY.z;
		matrix[10] = axisZ.z;
		matrix[11] = 0;

		matrix[12] = -dotProduct(axisX, position);
		matrix[13] = -dotProduct(axisY, position);
		matrix[14] = -dotProduct(axisZ, position);
		matrix[15] = 1;

		return outMatrix;
	}


	template <class T>
	TMatrix4D<T>::TMatrix4D()
	{
		makeIdentity();
	}

	template <class T>
	TMatrix4D<T>::TMatrix4D(const TMatrix4D<T>& other)
	{
		*this = other;
	}

	template <class T>
	TMatrix4D<T>::TMatrix4D(const T* matrix)
	{
		memcpy(m_matrix, matrix, 16 * sizeof(T));
	}

	template <class T>
	TMatrix4D<T>::TMatrix4D(T m0, T m1, T m2, T m3,
		T m4, T m5, T m6, T m7,
		T m8, T m9, T m10, T m11,
		T m12, T m13, T m14, T m15)
	{
		m_matrix[0] = m0;
		m_matrix[1] = m1;
		m_matrix[2] = m2;
		m_matrix[3] = m3;
		m_matrix[4] = m4;
		m_matrix[5] = m5;
		m_matrix[6] = m6;
		m_matrix[7] = m7;
		m_matrix[8] = m8;
		m_matrix[9] = m9;
		m_matrix[10] = m10;
		m_matrix[11] = m11;
		m_matrix[12] = m12;
		m_matrix[13] = m13;
		m_matrix[14] = m14;
		m_matrix[15] = m15;
	}

	template <class T>
	T& TMatrix4D<T>::operator () (u32 row, u32 col)
	{
		return m_matrix[row * 4 + col];
	}

	template <class T>
	const T& TMatrix4D<T>::operator () (u32 row, u32 col) const
	{
		return m_matrix[row * 4 + col];
	}

	template <class T>
	T& TMatrix4D<T>::operator [] (u32 index)
	{
		return m_matrix[index];
	}

	template <class T>
	const T& TMatrix4D<T>::operator [] (u32 index) const
	{
		return m_matrix[index];
	}


	template <class T>
	TMatrix4D<T> TMatrix4D<T>::operator + (const TMatrix4D<T>& other) const
	{
		TMatrix4D<T> temp;

		temp[0] = m_matrix[0] + other[0];
		temp[1] = m_matrix[1] + other[1];
		temp[2] = m_matrix[2] + other[2];
		temp[3] = m_matrix[3] + other[3];
		temp[4] = m_matrix[4] + other[4];
		temp[5] = m_matrix[5] + other[5];
		temp[6] = m_matrix[6] + other[6];
		temp[7] = m_matrix[7] + other[7];
		temp[8] = m_matrix[8] + other[8];
		temp[9] = m_matrix[9] + other[9];
		temp[10] = m_matrix[10] + other[10];
		temp[11] = m_matrix[11] + other[11];
		temp[12] = m_matrix[12] + other[12];
		temp[13] = m_matrix[13] + other[13];
		temp[14] = m_matrix[14] + other[14];
		temp[15] = m_matrix[15] + other[15];

		return temp;
	}

	template <class T>
	TMatrix4D<T>& TMatrix4D<T>::operator += (const TMatrix4D<T>& other)
	{
		m_matrix[0] += other[0];
		m_matrix[1] += other[1];
		m_matrix[2] += other[2];
		m_matrix[3] += other[3];
		m_matrix[4] += other[4];
		m_matrix[5] += other[5];
		m_matrix[6] += other[6];
		m_matrix[7] += other[7];
		m_matrix[8] += other[8];
		m_matrix[9] += other[9];
		m_matrix[10] += other[10];
		m_matrix[11] += other[11];
		m_matrix[12] += other[12];
		m_matrix[13] += other[13];
		m_matrix[14] += other[14];
		m_matrix[15] += other[15];

		return *this;
	}

	template <class T>
	TMatrix4D<T> TMatrix4D<T>::operator - (const TMatrix4D<T>& other) const
	{
		TMatrix4D<T> temp;

		temp[0] = m_matrix[0] - other[0];
		temp[1] = m_matrix[1] - other[1];
		temp[2] = m_matrix[2] - other[2];
		temp[3] = m_matrix[3] - other[3];
		temp[4] = m_matrix[4] - other[4];
		temp[5] = m_matrix[5] - other[5];
		temp[6] = m_matrix[6] - other[6];
		temp[7] = m_matrix[7] - other[7];
		temp[8] = m_matrix[8] - other[8];
		temp[9] = m_matrix[9] - other[9];
		temp[10] = m_matrix[10] - other[10];
		temp[11] = m_matrix[11] - other[11];
		temp[12] = m_matrix[12] - other[12];
		temp[13] = m_matrix[13] - other[13];
		temp[14] = m_matrix[14] - other[14];
		temp[15] = m_matrix[15] - other[15];

		return temp;
	}

	template <class T>
	TMatrix4D<T>& TMatrix4D<T>::operator -= (const TMatrix4D<T>& other)
	{

		m_matrix[0] -= other[0];
		m_matrix[1] -= other[1];
		m_matrix[2] -= other[2];
		m_matrix[3] -= other[3];
		m_matrix[4] -= other[4];
		m_matrix[5] -= other[5];
		m_matrix[6] -= other[6];
		m_matrix[7] -= other[7];
		m_matrix[8] -= other[8];
		m_matrix[9] -= other[9];
		m_matrix[10] -= other[10];
		m_matrix[11] -= other[11];
		m_matrix[12] -= other[12];
		m_matrix[13] -= other[13];
		m_matrix[14] -= other[14];
		m_matrix[15] -= other[15];

		return *this;
	}

	template <class T>
	TMatrix4D<T> TMatrix4D<T>::operator * (const T& scalar) const
	{
		TMatrix4D<T> temp;

		temp[0] = m_matrix[0] * scalar;
		temp[1] = m_matrix[1] * scalar;
		temp[2] = m_matrix[2] * scalar;
		temp[3] = m_matrix[3] * scalar;
		temp[4] = m_matrix[4] * scalar;
		temp[5] = m_matrix[5] * scalar;
		temp[6] = m_matrix[6] * scalar;
		temp[7] = m_matrix[7] * scalar;
		temp[8] = m_matrix[8] * scalar;
		temp[9] = m_matrix[9] * scalar;
		temp[10] = m_matrix[10] * scalar;
		temp[11] = m_matrix[11] * scalar;
		temp[12] = m_matrix[12] * scalar;
		temp[13] = m_matrix[13] * scalar;
		temp[14] = m_matrix[14] * scalar;
		temp[15] = m_matrix[15] * scalar;

		return temp;
	}

	template <class T>
	TMatrix4D<T>& TMatrix4D<T>::operator *= (const T& scalar)
	{
		m_matrix[0] *= scalar;
		m_matrix[1] *= scalar;
		m_matrix[2] *= scalar;
		m_matrix[3] *= scalar;
		m_matrix[4] *= scalar;
		m_matrix[5] *= scalar;
		m_matrix[6] *= scalar;
		m_matrix[7] *= scalar;
		m_matrix[8] *= scalar;
		m_matrix[9] *= scalar;
		m_matrix[10] *= scalar;
		m_matrix[11] *= scalar;
		m_matrix[12] *= scalar;
		m_matrix[13] *= scalar;
		m_matrix[14] *= scalar;
		m_matrix[15] *= scalar;

		return *this;
	}

	template <class T>
	TMatrix4D<T>& TMatrix4D<T>::operator *= (const TMatrix4D<T>& other)
	{
		TMatrix4D<T> temp(*this);

		const T * m1 = temp.m_matrix;
		const T * m2 = other.m_matrix;

		m_matrix[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
		m_matrix[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
		m_matrix[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
		m_matrix[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
		m_matrix[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
		m_matrix[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
		m_matrix[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
		m_matrix[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
		m_matrix[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
		m_matrix[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
		m_matrix[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
		m_matrix[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
		m_matrix[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
		m_matrix[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
		m_matrix[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
		m_matrix[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

		return *this;
	}

	template <class T>
	TMatrix4D<T> TMatrix4D<T>::operator * (const TMatrix4D<T>& other) const
	{
		TMatrix4D<T> temp;

		const T * m1 = m_matrix;
		const T * m2 = other.m_matrix;

		temp.m_matrix[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
		temp.m_matrix[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
		temp.m_matrix[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
		temp.m_matrix[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
		temp.m_matrix[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
		temp.m_matrix[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
		temp.m_matrix[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
		temp.m_matrix[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
		temp.m_matrix[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
		temp.m_matrix[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
		temp.m_matrix[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
		temp.m_matrix[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
		temp.m_matrix[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
		temp.m_matrix[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
		temp.m_matrix[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
		temp.m_matrix[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

		return temp;
	}

	template <class T>
	TVector3D<T> TMatrix4D<T>::getTranslation() const
	{
		return TVector3D<T>(m_matrix[12], m_matrix[13], m_matrix[14]);
	}

	template <class T>
	void TMatrix4D<T>::setTranslation(const TVector3D<T>& translation)
	{
		m_matrix[12] = translation.x;
		m_matrix[13] = translation.y;
		m_matrix[14] = translation.z;
	}


	template <class T>
	void TMatrix4D<T>::preScale(const TVector3D<T>& scale)
	{
		m_matrix[0] *= scale.x;
		m_matrix[1] *= scale.y;
		m_matrix[2] *= scale.z;

		m_matrix[4] *= scale.x;
		m_matrix[5] *= scale.y;
		m_matrix[6] *= scale.z;

		m_matrix[8] *= scale.x;
		m_matrix[9] *= scale.y;
		m_matrix[10] *= scale.z;

		m_matrix[12] *= scale.x;
		m_matrix[13] *= scale.y;
		m_matrix[14] *= scale.z;
	}

	template <class T>
	void TMatrix4D<T>::postScale(const TVector3D<T>& scale)
	{
		m_matrix[0] *= scale.x;
		m_matrix[1] *= scale.x;
		m_matrix[2] *= scale.x;

		m_matrix[4] *= scale.y;
		m_matrix[5] *= scale.y;
		m_matrix[6] *= scale.y;

		m_matrix[8] *= scale.z;
		m_matrix[9] *= scale.z;
		m_matrix[10] *= scale.z;
	}

	template <class T>
	void TMatrix4D<T>::setScale(const TVector3D<T>& scale)
	{
		m_matrix[0] = scale.x;
		m_matrix[5] = scale.y;
		m_matrix[10] = scale.z;
	}


	template <class T>
	TVector3D<T> TMatrix4D<T>::getScale() const
	{
		TVector3D<T> scale;
		scale.x = TVector3D<T>(m_matrix[0], m_matrix[1], m_matrix[2]).length();
		scale.y = TVector3D<T>(m_matrix[4], m_matrix[5], m_matrix[6]).length();
		scale.z = TVector3D<T>(m_matrix[8], m_matrix[9], m_matrix[10]).length();
		return scale;
	}

	template <class T>
	void TMatrix4D<T>::setRotation(const TVector3D<T>& rotation)
	{
		f64 rotX = (f64)rotation.x;
		f64 rotY = (f64)rotation.y;
		f64 rotZ = (f64)rotation.z;

		const f64 cr = cos(rotX);
		const f64 sr = sin(rotX);
		const f64 cp = cos(rotY);
		const f64 sp = sin(rotY);
		const f64 cy = cos(rotZ);
		const f64 sy = sin(rotZ);

		m_matrix[0] = (T)(cp*cy);
		m_matrix[1] = (T)(cp*sy);
		m_matrix[2] = (T)(-sp);

		const f64 srsp = sr * sp;
		const f64 crsp = cr * sp;

		m_matrix[4] = (T)(srsp * cy - cr * sy);
		m_matrix[5] = (T)(srsp * sy + cr * cy);
		m_matrix[6] = (T)(sr * cp);

		m_matrix[8] = (T)(crsp * cy + sr * sy);
		m_matrix[9] = (T)(crsp * sy - sr * cy);
		m_matrix[10] = (T)(cr * cp);
	}

	template <class T>
	TVector3D<T> TMatrix4D<T>::getRotation() const
	{
		const TMatrix4D<T> &mat = *this;

		f64 sin = clamp((f64)mat(0, 2), -1.0, 1.0);
		f64 Y = -asin(sin);
		const f64 C = cos(Y);

		f64 rotx, roty, X, Z;

		if (abs(C) > k_tolerance64)
		{
			const f64 invC = (f64)(1.0 / C);
			rotx = mat(2, 2) * invC;
			roty = mat(1, 2) * invC;
			X = atan2(roty, rotx);
			rotx = mat(0, 0) * invC;
			roty = mat(0, 1) * invC;
			Z = atan2(roty, rotx);
		}
		else
		{
			X = 0.0;
			rotx = mat(1, 1);
			roty = -mat(1, 0);
			Z = atan2(roty, rotx);
		}

		if (X < 0.0) X += k_2pi;
		if (Y < 0.0) Y += k_2pi;
		if (Z < 0.0) Z += k_2pi;

		return TVector3D<T>((T)X, (T)Y, (T)Z);
	}


	template <class T>
	void TMatrix4D<T>::makeIdentity()
	{

		m_matrix[1] = m_matrix[2] = m_matrix[3] = m_matrix[4] =
		m_matrix[6] = m_matrix[7] = m_matrix[8] = m_matrix[9] =
		m_matrix[11] = m_matrix[12] = m_matrix[13] = m_matrix[14] = (T)0;
		m_matrix[0] = m_matrix[5] = m_matrix[10] = m_matrix[15] = (T)1;
	}

	template <class T>
	bool TMatrix4D<T>::isIdentity() const
	{
		if (!isEquals(m_matrix[0], (T)1) ||
			!isEquals(m_matrix[5], (T)1) ||
			!isEquals(m_matrix[10], (T)1) ||
			!isEquals(m_matrix[15], (T)1))
		{
			return false;
		}

		for (s32 i = 0; i < 4; ++i)
		{
			for (s32 j = 0; j < 4; ++j)
			{
				if ((j != i) && (!isZero((*this)(i, j))))
				{
					return false;
				}
			}
		}

		return true;
	}

	template <class T>
	bool TMatrix4D<T>::getInverse(TMatrix4D<T>& outMatrix) const
	{
		const T t0 = m_matrix[10] * m_matrix[15] - m_matrix[11] * m_matrix[14];
		const T t1 = m_matrix[6] * m_matrix[15] - m_matrix[7] * m_matrix[14];
		const T t2 = m_matrix[6] * m_matrix[11] - m_matrix[7] * m_matrix[10];

		const T t3 = m_matrix[2] * m_matrix[15] - m_matrix[3] * m_matrix[14];
		const T t4 = m_matrix[2] * m_matrix[11] - m_matrix[3] * m_matrix[10];
		const T t5 = m_matrix[2] * m_matrix[7] - m_matrix[3] * m_matrix[6];

		const T t6 = m_matrix[8] * m_matrix[13] - m_matrix[9] * m_matrix[12];
		const T t7 = m_matrix[4] * m_matrix[13] - m_matrix[5] * m_matrix[12];
		const T t8 = m_matrix[4] * m_matrix[9] - m_matrix[5] * m_matrix[8];

		const T t9 = m_matrix[0] * m_matrix[13] - m_matrix[1] * m_matrix[12];
		const T t10 = m_matrix[0] * m_matrix[9] - m_matrix[1] * m_matrix[8];
		const T t11 = m_matrix[0] * m_matrix[5] - m_matrix[1] * m_matrix[4];

		T det = t0 * t11 - t1 * t10 + t2 * t9 + t3 * t8 - t4 * t7 + t5 * t6;

		if (isZero(det))
		{
			return false;
		}

		det = (T)1.0 / det;

		const T ft0 = (T)(t0 * det);
		const T ft1 = (T)(t1 * det);
		const T ft2 = (T)(t2 * det);

		const T ft3 = (T)(t3 * det);
		const T ft4 = (T)(t4 * det);
		const T ft5 = (T)(t5 * det);

		const T ft6 = (T)(t6 * det);
		const T ft7 = (T)(t7 * det);
		const T ft8 = (T)(t8 * det);

		const T ft9 = (T)(t9 * det);
		const T ft10 = (T)(t10 * det);
		const T ft11 = (T)(t11 * det);

		outMatrix.m_matrix[0] = m_matrix[5] * ft0 - m_matrix[9] * ft1 + m_matrix[13] * ft2;
		outMatrix.m_matrix[1] = m_matrix[9] * ft3 - m_matrix[1] * ft0 - m_matrix[13] * ft4;
		outMatrix.m_matrix[2] = m_matrix[1] * ft1 - m_matrix[5] * ft3 + m_matrix[13] * ft5;
		outMatrix.m_matrix[3] = m_matrix[5] * ft4 - m_matrix[1] * ft2 - m_matrix[9] * ft5;

		outMatrix.m_matrix[4] = m_matrix[8] * ft1 - m_matrix[4] * ft0 - m_matrix[12] * ft2;
		outMatrix.m_matrix[5] = m_matrix[0] * ft0 - m_matrix[8] * ft3 + m_matrix[12] * ft4;
		outMatrix.m_matrix[6] = m_matrix[4] * ft3 - m_matrix[0] * ft1 - m_matrix[12] * ft5;
		outMatrix.m_matrix[7] = m_matrix[0] * ft2 - m_matrix[4] * ft4 + m_matrix[8] * ft5;

		outMatrix.m_matrix[8] = m_matrix[7] * ft6 - m_matrix[11] * ft7 + m_matrix[15] * ft8;
		outMatrix.m_matrix[9] = m_matrix[11] * ft9 - m_matrix[3] * ft6 - m_matrix[15] * ft10;
		outMatrix.m_matrix[10] = m_matrix[3] * ft7 - m_matrix[7] * ft9 + m_matrix[15] * ft11;
		outMatrix.m_matrix[11] = m_matrix[7] * ft10 - m_matrix[3] * ft8 - m_matrix[11] * ft11;

		outMatrix.m_matrix[12] = m_matrix[10] * ft7 - m_matrix[6] * ft6 - m_matrix[14] * ft8;
		outMatrix.m_matrix[13] = m_matrix[2] * ft6 - m_matrix[10] * ft9 + m_matrix[14] * ft10;
		outMatrix.m_matrix[14] = m_matrix[6] * ft9 - m_matrix[2] * ft7 - m_matrix[14] * ft11;
		outMatrix.m_matrix[15] = m_matrix[2] * ft8 - m_matrix[6] * ft10 + m_matrix[10] * ft11;

		return true;
	}

	template <class T>
	bool TMatrix4D<T>::makeInverse()
	{
		TMatrix4D<T> temp;

		if (getInverse(temp))
		{
			*this = temp;
			return true;
		}

		return false;
	}

	template <class T>
	TMatrix4D<T>& TMatrix4D<T>::operator = (const T& scalar)
	{
		for (s32 i = 0; i < 16; ++i)
		{
			m_matrix[i] = scalar;
		}
		return *this;
	}

	template <class T>
	bool TMatrix4D<T>::operator == (const TMatrix4D<T> &other) const
	{
		for (s32 i = 0; i < 16; ++i)
		{
			if (m_matrix[i] != other.m_matrix[i])
			{
				return false;
			}
		}
		return true;
	}

	template <class T>
	bool TMatrix4D<T>::operator != (const TMatrix4D<T> &other) const
	{
		return !(*this == other);
	}

	template <class T>
	void TMatrix4D<T>::makeTransposed()
	{
		TMatrix4D<T> temp(*this);

		m_matrix[0] = temp.m_matrix[0];
		m_matrix[1] = temp.m_matrix[4];
		m_matrix[2] = temp.m_matrix[8];
		m_matrix[3] = temp.m_matrix[12];

		m_matrix[4] = temp.m_matrix[1];
		m_matrix[5] = temp.m_matrix[5];
		m_matrix[6] = temp.m_matrix[9];
		m_matrix[7] = temp.m_matrix[13];

		m_matrix[8] = temp.m_matrix[2];
		m_matrix[9] = temp.m_matrix[6];
		m_matrix[10] = temp.m_matrix[10];
		m_matrix[11] = temp.m_matrix[14];

		m_matrix[12] = temp.m_matrix[3];
		m_matrix[13] = temp.m_matrix[7];
		m_matrix[14] = temp.m_matrix[11];
		m_matrix[15] = temp.m_matrix[15];
	}

	template <class T>
	TMatrix4D<T> TMatrix4D<T>::getTransposed() const
	{
		TMatrix4D<T> out;

		out.m_matrix[0] = m_matrix[0];
		out.m_matrix[1] = m_matrix[4];
		out.m_matrix[2] = m_matrix[8];
		out.m_matrix[3] = m_matrix[12];

		out.m_matrix[4] = m_matrix[1];
		out.m_matrix[5] = m_matrix[5];
		out.m_matrix[6] = m_matrix[9];
		out.m_matrix[7] = m_matrix[13];

		out.m_matrix[8] = m_matrix[2];
		out.m_matrix[9] = m_matrix[6];
		out.m_matrix[10] = m_matrix[10];
		out.m_matrix[11] = m_matrix[14];

		out.m_matrix[12] = m_matrix[3];
		out.m_matrix[13] = m_matrix[7];
		out.m_matrix[14] = m_matrix[11];
		out.m_matrix[15] = m_matrix[15];

		return out;
	}

	template <class T>
	void TMatrix4D<T>::set(const T* matrix)
	{
		if (matrix)
			memcpy(m_matrix, matrix, 16 * sizeof(T));
	}

	template <class T>
	const T* TMatrix4D<T>::getPtr() const
	{
		return m_matrix;
	}

	template <class T>
	T* TMatrix4D<T>::getPtr()
	{
		return m_matrix;
	}

	template <class T>
	TMatrix4D<T> operator * (const T scalar, const TMatrix4D<T>& mat)
	{
		return mat * scalar;
	}

}
}
#endif //_F3D_MATRIX_4D_H_

#ifndef _F3D_VECTOR_2D_H_
#define _F3D_VECTOR_2D_H_

#include "Math.h"

namespace f3d
{
namespace core
{
	template <class T>
	class TVector2D
	{
	public:

		TVector2D();
		TVector2D(T nx, T ny);
		TVector2D(const TVector2D<T>& other);

		TVector2D<T>	operator -  () const;
		TVector2D<T>&	operator =  (const TVector2D<T>& other);
		TVector2D<T>	operator +  (const TVector2D<T>& other)		const;
		TVector2D<T>&	operator += (const TVector2D<T>& other);
		TVector2D<T>	operator +  (const T scalar)				const;
		TVector2D<T>&	operator += (const T scalar);
		TVector2D<T>	operator -  (const TVector2D<T>& other)		const;
		TVector2D<T>&	operator -= (const TVector2D<T>& other);
		TVector2D<T>	operator -  (const T scalar)				const;
		TVector2D<T>&	operator -= (const T scalar);
		TVector2D<T>	operator *  (const TVector2D<T>& other)		const;
		TVector2D<T>&	operator *= (const TVector2D<T>& other);
		TVector2D<T>	operator *  (const T scalar)				const;
		TVector2D<T>&	operator *= (const T scalar);
		TVector2D<T>	operator /  (const TVector2D<T>& other)		const;
		TVector2D<T>&	operator /= (const TVector2D<T>& other);
		TVector2D<T>	operator /  (const T scalar)				const;
		TVector2D<T>&	operator /= (const T scalar);
		bool			operator <= (const TVector2D<T>&other)		const;
		bool			operator >= (const TVector2D<T>&other)		const;
		bool			operator <  (const TVector2D<T>&other)		const;
		bool			operator >  (const TVector2D<T>&other)		const;
		bool			operator == (const TVector2D<T>& other)		const;
		bool			operator != (const TVector2D<T>& other)		const;

		TVector2D<T>&	set(const T nx, const T ny);
		TVector2D<T>&	set(const TVector2D<T>& other);

		bool			isEquals(const TVector2D<T>& other, const T tolerance = (T)k_tolerance32)	const;
		bool			isInLine(const TVector2D<T>& begin, const TVector2D<T>& end)				const;

		T				length()										const;
		T				lengthSQ()										const;

		T				distanceFrom(const TVector2D<T>& other)			const;
		T				distanceFromSQ(const TVector2D<T>& other)		const;

		TVector2D<T>&	normalize();
		TVector2D<T>&	invert();

		TVector2D<T>&	setLength(T newlength);

		T x;
		T y;
	};

	typedef TVector2D<f32> Vector2D;

	template<class T>
	T distance(const TVector2D<T>& v1, const TVector2D<T>& v2)
	{
		return TVector2D<T>(v1.x - v2.x, v1.y - v2.y).length();
	}

	template<class T>
	T dotProduct(const TVector2D<T>& v1, const TVector2D<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	template<class T>
	TVector2D<T> crossProduct(const TVector2D<T>& v1, const TVector2D<T>& v2)
	{
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	template<class T>
	TVector2D<T> interpolate(const TVector2D<T>& v1, const TVector2D<T>& v2, const T d)
	{
		return v1 + (v2 - v1) * d;
	}

	template<class S, class T>
	TVector2D<T> operator * (const S scalar, const TVector2D<T>& vector)
	{
		return vector * scalar;
	}

	template<class T>
	TVector2D<T>::TVector2D()
		: x(0)
		, y(0) 
	{}

	template<class T>
	TVector2D<T>::TVector2D(T nx, T ny)
		: x(nx)
		, y(ny) 
	{}

	template<class T>
	TVector2D<T>::TVector2D(const TVector2D<T>& other)
		: x(other.x)
		, y(other.y)
	{}

	template<class T>
	TVector2D<T> TVector2D<T>::operator - () const
	{
		return TVector2D<T>(-x, -y);
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::operator = (const TVector2D<T>& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	template<class T>
	TVector2D<T> TVector2D<T>::operator + (const TVector2D<T>& other) const
	{
		return TVector2D<T>(x + other.x, y + other.y);
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::operator += (const TVector2D<T>& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template<class T>
	TVector2D<T> TVector2D<T>::operator + (const T scalar) const
	{
		return TVector2D<T>(x + scalar, y + scalar);
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::operator += (const T scalar)
	{
		x += scalar;
		y += scalar;
		return *this;
	}

	template<class T>
	TVector2D<T> TVector2D<T>::operator - (const TVector2D<T>& other) const
	{
		return TVector2D<T>(x - other.x, y - other.y);
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::operator -= (const TVector2D<T>& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<class T>
	TVector2D<T> TVector2D<T>::operator - (const T scalar) const
	{
		return TVector2D<T>(x - scalar, y - scalar);
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::operator -= (const T scalar)
	{
		x -= scalar;
		y -= scalar;
		return *this;
	}

	template<class T>
	TVector2D<T> TVector2D<T>::operator * (const TVector2D<T>& other) const
	{
		return TVector2D<T>(x * other.x, y * other.y);
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::operator *= (const TVector2D<T>& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	template<class T>
	TVector2D<T> TVector2D<T>::operator * (const T scalar) const
	{
		return TVector2D<T>(x * scalar, y * scalar);
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::operator *= (const T scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	template<class T>
	TVector2D<T> TVector2D<T>::operator / (const TVector2D<T>& other) const
	{
		return TVector2D<T>(x / other.x, y / other.y);
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::operator /= (const TVector2D<T>& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	template<class T>
	TVector2D<T> TVector2D<T>::operator / (const T v) const
	{
		T i = (T)1.0 / v;
		TVector2D<T> out(x * i, y * i);
		return out;
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::operator /= (const T v)
	{
		T i = (T)1.0 / v;
		x *= i;
		y *= i;
		return *this;
	}

	template<class T>
	bool TVector2D<T>::operator <= (const TVector2D<T>&other) const
	{
		return x <= other.x && y <= other.y;
	}

	template<class T>
	bool TVector2D<T>::operator >= (const TVector2D<T>&other) const
	{
		return x >= other.x && y >= other.y;
	}

	template<class T>
	bool TVector2D<T>::operator < (const TVector2D<T>&other) const
	{
		return x < other.x && y < other.y;
	}

	template<class T>
	bool TVector2D<T>::operator >(const TVector2D<T>&other) const
	{
		return x > other.x && y > other.y;
	}

	template<class T>
	bool TVector2D<T>::operator == (const TVector2D<T>& other) const
	{
		return this->isEquals(other);;
	}

	template<class T>
	bool TVector2D<T>::operator != (const TVector2D<T>& other) const
	{
		return !this->isEquals(other);;
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::set(const T nx, const T ny)
	{
		x = nx;
		y = ny;
		return *this;
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::set(const TVector2D<T>& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	template<class T>
	T TVector2D<T>::length() const
	{
		const T length = (T)sqrt((f64)(x * x + y * y));
		return length;
	}

	template<class T>
	T TVector2D<T>::lengthSQ() const
	{
		return x * x + y * y;
	}

	template<class T>
	T TVector2D<T>::distanceFrom(const TVector2D<T>& other) const
	{
		const T distance = TVector2D<T>(x - other.x, y - other.y).length();
		return distance;
	}

	template<class T>
	T TVector2D<T>::distanceFromSQ(const TVector2D<T>& other) const
	{
		const T distanceSQ = TVector2D<T>(x - other.x, y - other.y).lengthSQ();
		return distanceSQ;
	}

	template<class T>
	bool TVector2D<T>::isEquals(const TVector2D<T>& other, const T tolerance) const
	{
		const bool isEquels = core::isEquals(x, other.x, tolerance) &&
			core::isEquals(y, other.y, tolerance);
		return isEquels;
	}

	template<class T>
	bool TVector2D<T>::isInLine(const TVector2D<T>& begin, const TVector2D<T>& end) const
	{
		const T f = (end - begin).lengthSQ();
		const bool isInLine = distanceFromSQ(begin) <= f && distanceFromSQ(end) <= f;
		return isInLine;
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::normalize()
	{
		T l = x * x + y * y;

		if (l == (T) 0.0)
		{
			return *this;
		}
		l = ((T) 1.0) / (T)sqrt((T)l);
		x *= l;
		y *= l;

		return *this;
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::invert()
	{
		x *= -1.0f;
		y *= -1.0f;
		return *this;
	}

	template<class T>
	TVector2D<T>& TVector2D<T>::setLength(T newlength)
	{
		normalize();
		return (*this *= newlength);
	}

}
}

#endif //_F3D_VECTOR_2D_H_
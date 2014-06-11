#ifndef _V3D_DIMENSION_2D_H_
#define _V3D_DIMENSION_2D_H_

#include "Math.h"
#include "Vector2D.h"

namespace v3d
{
namespace core
{
	template <class T>
	class TVector2D;

	template <class T>
	class TDimension2D
	{
	public:

		TDimension2D();
		TDimension2D(const T& width, const T& height);
		TDimension2D(const TVector2D<T>& other);
		TDimension2D(const TDimension2D<T>& other);

		bool operator == (const TDimension2D<T>& other)                 const;
		bool operator != (const TDimension2D<T>& other)                 const;
		bool operator == (const TVector2D<T>& other)                    const;
		bool operator != (const TVector2D<T>& other)                    const;

		TDimension2D<T>&	operator = (const TDimension2D<T>& other);
		TDimension2D<T>&	operator /= (const T& scale);
		TDimension2D<T>		operator /  (const T& scale)                const;
		TDimension2D<T>		operator *  (const T& scale)                const;
		TDimension2D<T>		operator +  (const TDimension2D<T>& other)  const;
		TDimension2D<T>&	operator += (const TDimension2D<T>& other);
		TDimension2D<T>&	operator -= (const TDimension2D<T>& other);
		TDimension2D<T>&	operator *= (const T& scale);

		TDimension2D<T>&	set(const T& width, const T& height);

		T getArea() const;

		T width;
		T height;
	};

	typedef TDimension2D<u32> Dimension2D;

	template <class T>
	TDimension2D<T>::TDimension2D()
		: width(0)
		, height(0)
	{
	}

	template <class T>
	TDimension2D<T>::TDimension2D(const T& width, const T& height)
		: width(width)
		, height(height)
	{
	}

	template <class T>
	TDimension2D<T>::TDimension2D(const TVector2D<T>& other)
		: width(other.x)
		, height(other.y)
	{
	}

	template <class T>
	TDimension2D<T>::TDimension2D(const TDimension2D<T>& other) 
		: width(other.width)
		, height(other.height)
	{
	}

	template <class T>
	TDimension2D<T>& TDimension2D<T>::operator = (const TDimension2D<T>& other)
	{
		width = other.width;
		height = other.height;
		return *this;
	}

	template <class T>
	bool TDimension2D<T>::operator == (const TDimension2D<T>& other) const
	{
		const bool isEquals = core::isEquals(width, other.width) &&
			core::isEquals(height, other.height);
		return isEquals;
	}

	template <class T>
	bool TDimension2D<T>::operator != (const TDimension2D<T>& other) const
	{
		return !(*this == other);
	}

	template <class T>
	bool TDimension2D<T>::operator == (const TVector2D<T>& other) const
	{
		const bool isEquals = core::isEquals(width, other.x) &&
			core::isEquals(height, other.y);
		return isEquals;
	}

	template <class T>
	bool TDimension2D<T>::operator != (const TVector2D<T>& other) const
	{
		return !(*this == other);
	}

	template <class T>
	TDimension2D<T>& TDimension2D<T>::set(const T& nWidth, const T& nHeight)
	{
		width = nWidth;
		height = nHeight;
		return *this;
	}

	template <class T>
	TDimension2D<T>& TDimension2D<T>::operator /= (const T& scale)
	{
		width /= scale;
		height /= scale;
		return *this;
	}

	template <class T>
	TDimension2D<T> TDimension2D<T>::operator / (const T& scale) const
	{
		return TDimension2D<T>(width / scale, height / scale);
	}

	template <class T>
	TDimension2D<T>& TDimension2D<T>::operator *= (const T& scale)
	{
		width *= scale;
		height *= scale;
		return *this;
	}

	template <class T>
	TDimension2D<T> TDimension2D<T>::operator * (const T& scale) const
	{
		return TDimension2D<T>(width * scale, height * scale);
	}

	template <class T>
	TDimension2D<T>& TDimension2D<T>::operator += (const TDimension2D<T>& other)
	{
		width += other.width;
		height += other.height;
		return *this;
	}

	template <class T>
	TDimension2D<T>& TDimension2D<T>::operator -= (const TDimension2D<T>& other)
	{
		width -= other.Width;
		height -= other.Height;
		return *this;
	}

	template <class T>
	TDimension2D<T> TDimension2D<T>::operator+(const TDimension2D<T>& other) const
	{
		return TDimension2D<T>(width + other.width, height + other.height);
	}

	template <class T>
	T TDimension2D<T>::getArea() const
	{
		return width * height;
	}

}
}

#endif //_V3D_DIMENSION_2D_H_


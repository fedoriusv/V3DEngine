using namespace v3d;
using namespace core;

template <class T>
TPoint2D<T>::TPoint2D()
: x(0)
, y(0)
{
}

template <class T>
TPoint2D<T>::TPoint2D(const T& x, const T& y)
: x(x)
, y(y)
{
}

template <class T>
TPoint2D<T>::TPoint2D(const TPoint2D<T>& other)
: x(other.x)
, y(other.y)
{
}

template <class T>
TPoint2D<T>::TPoint2D(const TVector2D<T>& other)
: x(other.x)
, y(other.y)
{
}

template <class T>
TPoint2D<T>::TPoint2D(const TDimension2D<T>& other)
: x(other.width)
, y(other.height)
{
}

template <class T>
TPoint2D<T>& TPoint2D<T>::operator = (const TPoint2D<T>& other)
{
    x = other.x;
    y = other.y;
    return *this;
}

template <class T>
bool TPoint2D<T>::operator == (const TPoint2D<T>& other) const
{
    const bool isEquals = core::isEquals(x, other.x) &&
        core::isEquals(y, other.y);
    return isEquals;
}

template <class T>
bool TPoint2D<T>::operator != (const TPoint2D<T>& other) const
{
    return !(*this == other);
}

template <class T>
bool TPoint2D<T>::operator == (const TDimension2D<T>& other) const
{
    const bool isEquals = core::isEquals(x, other.width) &&
        core::isEquals(y, other.height);
    return isEquals;
}

template <class T>
bool TPoint2D<T>::operator != (const TDimension2D<T>& other) const
{
    return !(*this == other);
}

template <class T>
bool TPoint2D<T>::operator == (const TVector2D<T>& other) const
{
    const bool isEquals = core::isEquals(x, other.x) &&
        core::isEquals(x, other.y);
    return isEquals;
}

template <class T>
bool TPoint2D<T>::operator != (const TVector2D<T>& other) const
{
    return !(*this == other);
}

template <class T>
TPoint2D<T>& TPoint2D<T>::set(const T& nx, const T& ny)
{
    x = nx;
    y = ny;
    return *this;
}

template <class T>
TPoint2D<T>& TPoint2D<T>::set(const TDimension2D<T>& other)
{
    x = other.width;
    y = other.height;
    return *this;
}

template <class T>
TPoint2D<T>& TPoint2D<T>::set(const TVector2D<T>& other)
{
    x = other.width;
    y = other.height;
    return *this;
}

template <class T>
void TPoint2D<T>::offset(const T& xOffset, const T& yOffset)
{
    x += xOffset;
    y += yOffset;
}

template <class T>
void TPoint2D<T>::offset(const TDimension2D<T>& offset)
{
    x += offset.width;
    y += offset.height;
}

template <class T>
void TPoint2D<T>::offset(const TVector2D<T>& offset)
{
    x += offset.x;
    y += offset.y;
}

template <class T>
TPoint2D<T>& TPoint2D<T>::operator *= (const T& scale)
{
    x *= scale;
    y *= scale;
    return *this;
}

template <class T>
TPoint2D<T> TPoint2D<T>::operator * (const T& scale) const
{
    return TPoint2D<T>(x * scale, y * scale);
}

template <class T>
TPoint2D<T>& TPoint2D<T>::operator += (const TPoint2D<T>& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

template <class T>
TPoint2D<T>& TPoint2D<T>::operator -= (const TPoint2D<T>& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

template <class T>
TPoint2D<T> TPoint2D<T>::operator+(const TPoint2D<T>& other) const
{
    return TPoint2D<T>(x + other.x, y + other.y);
}

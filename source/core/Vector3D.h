#ifndef _V3D_VECTOR_3D_H_
#define _V3D_VECTOR_3D_H_

#include "Math.h"
#include "Vector2D.h"

namespace v3d
{
namespace core
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    class TVector3D
    {
    public:

        TVector3D();
        TVector3D(T nx);
        TVector3D(T nx, T ny, T nz);
        TVector3D(const TVector3D<T>& other);
        TVector3D(const TVector2D<T>& other, T nz = 0);

        TVector3D<T>    operator -  ()                             const;
        TVector3D<T>&   operator =  (const TVector3D<T>& other);
        TVector3D<T>&   operator =  (const T* other);
        TVector3D<T>    operator +  (const TVector3D<T>& other)    const;
        TVector3D<T>&   operator += (const TVector3D<T>& other);
        TVector3D<T>    operator +  (const T scalar)               const;
        TVector3D<T>&   operator += (const T scalar);
        TVector3D<T>    operator -  (const TVector3D<T>& other)    const;
        TVector3D<T>&   operator -= (const TVector3D<T>& other);
        TVector3D<T>    operator -  (const T scalar)               const;
        TVector3D<T>&   operator -= (const T scalar);
        TVector3D<T>    operator *  (const TVector3D<T>& other)    const;
        TVector3D<T>&   operator *= (const TVector3D<T>& other);
        TVector3D<T>    operator *  (const T scalar)               const;
        TVector3D<T>&   operator *= (const T scalar);
        TVector3D<T>    operator /  (const TVector3D<T>& other)    const;
        TVector3D<T>&   operator /= (const TVector3D<T>& other);
        TVector3D<T>    operator /  (const T scalar)               const;
        TVector3D<T>&   operator /= (const T scalar);
        bool            operator <= (const TVector3D<T>&other)     const;
        bool            operator >= (const TVector3D<T>&other)     const;
        bool            operator <  (const TVector3D<T>&other)     const;
        bool            operator >  (const TVector3D<T>&other)     const;
        bool            operator == (const TVector3D<T>& other)    const;
        bool            operator!=  (const TVector3D<T>& other)    const;
        T               operator [] (u32 index)                    const;
        T&              operator [] (u32 index);

        TVector3D<T>&   set(const T nx, const T ny, const T nz);
        TVector3D<T>&   set(const TVector3D<T>& p);

        bool            isEquals(const TVector3D<T>& other, const T tolerance = (T)k_tolerance32) const;
        bool            isInLine(const TVector3D<T>& begin, const TVector3D<T>& end) const;

        T               length()                                   const;
        T               lengthSQ()                                 const;

        T               distanceFrom(const TVector3D<T>& other)    const;
        T               distanceFromSQ(const TVector3D<T>& other)  const;

        TVector3D<T>&   normalize();
        TVector3D<T>&   invert();

        TVector3D<T>&   setLength(T newlength);

        T               x;
        T               y;
        T               z;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef TVector3D<f32> Vector3D;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    T distance(const TVector3D<T>& v1, const TVector3D<T>& v2)
    {
        const T distance = TVector3D<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z).getLength();
        return distance;
    }

    template<class T>
    T dotProduct(const TVector3D<T>& v1, const TVector3D<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v2.z * v2.z;
    }

    template<class T>
    TVector3D<T> crossProduct(const TVector3D<T>& v1, const TVector3D<T>& v2)
    {
        return TVector3D<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    template<class T>
    TVector3D<T> interpolate(const TVector3D<T>& v1, const TVector3D<T>& v2, const T d)
    {
        return v1 + (v2 - v1) * d;
    }

    template<class S, class T>
    TVector3D<T> operator * (const S scalar, const TVector3D<T>& vector)
    {
        return vector * scalar;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#include "Vector3D.inl"

#endif //_V3D_VECTOR_3D_H_

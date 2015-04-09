#ifndef _V3D_DIMENSION_2D_H_
#define _V3D_DIMENSION_2D_H_

#include "Math.h"
#include "Vector2D.h"
//#include "Point2D.h"

namespace v3d
{
namespace core
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    class TDimension2D
    {
    public:

        TDimension2D();
        TDimension2D(const T& width, const T& height);
        TDimension2D(const TVector2D<T>& other);
        //TDimension2D(const TPoint2D<T>& other);
        TDimension2D(const TDimension2D<T>& other);

        bool                operator == (const TDimension2D<T>& other)  const;
        bool                operator != (const TDimension2D<T>& other)  const;
        bool                operator == (const TVector2D<T>& other)     const;
        bool                operator != (const TVector2D<T>& other)     const;
        //bool                operator == (const TPoint2D<T>& other)      const;
        //bool                operator != (const TPoint2D<T>& other)      const;

        TDimension2D<T>&    operator = (const TDimension2D<T>& other);
        TDimension2D<T>&    operator /= (const T& scale);
        TDimension2D<T>     operator /  (const T& scale)                const;
        TDimension2D<T>     operator *  (const T& scale)                const;
        TDimension2D<T>     operator +  (const TDimension2D<T>& other)  const;
        TDimension2D<T>&    operator += (const TDimension2D<T>& other);
        TDimension2D<T>&    operator -= (const TDimension2D<T>& other);
        TDimension2D<T>&    operator *= (const T& scale);

        TDimension2D<T>&    set(const T& width, const T& height);

        T                   getArea() const;

        T                   width;
        T                   height;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef TDimension2D<u32> Dimension2D;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#include "Dimension2D.inl"

#endif //_V3D_DIMENSION_2D_H_


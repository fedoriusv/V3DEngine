#ifndef _V3D_POINT_2D_H_
#define _V3D_POINT_2D_H_

#include "Math.h"
#include "Dimension2D.h"
#include "Vector2D.h"

namespace v3d
{
namespace core
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    class TPoint2D
    {
    public:

        TPoint2D();
        TPoint2D(const T& x, const T& y);
        TPoint2D(const TPoint2D<T>& other);
        TPoint2D(const TVector2D<T>& other);
        //TPoint2D(const TDimension2D<T>& other);

        bool            operator == (const TPoint2D<T>& other)      const;
        bool            operator != (const TPoint2D<T>& other)      const;
        //bool            operator == (const TDimension2D<T>& other)  const;
        //bool            operator != (const TDimension2D<T>& other)  const;
        bool            operator == (const TVector2D<T>& other)     const;
        bool            operator != (const TVector2D<T>& other)     const;

        TPoint2D<T>&    operator =  (const TPoint2D<T>& other);
        TPoint2D<T>     operator *  (const T& scale)                const;
        TPoint2D<T>     operator +  (const TPoint2D<T>& other)      const;
        TPoint2D<T>&    operator += (const TPoint2D<T>& other);
        TPoint2D<T>&    operator -= (const TPoint2D<T>& other);
        TPoint2D<T>&    operator *= (const T& scale);

        TPoint2D<T>&    set(const T& x, const T& y);
        TPoint2D<T>&    set(const TDimension2D<T>& other);
        TPoint2D<T>&    set(const TVector2D<T>& other);

        void            offset(const T& xOffset, const T& yOffset);
        void            offset(const TDimension2D<T>& offset);
        void            offset(const TVector2D<T>& offset);

        T               x;
        T               y;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef TPoint2D<s32> Point2D;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#include "Point2D.inl"

#endif //_V3D_POINT_2D_H_


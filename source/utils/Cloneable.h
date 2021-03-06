#ifndef _V3D_CLONEABLE_H_
#define _V3D_CLONEABLE_H_

namespace v3d
{
namespace utils
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    class TCloneable
    {
    public:

        TCloneable() {};
        virtual     ~TCloneable() {};

        virtual T   clone() const = 0;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d

#endif //_V3D_CLONEABLE_H_

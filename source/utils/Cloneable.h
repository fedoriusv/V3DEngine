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
        TCloneable(const TCloneable& pass) = delete;
        TCloneable& operator=(const TCloneable& pass) = delete;

        virtual T   clone() const = 0;

    protected:
        virtual     ~TCloneable() {};
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d

#endif //_V3D_CLONEABLE_H_

#ifndef _V3D_TARGET_H_
#define _V3D_TARGET_H_

#include "common.h"

namespace v3d
{
namespace scene
{
    class CTargetManager;
}

namespace renderer
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Base Interface for target entity.
    */
    class ITarget
    {
    public:

        enum class ETagetType
        {
            eRenderTarget,
            eGeometryTarget
        };

        ITarget();
        virtual                     ~ITarget();

        virtual void                bind()   = 0;
        virtual void                unbind() = 0;

        const std::string&          getName() const;
        ETagetType                  getTagetType() const;

    protected:

        void                        setName(const std::string& name);
        void                        setTargetType(ETagetType type);

        friend                      scene::CTargetManager;

        virtual bool                create()  = 0;
        virtual void                destroy() = 0;

        std::string                 m_name;
        ETagetType                  m_targetType;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<ITarget>            TargetPtr;

    typedef std::vector<TargetPtr>              TargetList;
    typedef std::map<std::string, TargetPtr>    TargetMap;
    typedef TargetMap::iterator                 TargetIter;
    typedef TargetMap::const_iterator           TargetCIter;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_TARGET_H_

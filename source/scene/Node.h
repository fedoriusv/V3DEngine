#ifndef _F3D_NODE_H_
#define _F3D_NODE_H_

#include "Object.h"

namespace f3d
{
namespace scene
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CNode : public CObject
    {
    public:
        CNode();
        virtual             ~CNode();

        void                setPosition (const core::Vector3D& position);
        void                setRotation (const core::Vector3D& rotation);
        void                setScale    (const core::Vector3D& scale);
        void                setTransform(const core::Matrix4D& transform);

        void                setParent   (CNode* parent);
        void                attachChild (CNode* child);
        void                dettachChild(CNode* child);

        core::Vector3D      getPosition     ()                           const;
        core::Vector3D      getRotation     ()                           const;
        core::Vector3D      getScale        ()                           const;
        core::Matrix4D      getTransform    ()                           const;
        core::Matrix4D      getAbsTransform ()                           const;

        CNode*              getParent       ()                           const;
        CNode*              getChildNodeByID(u32 id)                     const;
        CNode*              getChildNodeByName(const std::string& name)  const;

        virtual void        render()         = 0;
        virtual void        update(f64 time) = 0;
        
        virtual void        init()           = 0;

    protected:

        void                updateTransform(f64 time);

        CNode*              m_parentNode;
        std::vector<CNode*> m_childNodes;

        core::Matrix4D      m_transform;
        bool                m_visible;
        bool                m_needUpdate;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_NODE_H_
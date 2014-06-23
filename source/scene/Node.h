#ifndef _V3D_NODE_H_
#define _V3D_NODE_H_

#include "Object.h"

namespace v3d
{
namespace scene
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ENodeType
    {
        eUnknown = -1,
        eShape,
        eModel,
        eCamera,
        eLight,
        eFog,
        eSkyBox,
        eFont,

        eCount
     };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class ENodeTransform
    {
        eNone,
        eTransform,
        eTranslation,
        eRotation,
        eScale,
        eAll
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSceneManager;

    class CNode : public CObject
    {
    public:

        CNode();
        virtual                     ~CNode();

        void                        setPosition (const core::Vector3D& position);
        void                        setRotation (const core::Vector3D& rotation);
        void                        setScale    (const core::Vector3D& scale);
        void                        setTransform(const core::Matrix4D& transform);
                                    
        void                        setParent   (CNode* parent);
        void                        attachChild (CNode* child);
        void                        dettachChild(CNode* child);
                                    
        const core::Vector3D&       getPosition()           const;
        const core::Vector3D&       getRotation()           const;
        const core::Vector3D&       getScale()              const;
        core::Matrix4D              getTransform()          const;
        core::Matrix4D              getAbsTransform()       const;
                                    
        CNode*                      getParent       ()                           const;
        CNode*                      getChildNodeByID(u32 id)                     const;
        CNode*                      getChildNodeByName(const std::string& name)  const;
                                    
        virtual void                render()         = 0;
        virtual void                update(f64 time) = 0;
        
        virtual void                init()           = 0;

        void                        setVisible(bool visible);
        bool                        isVisible() const;

        ENodeType                   getNodeType() const;
        f32                         getPriority() const;
        static const std::string&   getNodeNameByType(ENodeType type);

    protected:

        friend                      CSceneManager;

        void                        updateTransform(ENodeTransform transform);

        CNode*                      m_parentNode;
        std::vector<CNode*>         m_childNodes;

        core::Vector3D              m_position;
        core::Vector3D              m_rotation;
        core::Vector3D              m_scale;
        core::Matrix4D              m_transform;

        bool                        m_visible;
        ENodeType                   m_nodeType;

        bool                        m_needUpdate;
        f32                         m_priority;

    private:

        static std::string          s_nodeTypes[ENodeType::eCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_NODE_H_

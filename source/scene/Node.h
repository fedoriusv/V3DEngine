#ifndef _V3D_NODE_H_
#define _V3D_NODE_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    class CRenderList;
}
namespace scene
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ENodeType
    {
        eUnknown = -1,
        eShape,
        eModel,
        eMesh,
        eCamera,
        eLight,
        eFog,
        eSkyBox,
        eText,
        eBillboard,

        eCount
     };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CNode
    {
    public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////

        enum ENodeTransform
        {
            eNodeTranslation    = 1 << 0,
            eNodeRotation       = 1 << 1,
            eNodeScale          = 1 << 2,
            eNodeTransform      = 1 << 3,

            eNodeAll = eNodeTranslation | eNodeRotation | eNodeScale,
        };

        CNode();
        virtual                     ~CNode();

        void                        setPosition (const core::Vector3D& position);
        void                        setRotation (const core::Vector3D& rotation);
        void                        setScale    (const core::Vector3D& scale);
        void                        setTransform(const core::Matrix4D& transform);

        const core::Vector3D&       getPosition()           const;
        const core::Vector3D&       getRotation()           const;
        const core::Vector3D&       getScale()              const;
        const core::Matrix4D&       getTransform()          const;
        core::Matrix4D              getAbsTransform()       const;
                                    
        void                        setID(s32 id);
        void                        setName(const std::string& name);
                                    
        const s32                   getID()                 const;
        const std::string&          getName()               const;

        void                        setParent(CNode* parent);
        void                        attachChild(CNode* child);
        void                        dettachChild(CNode* child);
                                    
        CNode*                      getParent()                                  const;
        CNode*                      getChildNodeByID(u32 id)                     const;
        CNode*                      getChildNodeByName(const std::string& name)  const;
                                    
        virtual void                init() = 0;
        virtual void                update(s32 dt)   = 0;

        void                        setVisible(bool visible);
        bool                        isVisible() const;

        ENodeType                   getNodeType() const;
        f32                         getPriority() const;
        static const std::string&   getNodeNameByType(ENodeType type);

    protected:

        friend                      renderer::CRenderList;

        ENodeType                   m_nodeType;
        bool                        m_initialiazed;

    private:

        void                        updateTransform() const;

        core::Vector3D              m_position;
        core::Vector3D              m_rotation;
        core::Vector3D              m_scale;
        mutable core::Matrix4D      m_modelMatrix;

        CNode*                      m_parentNode;
        std::vector<CNode*>         m_childNodes;

        f32                         m_priority;
        bool                        m_visible;

        mutable u16                m_transformFlag;

        s32                         m_id;
        std::string                 m_name;

    private:

        static std::string          s_nodeTypes[ENodeType::eCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CNode> NodePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_NODE_H_

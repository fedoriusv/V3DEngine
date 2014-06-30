#ifndef _V3D_DEBUG_LIGHT_H_
#define _V3D_DEBUG_LIGHT_H_

#include "Object.h"
#include "RenderPass.h"
#include "Vertex.h"

namespace v3d
{
namespace scene
{
    struct SLightData;
}
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDebugLightFlag
    {
        eLightFlagNone      = -1,
        eLightFlagPosition  = 0x00000001,
        eLightFlagDirection = 0x00000010,
        eLightFlagRadius    = 0x00000100,

        eFlagAll            = 0x10000000
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDebugLight
    {
        eDebugLightPosition = 0,
        eDebugLightDirection,
        eDebugLightRadius,

        eDebugLightCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SDebugLight
    {
        //SVertices<core::Vector3D> _vertex;
        u32                       _arrayId;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugLight : public CObject
    {
    public:

        CDebugLight(const core::Vector3D& position, const scene::SLightData& data);
        virtual                  ~CDebugLight();

        void                     init();
        void                     bind();

        virtual void             draw() = 0;
        virtual void             free() = 0;

        void                     setDebugFlag(s32 flag);

    protected:

        void                     initShader();
        virtual void             initDraw(SDebugLight& object) = 0;

        void                     initLightPosition();

        static RenderPassPtr     s_pass;

        s32                      m_flag;

        SDebugLight              m_objects[EDebugLight::eDebugLightCount];

        const core::Vector3D&    m_position;
        const scene::SLightData& m_data;

        const std::string*       m_vertex;
        const std::string*       m_fragment;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CDebugLight> DebugLightPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DEBUG_LIGHT_H_
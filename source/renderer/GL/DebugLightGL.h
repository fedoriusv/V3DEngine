#ifndef _V3D_DEBUG_LIGHT_GL_H_
#define _V3D_DEBUG_LIGHT_GL_H_

#include "renderer/DebugLight.h"

namespace v3d
{
namespace renderer
{
    class CDebugLightGL : public CDebugLight
    {
    public:

        CDebugLightGL(const core::Vector3D& position, const scene::SLightData& data);
        virtual             ~CDebugLightGL();

        void        draw() override;
        void        free() override;
    };
}
}

#endif //_V3D_DEBUG_LIGHT_GL_H_

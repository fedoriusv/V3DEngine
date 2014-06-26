#ifndef _V3D_DEBUG_DRAW_GL_H_
#define _V3D_DEBUG_DRAW_GL_H_

#include "renderer/DebugDraw.h"

namespace v3d
{
    namespace renderer
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        class CDebugDrawGL : public CDebugDraw
        {
        public:

            CDebugDrawGL(const GeometryPtr& geometry);
            CDebugDrawGL(const Vector3D* position, const f32* radius);
            virtual             ~CDebugDrawGL();

            void                draw()    override;
            void                free()    override;

        private:

            void                initDraw(SDebugObject& object)    override;
            void                refreshDraw(SDebugObject& object) override;

            void                drawNormals();
            void                drawEdges();
            void                drawLights();

        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}

#endif //_V3D_DEBUG_DRAW_GL_H_
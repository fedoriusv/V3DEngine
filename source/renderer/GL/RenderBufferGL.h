#ifndef _V3D_RENDERBUFFER_GL_H_
#define _V3D_RENDERBUFFER_GL_H_

#include "renderer/RenderBuffer.h"

namespace v3d
{
namespace renderer
{
namespace gl
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    *Inherited class for render buffer. GL render only.
    */
    class CRenderBufferGL : public CRenderBuffer
    {
    public:

        CRenderBufferGL(EImageFormat format, EImageType type, u32 samples = 0);
        ~CRenderBufferGL();

        bool        bind()     override;
        void        unbind()   override;

        u32         getRenderBufferID() const;

    private:

        void        renderbufferStorage(u32 internalFormat, const core::Dimension2D& size, u32 samplers);
        
        u32         m_renderBufferID;
        u32         m_samplesCount;

        static u32  s_currentBuffer;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDERBUFFER_GL_H_

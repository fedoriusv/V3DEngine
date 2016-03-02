#ifndef _V3D_SAMPLER_GL_H_
#define _V3D_SAMPLER_GL_H_

#include "renderer/Texture.h"

namespace v3d
{
namespace renderer
{
namespace gl
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTextureGL;

    /**
    * Class for texture properties management. GL render only.
    */
    class SamplerGL
    {
    public:

        SamplerGL();
        ~SamplerGL();

        bool            bind(u32 unit);
        bool            unbind(u32 unit);

        void            setFilterType(u32 min, u32 mag);
        void            setWrap(u32 wrap);
        void            setAnisotropicLevel(EAnisotropic level);
        void            setTextureCompare(u32 mode, u32 func);

        u32             getSamplerID() const;

    private:

        friend          CTextureGL;

        static s32      getActiveSampler();

        u32             m_samplerID;
        static u32      s_currentSamplerID;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_TEXTURE_GL_H_

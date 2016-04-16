#ifndef _V3D_RENDERBUFFER_H_
#define _V3D_RENDERBUFFER_H_

#include "common.h"
#include "ImageFormats.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Base Interface for render buffer entity.
    */
    class CRenderBuffer
    {
    public:

        CRenderBuffer();
        virtual                     ~CRenderBuffer();

        virtual bool                bind()     = 0;
        virtual void                unbind()   = 0;

        void                        setSize(const core::Dimension2D& size);

        const core::Dimension2D&    getSize() const;
        EImageFormat                getFormat() const;
        EImageType                  getType() const;

    protected:

        void                        setFormat(EImageFormat format);
        void                        setType(EImageType type);

        bool                        m_changed;

    private:

        core::Dimension2D           m_size;
        EImageFormat                m_format;
        EImageType                  m_type;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDERBUFFER_H_

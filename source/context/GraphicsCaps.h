#pragma once

#include "common.h"

namespace v3d
{
namespace renderer
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class GpraphicsCaps
    {
    public:

        GpraphicsCaps();
        ~GpraphicsCaps();

        u32     getSamplesCount() const;
        u32     getTextureUnitsCount() const;
        f32     getMaxAnisotropySize() const;
        u32     getMaxPatchVertices() const;

        void    setSamplesCount(u32 count);
        void    setTextureUnitsCount(u32 count);
        void    setMaxAnisotropySize(f32 size);
        void    setMaxPatchVertices(u32 size);

        bool    isSupportMultisample() const;

    private:

        u32     m_samplesCount;
        u32     m_maxTextureUnits;
        f32     m_maxAnisotropy;
        u32     m_maxPatchVertices;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

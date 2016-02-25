#ifndef _V3D_DRIVER_CONTEXT_H_
#define _V3D_DRIVER_CONTEXT_H_

#include "common.h"
#include "platform/Window.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDriverContext
    {
    public:

        CDriverContext(const platform::WindowPtr& window);
        virtual                     ~CDriverContext();

        virtual void                driverInfo()                                     = 0;
        virtual void                checkForErrors(const std::string& location = "") = 0;
        virtual bool                createContext()                                  = 0;
        virtual void                destroyContext()                                 = 0;

        virtual bool                setVSync(bool use)                               = 0;
        virtual void                flushBuffers()                                   = 0;

        const core::Dimension2D&    getWindowSize() const;
        const platform::WindowPtr&  getWindow()     const;

        u32                         getSamplesCount()       const;
        s32                         getTextureUnits()       const;
        f32                         getMaxAnisotropySize()  const;

    protected:

        const platform::WindowPtr   m_window;

        u32                         m_samplesCount;
        s32                         m_maxTextureUnits;
        f32                         m_maxAnisotropy;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CDriverContext> DriverContextPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_DRIVER_CONTEXT_H_

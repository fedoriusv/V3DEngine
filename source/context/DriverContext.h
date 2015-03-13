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

        virtual bool                setVSync(bool use)                               = 0;

        const core::Dimension2D&    getWindowSize() const;
        const platform::WindowPtr&  getWindow() const;

    protected:

        const platform::WindowPtr   m_window;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CDriverContext> DriverContextPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DRIVER_CONTEXT_H_

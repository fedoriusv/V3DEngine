#ifndef _V3D_DRIVER_CONTEXT_GL_H_
#define _V3D_DRIVER_CONTEXT_GL_H_

#include "DriverContext.h"

namespace v3d
{
namespace renderer
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDriverContextGL : public CDriverContext
    {
    public:

        CDriverContextGL(const platform::WindowPtr& window);
        virtual ~CDriverContextGL();

        void    driverInfo()                                     override;
        void    checkForErrors(const std::string& location = "") override;
        bool    createContext()                                  override;

        bool    setVSync(bool use)                               override;

    protected:

#if defined(_PLATFORM_WIN_)
        bool    createWin32Context();
#elif defined(_PLATFORM_LINUX_)
        bool    createLinuxContext();
#endif
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DRIVER_CONTEXT_GL_H_

#ifndef _V3D_BASE_APPLICATION_H_
#define _V3D_BASE_APPLICATION_H_

#include "Engine.h"

namespace v3d
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class BaseApplication
    {
    public:

        BaseApplication(int& argc, char** argv);
        virtual                                         ~BaseApplication();

        virtual void                                    init() = 0;
        virtual void                                    run() = 0;

        int                                             exec();

    protected:

        virtual const platform::PlatformPtr&            getPlatform()          const final;
        virtual const platform::WindowPtr               getWindow()            const final;
        virtual const event::InputEventHandlerPtr&      getInputEventHandler() const final;
        virtual const scene::SceneManagerPtr&           getSceneManager()      const final;

    private:

        CEngine*                                        m_engine;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
#endif //_V3D_BASE_APPLICATION_H_

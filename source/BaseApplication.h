#pragma once

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
        virtual void                                    run()  = 0;

        int                                             exec();

    protected:

        virtual const platform::WindowPtr               getWindow()            const final;
        virtual const event::InputEventHandlerPtr       getInputEventHandler() const final;
        virtual const scene::SceneManagerPtr            getSceneManager()      const final;
        Engine*                                         getEngine();

    private:

        Engine*                                         m_engine;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace v3d

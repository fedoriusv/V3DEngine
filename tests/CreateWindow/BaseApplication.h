#ifndef _BASE_APPLICATION_H_
#define _BASE_APPLICATION_H_

#include "Valera3D.h"

class BaseApplication
{
public:
												BaseApplication(int& argc, char** argv);
	virtual										~BaseApplication();

	virtual void								init() = 0;
	virtual void								run()  = 0;
	
	int											exec();

protected:

	virtual v3d::platform::PlatformPtr			getPlatform()          const final;
    virtual v3d::platform::WindowPtr			getWindow()            const final;
	virtual v3d::event::InputEventHandlerPtr	getInputEventHandler() const final;
	virtual v3d::scene::SceneManagerPtr		    getSceneManager()      const final;

private:

	v3d::CEngine*								m_engine;
};

#endif //_BASE_APPLICATION_H_

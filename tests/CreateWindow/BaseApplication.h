#ifndef _BASE_APPLICATION_H_
#define _BASE_APPLICATION_H_

#include "Engine.h"

class BaseApplication
{
public:
												BaseApplication(int& argc, char** argv);
	virtual										~BaseApplication();

	virtual void								init() = 0;
	virtual void								run()  = 0;
	
	int											exec();

protected:

	virtual f3d::platform::PlatformPtr			getPlatform()          const final;
	virtual f3d::event::InputEventHandlerPtr	getInputEventHandler() const final;
	virtual f3d::scene::SceneManagerPtr		    getSceneManager()      const final;

private:

	f3d::CEngine*								m_engine;
};

#endif //_BASE_APPLICATION_H_
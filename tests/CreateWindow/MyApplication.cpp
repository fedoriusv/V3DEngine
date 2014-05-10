#include "MyApplication.h"

///Temp
#include "GL/glew.h"
#if defined (_PLATFORM_WIN_)
#	include <windows.h>
#endif
///

MyApplication::MyApplication(int& argc, char** argv)
	: BaseApplication(argc, argv)
{
}

MyApplication::~MyApplication()
{
}

void MyApplication::init()
{
	LOG_INFO("--------Init Application--------")

	m_engine->getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
	glViewport( 0, 0, 800, 600 );
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void MyApplication::run()
{
	//Temporary
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

	int a = 0;
	glFlush();
}

void MyApplication::onKeyboard(const f3d::event::SKeyboardInputEventPtr& event)
{
	int a = 0;
}

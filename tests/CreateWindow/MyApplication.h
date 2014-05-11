#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_

#include "BaseApplication.h"

class MyApplication : public BaseApplication
{
public:
			MyApplication(int& argc, char** argv);
	virtual ~MyApplication();

	void	init() override;
	void	run()  override;

	void	onKeyboard(const f3d::event::SKeyboardInputEventPtr& event);
};

#endif //_MY_APPLICATION_H_
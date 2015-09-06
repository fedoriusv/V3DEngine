// Billboard.cpp : Defines the entry point for the console application.
//

#include "MyApplication.h"

int main(int argc, char* argv[])
{
    v3d::BaseApplication* app = new MyApplication(argc, argv);
    return app->exec();
}

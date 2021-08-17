#include "AppWindow.h"

int main()
{
	try
	{
		GraphicsEngine::create();
		InputSystem::create();

		AppWindow app;
		while(app.isRunning());
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		GraphicsEngine::release();
		InputSystem::release();
		return -1;
	}

	return 0;
}
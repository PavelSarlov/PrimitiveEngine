#include "AppWindow.h"

int main()
{
	try
	{
		GraphicsEngine::create();
		InputSystem::create();

		AppWindow app;
		while(app.isRunning());

		InputSystem::release();
		GraphicsEngine::release();
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		InputSystem::release();
		GraphicsEngine::release();
		return -1;
	}

	return 0;
}
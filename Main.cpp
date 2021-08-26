#include "BumpMappingDemo.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"

int main()
{
	try
	{
		GraphicsEngine::create();
		InputSystem::create();

		/*SpaceShooterGame app;
		while(app.isRunning());*/

		BumpMappingDemo app;
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
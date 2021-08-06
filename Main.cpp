#include <stdio.h>
#include <iostream>
#include "AppWindow.h"

int main()
{
	AppWindow app;
	if(app.Init())
	{
		while(app.IsRunning())
		{
			app.Process();
		}
	}

	return 0;
}
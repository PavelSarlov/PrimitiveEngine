#include "PrimitiveEngine.h"

class GameTest : public PrimitiveEngine
{
public:

	void OnUserChange()
	{
		for(int i = 200; i < 400; i++)
		{
			DrawPixel(i, 300, RGB(0xFF, 0xFF, 0xFF));
		}
	}

	void UserInit()
	{

	}

private:

};
#include "PrimitiveEngine.h"

class GameTest : public PrimitiveEngine
{
public:

	void OnUserChange()
	{
		COORD x1, y1, x2, y2, x3, y3;
		scanf_s("%d %d %d %d %d %d", &x1, &y1, &x2, &y2, &x3, &y3);

		DrawTriangle(x1, y1, x2, y2, x3, y3, RGB(0xFF, 0xFF, 0xFF));
	}

	void UserInit()
	{

	}

private:

};
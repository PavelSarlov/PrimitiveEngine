#pragma once
#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include <Windows.h>

class InputListener
{
public:
	InputListener()
	{}

	~InputListener() 
	{}

public:
	// keyboard events
	virtual void onKeyDown(USHORT key) = 0;
	virtual void onKeyUp(USHORT key) = 0;

public:
	// mouse events
	virtual void onMouseMove(const POINT &mouse_pos) = 0;
	virtual void onLeftMouseDown(const POINT &mouse_pos) = 0;
	virtual void onLeftMouseUp(const POINT &mouse_pos) = 0;
	virtual void onRightMouseDown(const POINT &mouse_pos) = 0;
	virtual void onRightMouseUp(const POINT &mouse_pos) = 0;
};


#endif // !INPUTLISTENER_H

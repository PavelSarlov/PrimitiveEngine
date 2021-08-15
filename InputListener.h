#pragma once
#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

class InputListener
{
public:
	InputListener()
	{}

	~InputListener() 
	{}

public:
	virtual void onKeyDown(USHORT key) = 0;
	virtual void onKeyUp(USHORT key) = 0;
};


#endif // !INPUTLISTENER_H

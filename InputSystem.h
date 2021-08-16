#pragma once
#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <unordered_set>
#include <Windows.h>

#include "InputListener.h"
#include "Point.h"

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

public:
	static InputSystem *get();

public:
	void update();
	void addListener(InputListener *listener);
	void removeListener(InputListener *listener);
	
	void setCursorPos(const Point &pos);
	void showCursor(bool show);

private:
	std::unordered_set<InputListener *> m_listeners;
	UCHAR m_keys_state[256] = {};
	UCHAR m_old_keys_state[256] = {};
	Point m_old_mouse_pos = Point();
	bool m_first_time = true;
};

#endif // !INPUTSYSTEM_H
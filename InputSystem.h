#pragma once
#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <map>
#include <Windows.h>

#include "InputListener.h"

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

private:
	std::map<InputListener *, InputListener *> m_map_listeners;
	UCHAR m_keys_state[256] = {};
	UCHAR m_old_keys_state[256] = {};
};

#endif // !INPUTSYSTEM_H
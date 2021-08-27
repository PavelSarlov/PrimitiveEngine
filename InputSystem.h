#pragma once
#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "InputListener.h"

#include <unordered_set>

class InputSystem
{
private:
	InputSystem();
	~InputSystem();

public:
	static InputSystem *get();
	static void create();
	static void release();

public:
	void update();
	void addListener(InputListener *listener);
	void removeListener(InputListener *listener);
	void setCursorPos(const POINT &pos);
	void showCursor(bool show);

private:
	std::unordered_set<InputListener *> m_listeners;
	short m_keys_state[256] = { 0 };
	short m_old_keys_state[256] = { 0 };
	POINT m_old_mouse_pos = {};
	bool m_first_time = true;
	static InputSystem *m_system;
};

#endif // !INPUTSYSTEM_H
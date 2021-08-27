// MIT License
// 
// C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)
// 
// Copyright (c) 2019-2021, PardCode
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "InputSystem.h"

#include <Windows.h>

InputSystem *InputSystem::m_system = nullptr;

InputSystem::InputSystem()
{}

InputSystem::~InputSystem()
{
	InputSystem::m_system = nullptr;
}

InputSystem *InputSystem::get()
{
	return InputSystem::m_system;
}

void InputSystem::create()
{
	if(InputSystem::m_system) throw std::exception("InputSystem already created");
	InputSystem::m_system = new InputSystem();
}

void InputSystem::release()
{
	if(!InputSystem::m_system) return;
	delete InputSystem::m_system;
}

void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if(this->m_first_time)
	{
		this->m_old_mouse_pos = { current_mouse_pos.x, current_mouse_pos.y };
		this->m_first_time = false;
	}

	if(current_mouse_pos.x != this->m_old_mouse_pos.x || current_mouse_pos.y != this->m_old_mouse_pos.y)
	{
		for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); it++)
		{
			(*it)->onMouseMove({ current_mouse_pos.x, current_mouse_pos.y });
		}
	}
	this->m_old_mouse_pos = { current_mouse_pos.x, current_mouse_pos.y };

	for(UINT i = 0; i < 256; i++)
	{
		this->m_keys_state[i] = ::GetAsyncKeyState(i);

		// key is down
		if(this->m_keys_state[i] & 0x8001)
		{
			for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); it++)
			{
				if(i == VK_LBUTTON)
				{
					if(this->m_keys_state[i] != this->m_old_keys_state[i])
					{
						(*it)->onLeftMouseDown({ current_mouse_pos.x, current_mouse_pos.y });
					}
				}
				else if(i == VK_RBUTTON)
				{
					if(this->m_keys_state[i] != this->m_old_keys_state[i])
					{
						(*it)->onRightMouseDown({ current_mouse_pos.x, current_mouse_pos.y });
					}
				}
				else
				{
					(*it)->onKeyDown(i);
				}
			}
		}
		// key is up
		else
		{
			if(this->m_keys_state[i] != this->m_old_keys_state[i])
			{
				for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); it++)
				{
					if(i == VK_LBUTTON)
					{
						(*it)->onLeftMouseUp({ current_mouse_pos.x, current_mouse_pos.y });
					}
					else if(i == VK_RBUTTON)
					{
						(*it)->onRightMouseUp({ current_mouse_pos.x, current_mouse_pos.y });
					}
					else
					{
						(*it)->onKeyUp(i);
					}
				}
			}
		}
		// store current keys state to old keys state buffer
		::memcpy(this->m_old_keys_state, this->m_keys_state, sizeof(short) * 256);
	}
}

void InputSystem::addListener(InputListener *listener)
{
	this->m_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener *listener)
{
	this->m_listeners.erase(listener);
}

void InputSystem::setCursorPos(const POINT &pos)
{
	::SetCursorPos(pos.x, pos.y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

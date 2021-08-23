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

	if(::GetKeyboardState(m_keys_state))
	{
		for(USHORT i = 0; i < 256; i++)
		{
			// key is down
			if(this->m_keys_state[i] & 0x80)
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
		}
		// store current keys state to old keys state buffer
		::memcpy(this->m_old_keys_state, this->m_keys_state, sizeof(unsigned char) * 256);
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

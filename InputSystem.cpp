#include "InputSystem.h"

InputSystem::InputSystem()
{
	POINT current_moust_pos = {};
	::GetCursorPos(&current_moust_pos);
	this->m_old_mouse_pos = Point(current_moust_pos.x, current_moust_pos.y);
}

InputSystem::~InputSystem()
{}

InputSystem *InputSystem::get()
{
	static InputSystem inSystem;
	return &inSystem;
}

void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	/*if(this->m_first_time)
	{
		this->m_old_mouse_pos = Point(current_moust_pos.x, current_moust_pos.y);
		this->m_first_time = false;
	}*/

	if(current_mouse_pos.x != this->m_old_mouse_pos.m_x || current_mouse_pos.y != this->m_old_mouse_pos.m_y)
	{
		for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); it++)
		{
			(*it)->onMouseMove(Point(current_mouse_pos.x, current_mouse_pos.y));
		}
	}
	this->m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);

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
							(*it)->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
					}
					else if(i == VK_RBUTTON)
					{
						if(this->m_keys_state[i] != this->m_old_keys_state[i])
						{
							(*it)->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
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
							(*it)->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if(i == VK_RBUTTON)
						{
							(*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
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

void InputSystem::setCursorPos(const Point &pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

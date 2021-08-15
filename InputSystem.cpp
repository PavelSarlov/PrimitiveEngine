#include "InputSystem.h"

InputSystem::InputSystem()
{}

InputSystem::~InputSystem()
{}

InputSystem *InputSystem::get()
{
	static InputSystem inSystem;
	return &inSystem;
}

void InputSystem::update()
{
	if(::GetKeyboardState(m_keys_state))
	{
		for(USHORT i = 0; i < 256; i++)
		{
			// key is down
			if(this->m_keys_state[i] & 0x80)
			{
				for(auto it = this->m_map_listeners.begin(); it != this->m_map_listeners.end(); it++)
				{
					it->second->onKeyDown(i);
				}
			}
			// key is up
			else
			{
				if(this->m_keys_state[i] != this->m_old_keys_state[i])
				{
					for(auto it = this->m_map_listeners.begin(); it != this->m_map_listeners.end(); it++)
					{
						it->second->onKeyUp(i);
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
	this->m_map_listeners.insert(std::make_pair<InputListener *, InputListener *>(std::forward<InputListener *>(listener), std::forward<InputListener *>(listener)));
}

void InputSystem::removeListener(InputListener *listener)
{
	auto it = this->m_map_listeners.find(listener);

	if(it != this->m_map_listeners.end())
	{
		this->m_map_listeners.erase(it);
	}
}

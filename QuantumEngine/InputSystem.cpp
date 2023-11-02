#include "stdafx.h"
#include "InputSystem.h"
#include <iostream>


InputSystem::InputSystem() {}
InputSystem::~InputSystem() {}

void InputSystem::FireKeyDown(WPARAM wparam)
{
	m_keyState[wparam] = true;
	ExecuteCallbacks(wparam);
}

void InputSystem::FireKeyUp(WPARAM wparam)
{
	m_keyState[wparam] = false;
	ExecuteCallbacks(wparam);
}

void InputSystem::MouseDown(WPARAM wparam)
{
	m_mouseState[wparam] = true;
	ExecuteCallbacks(wparam);
}

void InputSystem::MouseUp(WPARAM wparam)
{
	m_mouseState[wparam] = false;
	ExecuteCallbacks(wparam);
}

void InputSystem::RegisterCallback(InputCallback* callback)
{
	m_callbacklist.push_back(callback);
}

void InputSystem::ExecuteCallbacks(WPARAM wparam)
{
	for (auto& callback : m_callbacklist)
	{
		callback->OnKeyDown(wparam);
	}
}

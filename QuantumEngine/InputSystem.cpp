#include "stdafx.h"
#include "InputSystem.h"

#include <string.h>



InputSystem::InputSystem()
	: m_cursorX(0), m_cursorY(0), m_lastCursorX(0), m_lastCursorY(0)
{
	memset(m_keyState, 0, sizeof(m_keyState));
}

InputSystem::~InputSystem()
{

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

void InputSystem::FireKeyDown(unsigned short vkCode)
{
	m_keyState[vkCode] = true;

	for (auto& callback : m_callbacklist)
	{
		callback->OnKeyDown(vkCode);
	}
}

void InputSystem::FireKeyUp(unsigned short vkCode)
{
	m_keyState[vkCode] = false;
}

void InputSystem::FireMouseMove(int newX, int newY)
{
	m_lastCursorX = m_cursorX;
	m_lastCursorY = m_cursorY;

	m_cursorX = newX;
	m_cursorY = newY;
}

void InputSystem::FireMouseDown(int button)
{
	for (auto& callback : m_callbacklist)
	{
		callback->OnMouseDown(button);
	}
}


void InputSystem::RegisterCallback(InputCallback* callback)
{
	m_callbacklist.push_back(callback);
}

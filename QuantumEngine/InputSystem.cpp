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

void InputSystem::MouseDown(unsigned short vkCode)
{
	m_mouseState[vkCode] = true;
	if (vkCode == 1) {
		lMouseEvent = 0;
	}
	if (vkCode == 2) {
		rMouseEvent = 0;
	}
	for (auto& callback : m_callbacklist)
	{
		callback->OnMouseDown(vkCode);
	}
}

void InputSystem::MouseUp(unsigned short vkCode)
{
	if (rMouseEvent == 517) {
		m_mouseState[2] = false;
	}
	if (lMouseEvent == 514) {
		m_mouseState[1] = false;
	}
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

void InputSystem::RegisterCallback(InputCallback* callback)
{
	m_callbacklist.push_back(callback);
}

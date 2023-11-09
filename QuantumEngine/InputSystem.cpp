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

#include "Game.h"

void InputSystem::FireMouseDown(int vkCode)
{
	m_keyState[vkCode] = true;

	if (Game::getInstance().getUISystem().OnLeftClick(m_cursorX, m_cursorY, Game::getInstance().getOpenWidget())) {
		return;
	}

	for (auto& callback : m_callbacklist) callback->OnMouseDown(vkCode);
}

void InputSystem::FireMouseUp(int vkCode)
{
	m_keyState[vkCode] = false;

	for (auto& callback : m_callbacklist) callback->OnMouseUp(vkCode);
}



void InputSystem::RegisterCallback(InputCallback* callback)
{
	m_callbacklist.push_back(callback);
}

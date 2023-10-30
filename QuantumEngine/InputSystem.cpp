#include "stdafx.h"
#include "InputSystem.h"
#include <iostream>
#include "TestInput.h"

Test test;

InputSystem::InputSystem() {}
InputSystem::~InputSystem() {}

void InputSystem::FireKeyDown(WPARAM wparam) {
	m_keyState[wparam] = true;
	ExecuteCallbacks(wparam);
}

void InputSystem::FireKeyUp(WPARAM wparam)
{
	m_keyState[wparam] = false;
	ExecuteCallbacks(wparam);
}

void InputSystem::MouseDown(int button)
{
	switch (button) {
	case 1:
		std::cout << "left click";
		break;
	case 2:
		std::cout << "right click";
		break;
	default:
		break;
	}
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

#include "InputSystem.h"
#include <iostream>
#include "TestInput.h"

Test test;

InputSystem::InputSystem() {}
InputSystem::~InputSystem() {}

void InputSystem::KeyDown(WPARAM wparam) {
		m_wparam = wparam;
		ExecuteCallbacks();
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

void InputSystem::ExecuteCallbacks()
{
	for (auto& callback : m_callbacklist)
	{
		callback->RegisterInput();
	}
}
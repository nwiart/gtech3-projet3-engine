#include "stdafx.h"
#include "InputSystem.h"

InputSystem::InputSystem() {}
InputSystem::~InputSystem() {}

void InputSystem::KeyDown(WPARAM wparam) {
	switch (wparam) {
	case VK_UP:
		ExecuteCallbacks();
		break;
	case VK_DOWN:
		std::cout << "down";
		break;
	case VK_LEFT:
		std::cout << "left";
		break;
	case VK_RIGHT:
		std::cout << "right";
		break;
	default:
		break;
	}
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
	m_callbacks.push_back(callback);
}

void InputSystem::ExecuteCallbacks()
{
	for (auto& callback : m_callbacks)
	{
		callback->RegisterInput();
	}
}
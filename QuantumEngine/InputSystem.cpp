#include "stdafx.h"
#include "InputSystem.h"

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem() {}

void InputSystem::KeyDown(WPARAM wparam) {
	switch (wparam) {
	case VK_UP:
		std::cout << "up";
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

#include "InputSystem.h"
#include <iostream>
#include "TestInput.h"



Test::Test()
{
	InputSystem::Get().RegisterCallback(this);
}

Test::~Test()
{
}

void Test::RegisterInput()
{
	switch (InputSystem::Get().m_wparam) {
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


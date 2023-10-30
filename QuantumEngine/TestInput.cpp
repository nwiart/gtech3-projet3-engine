#include "stdafx.h"
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

void Test::OnKeyDown(WPARAM wparam)
{
	switch (wparam) {
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	default:
		break;
	}
}


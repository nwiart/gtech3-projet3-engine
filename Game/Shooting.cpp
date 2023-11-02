#include "Shooting.h"

#include <iostream>
#include "stdafx.h"
#include "InputSystem.h"

void Shooting::Shoot()
{
	if (InputSystem::Get().isMouseDown(1)) {
		std::cout << "tamere";
	}
}

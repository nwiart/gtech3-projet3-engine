#include "stdafx.h"
#include "EntityController.h"

void EntityController::OnKeyDown(WPARAM wparam)
{
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

void EntityController::SetCamera()
{
	//int mouseCurrStateX = m_graphics->cursorX;
	//int mouseCurrStateY = m_graphics->cursorY;
	//mouseCurrStateX = mouseCurrStateX - m_graphics->m_renderWidth / 2;
	//mouseCurrStateY = mouseCurrStateY - m_graphics->m_renderHeight / 2;

	//int deadZoneX = (m_graphics->m_renderWidth * 5) / 100;
	//int deadZoneY = (m_graphics->m_renderHeight * 5) / 100;

	//mouseLastStateX = mouseCurrStateX;
	//mouseLastStateY = mouseCurrStateY;
	//if (mouseCurrStateX < deadZoneX && mouseCurrStateX > -deadZoneX && mouseCurrStateY < deadZoneY && mouseCurrStateY > -deadZoneY)
	//	return;
	//else {
	//	camYaw += mouseLastStateX * 0.0001f;
	//	camPitch += mouseLastStateY * 0.0001f;

	//	// Limit pitch to straight up or straight down. To Remove
	//	if (camPitch > 1.570796f)
	//		camPitch = 1.570796f;
	//	if (camPitch < -1.570796f)
	//		camPitch = -1.570796f;
	//}
}

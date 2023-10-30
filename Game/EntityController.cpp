#include "EntityCamera.h"
#include "stdafx.h"
#include "Graphics.h"
#include "EntityController.h"
#include "InputSystem.h"

EntityController::EntityController()
{
	InputSystem::Get().RegisterCallback(this);
}

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

void EntityController::SetCamera(Graphics* graphics)
{
	m_graphics = graphics;
	int mouseCurrStateX = m_graphics->cursorX;
	int mouseCurrStateY = m_graphics->cursorY;
	mouseCurrStateX = mouseCurrStateX - m_graphics->m_renderWidth / 2;
	mouseCurrStateY = mouseCurrStateY - m_graphics->m_renderHeight / 2;

	int deadZoneX = (m_graphics->m_renderWidth * 5) / 100;
	int deadZoneY = (m_graphics->m_renderHeight * 5) / 100;

	mouseLastStateX = mouseCurrStateX;
	mouseLastStateY = mouseCurrStateY;
	if (mouseCurrStateX < deadZoneX && mouseCurrStateX > -deadZoneX && mouseCurrStateY < deadZoneY && mouseCurrStateY > -deadZoneY)
		return;
	else {
		m_graphics->camYaw += mouseLastStateX * 0.0001f;
		m_graphics->camPitch += mouseLastStateY * 0.0001f;

		// Limit pitch to straight up or straight down. To Remove
		if (m_graphics->camPitch > 1.570796f)
			m_graphics->camPitch = 1.570796f;
		if (m_graphics->camPitch < -1.570796f)
			m_graphics->camPitch = -1.570796f;
	}
}

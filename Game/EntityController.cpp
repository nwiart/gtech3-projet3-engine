#include "EntityController.h"
#include "InputSystem.h"

void EntityController::SetCamera()
{
	int mouseCurrStateX = InputSystem::Get().cursorX;
	int mouseCurrStateY = InputSystem::Get().cursorY;
	mouseCurrStateX = mouseCurrStateX - m_renderWidth / 2;
	mouseCurrStateY = mouseCurrStateY - m_renderHeight / 2;

	int deadZoneX = (m_renderWidth * 5) / 100;
	int deadZoneY = (m_renderHeight * 5) / 100;

	mouseLastStateX = mouseCurrStateX;
	mouseLastStateY = mouseCurrStateY;
	if (mouseCurrStateX < deadZoneX && mouseCurrStateX > -deadZoneX && mouseCurrStateY < deadZoneY && mouseCurrStateY > -deadZoneY)
		return;
	else {
		m_camYaw += mouseLastStateX * 0.0001f;
		m_camPitch += mouseLastStateY * 0.0001f;

		// Limit pitch to straight up or straight down. To Remove
		if (m_camPitch > 1.570796f)
			m_camPitch = 1.570796f;
		if (m_camPitch < -1.570796f)
			m_camPitch = -1.570796f;
	}
}

EntityController::EntityController()
{
}

void EntityController::OnUpdate(Timer timer)
{
	SetCamera();
}

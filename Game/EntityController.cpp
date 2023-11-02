#include "EntityController.h"
#include "InputSystem.h"

#include "Game.h"

#include "Quantum/Math/Math.h"

using namespace DirectX;



EntityController::EntityController()
	: m_camYaw(0.0F)
	, m_camPitch(0.0F)
{
	
}

void EntityController::OnUpdate(Timer timer)
{
	UpdateCamera(timer.getDeltaTime());
}

void EntityController::UpdateCamera(float dt)
{
	// Move camera.
	float speed = InputSystem::Get().isKeyDown(VK_SHIFT) ? 3.0F : 1.0F;
	speed *= dt;
	
	XMVECTOR pos = XMLoadFloat3(&this->GetTransform().getPosition());
	XMVECTOR camForward = this->GetTransform().getForwardVector();
	XMVECTOR camRight   = this->GetTransform().getRightVector();

	if (InputSystem::Get().isKeyDown('Z')) pos += camForward * speed;
	if (InputSystem::Get().isKeyDown('S')) pos -= camForward * speed;
	if (InputSystem::Get().isKeyDown('D')) pos += camRight * speed;
	if (InputSystem::Get().isKeyDown('Q')) pos -= camRight * speed;

	XMFLOAT3 fpos; XMStoreFloat3(&fpos, pos);
	this->setPosition(fpos);


	// Rotate camera.
	int mouseCurrStateX = InputSystem::Get().cursorX;
	int mouseCurrStateY = InputSystem::Get().cursorY;
	mouseCurrStateX = mouseCurrStateX - Game::getInstance().getRenderWidth() / 2;
	mouseCurrStateY = mouseCurrStateY - Game::getInstance().getRenderHeight() / 2;

	int deadZoneX = (Game::getInstance().getRenderWidth() * 5) / 100;
	int deadZoneY = (Game::getInstance().getRenderHeight() * 5) / 100;

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

	XMVECTOR quat = XMQuaternionRotationRollPitchYaw(m_camPitch, m_camYaw, 0.0F);
	XMFLOAT4 fquat; XMStoreFloat4(&fquat, quat);
	this->setRotation(fquat);
}

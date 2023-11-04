#include "EntityController.h"
#include "InputSystem.h"

#include "Game.h"

#include "Quantum/Math/Math.h"

#include <Windows.h> // Necessary for key codes.

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
	int mouseX = InputSystem::Get().getMouseX() - Game::getInstance().getRenderWidth() / 2;
	int mouseY = InputSystem::Get().getMouseY() - Game::getInstance().getRenderHeight() / 2;

	int deadZoneX = (Game::getInstance().getRenderWidth() * 5) / 100;
	int deadZoneY = (Game::getInstance().getRenderHeight() * 5) / 100;
	bool inDeadZone = (mouseX > -deadZoneX && mouseX < deadZoneX && mouseY > -deadZoneY && mouseY < deadZoneY);

	if (!inDeadZone)
	{
		m_camYaw += mouseX * dt * 0.005F;
		m_camPitch += mouseY * dt * 0.005F;

		// Limit pitch to straight up or straight down. To Remove
		m_camPitch = Quantum::Math::clamp(m_camPitch, -1.570796f, 1.570796f);
	}

	XMVECTOR quat = XMQuaternionRotationRollPitchYaw(m_camPitch, m_camYaw, 0.0F);
	XMFLOAT4 fquat; XMStoreFloat4(&fquat, quat);
	this->setRotation(fquat);
}

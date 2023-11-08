#include "EntityController.h"
#include "InputSystem.h"

#include "Game.h"

#include "Quantum/Math/Math.h"

#include <Windows.h> // Necessary for key codes.
#include "QuEntityRenderModel.h"
#include "Quantum/Generate/CapsuleGenerator.h"
#include "Model.h"
#include "Player.h"
#include "QuEntityPhysicsCollider.h"

using namespace DirectX;



EntityController::EntityController()
	: leftGunModel(0)
	, rightGunModel(0)
{
	
}

void EntityController::OnSpawn(QuWorld* world)
{
	// Create guns.
	Model* gunModel = new Model();
	Quantum::CapsuleGenerator::generate(gunModel);

	leftGunModel = new QuEntityRenderModel();
	leftGunModel->SetModel(gunModel);
	leftGunModel->setPosition(XMFLOAT3(-2.5f, -1.0f, 1.7f));
	leftGunModel->setRotation(XMFLOAT4(0.707, 0, 0, 0.707));
	leftGunModel->setScale(XMFLOAT3(1.f, 0.5f, 1.f));
	this->attachChild(leftGunModel);

	rightGunModel = new QuEntityRenderModel();
	rightGunModel->SetModel(gunModel);
	rightGunModel->setPosition(XMFLOAT3(2.5f, -1.0f, 1.7f));
	rightGunModel->setRotation(XMFLOAT4(-0.707, 0, 0, -0.707));
	rightGunModel->setScale(XMFLOAT3(1.f, 0.5f, 1.f));
	this->attachChild(rightGunModel);

	// Register callback (slowdown).
	m_inputCallback = new ControllerInputCallback();
	InputSystem::Get().RegisterCallback(m_inputCallback);
}

void EntityController::OnUpdate(const Timer& timer)
{
	UpdateCamera(timer.getDeltaTime());

}

void EntityController::SetPosition(DirectX::XMVECTOR Position)
{
	m_Position = Position;
}

DirectX::XMVECTOR EntityController::GetPosition()
{
	return m_Position;
}

void EntityController::UpdateCamera(float dt)
{
	// Move camera.
	float speed = InputSystem::Get().isKeyDown(VK_SHIFT) ? 6.0F : 3.0F;
	speed *= dt;
	
	XMVECTOR pos = XMLoadFloat3(&this->GetTransform().getPosition());
	XMVECTOR camForward = this->GetTransform().getForwardVector();
	XMVECTOR camRight   = this->GetTransform().getRightVector();

	float roll = 0.0F;
	if (InputSystem::Get().isKeyDown('Z')) pos += camForward * speed;
	if (InputSystem::Get().isKeyDown('S')) pos -= camForward * speed;
	if (InputSystem::Get().isKeyDown('D')) pos += camRight * speed;
	if (InputSystem::Get().isKeyDown('Q')) pos -= camRight * speed;
	if (InputSystem::Get().isKeyDown('A')) roll = 0.3 * speed;
	if (InputSystem::Get().isKeyDown('E')) roll = -0.3 * speed;

	XMFLOAT3 fpos; XMStoreFloat3(&fpos, pos);
	this->setPosition(fpos);

	// Rotate camera.
	int mouseX = InputSystem::Get().getMouseX() - Game::getInstance().getRenderWidth() / 2;
	int mouseY = InputSystem::Get().getMouseY() - Game::getInstance().getRenderHeight() / 2;

	int deadZoneX = (Game::getInstance().getRenderWidth() * 5) / 100;
	int deadZoneY = (Game::getInstance().getRenderHeight() * 5) / 100;
	bool inDeadZone = (mouseX > -deadZoneX && mouseX < deadZoneX && mouseY > -deadZoneY && mouseY < deadZoneY);

	XMVECTOR quat = XMLoadFloat4(&this->GetTransform().getRotation());

	if (!inDeadZone)
	{
		float yaw = mouseX * dt * 0.005F;
		float pitch = mouseY * dt * 0.005F;

		quat = XMQuaternionMultiply(quat, XMQuaternionRotationAxis(this->GetTransform().getUpVector(), yaw));
		quat = XMQuaternionMultiply(quat, XMQuaternionRotationAxis(this->GetTransform().getRightVector(), pitch));
	}

	quat = XMQuaternionMultiply(quat, XMQuaternionRotationAxis(this->GetTransform().getForwardVector(), roll));

	XMFLOAT4 rot; XMStoreFloat4(&rot, quat);
	this->setRotation(rot);
}



void EntityController::ControllerInputCallback::OnMouseDown(unsigned short btn)
{
	if (btn == 2)
		Game::getInstance().getTimer().setTimeDilation(0.2);
}

void EntityController::ControllerInputCallback::OnMouseUp(unsigned short btn)
{
	if (btn == 2)
		Game::getInstance().getTimer().setTimeDilation(1.0);
}

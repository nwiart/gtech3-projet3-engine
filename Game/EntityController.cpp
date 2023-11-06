#include "EntityController.h"
#include "InputSystem.h"

#include "Game.h"

#include "Quantum/Math/Math.h"

#include <Windows.h> // Necessary for key codes.
#include "QuEntityRenderModel.h"
#include "Quantum/Generate/CapsuleGenerator.h"
#include "Model.h"

using namespace DirectX;



EntityController::EntityController()
{
	
}

void EntityController::OnUpdate(const Timer& timer)
{
	UpdateCamera(timer.getDeltaTime());
}

void EntityController::OnSpawn(QuWorld* world)
{
	//Create guns
	Model* leftGun = new Model();
	Quantum::CapsuleGenerator::generate(leftGun);
	leftGunModel->SetModel(leftGun);
	leftGunModel->setPosition(XMFLOAT3(-3.0f, -1.0f, 2.0f));
	leftGunModel->setRotation(XMFLOAT4(0.707, 0, 0, 0.707));
	this->attachChild(leftGunModel);

	Model* rightGun = new Model();
	Quantum::CapsuleGenerator::generate(rightGun);
	rightGunModel->SetModel(rightGun);
	rightGunModel->setPosition(XMFLOAT3(3.0f, -1.0f, 2.0f));
	rightGunModel->setRotation(XMFLOAT4(-0.707, 0, 0, -0.707));
	this->attachChild(rightGunModel);
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

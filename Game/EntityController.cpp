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
#include "QuWorld.h"
#include "QuEntityCamera.h"
#include "Texture2D.h"
#include "Shooting.h"
#include "EntityParticleSmoke.h"
#include "ResourceLibrary.h"

using namespace DirectX;

EntityController::EntityController()
	: leftGunModel(0)
	, rightGunModel(0)
{
	
}

void ControllerCollider::onCollide(QuEntity* e)
{
	std::cout << hitCount << std::endl;
	hitCount++;
	if (hitCount >= 5) {
		this->Destroy(true);
	}
}

ControllerCollider::ControllerCollider(float radius)
	: QuEntityPhysicsCollider(radius, MOTION_DYNAMIC)
{

}

void EntityController::OnSpawn(QuWorld* world)
{
	// Create guns.
	Model* gunModel = new Model();
	Quantum::CapsuleGenerator::generate(gunModel);

	// Register callback (slowdown).
	m_inputCallback = new ControllerInputCallback();
	InputSystem::Get().RegisterCallback(m_inputCallback);

	// Create collider.
	controllerCollider = new ControllerCollider(1.0F);
	getWorld()->attachChild(controllerCollider);
	controllerCollider->setPosition(this->GetPosition());

	leftGunModel = new QuEntityRenderModel();
	leftGunModel->SetModel(gunModel);
	leftGunModel->setPosition(XMFLOAT3(-2.5f, -1.0f, 1.7f));
	leftGunModel->setRotation(XMFLOAT4(0.707, 0, 0, 0.707));
	leftGunModel->setScale(XMFLOAT3(1.f, 0.5f, 1.f));
	controllerCollider->attachChild(leftGunModel);

	rightGunModel = new QuEntityRenderModel();
	rightGunModel->SetModel(gunModel);
	rightGunModel->setPosition(XMFLOAT3(2.5f, -1.0f, 1.7f));
	rightGunModel->setRotation(XMFLOAT4(-0.707, 0, 0, -0.707));
	rightGunModel->setScale(XMFLOAT3(1.f, 0.5f, 1.f));
	controllerCollider->attachChild(rightGunModel);

	gunModel->setDefaultTexture(&ResourceLibrary::Get().metalic);

	shooting = new Shooting();
	shooting->setRotation(controllerCollider->GetTransform().getRotation());
	shooting->setPosition(rightGunModel->GetTransform().getPosition());
	controllerCollider->attachChild(shooting);

	camera = new QuEntityCamera();
	controllerCollider->attachChild(camera);

	EntityParticleSmoke* pe0 = new EntityParticleSmoke(&ResourceLibrary::Get().smoke);
	pe0->setPosition(XMFLOAT3(-3, -1.0F, 3));
	controllerCollider->attachChild(pe0);

	EntityParticleSmoke* pe1 = new EntityParticleSmoke(&ResourceLibrary::Get().smoke);
	pe1->setPosition(XMFLOAT3(3, -1.0F, 3));
	controllerCollider->attachChild(pe1);
}

void EntityController::OnUpdate(const Timer& timer)
{
	if (InputSystem::Get().isMouseDown(1) && !shooting->alreadyShooting) {
		if (LeftRight) {
			shooting->setPosition(rightGunModel->GetTransform().getPosition());
			LeftRight = false;
		}
		else if (!LeftRight) {
			shooting->setPosition(leftGunModel->GetTransform().getPosition());
			LeftRight = true;
		}
	}
	UpdateCamera(timer.getDeltaTime());

	shooting->DetectShooting(controllerCollider->GetLinearVelocity());
}

QuEntityPhysicsCollider* EntityController::GetCollider()
{
	return nullptr;
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
	float fov = camera->getFOV();

	if ((InputSystem::Get().isKeyDown(VK_SHIFT) && fov < 80)) {
		fov += dt * speed;
		camera->setFOV(fov);
	}
	else if(fov >= 70) {
		fov -= dt * speed;
		camera->setFOV(fov);
	}

	speed *= dt;

	XMVECTOR vel = controllerCollider->GetLinearVelocity();
	XMVECTOR camForward = controllerCollider->GetTransform().getForwardVector();
	XMVECTOR camRight = controllerCollider->GetTransform().getRightVector();

	float roll = 0.0F;
	if (InputSystem::Get().isKeyDown('Z')) vel += camForward * speed;
	if (InputSystem::Get().isKeyDown('S')) vel -= camForward * speed;
	if (InputSystem::Get().isKeyDown('D')) vel += camRight * speed;
	if (InputSystem::Get().isKeyDown('Q')) vel -= camRight * speed;
	if (InputSystem::Get().isKeyDown('A')) roll = 0.3 * speed;
	if (InputSystem::Get().isKeyDown('E')) roll = -0.3 * speed;

	controllerCollider->setLinearVelocity(vel);

	// Rotate camera.
	int mouseX = InputSystem::Get().getMouseX() - Game::getInstance().getRenderWidth() / 2;
	int mouseY = InputSystem::Get().getMouseY() - Game::getInstance().getRenderHeight() / 2;

	int deadZoneX = (Game::getInstance().getRenderWidth() * 5) / 100;
	int deadZoneY = (Game::getInstance().getRenderHeight() * 5) / 100;
	bool inDeadZone = (mouseX > -deadZoneX && mouseX < deadZoneX && mouseY > -deadZoneY && mouseY < deadZoneY);

	XMVECTOR quat = XMLoadFloat4(&controllerCollider->GetTransform().getRotation());

	if (!inDeadZone)
	{
		float yaw = mouseX * dt * 0.005F;
		float pitch = mouseY * dt * 0.005F;

		quat = XMQuaternionMultiply(quat, XMQuaternionRotationAxis(controllerCollider->GetTransform().getUpVector(), yaw));
		quat = XMQuaternionMultiply(quat, XMQuaternionRotationAxis(controllerCollider->GetTransform().getRightVector(), pitch));
	}

	quat = XMQuaternionMultiply(quat, XMQuaternionRotationAxis(controllerCollider->GetTransform().getForwardVector(), roll));

	controllerCollider->setRotation(quat);
}

void EntityController::ControllerInputCallback::OnMouseDown(unsigned short btn)
{
	if (btn == 2) {
		Game::getInstance().getTimer().setTimeDilation(0.2);
	}
}

void EntityController::ControllerInputCallback::OnMouseUp(unsigned short btn)
{
	if (btn == 2) {
		Game::getInstance().getTimer().setTimeDilation(1.0);
	}
}

#pragma once

#include "QuEntity.h"
#include "InputCallback.h"
#include "QuEntityPhysicsCollider.h"

class QuEntityRenderModel;
class QuEntityCamera;
class ControllerCollider;
class Shooting;

class EntityController : public QuEntity
{
public:

	EntityController();

	QuEntityCamera* camera;
	ControllerCollider* controllerCollider;
	Shooting* shooting;

	virtual void OnSpawn(QuWorld* world) override;
	virtual void OnUpdate(const Timer& timer) override;
	QuEntityPhysicsCollider* GetCollider();

	void SetPosition(DirectX::XMVECTOR Position);
	DirectX::XMVECTOR GetPosition();

	DirectX::XMVECTOR m_Position;

private:

	void UpdateCamera(float dt);
	bool LeftRight = false;

	class ControllerInputCallback : public InputCallback
	{
	public:

		virtual void OnMouseDown(unsigned short btn) override;
		virtual void OnMouseUp(unsigned short btn) override;
	};

	ControllerInputCallback* m_inputCallback;

	QuEntityRenderModel* leftGunModel;
	QuEntityRenderModel* rightGunModel;

	int mouseLastStateX;
	int mouseLastStateY;
};

class ControllerCollider : public QuEntityPhysicsCollider
{
public:
	int hitCount = 0;
	ControllerCollider(float radius);
	virtual void onCollide(QuEntity* e) override;
};
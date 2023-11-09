#include "Shooting.h"

#include <iostream>
#include "stdafx.h"
#include "InputSystem.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Bullet.h"
#include "Model.h"

#include "QuWorld.h"

Shooting::Shooting()
{
}

void Shooting::DetectShooting(DirectX::FXMVECTOR addedVelocity)
{
	if (InputSystem::Get().isMouseDown(1) && !alreadyShooting) {
		alreadyShooting = true;
		InstantiateBullet(addedVelocity);
	}
}
void Shooting::EnemyShooting(DirectX::FXMVECTOR addedVelocity)
{
		alreadyShooting = true;
		InstantiateBullet(addedVelocity);
	
}

void Shooting::OnUpdate(const Timer& timer)
{
	CoolDown(timer.getDeltaTime());
}

#include "EntityController.h"
void Shooting::OnSpawn(QuWorld* world)
{
	
}

void Shooting::InstantiateBullet(DirectX::FXMVECTOR addedVelocity) {
	Bullet* bullet = new Bullet;
	XMFLOAT3 pos; XMStoreFloat3(&pos, getWorldPosition());
	bullet->setPosition(pos);
	XMFLOAT4 rot; XMStoreFloat4(&rot, getWorldRotation());
	bullet->setRotation(rot);
	bullet->setSourceVelocity(addedVelocity);
	getWorld()->attachChild(bullet);
}

void Shooting::CoolDown(float dt) {
	if (alreadyShooting) {
		m_coolDown -= dt;
		if (m_coolDown <= 0) {
			m_coolDown = 0.2f;
			alreadyShooting = false;
		}
	}
}

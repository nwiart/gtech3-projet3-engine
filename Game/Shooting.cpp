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

void Shooting::DetectShooting()
{
	if (InputSystem::Get().isMouseDown(1) && !alreadyShooting) {
		alreadyShooting = true;
		InstantiateBullet();
	}
}

void Shooting::OnUpdate(const Timer& timer)
{
	DetectShooting();
	CoolDown(timer.getDeltaTime());
}

#include "EntityController.h"
void Shooting::OnSpawn(QuWorld* world)
{
	
}

void Shooting::InstantiateBullet() {
	Bullet* bullet = new Bullet;
	XMFLOAT3 pos; XMStoreFloat3(&pos, getWorldPosition());
	bullet->setPosition(pos);
	XMFLOAT4 rot; XMStoreFloat4(&rot, getWorldRotation());
	bullet->setRotation(rot);
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

#include "Shooting.h"

#include <iostream>
#include "stdafx.h"
#include "InputSystem.h"
#include "Quantum/Generate/SphereGenerator.h"

Shooting::Shooting()
{
}

void Shooting::Shoot()
{
	if (InputSystem::Get().isMouseDown(1) && !bullet->alreadyShooting) {
		std::cout << "Shoot" << std::endl;
		InstantiateBullet();
	}
}

void Shooting::OnUpdate(Timer timer)
{
	this->attachChild(bullet);
	Shoot();
	bullet->MoveBullet(timer.getDeltaTime());
	CoolDown(timer.getDeltaTime());
	bullet->DestroyBullet();
}

void Shooting::OnSpawn()
{
}

void Shooting::InstantiateBullet() {
	bullet->Shoot();
}

void Shooting::CoolDown(float dt) {
	if (bullet->alreadyShooting) {
		m_coolDown -= dt;
		if (m_coolDown <= 0) {
			m_coolDown = 0.5f;
			bullet->alreadyShooting = false;
		}
	}
}
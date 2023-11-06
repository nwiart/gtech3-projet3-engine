#include "Shooting.h"

#include <iostream>
#include "stdafx.h"
#include "InputSystem.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Bullet.h"

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

void Shooting::OnUpdate(Timer timer)
{
	DetectShooting();
	CoolDown(timer.getDeltaTime());
}

void Shooting::OnSpawn()
{
}

void Shooting::InstantiateBullet() {
	Bullet* bullet = new Bullet;
	this->attachChild(bullet);
	bullet->Shoot();
}

void Shooting::CoolDown(float dt) {
	if (alreadyShooting) {
		m_coolDown -= dt;
		if (m_coolDown <= 0) {
			m_coolDown = 0.5f;
			alreadyShooting = false;
		}
	}
}
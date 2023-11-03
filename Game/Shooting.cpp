#include "Shooting.h"

#include <iostream>
#include "stdafx.h"
#include "InputSystem.h"
#include "Quantum/Generate/SphereGenerator.h"

void Shooting::Shoot()
{
	if (InputSystem::Get().isMouseDown(1) && !alreadyShooting) {
		InstantiateBullet();
	}
}

void Shooting::OnUpdate(Timer timer)
{
	Shoot();
	MoveBullet();
	CoolDown(timer.getDeltaTime());
	DestroyBullet();
}

void Shooting::OnSpawn()
{
}

void Shooting::InstantiateBullet() {
	alreadyShooting = true;
	sphereEntity = new QuEntityRenderModel;
	Model* sphere = new Model();
	Quantum::SphereGenerator::generate(sphere);
	sphereEntity->SetModel(sphere);
	sphereEntity->setPosition(DirectX::XMFLOAT3(0, 0, 0));
	this->attachChild(sphereEntity);
	m_bullets.push_back(sphereEntity);
}

void Shooting::MoveBullet() {
	if (m_bullets.size() > 0) {
		for (int i = 0; i < m_bullets.size(); i++) {
			XMVECTOR vectForward = XMVectorSet(0,0,1,0);
			XMVECTOR pos = XMLoadFloat3(&m_bullets[i]->GetTransform().getPosition());
			pos += vectForward;

			XMFLOAT3 fpos;
			XMStoreFloat3(&fpos, pos);

			m_bullets[i]->setPosition(fpos);
		}
	}
}

void Shooting::DestroyBullet() {
	for (int i = 0; i < m_bullets.size(); i++) {
		if (m_bullets[i]->GetTransform().getPosition().z > 100) {
			//m_bullets[i]->DetachFromParent();
			m_bullets.erase(m_bullets.begin() + i);
		}
	}
}

void Shooting::CoolDown(float dt) {
	m_coolDown -= dt;
	if (m_coolDown <= 0) {
		m_coolDown = 0.5f;
		alreadyShooting = false;
	}
}

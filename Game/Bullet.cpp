#include "Bullet.h"

#include <iostream>
#include "stdafx.h"
#include "InputSystem.h"
#include "Quantum/Generate/SphereGenerator.h"

void Bullet::OnUpdate(Timer timer)
{
}

void Bullet::OnSpawn()
{
}

void Bullet::Shoot() {
	alreadyShooting = true;
	sphereEntity = new QuEntityRenderModel;
	Model* sphere = new Model();
	Quantum::SphereGenerator::generate(sphere);
	sphereEntity->SetModel(sphere);
	getWorld()->attachChild(sphereEntity);
	m_bullets.push_back(sphereEntity);
}

void Bullet::MoveBullet(float dt) {
	if (m_bullets.size() > 0) {
		for (int i = 0; i < m_bullets.size(); i++) {
			XMVECTOR vectForward = getForwardVector();
			XMVECTOR pos = XMLoadFloat3(&m_bullets[i]->GetTransform().getPosition());
			pos += vectForward * dt * 100;

			XMFLOAT3 fpos;
			XMStoreFloat3(&fpos, pos);

			m_bullets[i]->setPosition(fpos);
			//std::cout << m_bullets[i]->GetTransform().getPosition().z << std::endl;
		}
	}
}

void Bullet::DestroyBullet() {
	if (m_bullets.size() > 0) {
		for (int i = 0; i < m_bullets.size(); i++) {
			if (m_bullets[i]->GetTransform().getPosition().z > 100) {
				//m_bullets[i]->DetachFromParent();
				m_bullets.erase(m_bullets.begin() + i);
			}
		}
	}
}
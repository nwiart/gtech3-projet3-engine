#include "Bullet.h"

#include <iostream>
#include "stdafx.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Model.h"

#include "QuWorld.h"

#include "QuEntityRenderModel.h"


void Bullet::OnUpdate(const Timer& timer)
{
	MoveBullet(timer.getDeltaTime());

	if (this->GetTransform().getPosition().z > 100)
		this->Destroy(true);
}

void Bullet::OnSpawn(QuWorld* world)
{
	this->Shoot();
}

void Bullet::Shoot() {
	alreadyShooting = true;

	Model* sphere = new Model();
	Quantum::SphereGenerator::generate(sphere);

	QuEntityRenderModel* sphereEntity = new QuEntityRenderModel;
	sphereEntity->SetModel(sphere);
	sphereEntity->setScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	this->attachChild(sphereEntity);
}

void Bullet::MoveBullet(float dt) {
	XMVECTOR vectForward = this->getForwardVector();
	XMVECTOR pos = XMLoadFloat3(&this->GetTransform().getPosition());

	pos += vectForward * dt * 100;

	XMFLOAT3 fpos;
	XMStoreFloat3(&fpos, pos);

	this->setPosition(fpos);
}

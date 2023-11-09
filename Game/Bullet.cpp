#include "Bullet.h"

#include <iostream>
#include "stdafx.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Model.h"

#include "QuWorld.h"

#include "QuEntityRenderModel.h"


BulletCollider::BulletCollider(Bullet* b): QuEntityPhysicsCollider(0.25F, MOTION_DYNAMIC), m_bullet(b)
{

}

void BulletCollider::onCollide(QuEntity* e)
{
	m_bullet->Destroy(true);
	this->Destroy(true);
}


void Bullet::OnUpdate(const Timer& timer)
{
	//MoveBullet(timer.getDeltaTime());

	time += timer.getDeltaTime();


	if (time > 50) {
		this->Destroy(true);
	}
}

void Bullet::OnSpawn(QuWorld* world)
{
	this->Shoot();
}

void Bullet::Shoot() {
	alreadyShooting = true;

	Model* sphere = new Model();
	Quantum::SphereGenerator::generate(sphere, 1.0F);

	BulletCollider* collider = new BulletCollider(this);
	collider->setPosition(this->getWorldPosition());
	collider->applyImpulse(this->getForwardVector() * 100);

	QuEntityRenderModel* sphereEntity = new QuEntityRenderModel;
	sphereEntity->SetModel(sphere);
	sphereEntity->setScale(XMFLOAT3(0.25f, 0.25f, 0.25f));

	getWorld()->attachChild(collider);
	collider->attachChild(sphereEntity);
}


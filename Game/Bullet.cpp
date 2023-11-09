#include "Bullet.h"

#include <iostream>
#include "stdafx.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Model.h"

#include "QuWorld.h"
#include "EntityController.h"
#include "ResourceLibrary.h"

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
	time += timer.getDeltaTime();
	if (time > 5) {
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
	sphere->setDefaultTexture(&ResourceLibrary::Get().metalic);
	Quantum::SphereGenerator::generate(sphere, 1.0F);

	BulletCollider* collider = new BulletCollider(this);
	collider->setPosition(this->getWorldPosition());
	collider->applyImpulse(this->getForwardVector() * 600 + m_sourceVelocity);

	QuEntityRenderModel* sphereEntity = new QuEntityRenderModel;
	sphereEntity->SetModel(sphere);
	sphereEntity->setScale(XMFLOAT3(0.5f, 0.5f, 0.5f));

	getWorld()->attachChild(collider);
	collider->attachChild(sphereEntity);
}

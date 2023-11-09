#include "stdafx.h"
#include "EntityEnemySwarm.h"

#include "Quantum/Math/Math.h"
#include "Quantum/Generate/BoxGenerator.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Quantum/Generate/CapsuleGenerator.h"
#include "Model.h"

#include "QuEntityPhysicsCollider.h"
#include "EntityController.h"
#include "Player.h"
#include "QuWorld.h"
#include "Shooting.h"

#include "ResourceLibrary.h"
#include "Timer.h"
#include "HUDGame.h"


DirectX::XMVECTOR m_PlayerPosition;


ShipCollider::ShipCollider(Enemy* e, float radius)
	: QuEntityPhysicsCollider(radius, MOTION_DYNAMIC)
{
	m_enemy = e;
}

void ShipCollider::onCollide(QuEntity* e)
{
	Player::GetHUD()->setScore(1);
	this->Destroy(true);
	m_enemy->Destroy(true);
}


EntityEnemySwarm::EntityEnemySwarm()
{
	m_PlayerPosition = XMVectorSet(0, 0, 0, 0);
}
	
void EntityEnemySwarm::OnUpdate(const Timer& timer)
{
	if (m_Enemy.size() < ENEMY_COUNT)
		SpawnEntityEnemySwarm(timer);

}

void EntityEnemySwarm::OnSpawn(QuWorld* world)
{
	
}

void EntityEnemySwarm::SpawnEntityEnemySwarm(const Timer& timer)
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		Enemy* e = new Enemy();
		getWorld()->attachChild(e);
		m_Enemy.push_back(e);
	}
}

void EntityEnemySwarm::UpdateDirection(XMVECTOR axis , const Timer& timer, ShipCollider* shipCollider)
{
	shipCollider->applyImpulse(XMVectorMultiply(axis, XMVectorReplicate(timer.getDeltaTime())));

	if (XMVectorGetX(XMVector3Length(shipCollider->GetLinearVelocity())) > 10)
	{
		shipCollider->setLinearVelocity(XMVector3Normalize(shipCollider->GetLinearVelocity())*10);
	}
	
}

Enemy::Enemy()
{
	m_State = (EnemyState*) new StatePatrol();
	
}

Enemy::~Enemy()
{
}

void Enemy::OnUpdate(const Timer& timer)
{
	if(this->m_State != nullptr)
	this->m_State->Update(timer, this);

	XMVECTOR axis = XMVector3Cross(XMVectorSet(0, 0, 1, 0), m_collider->GetLinearVelocity());
	if (!XMVector3Equal(axis, XMVectorZero()))
	{
		XMVECTOR q = XMQuaternionRotationAxis(
			XMVector3Normalize(axis),
			acos(XMVectorGetX(XMVector3Dot(XMVector3Normalize(m_collider->GetLinearVelocity()), XMVectorSet(0, 0, 1, 0))))
		);
		XMFLOAT4 fq; XMStoreFloat4(&fq, q);
		this->m_collider->setRotation(q);
	}
}

void Enemy::OnSpawn(QuWorld* world)
{
	Model* shipBody    = new Model();
	Model* shipShooter = new Model();
	Model* shipCockpit = new Model();
	Model* shipWing    = new Model();

	Quantum::CapsuleGenerator::generate(shipBody);
	Quantum::CapsuleGenerator::generate(shipShooter);
	Quantum::SphereGenerator::generate(shipCockpit, 2.4F);
	Quantum::BoxGenerator::generate(shipWing, XMVectorSet(0.2F, 1.0F, 2.4F, 0.0F));

	shipBody->setDefaultTexture(&ResourceLibrary::Get().alien);
	shipShooter->setDefaultTexture(&ResourceLibrary::Get().mars);
	shipCockpit->setDefaultTexture(&ResourceLibrary::Get().neptune);
	shipWing->setDefaultTexture(&ResourceLibrary::Get().alien);
	m_dir = XMVectorSet(Quantum::Math::randomFloat(-1, 1), Quantum::Math::randomFloat(-1, 1), Quantum::Math::randomFloat(-1, 1), 0);


	m_collider = new ShipCollider(this, 3.0F);
	m_collider->setPosition(XMVectorSet(Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), 0));
	world->attachChild(m_collider);

	QuEntityRenderModel* modelShipBody = new QuEntityRenderModel;
	modelShipBody->SetModel(shipBody);
	modelShipBody->setScale(XMFLOAT3(4.0F, 4.0F, 4.0F));
	modelShipBody->setRotation(XMFLOAT4(0.707F, 0, 0, 0.707F));
	modelShipBody->setPosition(XMFLOAT3(0, 0, -4.0F));
	m_collider->attachChild(modelShipBody);

	QuEntityRenderModel* modelShipCockpit = new QuEntityRenderModel;
	modelShipCockpit->SetModel(shipCockpit);
	modelShipCockpit->setPosition(XMFLOAT3(0.0F, 2.0F, -4.0F + 3.0F));
	m_collider->attachChild(modelShipCockpit);

	QuEntityRenderModel* modelShipShooterL = new QuEntityRenderModel;
	modelShipShooterL->SetModel(shipShooter);
	modelShipShooterL->setScale(XMFLOAT3(2.0F, 8.0F, 2.0F));
	modelShipShooterL->setRotation(XMFLOAT4(0.707F, 0, 0, 0.707F));
	modelShipShooterL->setPosition(XMFLOAT3(-3.0F, 0.0F, -4.0F));
	m_collider->attachChild(modelShipShooterL);

	QuEntityRenderModel* modelShipShooterR = new QuEntityRenderModel;
	modelShipShooterR->SetModel(shipShooter);
	modelShipShooterR->setScale(XMFLOAT3(2.0F, 8.0F, 2.0F));
	modelShipShooterR->setRotation(XMFLOAT4(0.707F, 0, 0, 0.707F));
	modelShipShooterR->setPosition(XMFLOAT3(3.0F, 0.0F, -4.0F));
	m_collider->attachChild(modelShipShooterR);

	QuEntityRenderModel* modelShipWingL = new QuEntityRenderModel;
	modelShipWingL->SetModel(shipWing);
	modelShipWingL->setPosition(XMFLOAT3(-3.0F, 2.0F, -4.0F));
	m_collider->attachChild(modelShipWingL);

	QuEntityRenderModel* modelShipWingR = new QuEntityRenderModel;
	modelShipWingR->SetModel(shipWing);
	modelShipWingR->setPosition(XMFLOAT3(3.0F, 2.0F, -4.0F));
	m_collider->attachChild(modelShipWingR);
}

void EnemyState::Update(const Timer& timer, Enemy* e)
{
}


StatePatrol::StatePatrol()
{
}

StatePatrol::~StatePatrol()
{
}

void StatePatrol::Update(const Timer& timer, Enemy* e)
{
	if (XMVectorGetX(XMVector3Length(e->m_collider->GetLinearVelocity())) < 20)
	{
		e->m_collider->applyImpulse(e->m_dir);

	}
	if ( XMVectorGetX(XMVector3Length( XMVectorSubtract(e->getWorldPosition(), m_PlayerPosition))) < 50)
	{
		delete  e->m_State;
		e->m_State = (EnemyState*) new StateCharge();
	}
}

StateShoot::StateShoot()
{
}

StateShoot::~StateShoot()
{
}

void StateShoot::Update(const Timer& timer, Enemy* e)
{
	if (XMVectorGetX(XMVector3Length(e->m_collider->GetLinearVelocity())) < 20)
	{
		e->m_collider->applyImpulse(e->m_dir);

	}

	e->m_Shoot->EnemyShooting(e->m_collider->GetLinearVelocity());

}

StateCharge::StateCharge()
{
}

StateCharge::~StateCharge()
{
}

void StateCharge::Update(const Timer& timer, Enemy* e)
{
	XMVECTOR playerPos = Player::GetEntityController()->controllerCollider->getWorldPosition();
	XMVECTOR enemyPos = e->m_collider->getWorldPosition();

	XMVECTOR dir = XMVector3Normalize(XMVectorSubtract(playerPos, enemyPos));
	e->m_collider->applyImpulse(XMVectorMultiply(dir, XMVectorReplicate(timer.getDeltaTime() * 10.0F)));

	if (XMVectorGetX(XMVector3Length(e->m_collider->GetLinearVelocity())) > 100)
	{
		e->m_collider->setLinearVelocity(XMVector3Normalize(e->m_collider->GetLinearVelocity()) * 15);
	}	
	if (XMVectorGetX(XMVector3Length(XMVectorSubtract(e->getWorldPosition(), m_PlayerPosition))) < 50)
	{
		delete  e->m_State;
		e->m_State = (EnemyState*) new StateShoot();
	}*/
}

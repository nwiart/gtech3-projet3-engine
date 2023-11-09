#include "stdafx.h"
#include "EntityEnemySwarm.h"
#include "Quantum/Generate/BoxGenerator.h"
#include "Model.h"
#include "Quantum/Math/Math.h"
#include "QuEntityPhysicsCollider.h"
#include "EntityController.h"
#include "Player.h"
#include "QuWorld.h"
#include "Timer.h"
#include "ResourceLibrary.h"
#include "Shooting.h"

DirectX::XMVECTOR m_PlayerPosition;


ShipCollider::ShipCollider(Enemy* e, float radius)
	: QuEntityPhysicsCollider(radius, MOTION_DYNAMIC)
{
	m_enemy = e;
}

void ShipCollider::onCollide(QuEntity* e)
{
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
}

void Enemy::OnSpawn(QuWorld* world)
{
	XMVECTOR pos = XMVectorSet(Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), 0);
	QuEntityRenderModel* EnemyShipEntity = new QuEntityRenderModel;
	Model* Ship = new Model();
	Ship->setDefaultTexture(&ResourceLibrary::Get().alien);
	float radius = Quantum::Math::randomFloat(0.5F, 3.0F);
	Quantum::BoxGenerator::generate(Ship, radius);
	m_collider = new ShipCollider(this, radius);
	EnemyShipEntity->SetModel(Ship);
	m_collider->AttachToParent(getWorld());
	EnemyShipEntity->AttachToParent(m_collider);
	m_collider->setPosition(pos);
	
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
	e->m_collider->applyImpulse(XMVectorSet(Quantum::Math::randomFloat(-1, 1), Quantum::Math::randomFloat(-1, 1), Quantum::Math::randomFloat(-1, 1), 0));
	if ( XMVectorGetX(XMVector3Length( XMVectorSubtract(e->getWorldPosition(), m_PlayerPosition))) > 100)
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
	XMVECTOR dir = XMVector3Normalize(XMVectorSubtract(m_PlayerPosition, e->m_collider->getWorldPosition()));
	e->m_collider->applyImpulse(XMVectorMultiply(dir, XMVectorReplicate(timer.getDeltaTime())));

	if (XMVectorGetX(XMVector3Length(e->m_collider->GetLinearVelocity())) > 10)
	{
		e->m_collider->setLinearVelocity(XMVector3Normalize(e->m_collider->GetLinearVelocity()) * 10);
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
	XMVECTOR dir = XMVector3Normalize(XMVectorSubtract(m_PlayerPosition, e->m_collider->getWorldPosition()));
	e->m_collider->applyImpulse(XMVectorMultiply(dir, XMVectorReplicate(timer.getDeltaTime())));

	if (XMVectorGetX(XMVector3Length(e->m_collider->GetLinearVelocity())) > 10)
	{
		e->m_collider->setLinearVelocity(XMVector3Normalize(e->m_collider->GetLinearVelocity()) * 10);
	}	e->m_collider->applyImpulse(XMVectorSet(Quantum::Math::randomFloat(-1, 1), Quantum::Math::randomFloat(-1, 1), Quantum::Math::randomFloat(-1, 1), 0));
	if (XMVectorGetX(XMVector3Length(XMVectorSubtract(e->getWorldPosition(), m_PlayerPosition))) > 50)
	{
		delete  e->m_State;
		e->m_State = (EnemyState*) new StateShoot();
	}
}

#include "EntityEnemySwarm.h"
#include "stdafx.h"
#include "Quantum/Generate/BoxGenerator.h"
#include "Model.h"
#include "Quantum/Math/Math.h"
#include "QuEntityPhysicsCollider.h"
#include "EntityController.h"
#include "Player.h"
#include "QuWorld.h"
#include "Timer.h"

EntityEnemySwarm::EntityEnemySwarm(Texture2D* tex)
{
	m_texture = tex;
	m_PlayerPosition = XMVectorSet(0, 0, 0, 0);
}
	
void EntityEnemySwarm::OnUpdate(const Timer& timer)
{
	m_PlayerPosition = Player::GetEntityController()->getWorldPosition();

	for (int i = 0; i < m_Ship.size(); i++)
	{
		XMVECTOR dir = XMVector3Normalize(XMVectorSubtract(m_PlayerPosition, m_Collider[i]->getWorldPosition()));
		UpdateDirection(dir, i, timer);
		m_axis.push_back(dir);
		if (m_Ship[i]->GetTransform().getPosition().z > 1000)
		{
			m_Ship.erase(m_Ship.begin() + i);
			m_axis.erase(m_axis.begin() + i);
			m_Collider.erase(m_Collider.begin() + 1);
		}
	}
	if (m_Ship.size() < ENEMY_COUNT)
		SpawnEntityEnemySwarm();

}

void EntityEnemySwarm::OnSpawn(QuWorld* world)
{
}

void EntityEnemySwarm::SpawnEntityEnemySwarm()
{
	XMVECTOR pos = XMVectorSet(Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), 0);
	m_Pos.push_back(pos);
	QuEntityRenderModel* EnemyShipEntity = new QuEntityRenderModel;
	m_Ship.push_back(EnemyShipEntity);
	Model* Ship = new Model();
	Ship->setDefaultTexture(m_texture);
	float radius = Quantum::Math::randomFloat(0.5F, 3.0F);
	Quantum::BoxGenerator::generate(Ship, radius);
	QuEntityPhysicsCollider* shipCollider = new QuEntityPhysicsCollider(radius, MOTION_DYNAMIC);
	m_Collider.push_back(shipCollider);
	
	EnemyShipEntity->SetModel(Ship);
	shipCollider->AttachToParent(getWorld());
	EnemyShipEntity->AttachToParent(shipCollider);
	shipCollider->setPosition(pos);
}

void EntityEnemySwarm::UpdateDirection(XMVECTOR axis, int i , const Timer& timer)
{
	m_Collider[i]->applyImpulse(XMVectorMultiply(axis, XMVectorReplicate(timer.getDeltaTime())));

	if (XMVectorGetX(XMVector3Length(m_Collider[i]->GetLinearVelocity())) > 10)
	{
		m_Collider[i]->setLinearVelocity(XMVector3Normalize(m_Collider[i]->GetLinearVelocity())*10);
	}
	
}



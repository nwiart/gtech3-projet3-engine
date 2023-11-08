#include "MeteorShower.h"
#include "stdafx.h"
#include "Quantum/Generate/MeteorGenerator.h"
#include "Model.h"
#include "Quantum/Math/Math.h"
#include "QuEntityPhysicsCollider.h"
#include "QuWorld.h"

MeteorShower::MeteorShower()
{
}

void MeteorShower::OnUpdate(const Timer& timer)
{
	for (int i = 0; i < m_meteors.size(); i++)
	{
		if (m_meteors[i]->GetTransform().getPosition().z > 1000)
		{
			//m_meteors[i]->DetachFromParent();
			m_meteors.erase(m_meteors.begin() + i);
			m_axis.erase(m_axis.begin() + i);
		}
	}
	if (m_meteors.size() < METEOR_COUNT)
		SpawnMeteor();

}

void MeteorShower::OnSpawn(QuWorld* world)
{
}

void MeteorShower::SpawnMeteor()
{
	QuEntityRenderModel* meteorEntity = new QuEntityRenderModel;
	m_meteors.push_back(meteorEntity);		
	XMFLOAT3 axis = XMFLOAT3(Quantum::Math::randomFloat(-50, 50), Quantum::Math::randomFloat(-50, 50), Quantum::Math::randomFloat(-50, 50));
	m_axis.push_back(axis);
	Model* meteor = new Model();
	float radius = Quantum::Math::randomFloat(0.5F, 3.0F);
	Quantum::MeteorGenerator::generate(meteor, radius, Quantum::Math::randomInt(5, 20), Quantum::Math::randomInt(5, 20));
	QuEntityPhysicsCollider* meteorCollider = new QuEntityPhysicsCollider(radius, MOTION_DYNAMIC);
	meteorCollider->applyImpulse(XMLoadFloat3(&axis));
	meteorEntity->SetModel(meteor);
	meteorCollider->AttachToParent(getWorld());
	meteorEntity->AttachToParent(meteorCollider);
	XMVECTOR pos = XMVectorSet(Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), 0);
	meteorCollider->setPosition(pos);
}

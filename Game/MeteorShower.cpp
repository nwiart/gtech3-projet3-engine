#include "MeteorShower.h"
#include "stdafx.h"
#include "Quantum/Generate/MeteorGenerator.h"
#include "Model.h"
#include "Quantum/Math/Math.h"
#include "QuEntityPhysicsCollider.h"
#include "QuWorld.h"
#include "Player.h"


int im;

MeteorCollider::MeteorCollider(float radius)
	: QuEntityPhysicsCollider(radius, MOTION_DYNAMIC)
{

}

void MeteorCollider::onCollide(QuEntity* e)
{
	int i = 0;
	this->Destroy(true);
}



MeteorShower::MeteorShower()
{
}

void MeteorShower::OnUpdate(const Timer& timer)
{
	time += timer.getDeltaTime();
	if (time > 10) {
		im = Quantum::Math::randomInt(0, m_meteors.size() - 1);
		m_meteors[im]->Destroy(true);
		m_meteors.erase(m_meteors.begin() + im);
		time = 0;
	}
	if (m_meteors.size() < METEOR_COUNT)
		SpawnMeteor();

}

void MeteorShower::OnSpawn(QuWorld* world)
{
	for (int i = 0; i < METEOR_COUNT; i++) {
		SpawnMeteor();
	}
}

void MeteorShower::SpawnMeteor()
{
	QuEntityRenderModel* meteorEntity = new QuEntityRenderModel;

	XMFLOAT3 axis = XMFLOAT3(Quantum::Math::randomFloat(-50, 50), Quantum::Math::randomFloat(-50, 50), Quantum::Math::randomFloat(-50, 50));
	m_axis.push_back(axis);

	Model* meteor = new Model();
	float radius = Quantum::Math::randomFloat(0.5F, 3.0F);
	Quantum::MeteorGenerator::generate(meteor, radius, Quantum::Math::randomInt(5, 20), Quantum::Math::randomInt(5, 20));
	MeteorCollider* meteorCollider = new MeteorCollider(radius);
	
	meteorCollider->applyImpulse(XMLoadFloat3(&axis)/3);
	meteorEntity->SetModel(meteor);
	meteorCollider->AttachToParent(getWorld());
	meteorEntity->AttachToParent(meteorCollider);
	XMVECTOR pos = XMVectorSet(Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), Quantum::Math::randomFloat(-400, 400), 0);
	meteorCollider->setPosition(pos);
	m_meteors.push_back(meteorCollider);
}

QuEntityPhysicsCollider* MeteorShower::GetCollider()
{
	return m_meteors[im];
}
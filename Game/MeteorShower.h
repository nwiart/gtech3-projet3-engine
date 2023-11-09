#pragma once
#include "QuEntity.h"
#include "QuEntityRenderModel.h"
#include "QuEntityPhysicsCollider.h"
#include <vector>
#define METEOR_COUNT 100


class MeteorShower;

class MeteorCollider : public QuEntityPhysicsCollider
{
public :
	MeteorCollider(float r);

	virtual void onCollide(QuEntity* e) override;
	
};

class MeteorShower : public QuEntity
{
public :
	MeteorShower();

	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	void SpawnMeteor();

private:
	int im = 0;
	float time = 0;
	std::vector <QuEntityPhysicsCollider*> m_meteors;
	std::vector <DirectX::XMFLOAT3> m_axis;
};


#pragma once
#include "QuEntity.h"
#include "QuEntityRenderModel.h"
#include <vector>
#define METEOR_COUNT 50

class MeteorShower : public QuEntity
{
public :
	MeteorShower();

	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	void SpawnMeteor();

private:
	std::vector <QuEntityRenderModel*> m_meteors;
	std::vector <DirectX::XMFLOAT3> m_axis;
};


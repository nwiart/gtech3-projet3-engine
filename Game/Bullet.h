#pragma once
#include "QuEntity.h"
#include <vector>
#include "QuEntityRenderModel.h"

class Bullet : public QuEntity
{
public:
	void Shoot();

private:
	void MoveBullet(float dt);
	void DestroyBullet();
	bool alreadyShooting = false;
	std::vector<QuEntityRenderModel*> m_bullets;
	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
};

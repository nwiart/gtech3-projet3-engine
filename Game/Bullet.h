#pragma once

#include "QuEntity.h"


class Bullet : public QuEntity
{
public:
	void Shoot();

private:
	void MoveBullet(float dt);

	bool alreadyShooting = false;

	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	float time = 0;
};

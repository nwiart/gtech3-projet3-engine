#pragma once

#include "QuEntityPhysicsCollider.h"


class Bullet;

class BulletCollider : public QuEntityPhysicsCollider
{
public:

	BulletCollider(Bullet* b);

	virtual void onCollide(QuEntity* e) override;

private:

	Bullet* m_bullet;
};


class Bullet : public QuEntity
{
public:
	void Shoot();
	inline void setSourceVelocity(DirectX::FXMVECTOR vel) { m_sourceVelocity = vel; }

private:

	bool alreadyShooting = false;

	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	float time = 0;


	DirectX::XMVECTOR m_sourceVelocity;
};

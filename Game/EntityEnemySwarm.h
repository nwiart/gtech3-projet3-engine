#pragma once
#include "QuEntity.h"
#include "QuEntityRenderModel.h"
#include "QuEntityPhysicsCollider.h"
#include "Shooting.h"

#include <vector>

#define ENEMY_COUNT 50


class EntityEntitySwarm;
class EnemyState;
class Enemy;


class EnemyState
{
public:

	EnemyState() {};
	virtual ~EnemyState() {};

	virtual void Update(const Timer& timer, Enemy* e);
};

class StatePatrol :public EnemyState
{
public:
	StatePatrol();
	~StatePatrol();
	virtual void Update(const Timer& timer, Enemy* e) override;

private:

};

class StateShoot : public EnemyState
{
public:
	StateShoot();
	~StateShoot();
	virtual void Update(const Timer& timer, Enemy* e) override;


private:

};

class StateCharge : public EnemyState
{
public:
	StateCharge();
	~StateCharge();
	virtual void Update(const Timer& timer, Enemy* e) override;


private:

};



class ShipCollider : public QuEntityPhysicsCollider
{
public:
	ShipCollider(float r);

	virtual void onCollide(QuEntity* e) override;
	
private:
};


class EntityEnemySwarm : public QuEntity
{
public:
	EntityEnemySwarm();

	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	void SpawnEntityEnemySwarm(const Timer& timer);
	void UpdateDirection(DirectX::XMVECTOR axis, const Timer& timer, ShipCollider* shipCollider);
	
	std::vector<Enemy*> m_Enemy;
	DirectX::XMVECTOR m_PlayerPosition;
};


class Enemy : public QuEntity
{
public:
	Enemy();
	~Enemy();
	
	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	EnemyState* m_State;
	ShipCollider* m_collider;
	Shooting* m_Shoot;
};

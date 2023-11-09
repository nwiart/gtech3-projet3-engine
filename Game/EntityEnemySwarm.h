#pragma once
#include "QuEntity.h"
#include "QuEntityRenderModel.h"
#include "QuEntityPhysicsCollider.h"
#include <vector>
#define ENEMY_COUNT 5

class Texture2D;

class EntityEnemySwarm : public QuEntity
{
public:
	EntityEnemySwarm(Texture2D* tex);

	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	void SpawnEntityEnemySwarm();
	void UpdateDirection(DirectX::XMVECTOR axis, int i, const Timer& timer);
	

private:

	std::vector<QuEntityPhysicsCollider*> m_Collider;
	std::vector<DirectX::XMVECTOR> m_Pos;
	std::vector<QuEntityRenderModel*> m_Ship;
	std::vector<DirectX::XMVECTOR> m_axis;
	DirectX::XMVECTOR m_PlayerPosition;

	Texture2D* m_texture;
};


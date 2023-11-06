#pragma once

#include "QuEntity.h"
#include "QuEntityRenderModel.h"

class EnnemySpawner : public QuEntity
{
public:
	EnnemySpawner();
	~EnnemySpawner();


	inline void SetRenderEntity(QuEntityRenderModel* entityToSpawn) { m_entityToSpawn = entityToSpawn; }
	inline QuEntityRenderModel* GetRenderEntity() { return m_entityToSpawn; }

	virtual void OnSpawn(QuWorld* world) override;
	virtual void OnUpdate(const Timer& timer) override;

	bool ChildNotEmpty;




private:
	QuEntityRenderModel* m_entityToSpawn;

	DirectX::XMVECTOR m_axisRotation;

};


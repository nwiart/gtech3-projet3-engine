#include "EnnemySpawner.h"

using namespace DirectX;

EnnemySpawner::EnnemySpawner()
{
	m_axisRotation = XMQuaternionRotationRollPitchYaw(0.0F, XMConvertToRadians(-30.0F), XMConvertToRadians(36.0F));
}

EnnemySpawner::~EnnemySpawner()
{
}

void EnnemySpawner::OnSpawn(QuWorld* world)
{
	if (m_entityToSpawn == nullptr) return;

	this->setPosition(DirectX::XMFLOAT3(0, 0, 0));



	this->attachChild(m_entityToSpawn);

}

void EnnemySpawner::OnUpdate(const Timer& timer)
{
	//DirectX::XMVECTOR rot = XMLoadFloat4(&this->GetTransform().getRotation());
	//DirectX::XMVECTOR inv = DirectX::XMQuaternionInverse(m_axisRotation);

	//rot = XMQuaternionMultiply(rot, inv);
	//rot = XMQuaternionMultiply(rot, XMQuaternionRotationRollPitchYaw(0.0F, timer.getDeltaTime(), 0.0F));
	//rot = XMQuaternionMultiply(rot, m_axisRotation);

	//XMFLOAT4 frot; XMStoreFloat4(&frot, rot);
	//this->setRotation(frot);

}

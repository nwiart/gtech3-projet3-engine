#include "EntityPlanetarySystem.h"

#include <Model.h>

#include <QuEntityRenderModel.h>
#include <Quantum/Generate/SphereGenerator.h>

using namespace DirectX;



static Model* unitSphere = 0;



EntityPlanetarySystem::EntityPlanetarySystem(float parentRadius, float childDistance)
	: m_parentRadius(parentRadius)
	, m_childRadius(1.0F)
	, m_childDistance(childDistance)
{
	if (!unitSphere) {
		unitSphere = new Model();
		Quantum::SphereGenerator::generate(unitSphere, 1.0F);
	}

	m_axisRotation = XMQuaternionRotationRollPitchYaw(0.0F, XMConvertToRadians(-30.0F), XMConvertToRadians(36.0F));
	XMFLOAT4 rot; XMStoreFloat4(&rot, m_axisRotation);
	this->setRotation(rot);
}

void EntityPlanetarySystem::OnSpawn(QuWorld* world)
{
	QuEntityRenderModel* m = new QuEntityRenderModel();
	m->setScale(DirectX::XMFLOAT3(m_parentRadius, m_parentRadius, m_parentRadius));
	m->SetModel(unitSphere);
	this->attachChild(m);

	m = new QuEntityRenderModel();
	m->setPosition(DirectX::XMFLOAT3(0, 0, m_childDistance));
	m->SetModel(unitSphere);
	this->attachChild(m);

	m = new QuEntityRenderModel();
	m->setPosition(DirectX::XMFLOAT3(0, 0, -m_childDistance));
	m->SetModel(unitSphere);
	this->attachChild(m);

	m = new QuEntityRenderModel();
	m->setPosition(DirectX::XMFLOAT3(m_childDistance, 0, 0));
	m->SetModel(unitSphere);
	this->attachChild(m);

	m = new QuEntityRenderModel();
	m->setPosition(DirectX::XMFLOAT3(-m_childDistance, 0, 0));
	m->SetModel(unitSphere);
	this->attachChild(m);
}

void EntityPlanetarySystem::OnUpdate(const Timer& timer)
{
	XMVECTOR rot = XMLoadFloat4(&this->GetTransform().getRotation());

	XMVECTOR inv = XMQuaternionInverse(m_axisRotation);

	rot = XMQuaternionMultiply(rot, inv);
	rot = XMQuaternionMultiply(rot, XMQuaternionRotationRollPitchYaw(0.0F, timer.getDeltaTime(), 0.0F));
	rot = XMQuaternionMultiply(rot, m_axisRotation);

	XMFLOAT4 frot; XMStoreFloat4(&frot, rot);
	this->setRotation(frot);
}

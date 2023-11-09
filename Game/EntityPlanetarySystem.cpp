#include "EntityPlanetarySystem.h"

#include <Model.h>

#include <QuEntityRenderModel.h>
#include <Quantum/Generate/SphereGenerator.h>

using namespace DirectX;

static Model* unitSphere = 0;

EntityPlanetarySystem::EntityPlanetarySystem(float parentRadius, float childDistance, int numberofChild, Texture2D* texture)
	: m_parentRadius(parentRadius)
	, m_childRadius(parentRadius / 4)
	, m_childDistance(childDistance + parentRadius)
	, m_numberofChild(numberofChild)
	, m_texture(texture)
{
	if (!unitSphere) {
	}

	m_axisRotation = XMQuaternionRotationRollPitchYaw(0.0F, XMConvertToRadians(-30.0F), XMConvertToRadians(36.0F));
	XMFLOAT4 rot; XMStoreFloat4(&rot, m_axisRotation);
	this->setRotation(rot);
}

void EntityPlanetarySystem::OnSpawn(QuWorld* world)
{
	unitSphere = new Model();
	Quantum::SphereGenerator::generate(unitSphere, 1.0F);
	unitSphere->setDefaultTexture(m_texture);
	unitSphere->setSpecularIntensity(0.0F);

	QuEntityRenderModel* m = new QuEntityRenderModel();
	m->setScale(DirectX::XMFLOAT3(m_parentRadius, m_parentRadius, m_parentRadius));
	m->SetModel(unitSphere);
	this->attachChild(m);

	for (int i = 0; i < m_numberofChild; i++) {
		m = new QuEntityRenderModel();
		m->setScale(DirectX::XMFLOAT3(m_childRadius, m_childRadius, m_childRadius));


		m->setPosition(DirectX::XMFLOAT3(m_childDistance * Quantum::Math::randomInt(-1,1), 0, m_childDistance * Quantum::Math::randomInt(-1, 1)));
		m->SetModel(unitSphere);
		this->attachChild(m);
	}
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

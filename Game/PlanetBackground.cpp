#include "PlanetBackground.h"

#include "stdafx.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Model.h"
#include "QuEntityRenderModel.h"
#include "EntityPlanetarySystem.h"
#include "EntityController.h"
#include "Quantum/Math/Math.h"

namespace qm = Quantum::Math;

PlanetBackground::PlanetBackground()
{
}

void PlanetBackground::OnUpdate(const Timer& timer)
{
	for (int i = 0; i < m_planets.size(); i++) {
		m_planets[i]->setPosition(m_planets[i]->GetTransform().getPosition());
	}
}

void PlanetBackground::OnSpawn(QuWorld* world)
{
	for (int i = 0; i < 5; i++) {
		float radius = qm::randomFloat(20, 50);
		int childCount = qm::randomInt(0, 7);
		int distance = qm::randomInt(50, 200);

		EntityPlanetarySystem* planet = new EntityPlanetarySystem(radius, distance, childCount);

		float posX = qm::randomFloat(-100, 100);
		float posY = qm::randomFloat(-100, 100);
		float posZ = qm::randomFloat(-100, 100);
		while (posX < 10 && posX > -10) {
			posX = qm::randomFloat(-1000, 1000);
		}
		while (posY < 10 && posY > -10) {
			posY = qm::randomFloat(-1000, 1000);
		}
		while (posZ < 10 && posZ > -10) {
			posZ = qm::randomFloat(-1000, 1000);
		}

		planet->setPosition(XMFLOAT3(posX, posY, posZ));
		planet->AttachToParent(this);
		m_planets.push_back(planet);
	}
}

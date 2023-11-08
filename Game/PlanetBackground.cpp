#include "PlanetBackground.h"

#include "stdafx.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Model.h"
#include "QuEntityRenderModel.h"
#include "Player.h"
#include "QuWorld.h"
#include "Quantum/Math/Math.h"

namespace qm = Quantum::Math;

PlanetBackground::PlanetBackground()
{
}

void PlanetBackground::OnUpdate(const Timer& timer)
{
}

void PlanetBackground::OnSpawn(QuWorld* world)
{
	for (int i = 0; i < 5; i++) {
		float radius = qm::randomFloat(50, 200);
		int distance = qm::randomInt(50, 100);
		int childCount = qm::randomInt(0, 7);

		EntityPlanetarySystem* planet = new EntityPlanetarySystem(radius, distance, childCount);

		float posX = qm::randomFloat(-1000, 1000);
		float posY = qm::randomFloat(-1000, 1000);
		float posZ = qm::randomFloat(-1000, 1000);

		while (posX < 100 && posX > -100) {
			posX = qm::randomFloat(-1000, 1000);
		}
		while (posY < 100 && posY > -100) {
			posY = qm::randomFloat(-1000, 1000);
		}
		while (posZ < 100 && posZ > -100) {
			posZ = qm::randomFloat(-1000, 1000);
		}

		planet->setPosition(XMFLOAT3(posX, posY, posZ));
		planet->AttachToParent(this);
		m_planets.push_back(planet);
	}
}

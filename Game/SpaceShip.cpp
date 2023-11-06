#include "SpaceShip.h"

SpaceShip::SpaceShip(float radius)
	: QuEntityPhysicsCollider(radius, MOTION_DYNAMIC)
{
}

SpaceShip::~SpaceShip()
{
}

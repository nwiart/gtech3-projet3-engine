#include "Quantum/Math/Math.h"
#include "PhysicsWorld.h"

#include "Quantum/Physics/RigidBody.h"

using namespace DirectX;



struct Sphere
{
	DirectX::XMFLOAT3 m_center;
	float m_radius;
};



PhysicsWorldCinfo::PhysicsWorldCinfo()
	: m_gravity(0.0F, -9.8F, 0.0F)
	, m_broadphase(128.0F)
{

}



PhysicsWorld::PhysicsWorld(const PhysicsWorldCinfo& info)
{
	m_gravity = info.m_gravity;
}

void PhysicsWorld::step(float deltaTime)
{
	XMVECTOR deltaVec = XMVectorReplicate(deltaTime);
	XMVECTOR gravity = XMVectorMultiply(XMLoadFloat3(&m_gravity), deltaVec);

	for (RigidBody* rb : m_dynamicRigidBodies)
	{
		// Gravity.
		rb->applyImpulse(gravity);

		XMVECTOR vel = XMLoadFloat4(&rb->getLinearVelocity());
		vel = XMVectorMultiply(vel, deltaVec);
		vel = XMVectorAdd(vel, XMLoadFloat4(&rb->getPosition()));
		rb->setPosition(vel);
	}
}


void PhysicsWorld::addRigidBody(RigidBody* rb)
{
	if (rb->m_initiallyStatic) {
		m_staticRigidBodies.push_back(rb);
	}
	else {
		m_dynamicRigidBodies.push_back(rb);
	}

	rb->m_world = this;
}

void PhysicsWorld::removeRigidBody(RigidBody* rb)
{

}

bool PhysicsWorld::rayCast(RayHitResult& outHit, DirectX::FXMVECTOR origin, DirectX::FXMVECTOR direction, float length)
{
	/*float minT;
	for (RigidBody* rb : m_staticRigidBodies)
	{
		Sphere s = { rb->getPosition(), 1.0F };
		bool hit = rayCast_sphere(outHit, origin, direction, length, s);
	}

	return this->rayCast(outHit, XMLoadFloat3(&ray.m_begin), XMLoadFloat3(&ray.m_end));*/
	return false;
}

bool PhysicsWorld::rayCast(RayHitResult& outHit, DirectX::FXMVECTOR begin, DirectX::FXMVECTOR end)
{
	XMVECTOR diff = XMVectorSubtract(end, begin);

	float length = XMVectorGetX(diff);
	diff = XMVector3Normalize(diff);

	return this->rayCast(outHit, begin, diff, length);
}

bool PhysicsWorld::rayCast(RayHitResult& outHit, const Ray& ray)
{
	return false;
}



bool rayCast_sphere(RayHitResult& outResult, FXMVECTOR rayOrigin, FXMVECTOR rayDirection, float length, const Sphere& sphere)
{
	XMVECTOR sphereToLine = XMVectorSubtract(rayOrigin, XMLoadFloat3(&sphere.m_center));

	// Quadratic equation.
	float hb = XMVectorGetX(XMVector3Dot(rayDirection, sphereToLine));
	float c = XMVectorGetX(XMVector3LengthSq(sphereToLine)) - (sphere.m_radius * sphere.m_radius);

	float d = hb * hb - c;

	// No intersection.
	if (d < 0.0F) {
		return false;
	}

	// Select closest hit.
	float t0 = (-hb - sqrt(d));

	// Intersection T is out of range.
	if (t0 < 0.0F || t0 > length) {
		return false;
	}

	XMVECTOR hitLocation = XMVectorMultiplyAdd(XMVectorReplicate(t0), rayDirection, rayOrigin);

	outResult.m_hitParameter = t0;
	XMStoreFloat3(&outResult.m_hitLocation, hitLocation);

	return true;
}
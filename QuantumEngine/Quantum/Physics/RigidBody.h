#pragma once

#include <vector>

class PhysicsShape;

class PhysicsContactListener;



enum EMotionType
{
	MOTION_STATIC,
	MOTION_KEYFRAMED,
	MOTION_DYNAMIC,
};

enum ECollisionQuality
{
	QUALITY_DEFAULT,
	QUALITY_BULLET,    // More accurate collision detection, for small and/or fast moving objects.
};



struct RigidBodyCinfo
{
	PhysicsShape* m_shape;

	EMotionType m_motionType;
	ECollisionQuality m_collisionQuality;
	bool m_isTrigger;

		/// Initial transform.
	DirectX::XMFLOAT4 m_position;
	DirectX::XMFLOAT4 m_rotation;

		/// Initial velocity.
	DirectX::XMFLOAT4 m_linearVelocity;
	DirectX::XMFLOAT4 m_angularVelocity;

	RigidBodyCinfo();
};



class RigidBody
{
	friend class PhysicsWorld;

public:

	RigidBody(const RigidBodyCinfo& info);

	inline const DirectX::XMFLOAT4& getPosition() const { return m_position; }
	inline const DirectX::XMFLOAT4& getRotation() const { return m_rotation; }

	inline const DirectX::XMFLOAT4& getLinearVelocity() const { return m_linearVelocity; }

	inline EMotionType getMotionType() const { return m_motionType; }
	inline bool isTrigger() const { return m_trigger; }

	inline void* getUserData() const { return m_userData; }


	void setPosition(DirectX::FXMVECTOR p);
	void setRotation(DirectX::FXMVECTOR p);

	void setLinearVelocity(DirectX::FXMVECTOR p);

		/// Apply a force to this rigid body.
	void applyImpulse(DirectX::FXMVECTOR force);

		/// Change the rigid body's motion type during runtime.
		/// This is only possible for objects initially created with a dynamic motion type.
	void setMotionType(EMotionType m);

	inline void addContactListener(PhysicsContactListener* l) { m_contactListeners.push_back(l); }

	inline void setUserData(void* d) { m_userData = d; }

	inline PhysicsWorld* getWorld() const { return m_world; }

	inline PhysicsShape* getShape() const { return m_shape; }

	inline const std::vector<PhysicsContactListener*>& getContactListeners() const { return m_contactListeners; }



private:

	PhysicsWorld* m_world;

	PhysicsShape* m_shape;

	std::vector<PhysicsContactListener*> m_contactListeners;

	DirectX::XMFLOAT4 m_position;
	DirectX::XMFLOAT4 m_rotation;

	DirectX::XMFLOAT4 m_linearVelocity;
	DirectX::XMFLOAT4 m_angularVelocity;


	void* m_userData;

	EMotionType m_motionType;

	bool m_trigger : 1;
	bool m_initiallyStatic : 1;
};

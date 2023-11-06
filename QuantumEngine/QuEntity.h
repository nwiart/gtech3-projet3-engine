#pragma once

#include "Quantum/Math/Transform.h"

#include "Timer.h"

#include <string>

class QuWorld;
class Timer;



class QuEntity
{
	friend class Game;

	public:
		QuEntity();
		virtual ~QuEntity();

		virtual void OnSpawn(QuWorld* world) {}
		virtual void OnUpdate(const Timer& timer) {}
		virtual void OnDestroy(QuWorld* world) {}

		void attachChild(QuEntity* child);
		void AttachToParent(QuEntity* Parent);

			/// Removes the entity from its parent, moving it to the root, but does
			/// not remove from the world.
		void DetachFromParent();

			/// Looks for an entity of the matching type in this entity's subtree.
		template<class T>
		T* findSubEntity() const;

		const Quantum::Transform& GetTransform() const { return m_Transform; }

		const DirectX::XMFLOAT4X4& GetWorldTransformMatrix() { this->updateWorldMatrix(); return m_cachedWorldMatrix; }

			/// Get world transform of this entity.
		DirectX::XMVECTOR getWorldPosition();
		DirectX::XMVECTOR getWorldRotation();

			/// Direction vectors in world space.
		DirectX::XMVECTOR getForwardVector();
		DirectX::XMVECTOR getRightVector();
		DirectX::XMVECTOR getUpVector();

			/// Set local transform components.
		void setPosition(DirectX::XMFLOAT3 positon);
		void setRotation(DirectX::XMFLOAT4 quat);
		void setScale(DirectX::XMFLOAT3 scale);

		void applyRotation(DirectX::XMVECTOR quat);



	private:

		void updateWorldMatrix();

		void setDirtyWorldMatrix();


	protected:
		QuEntity *m_Parent;
		QuEntity *m_FirstChild;
		QuEntity *m_Sibling;

		std::string m_Name;

		DirectX::XMFLOAT4X4 m_cachedWorldMatrix;
		Quantum::Transform m_Transform;

		bool m_Static;
		bool m_dirtyWorldMatrix;

		virtual void ExecuteProcedure(){}
};



template<class T>
T* QuEntity::findSubEntity() const
{
	for (QuEntity* c = this->m_FirstChild; c; c = c->m_Sibling) {
		if (dynamic_cast<T*>(c)) {
			return reinterpret_cast<T*>(c);
		}

		QuEntity* cf = c->findSubEntity<T>();
		if (cf) {
			return reinterpret_cast<T*>(cf);
		}
	}

	return 0;
}

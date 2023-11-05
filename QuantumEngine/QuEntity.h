#pragma once
#include <string>
#include "Quantum/Math/Transform.h"
#include "Timer.h"

class QuEntity
{
	friend class Game;

	public:
		QuEntity();
		~QuEntity();

		void attachChild(QuEntity* child);
		void AttachToParent(QuEntity* Parent);
		void DetachFromParent();

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
		
		virtual void OnSpawn(){}
		virtual void OnUpdate(Timer timer){}
		virtual void OnDestroy(){}
};


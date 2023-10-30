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

		void AttachToParent(QuEntity* Parent);
		void DetachFromParent();

		Quantum::Transform& GetTransform() { return m_Transform; }

		void setPosition(DirectX::XMFLOAT3 positon);

	protected:
		QuEntity *m_Parent;
		QuEntity *m_FirstChild;
		QuEntity *m_Sibling;

		std::string m_Name;

		Quantum::Transform m_Transform;

		bool m_Static;

		virtual void ExecuteProcedure(){}
		
		virtual void OnSpawn(){}
		virtual void OnUpdate(Timer timer){}
		virtual void OnDestroy(){}
};


#pragma once
#include <string>
#include "Quantum/Math/Transform.h"

class QuEntity
{
	public:
		QuEntity();
		~QuEntity();

		void AttachToParent(QuEntity* Parent);
		void DetachFromParent();

		Quantum::Transform& GetTransform() { return m_Transform; }

	private:
		QuEntity *m_Parent;
		QuEntity *m_FirstChild;
		QuEntity *m_Sibling;

		std::string m_Name;

		Quantum::Transform m_Transform;

		bool m_Static;

		virtual void ExecuteProcedure(){}

};


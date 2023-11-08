#pragma once

#include "QuEntity.h"

#include <vector>

class PhysicsWorld;



class QuWorld : public QuEntity
{
	friend class Game;

public:

	QuWorld();
	~QuWorld();

	//virtual void ExecuteProcedure();

	void markForDeletion(QuEntity* e);

	inline PhysicsWorld* getPhysicsWorld() const { return m_physicsWorld; }

	inline bool isOpen() const { return m_isOpen; }
	inline void setOpen(bool b) { m_isOpen = b; }
private:

		/// Performs destroy on all entities marked for deletion.
	void deletePendingEntities();


private:

	PhysicsWorld* m_physicsWorld;

	bool m_isOpen;
	std::vector<QuEntity*> m_markedForDelete;
};
